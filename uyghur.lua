--[[
    main compiler program
]]

-- https://github.com/kompasim/pure-lua-tools.git
require "pure-lua-tools.initialize"

local TOKEN_TYPE = {
    NAME = "NAME", -- variable
    STRING = "STRING", -- string
    NUMBER = "NUMBER", -- number
    BOOL = "BOOL", -- value
    EMPTY = "EMPTY", -- value
    --
    CODE_START = "CODE_START",
    CODE_END = "CODE_END",
    -- 
    VARIABLE = "VARIABLE",
    VALUE = "VALUE",
    MADE = "MADE",
    --
    RESULT = "RESULT",
    RETURN = "RETURN",
    --
    FUNC = "FUNC",
    WITH = "WITH",
    CALL = "CALL",
    FURTHER = "FURTHER",
    -- 
    IF = "IF",
    WHILE = "WHILE",
    --
    TARGET = "TARGET",
    OUTPUT = "OUTPUT",
    INPUT = "INPUT",
    AND = "AND",
    OR = "OR",
    NOT = "NOT",
    -- 
    OPERATION_NUMBER = "OPERATION_NUMBER", -- operation
    OPERATION = "OPERATION", -- operation
}

local TOKEN_TYPES_VALUES = {TOKEN_TYPE.NAME, TOKEN_TYPE.STRING, TOKEN_TYPE.NUMBER, TOKEN_TYPE.BOOL, TOKEN_TYPE.EMPTY}
local TOKEN_TYPES_STRING = {TOKEN_TYPE.NAME, TOKEN_TYPE.STRING}
local TOKEN_TYPES_NUMBER = {TOKEN_TYPE.NAME, TOKEN_TYPE.NUMBER}
local TOKEN_TYPES_LOGICS = {TOKEN_TYPE.NAME, TOKEN_TYPE.BOOL, TOKEN_TYPE.EMPTY}

local TOKEN_VALUES = {
    -- 
    OUTPUT = "yezilsun",
    INPUT = "oqulsun",
    -- 
    EMPTY = "quruq",
    --
    TRUE = "rast",
    FALSE = "yalghan",
    --
    AND = "xemde",
    OR = "yaki",
    NOT = "ekische",
    --
    ADD = "qushulghan",
    SUB = "elinghan",
    MUL = "kupeytilgen",
    DIV = "bulungen",
    LESS = "kichik",
    MORE = "chong",
    -- 
    CONCAT = "ulanghan",
    EQUAL = "teng",
}

local TOKEN_TYPE_MAP = {
    -- block
    bolsa = TOKEN_TYPE.CODE_START,
    tamamlansun = TOKEN_TYPE.CODE_END,
    -- variable
    mixtar = TOKEN_TYPE.VARIABLE,
    qimmiti = TOKEN_TYPE.VALUE,
    bolsun = TOKEN_TYPE.MADE,
    -- result retrun
    netije = TOKEN_TYPE.RESULT,
    qayturulsun = TOKEN_TYPE.RETURN,
    -- function
    fonkisiye = TOKEN_TYPE.FUNC,
    bilen = TOKEN_TYPE.WITH,
    ishlitilsun = TOKEN_TYPE.CALL,
    we = TOKEN_TYPE.FURTHER,
    -- if
    eger = TOKEN_TYPE.IF,
    -- while
    nawada = TOKEN_TYPE.WHILE,
    -- io
    ikrangha = TOKEN_TYPE.TARGET,
    ikrandin = TOKEN_TYPE.TARGET,
    [TOKEN_VALUES.OUTPUT] = TOKEN_TYPE.OUTPUT,
    [TOKEN_VALUES.INPUT] = TOKEN_TYPE.INPUT,
    -- types
    [TOKEN_VALUES.EMPTY] = TOKEN_TYPE.EMPTY,
    -- bool
    [TOKEN_VALUES.TRUE] = TOKEN_TYPE.BOOL,
    [TOKEN_VALUES.FALSE] = TOKEN_TYPE.BOOL,
    -- operation logic
    [TOKEN_VALUES.AND] = TOKEN_TYPE.AND,
    [TOKEN_VALUES.OR] = TOKEN_TYPE.OR,
    [TOKEN_VALUES.NOT] = TOKEN_TYPE.NOT,
    -- operation number
    [TOKEN_VALUES.ADD] = TOKEN_TYPE.OPERATION_NUMBER,
    [TOKEN_VALUES.SUB] = TOKEN_TYPE.OPERATION_NUMBER,
    [TOKEN_VALUES.MUL] = TOKEN_TYPE.OPERATION_NUMBER,
    [TOKEN_VALUES.DIV] = TOKEN_TYPE.OPERATION_NUMBER,
    [TOKEN_VALUES.LESS] = TOKEN_TYPE.OPERATION_NUMBER,
    [TOKEN_VALUES.MORE] = TOKEN_TYPE.OPERATION_NUMBER,
    -- operation
    [TOKEN_VALUES.CONCAT] = TOKEN_TYPE.OPERATION,
    [TOKEN_VALUES.EQUAL] = TOKEN_TYPE.OPERATION,
}

local SIGNS = {
    LINE = "LINE",
    EMPTY = "EMPTY",
    COMMENT = "COMMENT",
    NUMBER = "NUMBER",
    LETTER = "LETTER",
    OPEN = "OPEN",
    CLOSE = "CLOSE",
    OTHER = "OTHER",
}

local ORDER = {
    SIGNS.LINE,
    SIGNS.EMPTY,
    SIGNS.COMMENT,
    SIGNS.NUMBER,
    SIGNS.LETTER,
    SIGNS.OPEN,
    SIGNS.CLOSE,
    SIGNS.OTHER,
}

local EXPRESSIONS = {
    [SIGNS.LINE] = "\n",
    [SIGNS.EMPTY] = "%s",
    [SIGNS.COMMENT] = "%#",
    [SIGNS.NUMBER] = "%d",
    [SIGNS.LETTER] = "%a",
    [SIGNS.OPEN] = "%[",
    [SIGNS.CLOSE] = "%]",
    [SIGNS.OTHER] = ".",
}

local STATE = {
    NEW = "NEW",
    COMMENT = "COMMENT",
    NUMBER = "NUMBER",
    LETTER = "LETTER",
    OPEN = "OPEN",
    STRING = "STRING",
    CLOSE = "CLOSE",
    END = "END",
    ERROR = "ERROR",
}

local TOKENIZER_STATE_MAP = {
    [STATE.NEW] = {
        [SIGNS.LINE] = STATE.NEW,
        [SIGNS.EMPTY] = STATE.NEW,
        [SIGNS.COMMENT] = STATE.COMMENT,
        [SIGNS.NUMBER] = STATE.NUMBER,
        [SIGNS.LETTER] = STATE.LETTER,
        [SIGNS.OPEN] = STATE.OPEN,
        [SIGNS.CLOSE] = STATE.ERROR,
        [SIGNS.OTHER] = STATE.ERROR,

    },
    [STATE.COMMENT] = {
        [SIGNS.LINE] = STATE.NEW,
        [SIGNS.EMPTY] = STATE.COMMENT,
        [SIGNS.COMMENT] = STATE.COMMENT,
        [SIGNS.NUMBER] = STATE.COMMENT,
        [SIGNS.LETTER] = STATE.COMMENT,
        [SIGNS.OPEN] = STATE.COMMENT,
        [SIGNS.CLOSE] = STATE.COMMENT,
        [SIGNS.OTHER] = STATE.COMMENT,

    },
    [STATE.NUMBER] = {
        [SIGNS.LINE] = STATE.END,
        [SIGNS.EMPTY] = STATE.END,
        [SIGNS.NUMBER] = STATE.NUMBER,
    },
    [STATE.LETTER] = {
        [SIGNS.LINE] = STATE.END,
        [SIGNS.EMPTY] = STATE.END,
        [SIGNS.LETTER] = STATE.LETTER,
    },
    [STATE.OPEN] = {
        [SIGNS.LINE] = STATE.ERROR,
        [SIGNS.EMPTY] = STATE.STRING,
        [SIGNS.COMMENT] = STATE.STRING,
        [SIGNS.NUMBER] = STATE.STRING,
        [SIGNS.LETTER] = STATE.STRING,
        [SIGNS.OPEN] = STATE.ERROR,
        [SIGNS.CLOSE] = STATE.CLOSE,
        [SIGNS.OTHER] = STATE.STRING,
    },
    [STATE.STRING] = {
        [SIGNS.LINE] = STATE.ERROR,
        [SIGNS.EMPTY] = STATE.STRING,
        [SIGNS.COMMENT] = STATE.STRING,
        [SIGNS.NUMBER] = STATE.STRING,
        [SIGNS.LETTER] = STATE.STRING,
        [SIGNS.OPEN] = STATE.ERROR,
        [SIGNS.CLOSE] = STATE.CLOSE,
        [SIGNS.OTHER] = STATE.STRING,
    },
    [STATE.CLOSE] = {
        [SIGNS.LINE] = STATE.NEW,
        [SIGNS.EMPTY] = STATE.NEW,
    },
}

local AST_TYPE = {
    AST_TOKEN = "AST_TOKEN",
    AST_PROGRAM = "AST_PROGRAM",
    AST_END = "AST_END",
    AST_VARIABLE = "AST_VARIABLE",
    AST_RESULT = "AST_RESULT",
    AST_FUNC = "AST_FUNC",
    AST_CALL = "AST_CALL",
    AST_IF = "AST_IF",
    AST_WHILE = "AST_WHILE",
    AST_EXPRESSION = "AST_EXPRESSION",
    AST_EXPRESSION_NUMBER = "AST_EXPRESSION_NUMBER",
    AST_EXPRESSION_LOGIC = "AST_EXPRESSION_LOGIC",
    AST_OPERATE = "AST_OPERATE",
}

local REPL_AST = {
    [AST_TYPE.AST_VARIABLE] = true,
    [AST_TYPE.AST_EXPRESSION] = true,
    [AST_TYPE.AST_EXPRESSION_NUMBER] = true,
    [AST_TYPE.AST_EXPRESSION_LOGIC] = true,
    [AST_TYPE.AST_OPERATE] = true,
}

local PARSER_STATE_MAP = {
    [TOKEN_TYPE.CODE_END] = AST_TYPE.AST_END,
    [TOKEN_TYPE.VARIABLE] = {
        [TOKEN_TYPE.NAME] = {
            [TOKEN_TYPE.VALUE] = {
                [TOKEN_TYPE.NAME] = {
                    [TOKEN_TYPE.MADE] = AST_TYPE.AST_VARIABLE,
                },
                [TOKEN_TYPE.STRING] = {
                    [TOKEN_TYPE.MADE] = AST_TYPE.AST_VARIABLE,
                },
                [TOKEN_TYPE.NUMBER] = {
                    [TOKEN_TYPE.MADE] = AST_TYPE.AST_VARIABLE,
                },
                [TOKEN_TYPE.BOOL] = {
                    [TOKEN_TYPE.MADE] = AST_TYPE.AST_VARIABLE,
                },
                [TOKEN_TYPE.EMPTY] = {
                    [TOKEN_TYPE.MADE] = AST_TYPE.AST_VARIABLE,
                },
            }
        }
    },
    [TOKEN_TYPE.RESULT] = {
        [TOKEN_TYPE.NAME] = {
            [TOKEN_TYPE.RETURN] = AST_TYPE.AST_RESULT,
        },
        [TOKEN_TYPE.STRING] = {
            [TOKEN_TYPE.RETURN] = AST_TYPE.AST_RESULT,
        },
        [TOKEN_TYPE.NUMBER] = {
            [TOKEN_TYPE.RETURN] = AST_TYPE.AST_RESULT,
        },
        [TOKEN_TYPE.BOOL] = {
            [TOKEN_TYPE.RETURN] = AST_TYPE.AST_RESULT,
        },
        [TOKEN_TYPE.EMPTY] = {
            [TOKEN_TYPE.RETURN] = AST_TYPE.AST_RESULT,
        },
    },
    [TOKEN_TYPE.FUNC] = {
        [TOKEN_TYPE.NAME] = {
            [TOKEN_TYPE.CODE_START] = AST_TYPE.AST_FUNC,
            [TOKEN_TYPE.VARIABLE] = AST_TYPE.AST_FUNC,
            [TOKEN_TYPE.CALL] = AST_TYPE.AST_CALL,
            [TOKEN_TYPE.WITH] = AST_TYPE.AST_CALL,
        },
    },
    [TOKEN_TYPE.IF] = {
        [TOKEN_TYPE.NAME] = {
            [TOKEN_TYPE.VALUE] = {
                [TOKEN_TYPE.NAME] = {
                    [TOKEN_TYPE.CODE_START] = AST_TYPE.AST_IF,
                },
                [TOKEN_TYPE.STRING] = {
                    [TOKEN_TYPE.CODE_START] = AST_TYPE.AST_IF,
                },
                [TOKEN_TYPE.NUMBER] = {
                    [TOKEN_TYPE.CODE_START] = AST_TYPE.AST_IF,
                },
                [TOKEN_TYPE.BOOL] = {
                    [TOKEN_TYPE.CODE_START] = AST_TYPE.AST_IF,
                },
                [TOKEN_TYPE.EMPTY] = {
                    [TOKEN_TYPE.CODE_START] = AST_TYPE.AST_IF,
                },
            }
        }
    },
    [TOKEN_TYPE.WHILE] = {
        [TOKEN_TYPE.NAME] = {
            [TOKEN_TYPE.VALUE] = {
                [TOKEN_TYPE.NAME] = {
                    [TOKEN_TYPE.CODE_START] = AST_TYPE.AST_WHILE,
                },
                [TOKEN_TYPE.STRING] = {
                    [TOKEN_TYPE.CODE_START] = AST_TYPE.AST_WHILE,
                },
                [TOKEN_TYPE.NUMBER] = {
                    [TOKEN_TYPE.CODE_START] = AST_TYPE.AST_WHILE,
                },
                [TOKEN_TYPE.BOOL] = {
                    [TOKEN_TYPE.CODE_START] = AST_TYPE.AST_WHILE,
                },
                [TOKEN_TYPE.EMPTY] = {
                    [TOKEN_TYPE.CODE_START] = AST_TYPE.AST_WHILE,
                },
            }
        }
    },
    [TOKEN_TYPE.NAME] = {
        [TOKEN_TYPE.VALUE] = {
            [TOKEN_TYPE.NAME] = {
                [TOKEN_TYPE.OPERATION_NUMBER] = {
                    [ TOKEN_TYPE.NAME] = {
                        [TOKEN_TYPE.MADE] = AST_TYPE.AST_EXPRESSION_NUMBER,
                    },
                    [ TOKEN_TYPE.NUMBER] = {
                        [TOKEN_TYPE.MADE] = AST_TYPE.AST_EXPRESSION_NUMBER,
                    },
                },
                [TOKEN_TYPE.OPERATION] = {
                    [ TOKEN_TYPE.NAME] = {
                        [TOKEN_TYPE.MADE] = AST_TYPE.AST_EXPRESSION,
                    },
                    [ TOKEN_TYPE.STRING] = {
                        [TOKEN_TYPE.MADE] = AST_TYPE.AST_EXPRESSION,
                    },
                    [ TOKEN_TYPE.NUMBER] = {
                        [TOKEN_TYPE.MADE] = AST_TYPE.AST_EXPRESSION,
                    },
                },
                [TOKEN_TYPE.AND] = {
                    [ TOKEN_TYPE.NAME] = {
                        [TOKEN_TYPE.MADE] = AST_TYPE.AST_EXPRESSION_LOGIC,
                    },
                    [ TOKEN_TYPE.BOOL] = {
                        [TOKEN_TYPE.MADE] = AST_TYPE.AST_EXPRESSION_LOGIC,
                    },
                },
                [TOKEN_TYPE.OR] = {
                    [ TOKEN_TYPE.NAME] = {
                        [TOKEN_TYPE.MADE] = AST_TYPE.AST_EXPRESSION_LOGIC,
                    },
                    [ TOKEN_TYPE.BOOL] = {
                        [TOKEN_TYPE.MADE] = AST_TYPE.AST_EXPRESSION_LOGIC,
                    },
                },
                [ TOKEN_TYPE.NOT] = {
                    [TOKEN_TYPE.MADE] = AST_TYPE.AST_EXPRESSION_LOGIC,
                },
            },
            [TOKEN_TYPE.STRING] = {
                [TOKEN_TYPE.OPERATION] = {
                    [ TOKEN_TYPE.NAME] = {
                        [TOKEN_TYPE.MADE] = AST_TYPE.AST_EXPRESSION,
                    },
                    [ TOKEN_TYPE.STRING] = {
                        [TOKEN_TYPE.MADE] = AST_TYPE.AST_EXPRESSION,
                    },
                },
            },
            [TOKEN_TYPE.NUMBER] = {
                [TOKEN_TYPE.OPERATION_NUMBER] = {
                    [ TOKEN_TYPE.NAME] = {
                        [TOKEN_TYPE.MADE] = AST_TYPE.AST_EXPRESSION_NUMBER,
                    },
                    [ TOKEN_TYPE.NUMBER] = {
                        [TOKEN_TYPE.MADE] = AST_TYPE.AST_EXPRESSION_NUMBER,
                    },
                },
                [TOKEN_TYPE.OPERATION] = {
                    [ TOKEN_TYPE.NAME] = {
                        [TOKEN_TYPE.MADE] = AST_TYPE.AST_EXPRESSION,
                    },
                    [ TOKEN_TYPE.STRING] = {
                        [TOKEN_TYPE.MADE] = AST_TYPE.AST_EXPRESSION,
                    },
                    [ TOKEN_TYPE.NUMBER] = {
                        [TOKEN_TYPE.MADE] = AST_TYPE.AST_EXPRESSION,
                    },
                },
            },
            [TOKEN_TYPE.BOOL] = {
                [ TOKEN_TYPE.AND] = {
                    [ TOKEN_TYPE.NAME] = {
                        [TOKEN_TYPE.MADE] = AST_TYPE.AST_EXPRESSION_LOGIC,
                    },
                    [ TOKEN_TYPE.BOOL] = {
                        [TOKEN_TYPE.MADE] = AST_TYPE.AST_EXPRESSION_LOGIC,
                    },
                },
                [ TOKEN_TYPE.OR] = {
                    [ TOKEN_TYPE.NAME] = {
                        [TOKEN_TYPE.MADE] = AST_TYPE.AST_EXPRESSION_LOGIC,
                    },
                    [ TOKEN_TYPE.BOOL] = {
                        [TOKEN_TYPE.MADE] = AST_TYPE.AST_EXPRESSION_LOGIC,
                    },
                },
                [ TOKEN_TYPE.NOT] = {
                    [TOKEN_TYPE.MADE] = AST_TYPE.AST_EXPRESSION_LOGIC,
                },
            },
        },
    },
    [TOKEN_TYPE.TARGET] = {
        [TOKEN_TYPE.NAME] = {
            [TOKEN_TYPE.OUTPUT] = AST_TYPE.AST_OPERATE,
            [TOKEN_TYPE.INPUT] = AST_TYPE.AST_OPERATE,
        },
        [TOKEN_TYPE.STRING] = {
            [TOKEN_TYPE.OUTPUT] = AST_TYPE.AST_OPERATE,
        },
        [TOKEN_TYPE.NUMBER] = {
            [TOKEN_TYPE.OUTPUT] = AST_TYPE.AST_OPERATE,
        },
        [TOKEN_TYPE.BOOL] = {
            [TOKEN_TYPE.OUTPUT] = AST_TYPE.AST_OPERATE,
        },
        [TOKEN_TYPE.EMPTY] = {
            [TOKEN_TYPE.OUTPUT] = AST_TYPE.AST_OPERATE,
        },
    },
}

local function str(value)
    local res = ""
    if type(value) ~= "table" then
        res = tostring(value)
    elseif #value == table.count(value) then
        for i,v in ipairs(value) do
            res = res .. v
            if i ~= #value then
                res = res .. ","
            end
        end
    else
        res = res .. "{"
        for k,v in pairs(value) do
            res = res .. k .. "=" .. tostring(v) .. ","
        end
        res = res .. "}"
    end
    return res
end

-- tokenizer
local tokenizer = {}

function tokenizer:tokenize(input, line, path)
    self.input = input .. " "
    self.line = line or 1
    self.path = path or "ikran"
    self.length = #self.input
    self.tokens = {}
    self.index = 0
    self.column = 0
    self.state = STATE.NEW
    self.value = ""
    for i=1,self.length do
        self:process(i)
    end
    return self.tokens
end

function tokenizer:process(i)
    --
    self.index = i
    self.column = self.column + 1
    -- get sign type
    local char = string.sub(self.input, self.index, self.index)
    local signType = nil
    for _,tp in pairs(ORDER) do
        local exp = EXPRESSIONS[tp]
        if not signType then
            if string.match(char, exp) then
                signType = tp
            end
        end
    end
    -- invalid sign
    if not signType
    or (signType == SIGNS.OTHER and self.state ~= STATE.COMMENT) and (signType == SIGNS.OTHER and self.state ~= STATE.STRING) then
        self:assert(false, "nime bu yazghining, taza bilelmidimghu")
    end
    -- get new state
    local machineMap = TOKENIZER_STATE_MAP[self.state]
    self:assert(machineMap ~= nil, string.format("xalet nami [%s] inawatsiz", self.state))
    local newState = machineMap[signType]
    self:assert(newState ~= nil, string.format("belge tipi [%s] inawatsiz", signType))
    local lastState = self.state
    self.state = newState
    self:assert(self.state ~= STATE.ERROR, string.format('xata xalet kuruldi'))
    -- handle current info
    if self.state == STATE.NEW then
        -- ignore
    elseif self.state == STATE.COMMENT then
        --ignore
    elseif self.state == STATE.NUMBER then
        self.value = self.value .. char
    elseif self.state == STATE.LETTER then
        self.value = self.value .. char
    elseif self.state == STATE.OPEN then
        --ignore
    elseif self.state == STATE.STRING then
        self.value = self.value .. char
    elseif self.state == STATE.CLOSE then
        self:save(lastState)
    elseif self.state == STATE.END then
        self:save(lastState)
    else
        assert(false, string.format("xalet [%s] inawetsiz", self.state))
    end
end

function tokenizer:save(lastState)
    local token = nil
    local value = self.value
    if lastState == STATE.NUMBER then
        token = self:token(TOKEN_TYPE.NUMBER, value)
    elseif lastState == STATE.LETTER then
        if TOKEN_TYPE_MAP[value] then
            token = self:token(TOKEN_TYPE_MAP[value], value)
        else
            token = self:token(TOKEN_TYPE.NAME, value)
        end
    elseif lastState == STATE.STRING then
        token = self:token(TOKEN_TYPE.STRING, value)
    elseif lastState == STATE.OPEN then
        token = self:token(TOKEN_TYPE.STRING, value) -- empty string
    end
    assert(token ~= nil)
    table.insert(self.tokens, token)
    self.value = ""
    self.state = STATE.NEW
end

function tokenizer:token(tp, value)
    return {
        isToken = true,
        type = tp,
        value = value,
        path = self.path,
        line = self.line,
        column = self.column - #value,
    }
end

function tokenizer:assert(v, msg)
    local char = string.sub(self.input, self.index, self.index)
    assert(v == true, string.format("%s: xojjet:[%s], qur:[%d], qatar:[%d], belge:[%s]", msg, self.path, self.line, self.column, char))
end


-- parser
local parser = {}

function parser:init()
    self.tree = self:node(AST_TYPE.AST_PROGRAM)
    self.current = {self.tree}
end

function parser:parse(tokens, line, path)
    self.line = line or 1
    self.path = path or "ikran"
    self.tokens = tokens
    self.length = #self.tokens
    self.index = 1
    if #tokens == 0 then
        return
    end
    if not self.tree then
        self:init()
    end
    local node = self:consume()
    return self.tree, node
end

function parser:next(tp)
    self.index = self.index + 1
    if tp then self:check(tp) end
    return self.tokens[self.index]
end

function parser:last(tp)
    self.index = self.index - 1
    if tp then self:check(tp) end
    return self.tokens[self.index]
end

function parser:expect(tp)
    if tp then self:check(tp) end
    return self.tokens[self.index]
end

function parser:check(tp)
    local token = self.tokens[self.index]
    if not token then
        self:assert(false, string.format("keynidin sanliq melumat tipi [%s] umut qilindi emma tepilmidi", tp), self:last())
    end
    tp = type(tp) == "string" and {tp} or tp
    local is = self:is(tp)
    self:assert(is, string.format("sanliq melumat tipi [%s] umut qilindi emma tepilmidi", str(tp)))
end

function parser:is(tp)
    local token = self.tokens[self.index]
    if not token then
        return false, true
    end
    tp = type(tp) == "string" and {tp} or tp
    local isValid = false
    for i,v in ipairs(tp) do
        if v == token.type then
            isValid = true
        end
    end
    return isValid, false
end

function parser:consume()
    -- ast type
    local index = self.index
    local token = self.tokens[index]
    local tokenType = token.type
    local nextState = PARSER_STATE_MAP[tokenType]
    while type(nextState) == "table" do
        index = index + 1
        token = self.tokens[index]
        if token then
            tokenType = token.type
            nextState = nextState[tokenType]
        else
            tokenType = nil
            nextState = nil
        end
    end
    -- invalid
    if not tokenType then
        self:assert(false, "inawetsiz qur", self.tokens[1])
    elseif not nextState then
        self:assert(false, "inawetsiz sozluk", self.tokens[index])
    end
    -- consume
    local astType = nextState
    local astFunc = self["consume_" .. astType]
    assert(astFunc ~= nil, string.format("consume func for [%s] is unimplemented", astType))
    local astTokens = astFunc(self)
    local astNode = self:insert(astType, astTokens)
    -- surplus
    if self.index ~= self.length then
        self:assert(false, string.format("artuxche sozluk", self:next()))
    end
    return astNode
end

function parser:consume_AST_VARIABLE()
    self:expect(TOKEN_TYPE.VARIABLE)
    local name = self:next(TOKEN_TYPE.NAME)
    self:next(TOKEN_TYPE.VALUE)
    local arg = self:next(TOKEN_TYPES_VALUES)
    self:next(TOKEN_TYPE.MADE)
    return {name, arg}
end

function parser:consume_AST_EXPRESSION()
    local name = self:expect(TOKEN_TYPE.NAME)
    self:next(TOKEN_TYPE.VALUE)
    local arg1 = self:next(TOKEN_TYPES_VALUES)
    local exp = self:next(TOKEN_TYPE.OPERATION)
    local arg2 = self:next(TOKEN_TYPES_VALUES)
    self:next(TOKEN_TYPE.MADE)
    return {name, arg1, exp, arg2}
end

function parser:consume_AST_EXPRESSION_NUMBER()
    local name = self:expect(TOKEN_TYPE.NAME)
    self:next(TOKEN_TYPE.VALUE)
    local arg1 = self:next(TOKEN_TYPES_VALUES)
    local exp = self:next(TOKEN_TYPE.OPERATION_NUMBER)
    local arg2 = self:next(TOKEN_TYPES_VALUES)
    self:next(TOKEN_TYPE.MADE)
    return {name, arg1, exp, arg2}
end

function parser:consume_AST_EXPRESSION_LOGIC()
    local name = self:expect(TOKEN_TYPE.NAME)
    self:next(TOKEN_TYPE.VALUE)
    local arg1 = self:next(TOKEN_TYPES_LOGICS)
    local exp = self:next({TOKEN_TYPE.AND, TOKEN_TYPE.OR, TOKEN_TYPE.NOT})
    local arg2 = (exp.type == TOKEN_TYPE.AND or exp.type == TOKEN_TYPE.OR) and self:next(TOKEN_TYPES_LOGICS) or nil
    self:next(TOKEN_TYPE.MADE)
    return {name, arg1, exp, arg2}
end

function parser:consume_AST_OPERATE()
    local target = self:expect(TOKEN_TYPE.TARGET)
    local arg = self:next()
    local handle = nil
    if arg.type == TOKEN_TYPE.NAME then
        handle = self:next({TOKEN_TYPE.OUTPUT, TOKEN_TYPE.INPUT})
    else
        self:check(TOKEN_TYPES_VALUES)
        handle = self:next(TOKEN_TYPE.OUTPUT)
    end
    return {target, arg, handle}
end

function parser:consume_AST_IF()
    self:expect(TOKEN_TYPE.IF)
    local arg1 = self:next(TOKEN_TYPE.NAME)
    self:next(TOKEN_TYPE.VALUE)
    local arg2 = self:next(TOKEN_TYPES_VALUES)
    self:next({TOKEN_TYPE.CODE_START})
    return {arg1, arg2}
end

function parser:consume_AST_WHILE()
    self:expect(TOKEN_TYPE.WHILE)
    local arg1 = self:next(TOKEN_TYPE.NAME)
    self:next(TOKEN_TYPE.VALUE)
    local arg2 = self:next(TOKEN_TYPES_VALUES)
    self:next({TOKEN_TYPE.CODE_START})
    return {arg1, arg2}
end

function parser:consume_AST_RESULT()
    self:expect(TOKEN_TYPE.RESULT)
    local arg1 = self:next(TOKEN_TYPES_VALUES)
    self:next(TOKEN_TYPE.RETURN)
    return {arg1}
end

function parser:consume_AST_FUNC()
    self:expect(TOKEN_TYPE.FUNC)
    local funcName = self:next(TOKEN_TYPE.NAME)
    local tokens = {}
    table.insert(tokens, funcName)
    --
    local next = self:next()
    if next.type == TOKEN_TYPE.VARIABLE then
        next = self:next()
    end
    while next.type == TOKEN_TYPE.NAME do
        table.insert(tokens, next)
        next = self:next()
    end
    self:check(TOKEN_TYPE.CODE_START)
    return tokens
end

function parser:consume_AST_CALL()
    self:expect(TOKEN_TYPE.FUNC)
    local funcName = self:next(TOKEN_TYPE.NAME)
    local tokens = {}
    table.insert(tokens, funcName)
    --
    local next = self:next()
    if next.type == TOKEN_TYPE.WITH then
        next = self:next()
    end
    while self:is(TOKEN_TYPES_VALUES) do
        table.insert(tokens, next)
        next = self:next()
    end
    self:check(TOKEN_TYPE.CALL)
    next = self:next()
    if not next then
        self:last()
    else
        self:check(TOKEN_TYPE.FURTHER)
        self:next(TOKEN_TYPE.RESULT)
        local name = self:next(TOKEN_TYPE.NAME)
        self:next(TOKEN_TYPE.MADE)
        table.insert(tokens, name)
    end
    return tokens
end

function parser:consume_AST_END()
    self:expect(TOKEN_TYPE.CODE_END)
end

function parser:node(name, astTokens)
    return {
        name = name,
        children = {},
        tokens = astTokens,
    }
end

function parser:insert(name, astTokens)
    local node = self:node(name, astTokens)
    local current = self.current[#self.current]
    table.insert(current.children, node)
    --
    local lastToken = self:expect()
    assert(lastToken ~= nil)
    local lastType = lastToken.type
    if lastType == TOKEN_TYPE.CODE_START then
        table.insert(self.current, node)
    elseif lastType == TOKEN_TYPE.CODE_END then
        table.remove(self.current, #self.current)
    end
    --
    self:assert(#self.current > 0, "asasiy programmini tamamlash inawetsiz")
    return node
end

function parser:assert(v, msg, token)
    token = token or self.tokens[self.index]
    assert(v == true, string.format("%s: xojjet:[%s], qur:[%d], soz:[%s]", msg, self.path, self.line, token.value))
end


-- executer
local executer = {}

function executer:init()
    self.heap = {}
    self.scopeStack = {}
    self.callStack = {}
end

function executer:execute(tree, node)
    if not self.heap then
        self:init()
    end
    if node and not REPL_AST[node.name] then
        print("ug > ikran xalitide bu mashghulat qollanmidi, hojjet xalitide ishliting!")
        return
    end
    self:executeAst(tree or node)
end

function executer:getValue(token)
    if token.type == TOKEN_TYPE.NAME then
        local name = token.value
        local value = self.heap[name]
        self:assert(value ~= nil, token, string.format("mixtar texi iniqlanmighan"))
        return value
    elseif token.type == TOKEN_TYPE.STRING then
        return token.value
    elseif token.type == TOKEN_TYPE.NUMBER then
        return tonumber(token.value)
    elseif token.type == TOKEN_TYPE.BOOL then
        return token.value
    elseif token.type == TOKEN_TYPE.EMPTY then
        return token.value
    else
        self:assert(false, token, string.format("invalid token type [%s] for executer", token.type))
    end
end

function executer:setValue(nameToken, value, isDefine)
    local name = nameToken.value
    if not isDefine then
        self:assert(self.heap[name] ~= nil, nameToken, string.format("mixtar texi iniqlanmighan"))
    end
    self.heap[name] = value
end

function executer:executeAst(ast)
    -- print("=====================================================================EXECUTE")
    -- print("tree:", ast)
    -- table.print(ast)
    -- print("=======================================================================END")
    local name = ast.name
    local func = self["execute_" .. name]
    assert(func ~= nil, string.format("execute func for [%s] is unimplemented", name))
    func(self, ast)
end

function executer:execute_AST_PROGRAM(node)
    for i,v in ipairs(node.children) do
        self:executeAst(v)
    end
end

function executer:execute_AST_OPERATE(node)
    local token = node.tokens[2]
    local operation = node.tokens[3]
    if operation.type == TOKEN_TYPE.OUTPUT then
        print(self:getValue(token)) -- io.write
    elseif operation.type == TOKEN_TYPE.INPUT then
        local value = io.read()
        self.heap[token.value] = tonumber(value) or value
    end
end

function executer:execute_AST_VARIABLE(node)
    local value = self:getValue(node.tokens[2])
    self:setValue(node.tokens[1], value, true)
end

function executer:execute_AST_EXPRESSION(node)
    local resultToken = node.tokens[1]
    local leftToken = node.tokens[2]
    local operationToken = node.tokens[3]
    local rightToken = node.tokens[4]
    --
    local result = nil
    local leftVlaue = self:getValue(leftToken)
    local rightVlaue = self:getValue(rightToken)
    local operationType = node.tokens[3].type
    local operationValue = node.tokens[3].value
    --
    assert(operationType == TOKEN_TYPE.OPERATION)
    if operationValue == TOKEN_VALUES.EQUAL then
        result = leftVlaue == rightVlaue and TOKEN_VALUES.TRUE or TOKEN_VALUES.FALSE
    elseif operationValue == TOKEN_VALUES.CONCAT then
        result = tostring(leftVlaue) .. tostring(rightVlaue)
    end
    assert(result ~= nil, string.format("operation [%s] not implemented!", operationValue))
    --
    self:setValue(resultToken, result)
end

function executer:execute_AST_EXPRESSION_NUMBER(node)
    local resultToken = node.tokens[1]
    local leftToken = node.tokens[2]
    local operationToken = node.tokens[3]
    local rightToken = node.tokens[4]
    --
    local result = nil
    local leftVlaue = self:getValue(leftToken)
    local rightVlaue = self:getValue(rightToken)
    local operationType = node.tokens[3].type
    local operationValue = node.tokens[3].value
    --
    assert(operationType == TOKEN_TYPE.OPERATION_NUMBER)
    local valueError = "qimmiti [%s] bolghan mixtar ustide xisablash elip barghili bolmaydu"
    self:assert(type(leftVlaue) == "number", leftToken, string.format(valueError, tostring(leftVlaue)))
    self:assert(type(rightVlaue) == "number", rightToken, string.format(valueError, tostring(rightVlaue)))
    --
    if operationValue == TOKEN_VALUES.ADD then
        result = leftVlaue + rightVlaue
    elseif operationValue == TOKEN_VALUES.SUB then
        result = leftVlaue - rightVlaue
    elseif operationValue == TOKEN_VALUES.MUL then
        result = leftVlaue * rightVlaue
    elseif operationValue == TOKEN_VALUES.DIV then
        result = leftVlaue / rightVlaue
    elseif operationValue == TOKEN_VALUES.LESS then
        result = leftVlaue < rightVlaue and TOKEN_VALUES.TRUE or TOKEN_VALUES.FALSE
    elseif operationValue == TOKEN_VALUES.MORE then
        result = leftVlaue > rightVlaue and TOKEN_VALUES.TRUE or TOKEN_VALUES.FALSE
    end
    assert(result ~= nil, string.format("operation [%s] not implemented!", operationValue))
    --
    self:setValue(resultToken, result)
end

function executer:execute_AST_EXPRESSION_LOGIC(node)
    local resultToken = node.tokens[1]
    local leftToken = node.tokens[2]
    local operationToken = node.tokens[3]
    local rightToken = node.tokens[4]
    --
    local result = nil
    local leftVlaue = self:getValue(leftToken)
    local rightVlaue = rightToken and self:getValue(rightToken) or nil
    local operationType = node.tokens[3].type
    --
    if operationType == TOKEN_TYPE.NOT then
        result = not (leftVlaue == TOKEN_VALUES.TRUE)
    elseif operationType == TOKEN_TYPE.AND then
        result = leftVlaue == TOKEN_VALUES.TRUE and rightVlaue == TOKEN_VALUES.TRUE
    elseif operationType == TOKEN_TYPE.OR then
        result = leftVlaue == TOKEN_VALUES.TRUE or rightVlaue == TOKEN_VALUES.TRUE
    end
    result = result and  TOKEN_VALUES.TRUE or TOKEN_VALUES.FALSE
    --
    self:setValue(resultToken, result)
end

function executer:execute_AST_IF(node)
    local leftToken = node.tokens[1]
    local rightToken = node.tokens[2]
    --
    local leftVlaue = self:getValue(leftToken)
    local rightVlaue = self:getValue(rightToken)
    local isOk = leftVlaue == rightVlaue
    if not isOk then return end
    for i,v in ipairs(node.children) do
        if v.name == AST_TYPE.AST_END then
            return
        end
        self:executeAst(v)
    end
end

function executer:execute_AST_WHILE(node)
    local leftToken = node.tokens[1]
    local rightToken = node.tokens[2]
    --
    local function checkCondition()
        local leftVlaue = self:getValue(leftToken)
        local rightVlaue = self:getValue(rightToken)
        return leftVlaue == rightVlaue
    end
    while checkCondition() do
        for i,v in ipairs(node.children) do
            if v.name ~= AST_TYPE.AST_END then
                self:executeAst(v)
            end
        end
    end
end

function executer:assert(v, token, msg)
    assert(v == true, string.format("%s: xojjet:[%s], qur:[%d], qatar:[%d], soz:[%s]", msg, token.path, token.line, token.column, token.value))
end


-- compiler
local compiler = {}

function compiler:compile(tree)
    -- empty lines
    if not tree then return "empty ..." end
    -- 
    return "-- todo compiler ..."
end


local function runLine(line, lineNo, fileName, isExecute)
    local tokens = tokenizer:tokenize(line, lineNo, fileName)
    local tree, node = parser:parse(tokens, lineNo, fileName)
    return tree, node
end

local function runFile(fromPath, isCompile)
    -- read file
    local lines = {}
    local fromFile = io.open(fromPath, "r")
    assert(fromFile ~= nil, string.format("bu hojjet tepilmidi:[%s]", fromPath))
    io.input(fromFile)
    for line in io.lines() do
        table.insert(lines, line)
    end
    io.close(fromFile)
    io.input(io.stdin)
    -- check target
    local isCompile = isCompile == "TRUE" or isCompile == "true"
    -- get ast
    local tree = nil
    for lineNo,line in ipairs(lines) do
        tree, _ = runLine(line, lineNo, fromPath, not isCompile) or tree
    end
    -- execute
    if not isCompile then
        executer:execute(tree, nil)
    end
    -- compile
    if not isCompile then return end
    local toPath = fromPath .. ".lua"
    local compileTime = os.date("%Y-%m-%d %H:%M", os.time())
    local toFile = io.open(toPath, "w")
    io.output(toFile)
    io.write(string.format("-- compiled from [%s] at %s", fromPath, compileTime))
    io.write("\n\n")
    io.write(compiler:compile(tree))
    io.write("\n")
    io.close(toFile)
end

local function runInput()
    print("\n UyghurScript:\n")
    local status = 0
    while status == 0 do
        io.write("ug > ")
        local isOK, err = xpcall(function()
            local line = io.read()
            if not line then
                status = -1
            elseif line == "tamam" then
                status = -2
            elseif line then
                local _, line = runLine(line, nil, nil, true)
                 executer:execute(nil, line)
            end
        end, function(err)
            status = -3
            print("\nXataliq:", debug.traceback(err), "\n")
        end)
        if status == -1 then
            print("\n\n Tamam!\n")
        elseif status == -2 then
            print("\n Tamam!\n")
        end
    end
end

local function entry()
    if arg[1] then
        runFile(arg[1], arg[2])
    else
        runInput()
    end
end

entry()
