const Command = (() => {
    function Command(type, length, pos, realPos) {
        this.type = type;
        this.length = length;
        this.pos = pos;
        this.realPos = realPos;
        this.next = pos + 1;
    }
    
    Command.Types = {
        PLUS: "PLUS",
        MINUS: "MINUS",
        NEXT: "NEXT",
        PREV: "PREV",
        OPEN_BRACKET: "OPEN_BRACKET",
        CLOSE_BRACKET: "CLOSE_BRACKET",
        COMMA: "COMMA",
        DOT: "DOT"
    };

    return Command;
})();

const Interpreter = (() => {
    function Interpreter(commands, input, maxSize, maxOperations) {
        this.commands = commands;
        this.commandsPos = 0;

        this.input = input;
        this.inputPos = 0;

        this.data = Array(Interpreter.BASE_SIZE).fill(Interpreter.DEFAULT_VALUE);
        this.dataPos = 0;

        this.maxSize = maxSize;

        this.count = 0;
        this.maxOperations = maxOperations;
    }

    Interpreter.prototype.error = function (msg) {
        throw new Error(`Interpreting error: ${msg} (at pos. ${this.program[this.programPos].realPos})`);
    };

    Interpreter.prototype.interpret = function () {
        while (this.commandsPos < this.commands.length) {
            this.execute(this.commands[this.commandsPos]);
        }
    };

    Interpreter.prototype.execute = function (command) {
        if (this.count == this.maxOperations) {
            this.error("Amount of operations has been exceeded.");
        }

        ++this.count;

        switch (command.type) {
            case Command.Types.PLUS: {
                ++this.data[this.dataPos];
                ++this.commandsPos;
                break;
            }
            case Command.Types.MINUS: {
                --this.data[this.dataPos];
                ++this.commandsPos;
                break;
            }
            case Command.Types.NEXT: {
                if (this.dataPos == this.data.length) {
                    this.expand();
                }
                ++this.dataPos;
                ++this.commandsPos;
                break;
            }
            case Command.Types.PREV: {
                if (this.dataPos == 0) {
                    this.error("Out Of Bounds.");
                }
                --this.dataPos;
                ++this.commandsPos;
                break;
            }
            case Command.Types.DOT: {
                console.log(String.fromCharCode(this.data[this.dataPos]));
                ++this.commandsPos;
                break;
            }
            case Command.Types.COMMA: {
                this.data[this.dataPos] = this.input.charCodeAt(this.inputPos++);
                ++this.commandsPos;
                break;
            }
            case Command.Types.OPEN_BRACKET: {
                if (!this.data[this.dataPos]) {
                    this.commandsPos = command.next;
                }
                else {
                    ++this.commandsPos;
                }
                break;
            }
            case Command.Types.CLOSE_BRACKET: {
                if (this.data[this.dataPos]) {
                    this.commandsPos = command.next;
                }
                else {
                    ++this.commandsPos;
                }
            }
        }
    };

    Interpreter.prototype.expand = function () {
        if (this.data.length == this.maxSize) {
            throw new Error("Memory limit has exceeded.");
        }
        this.data.push(Interpreter.DEFAULT_VALUE);
    };

    Interpreter.DEFAULT_VALUE = 0;
    Interpreter.BASE_SIZE = 30000;

    return Interpreter;
})();

const Parser = (() => {
    function Parser(str) {
        this.str = str;
        this.pos = 0;
        this.cnt = -1;
    }

    Parser.prototype.error = function (msg) {
        throw new Error(`Parser error: ${msg} (at pos. ${this.pos})`);
    };

    Parser.prototype.parseCommand = function () {
        let command = "";

        while (this.pos < this.str.length) {
            command += this.str[this.pos++];

            if (Parser.COMMANDS[command]) {
                ++this.cnt;
                return new Command(Parser.COMMANDS[command], command.length, this.pos - command.length, this.cnt);
            }

        }

        this.error("Unrecognized command.");
    };
    Parser.prototype.parseInternal = function (depth = 0) {
        let result = [];

        while (this.pos < this.str.length) {
            let command = this.parseCommand();
            if (command.type === Command.Types.OPEN_BRACKET) {
                let tmp = this.parseInternal(depth + 1);

                if (tmp[tmp.length - 1].type !== Command.Types.CLOSE_BRACKET) {
                    this.error("Cycle was never closed.");
                }

                command.next = tmp[tmp.length - 1].pos + 1;
                tmp[tmp.length - 1].next = command.pos;

                result.push(command);
                result = result.concat(tmp);
                continue;
            }
            if (command.type == Command.Types.CLOSE_BRACKET) {
                if (depth == 0) {
                    this.error("Cycle was never opened.");
                }

                result.push(command);
                break;
            }
            
            result.push(command);
        }

        return result;
    };
    Parser.prototype.parse = function () {
        let result = this.parseInternal();

        return result;
    };

    Parser.COMMANDS = {
        "1": Command.Types.PLUS,
        "000": Command.Types.MINUS,
        "010": Command.Types.NEXT,
        "011": Command.Types.PREV,
        "00100": Command.Types.OPEN_BRACKET,
        "0011": Command.Types.CLOSE_BRACKET,
        "0010110": Command.Types.COMMA,
        "001010": Command.Types.DOT
    };

    return Parser;
})();

function runProgram(program, input) {
    const maxSize = 30000;
    const maxOperations = 30000;
    const parser = new Parser(program);
    const interpreter = new Interpreter(parser.parse(), input, maxSize, maxOperations);
    interpreter.interpret();
}

// Hello World Example:
runProgram("11111111110010001011111110101111111111010111010101101101101100000110101100101001010010101111111001010001010111001010010110010100110111111111111111110010100100010101110010100000000000000000000010100000000000000000000000000010100101001010010001010", "");