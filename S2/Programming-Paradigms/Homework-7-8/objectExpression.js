"use strict";

function Expressionize(base, evaluate, diff, prefix, postfix, toString) {
    base.prototype.evaluate = evaluate;
    base.prototype.diff = diff;
    base.prototype.prefix = prefix;
    base.prototype.postfix = postfix;
    base.prototype.toString = toString;
}

const Const = (() => {
    function Const(value) {
        this.value = value;
    }

    const toString = function () { return this.value.toString() };

    Expressionize(Const,
        function () { return this.value },
        function () { return Const.basic.zero },
        toString,
        toString,
        toString
    );
    
    Const.basic = {
        zero: new Const(0),
        one: new Const(1)
    };
    
    return Const;
})();

const Variable = (() => {
    function Variable(name) {
        this.name = name;
    }
        
    Variable.variables = {
        x: 0,
        y: 1,
        z: 2
    };

    const toString = function () { return this.name; };

    Expressionize(Variable,
        function (...args) { return args[Variable.variables[this.name]]; },
        function (variable) { return variable === this.name ? Const.basic.one : Const.basic.zero },
        toString,
        toString,
        toString
    );

    return Variable;
})();

function Operation(title, calculate, diff) {
    function Operation(...values) {
        this.values = values;
    }

    Expressionize(Operation,
        function (...args) {
            return calculate.call(this, ...this.values.map((value) => value.evaluate(...args)));
        },
        function (variable) { return diff.call(this, variable, ...this.values); },
        function () {
            return `(${Operation.title} ${this.values.map((value) => value.prefix()).join(" ")})`;
        },
        function () {
            return `(${this.values.map((value) => value.postfix()).join(" ")} ${Operation.title})`;
        },
        function () {
            return `${this.values.join(" ")} ${Operation.title}`;
        },
    );

    Operation.title = title;
    Operation.count = calculate.length;

    return Operation;
};

const Negate = Operation(
    "negate",
    (x) => -x,
    (variable, x) => new Negate(x.diff(variable))
);

const Add = Operation(
    "+",
    (x, y) => x + y,
    (variable, x, y) => new Add(x.diff(variable), y.diff(variable))
);

const Subtract = Operation(
    "-",
    (x, y) => x - y,
    (variable, x, y) => new Subtract(x.diff(variable), y.diff(variable))
);

const Multiply = Operation(
    "*",
    (x, y) => x * y,
    (variable, x, y) => new Add(
        new Multiply(x.diff(variable), y), 
        new Multiply(x, y.diff(variable))
    )
);

const Divide = Operation(
    "/",
    (x, y) => x / y,
    (variable, x, y) => new Divide(
        new Subtract(new Multiply(x.diff(variable), y), new Multiply(x, y.diff(variable))), 
        new Multiply(y, y)
    )
);

const Sumexp = Operation(
    "sumexp",
    (...args) => args.reduce((prev, curr) => prev + Math.exp(curr), 0),
    (variable, ...args) =>
        args.reduce((prev, curr) => new Add(prev, new Multiply(new Sumexp(curr), curr.diff(variable))), Const.basic.zero)
);

const LSE = Operation(
    "lse",
    (...args) => Math.log(args.reduce((prev, curr) => prev + Math.exp(curr), 0)),
    (variable, ...args) => new Divide(new Sumexp(...args).diff(variable), new Sumexp(...args))
);

function Sumsq(length, subSq) {
    return Operation(
        "sumsq" + length,
        Object.defineProperty((...args) => args.reduce((prev, curr) => prev + curr * curr, 0),
            "length",
            { value: length }
        ),
        (variable, ...args) => new Add(
            new Multiply(args[0], args[0]), 
            length === 2 ? new Multiply(args[1], args[1]) : new subSq(...args.slice(1))
        ).diff(variable)
    );
}

const Sumsq2 = Sumsq(2);
const Sumsq3 = Sumsq(3, Sumsq2);
const Sumsq4 = Sumsq(4, Sumsq3);
const Sumsq5 = Sumsq(5, Sumsq4);

function Distance(sumsq) {
    return Operation(
        "distance" + sumsq.length,
        Object.defineProperty((...args) => Math.sqrt(args.reduce((prev, curr) => prev + curr * curr, 0)),
            "length",
            { value: sumsq.length }
        ),
        function (variable, ...args) { return new Divide(new sumsq(...args).diff(variable), new Multiply(new Const(2), this)); }
    );
}

const Distance2 = Distance(Sumsq2);
const Distance3 = Distance(Sumsq3);
const Distance4 = Distance(Sumsq4);
const Distance5 = Distance(Sumsq5);

const operationsEntries = [
    Negate, Add, Subtract, 
    Multiply, Divide, Sumsq2, 
    Sumsq3, Sumsq4, Sumsq5, 
    Distance2, Distance3, Distance4, 
    Distance5, Sumexp, LSE
].map((operation) => [operation.title, operation]);

const operations = Object.fromEntries(operationsEntries);

function parse(expression) {
    const stack = [];

    function parseToken(token) {
        if (token in operations) {
            const operation = operations[token];
            return new operation(...stack.splice(-operation.count, operation.count));
        }

        const constant = parseFloat(token);

        return !isNaN(constant) ? new Const(constant) : new Variable(token);
    }

    expression.trim().split(/\s+/).forEach((token) => {
        stack.push(parseToken(token));
    });

    return stack.pop();
}

function ParserError(name, msgfn) {    
    ParserError.prototype = Object.create(Error.prototype, {
        name: {
            value: name
        },
        constructor: {
            value: ParserError
        }
    });
    
    function ParserError(...args) {
        const superError = Error.call(this, msgfn(...args));

        this.message = superError.message;
        this.stack = superError.stack;
    }

    return ParserError;
}

const UnexpectedTokenError = ParserError("UnexpectedTokenError", (expect, found, pos) =>
    `at ch. ${pos}: Expected '${expect}', found '${found}'.`
);

const IllegalArgumentsError = ParserError("IllegalArgumentsError", (expect, found, operation, pos) =>
    `at ch. ${pos}: Operation '${operation}' expects ${expect} argument(s), found ${found}.`
);

const NumberFormatError = ParserError("NumberFormatError", (found, pos) =>
    `at ch. ${pos}: Cannot parse value '${found}'.`
);

const IllegalFormatError = ParserError("IllegalFormatError", (message, pos) =>
    `at ch. ${pos}: ${message}.`
);

function Parser(mode) {
    function Parser(expression) {
        this.expression = expression;
        this.pos = 0;
    }

    Parser.delimex = /(\s|\(|\))+/;

    Parser.prototype.makeError = function (errorKind, ...args) {
        throw new errorKind(...args, this.pos);
    };

    Parser.prototype.makeOperation = function (operation, ...args) {
        if (operation.count !== 0 && operation.count !== args.length) {
            throw this.makeError(IllegalArgumentsError, operation.count, args.length, operation.title)
        }
        
        return new operation(...args);
    };

    Parser.prototype.skipWhitespace = function () {
        while (/\s/.test(this.get())) {
            this.pos++;
        }
    };

    Parser.prototype.get = function () {
        return this.pos !== this.expression.length ? this.expression[this.pos] : null;
    };

    Parser.prototype.hasToken = function (query) {
        const prev = this.pos;

        const result = this.getToken();

        this.pos = prev;

        return (typeof query === "object") ? result in query : result === query;
    };

    Parser.prototype.getToken = function () {
        this.skipWhitespace();

        if (Parser.delimex.test(this.get())) {
            const result = this.get();
            this.pos++;
            return result;
        }

        let result = "";
        
        while (this.get() && !Parser.delimex.test(this.get())) {
            result += this.get();
            this.pos++;
        }

        return result;
    };

    Parser.prototype.parseHigh = function () {
        this.skipWhitespace();

        if (this.hasToken('(')) {
            this.getToken();
            const result = this.parseMid();
            if (this.getToken() !== ')') {
                this.makeError(IllegalFormatError, "Missing closing parenthesis");
            }
            return result;
        }
        
        return this.parseLow(false);
    };

    Parser.prototype.parseMid = function () {
        let operation = null;

        if (mode === "prefix") {
            operation = this.parseLow(true);
        }

        const args = [];

        while (!this.hasToken(')') && !this.hasToken(operations)) {
            args.push(this.parseHigh());
        }

        if (mode === "postfix") {
            operation = this.parseLow(true);
        }

        return this.makeOperation(operation, ...args);
    };

    Parser.prototype.parseLow = function (isOperation) {
        const token = this.getToken();

        if (!token) {
            this.makeError(UnexpectedTokenError, "token", "nothing");
        }

        if (isOperation) {
            if (token in operations) {
                return operations[token];
            }
            else {
                this.makeError(IllegalFormatError, "Operation was expected");
            }
        }

        if (token in Variable.variables) {
            return new Variable(token);
        }

        if (!isNaN(token)) {
            return new Const(parseFloat(token));
        }

        this.makeError(NumberFormatError, token);
    };

    Parser.prototype.parse = function () {
        const result = this.parseHigh();

        const token = this.getToken();

        if (token) {
            this.makeError(UnexpectedTokenError, "end", token);
        }

        return result;
    };

    return Parser;
}

function parsePrefix(expression) {
    const PrefixParser = Parser("prefix");

    return new PrefixParser(expression).parse();
}

function parsePostfix(expression) {
    const PostfixParser = Parser("postfix");

    return new PostfixParser(expression).parse();
}


console.log(parsePostfix("(y negate)"));