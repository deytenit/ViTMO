"use strict";

const operation = (fn) => Object.defineProperty(
        (...values) => (...args) => fn(...values.map(value => value(...args))),
        "length",
        { "value": fn.length }
    );

const add = operation((x, y) => x + y);
const subtract = operation((x, y) => x - y);
const multiply = operation((x, y) => x * y);
const divide = operation((x, y) => x / y);

const negate = operation((x) => -x);

const floor = operation(Math.floor);
const ceil = operation(Math.ceil);

const madd = operation((x, y, z) => x * y + z);

const cnst = (value) => () => value;

const one = cnst(1);
const two = cnst(2);

const variables = {
    x: 0,
    y: 1,
    z: 2
};

const variable = (name) => (...args) => args[variables[name]];

const OPERATIONS = {
    "one": () => one,
    "two": () => two,

    "negate": negate,
    "_": floor, 
    "^": ceil,
    
    "+": add,
    "-": subtract,
    "*": multiply,
    "/": divide,
    
    "*+": madd
};

function parse(expression) {
    const stack = [];

    function parseToken(token) {
        if (token in OPERATIONS) {
            const operation = OPERATIONS[token];
            return operation(...stack.splice(-operation.length, operation.length));
        } else {
            const constant = parseFloat(token);

            return !isNaN(constant) ? cnst(constant) : variable(token);
        }
    }

    expression.trim().split(/\s+/)
        .forEach((token) => stack.push(parseToken(token)));

    return stack.pop();
}

console.log(parse("y one -")(1, 1, 1));
