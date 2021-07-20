--[[
    main compiler program
]]

-- https://github.com/kompasim/pure-lua-tools.git
require "./pure-lua-tools/initialize"

local TOKEN_TYPE = {
    NAME = "NAME", -- variable
    STRING = "STRING", -- string
    NUMBER = "NUMBER", -- number
    BOOL = "BOOL", -- value
    EMPTY = "EMPTY", -- value
    BOX = "BOX",
    -- 
    DOT = "DOT",
    -- 
    SOMEVALUE = "SOMEVALUE",
    SOMETYPE = "SOMETYPE",
    STR = "STR",
    NUM = "NUM",
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
    ELSE_IF = "ELSE_IF",
    ELSE = "ELSE",
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

local TOKEN_TYPES_VALUES_ALL = {TOKEN_TYPE.NAME, TOKEN_TYPE.STRING, TOKEN_TYPE.NUMBER, TOKEN_TYPE.BOOL, TOKEN_TYPE.EMPTY, TOKEN_TYPE.BOX}
local TOKEN_TYPES_VALUES = {TOKEN_TYPE.NAME, TOKEN_TYPE.STRING, TOKEN_TYPE.NUMBER, TOKEN_TYPE.BOOL, TOKEN_TYPE.EMPTY}
local TOKEN_TYPES_STRING = {TOKEN_TYPE.NAME, TOKEN_TYPE.STRING}
local TOKEN_TYPES_NUMBER = {TOKEN_TYPE.NAME, TOKEN_TYPE.NUMBER}
local TOKEN_TYPES_LOGICS = {TOKEN_TYPE.NAME, TOKEN_TYPE.BOOL, TOKEN_TYPE.EMPTY}

local TOKEN_VALUES = {
    -- 
    IF = "eger",
    ELSE_IF = "egerde",
    ELSE = "bolmisa",
    -- 
    OUTPUT = "yezilsun",
    INPUT = "oqulsun",
    -- 
    EMPTY = "quruq",
    BOX = "sanduq",
    -- 
    DOT = ".",
    -- 
    SOMEVALUE = "qimmet",
    SOMETYPE = "tipi",
    FUNC = "fonkisiye",
    STR = "xet",
    NUM = "san",
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
    [TOKEN_VALUES.FUNC] = TOKEN_TYPE.FUNC,
    bilen = TOKEN_TYPE.WITH,
    ishlitilsun = TOKEN_TYPE.CALL,
    we = TOKEN_TYPE.FURTHER,
    -- if
    [TOKEN_VALUES.IF] = TOKEN_TYPE.IF,
    [TOKEN_VALUES.ELSE_IF] = TOKEN_TYPE.ELSE_IF,
    [TOKEN_VALUES.ELSE] = TOKEN_TYPE.ELSE,
    -- while
    nawada = TOKEN_TYPE.WHILE,
    -- io
    ikrangha = TOKEN_TYPE.TARGET,
    ikrandin = TOKEN_TYPE.TARGET,
    [TOKEN_VALUES.OUTPUT] = TOKEN_TYPE.OUTPUT,
    [TOKEN_VALUES.INPUT] = TOKEN_TYPE.INPUT,
    -- types
    [TOKEN_VALUES.EMPTY] = TOKEN_TYPE.EMPTY,
    [TOKEN_VALUES.BOX] = TOKEN_TYPE.BOX,
    -- 
    [TOKEN_VALUES.DOT] = TOKEN_TYPE.DOT,
    -- type
    [TOKEN_VALUES.SOMEVALUE] = TOKEN_TYPE.SOMEVALUE,
    [TOKEN_VALUES.SOMETYPE] = TOKEN_TYPE.SOMETYPE,
    [TOKEN_VALUES.STR] = TOKEN_TYPE.STR,
    [TOKEN_VALUES.NUM] = TOKEN_TYPE.NUM,
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
    DOT = "DOT",
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
    SIGNS.DOT,
    SIGNS.LETTER,
    SIGNS.OPEN,
    SIGNS.CLOSE,
    SIGNS.OTHER,
}

local EXPRESSIONS = {
    [SIGNS.LINE] = "\n",
    [SIGNS.EMPTY] = "%s",
    [SIGNS.DOT] = "%.",
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
        [SIGNS.DOT] = STATE.ERROR,
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
        [SIGNS.DOT] = STATE.COMMENT,
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
        [SIGNS.DOT] = STATE.NUMBER,
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
        [SIGNS.DOT] = STATE.STRING,
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
        [SIGNS.DOT] = STATE.STRING,
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
    AST_ASSIGN = "AST_ASSIGN",
    AST_TRANSFORM = "AST_TRANSFORM",
    AST_RESULT = "AST_RESULT",
    AST_FUNC = "AST_FUNC",
    AST_CALL = "AST_CALL",
    AST_IF = "AST_IF",
    AST_IF_FIRST = "AST_IF_FIRST",
    AST_IF_MIDDLE = "AST_IF_MIDDLE",
    AST_IF_LAST = "AST_IF_LAST",
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
                [TOKEN_TYPE.BOX] = {
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
                    [TOKEN_TYPE.CODE_START] = AST_TYPE.AST_IF_FIRST,
                },
                [TOKEN_TYPE.STRING] = {
                    [TOKEN_TYPE.CODE_START] = AST_TYPE.AST_IF_FIRST,
                },
                [TOKEN_TYPE.NUMBER] = {
                    [TOKEN_TYPE.CODE_START] = AST_TYPE.AST_IF_FIRST,
                },
                [TOKEN_TYPE.BOOL] = {
                    [TOKEN_TYPE.CODE_START] = AST_TYPE.AST_IF_FIRST,
                },
                [TOKEN_TYPE.EMPTY] = {
                    [TOKEN_TYPE.CODE_START] = AST_TYPE.AST_IF_FIRST,
                },
            }
        }
    },
    [TOKEN_TYPE.ELSE_IF] = {
        [TOKEN_TYPE.NAME] = {
            [TOKEN_TYPE.VALUE] = {
                [TOKEN_TYPE.NAME] = {
                    [TOKEN_TYPE.CODE_START] = AST_TYPE.AST_IF_MIDDLE,
                },
                [TOKEN_TYPE.STRING] = {
                    [TOKEN_TYPE.CODE_START] = AST_TYPE.AST_IF_MIDDLE,
                },
                [TOKEN_TYPE.NUMBER] = {
                    [TOKEN_TYPE.CODE_START] = AST_TYPE.AST_IF_MIDDLE,
                },
                [TOKEN_TYPE.BOOL] = {
                    [TOKEN_TYPE.CODE_START] = AST_TYPE.AST_IF_MIDDLE,
                },
                [TOKEN_TYPE.EMPTY] = {
                    [TOKEN_TYPE.CODE_START] = AST_TYPE.AST_IF_MIDDLE,
                },
            }
        }
    },
    [TOKEN_TYPE.ELSE] = AST_TYPE.AST_IF_LAST,
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
                [TOKEN_TYPE.MADE] = AST_TYPE.AST_ASSIGN,
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
                [TOKEN_TYPE.MADE] = AST_TYPE.AST_ASSIGN,
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
                [TOKEN_TYPE.MADE] = AST_TYPE.AST_ASSIGN,
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
                [TOKEN_TYPE.MADE] = AST_TYPE.AST_ASSIGN,
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
            [ TOKEN_TYPE.EMPTY] = {
                [TOKEN_TYPE.MADE] = AST_TYPE.AST_ASSIGN,
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
    [TOKEN_TYPE.SOMEVALUE] = {
        [TOKEN_TYPE.NAME] = {
            [TOKEN_TYPE.SOMETYPE] = {
                [TOKEN_TYPE.FUNC] = {
                    [TOKEN_TYPE.MADE] = AST_TYPE.AST_TRANSFORM,
                },
                [TOKEN_TYPE.STR] = {
                    [TOKEN_TYPE.MADE] = AST_TYPE.AST_TRANSFORM,
                },
                [TOKEN_TYPE.NUM] = {
                    [TOKEN_TYPE.MADE] = AST_TYPE.AST_TRANSFORM,
                },
            },
        },
    }
}

-- tools

local unpack = unpack or table.unpack

function table.count(this, countKeyType, countValueType)
    local totalCount = 0
    local keyCount = 0
    local valueCount = 0
    local k,v = next(this)
    while k do
        totalCount = totalCount + 1
        if countKeyType and type(k) == countKeyType then
            keyCount = keyCount + 1
        end
        if countValueType and type(v) == countValueType then
            valueCount = valueCount + 1
        end
        k, v = next(this, k)
    end
    return totalCount, keyCount, valueCount
end

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

local function split(this, separator, maxCount)
    local startIndex = 1
    local splitIndex = 1
    local splitArray = {}
    while true do
        local foundIndex, endIndex = string.find(this, separator, startIndex)
        if not foundIndex or (maxCount and #splitArray >= maxCount) then
            splitArray[splitIndex] = string.sub(this, startIndex, string.len(this))
            break
        end
        splitArray[splitIndex] = string.sub(this, startIndex, foundIndex - 1)
        startIndex = foundIndex + (endIndex - foundIndex + 1)
        splitIndex = splitIndex + 1
    end
    return splitArray
end

local function is_valid_str(v)
    return type(v) == "string" and #v > 0
end

-- bridge
local lua = {}

local function parse_lua_func(funcName)
    assert(is_valid_str(funcName), string.format("invalid lua func name [%s]", funcName))
    local names = split(funcName, "%.")
    local length = #names
    local index = 1
    local func = nil
    while index <= length do
        local name = names[index]
        assert(is_valid_str(name), string.format("invalid lua func name [%s]", name))
        func = func and func[name] or _G[name]
        assert(func ~= nil, string.format("invalid lua func name [%s]", name))
        index = index + 1
    end
    assert(func ~= nil, string.format("function [%s] is not found in lua!", funcName))
    return func
end

local function get_lua_func(value)
    local func = nil
    if type(value) == "string" then
        func = parse_lua_func(value)
    elseif type(value) == "function" then
        func = value
    end
    if not func then
        return
    end
    local luaFunc = {}
    luaFunc.isLuaFunction = true
    luaFunc.name = tostring(value)
    luaFunc.action = function(...)
        return func(...)
    end
    return luaFunc
end

local function set_lua_value(valueName, value)
    assert(is_valid_str(valueName), string.format("invalid lua func name [%s]", valueName))
    local names = split(valueName, "%.")
    local length = #names
    local index = 1
    local parent = _G
    while index <= length do
        local name = names[index]
        assert(is_valid_str(name), string.format("invalid lua func name [%s]", name))
        if index ~= length then
            parent[name] = parent[name] or {}
            parent = parent[name]
            assert(type(parent) == "table", "invalid lua table name")
        else
            parent[name] = value
        end
        index = index + 1
    end
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
    or (
        (signType == SIGNS.OTHER and self.state ~= STATE.COMMENT)
        and (signType == SIGNS.OTHER and self.state ~= STATE.STRING)
        and (signType == SIGNS.OTHER and self.state ~= STATE.OPEN)
    ) then
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
        if signType == SIGNS.DOT then
            self:assert(string.find(self.value, "%.") == nil, "sanliq qimmette peqat birla kesir chikiti bulishi kerek")
        end
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

function parser:pushCurrent(node)
    table.insert(self.current, node)
end

function parser:popCurrent()
    table.remove(self.current, #self.current)
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
    local tokens = astFunc(self)
    -- add
    local astNode = nil
    if tokens then
        astNode = self:insert(astType, tokens, node)
    else
        astNode = self.current[#self.current]
    end
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
    local arg = self:next(TOKEN_TYPES_VALUES_ALL)
    self:next(TOKEN_TYPE.MADE)
    return {name, arg}
end

function parser:consume_AST_ASSIGN()
    local name = self:expect(TOKEN_TYPE.NAME)
    self:next(TOKEN_TYPE.VALUE)
    local arg = self:next(TOKEN_TYPES_VALUES_ALL)
    self:next(TOKEN_TYPE.MADE)
    return {name, arg}
end

function parser:consume_AST_TRANSFORM()
    self:expect(TOKEN_TYPE.SOMEVALUE)
    local name = self:next(TOKEN_TYPE.NAME)
    self:next(TOKEN_TYPE.SOMETYPE)
    local arg = self:next({TOKEN_TYPE.FUNC, TOKEN_TYPE.STR, TOKEN_TYPE.NUM})
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
    local arg1 = self:next(TOKEN_TYPES_NUMBER)
    local exp = self:next(TOKEN_TYPE.OPERATION_NUMBER)
    local arg2 = self:next(TOKEN_TYPES_NUMBER)
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
        self:check(TOKEN_TYPES_VALUES_ALL)
        handle = self:next(TOKEN_TYPE.OUTPUT)
    end
    return {target, arg, handle}
end

-- 
function parser:consume_AST_IF_FIRST()
    local starting = self:expect(TOKEN_TYPE.IF)
    local arg1 = self:next(TOKEN_TYPE.NAME)
    self:next(TOKEN_TYPE.VALUE)
    local arg2 = self:next(TOKEN_TYPES_VALUES)
    self:next(TOKEN_TYPE.CODE_START)
    --
    local ifNode = self:node(AST_TYPE.AST_IF_FIRST, {arg1, arg2})
    self:insert(AST_TYPE.AST_IF, {}, ifNode)
end

function parser:consume_AST_IF_MIDDLE()
    self:expect( TOKEN_TYPE.ELSE_IF)
    local arg1 = self:next(TOKEN_TYPE.NAME)
    self:next(TOKEN_TYPE.VALUE)
    local arg2 = self:next(TOKEN_TYPES_VALUES)
    self:next(TOKEN_TYPE.CODE_START)
    self:insert(AST_TYPE.AST_IF_MIDDLE, {arg1, arg2})
end

function parser:consume_AST_IF_LAST()
    self:expect( TOKEN_TYPE.ELSE)
    self:insert(AST_TYPE.AST_IF_LAST, {})
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
    local arg1 = self:next(TOKEN_TYPES_VALUES_ALL)
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
    while self:is(TOKEN_TYPES_VALUES_ALL) do
        table.insert(tokens, next)
        next = self:next()
    end
    self:check(TOKEN_TYPE.CALL)
    next = self:next()
    if not next then
        self:last()
    else
        self:check(TOKEN_TYPE.FURTHER)
        local result = self:next(TOKEN_TYPE.RESULT)
        table.insert(tokens, result)
        local name = self:next(TOKEN_TYPE.NAME)
        table.insert(tokens, name)
        self:next(TOKEN_TYPE.MADE)
    end
    return tokens
end

function parser:consume_AST_END()
    self:expect(TOKEN_TYPE.CODE_END)
    return {}
end

function parser:node(name, astTokens, child)
    return {
        name = name,
        children = {child},
        tokens = astTokens or {},
    }
end

function parser:insert(name, astTokens, child)
    local node = self:node(name, astTokens, child)
    -- 
    local current = self.current[#self.current]
    if name ~= AST_TYPE.AST_END then
        table.insert(current.children, node)
    end
    --
    local lastToken = self:expect()
    assert(lastToken ~= nil)
    local lastType = lastToken.type
    if lastType == TOKEN_TYPE.CODE_START and current.name ~= AST_TYPE.AST_IF then
        self:pushCurrent(node)
    elseif lastType == TOKEN_TYPE.CODE_END then
        self:popCurrent()
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
    self.callStack = {}
    self.scopeStack = {}
    self.currentScope = nil
    self:newScope()
end

function executer:execute(tree, node)
    if not self.currentScope then
        self:init()
    end
    if node and not REPL_AST[node.name] then
        print("ug > ikran xalitide bu mashghulat qollanmidi, hojjet xalitide ishliting!")
        return
    end
    self:executeAst(tree or node)
end

function executer:newScope()
    table.insert(self.scopeStack, {})
    self.currentScope = self.scopeStack[#self.scopeStack]
end

function executer:newBox()
    local box = {}
    box.isUyghurBox = true
    box.values = {}
    box.get = function(k)
        return box.values[k] or TOKEN_TYPE.EMPTY
    end
    box.set = function(k, v)
        box.values[k] = v
    end
    return box
end

function executer:exitScope()
    table.remove(self.scopeStack, #self.scopeStack)
    self.currentScope = self.scopeStack[#self.scopeStack]
    assert(self.currentScope ~= nil, "exiting from global scope")
end

function executer:findScope(token, isInCurrentScope)
    assert(token.type == TOKEN_TYPE.NAME, "invalid token for find scope")
    local name = token.value
    local scope = nil
    local value = nil
    local scopeIndex = #self.scopeStack
    while not value and scopeIndex > 0 do
        scope = self.scopeStack[scopeIndex]
        value = scope[name]
        if isInCurrentScope then
            break
        end
        scopeIndex = scopeIndex - 1
    end
    return scope, value
end

function executer:getValue(token, isGetLuaValue, ignoreError)
    if token.type == TOKEN_TYPE.NAME then
        local value = nil
        if isGetLuaValue and lua[token.value] then
            value = lua[token.value]
        end
        if not value then 
            _, value = self:findScope(token)
        end
        if value then
            return value
        elseif ignoreError then
            return TOKEN_VALUES.EMPTY
        else
            self:assert(false, token, string.format("mixtar texi iniqlanmighan"))
        end
    elseif token.type == TOKEN_TYPE.STRING then
        return token.value
    elseif token.type == TOKEN_TYPE.NUMBER then
        return tonumber(token.value)
    elseif token.type == TOKEN_TYPE.BOOL then
        if isGetLuaValue then
            return token.value == TOKEN_VALUES.TRUE
        else
            return token.value
        end
    elseif token.type == TOKEN_TYPE.EMPTY then
        if isGetLuaValue then
            return nil
        else
            return token.value
        end
    elseif token.type == TOKEN_TYPE.BOX then
        return self:newBox()
    else
        self:assert(false, token, string.format("invalid token type [%s] for executer", token.type))
    end
end

function executer:setValue(nameToken, value, isDefine)
    local name = nameToken.value
    local scope, oldValue = self:findScope(nameToken, isDefine)
    if isDefine then
        self:assert(oldValue == nil, nameToken, string.format("mixtar alliburun iniqlanghan"))
        self.currentScope[name] = value
    else
        self:assert(oldValue ~= nil, nameToken, string.format("mixtar texi iniqlanmighan"))
        scope[name] = value
    end
end

function executer:isStr(value)
    return type(value) == "string"
end

function executer:isNum(value)
    return type(value) == "number"
end

function executer:isFunc(value)
    if type(value) ~= "table" then return false end
    if value.isUyghurFunction then
        return true
    end
    if value.isLuaFunction then
        return true
    end
end

function executer:isBox(value)
    if type(value) ~= "table" then return false end
    if value.isUyghurBox then
        return true
    end
end

function executer:executeAst(ast)
    if not ast then return end
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
        local value = self:getValue(token)
        if self:isFunc(value) then
            value = string.format("[%s %s]", TOKEN_VALUES.FUNC, split(tostring(value), ":")[2])
        elseif self:isBox(value) then
            value = string.format("[%s %s]", TOKEN_VALUES.BOX, split(tostring(value), ":")[2])
        end
        print(value) -- io.write
    elseif operation.type == TOKEN_TYPE.INPUT then
        local value = io.read()
        self.currentScope[token.value] = tonumber(value) or value
    end
end

function executer:execute_AST_VARIABLE(node)
    local value = self:getValue(node.tokens[2])
    self:setValue(node.tokens[1], value, true)
end

function executer:execute_AST_ASSIGN(node)
    local value = self:getValue(node.tokens[2])
    self:setValue(node.tokens[1], value, false)
end

function executer:execute_AST_TRANSFORM(node)
    local nameToken = node.tokens[1]
    local typeToken = node.tokens[2]
    local value = self:getValue(nameToken)
    if typeToken.type == TOKEN_TYPE.STR then
        value = tostring(value)
    elseif typeToken.type == TOKEN_TYPE.NUM then
        if self:isStr(value) then
            value = tonumber(value)
        end
        if not self:isNum(value) then
            value = TOKEN_VALUES.EMPTY
        end
    elseif typeToken.type == TOKEN_TYPE.FUNC then
        if self:isStr(value) then
            local oldValue = nameToken.value
            nameToken.value = value
            value = self:getValue(nameToken, true, true)
            nameToken.value = oldValue
        end
        if not self:isFunc(value) then
            value = TOKEN_VALUES.EMPTY
        end
    else
        assert(false, string.format("type transform is not implemented for type [%s]", typeToken.type))
    end
    self:setValue(nameToken, value, false)
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
    local index = 1
    local length = #node.children
    local active = false
    while index <= length do
        local leaf = node.children[index]
        if leaf.name == AST_TYPE.AST_IF_FIRST or leaf.name == AST_TYPE.AST_IF_MIDDLE then
            if active then
                break
            end
            local leftToken = leaf.tokens[1]
            local rightToken = leaf.tokens[2]
            local leftVlaue = self:getValue(leftToken)
            local rightVlaue = self:getValue(rightToken)
            active = leftVlaue == rightVlaue
        elseif leaf.name == AST_TYPE.AST_IF_LAST then
            if active then
                break
            end
            active = true
        else
            if active then
                self:executeAst(leaf)
            end
        end
        index = index + 1
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
            self:executeAst(v)
        end
    end
end

function executer:execute_AST_FUNC(node)
    local nameToken = node.tokens[1]
    local funcName = nameToken.value
    assert(nameToken.type == TOKEN_TYPE.NAME)
    local _, oldValue = self:findScope(nameToken)
    self:assert(oldValue == nil, nameToken, "fonkisiye ismi ishlitilip bulunghan")
    local uyghurFunc = {}
    uyghurFunc.isUyghurFunction = true
    uyghurFunc.name = nameToken.value
    uyghurFunc.action = function()
        return node
    end
    self.currentScope[funcName] = uyghurFunc
end

function executer:execute_AST_CALL(node)
    local nameToken = node.tokens[1]
    local funcName = nameToken.value
    -- 
    local func = self:getValue(nameToken, true)
    self:assert(self:isFunc(func), nameToken, "bu mixtar fonkisiye emes")
    local isLuaFunc = is_table(func) and func.isLuaFunction == true
    -- push args
    local resultToken = nil
    self.callStack = {}
    for i,v in ipairs(node.tokens) do
        if v.type == TOKEN_TYPE.RESULT then
            resultToken = node.tokens[i + 1]
            assert(resultToken ~= nil)
            break
        elseif i == 1 then
            table.insert(self.callStack, func)
        else
            local value = self:getValue(v, isLuaFunc)
            table.insert(self.callStack, value)
        end
    end
    -- run func
    if isLuaFunc then
        self:runLuaFunc()
    else
        self:runUyghurFunc()
    end
    -- get result
    if resultToken then
        local resultValue = table.remove(self.callStack, 1)
        self:assert(resultValue ~= nil and resultValue ~= nameToken, resultToken, "fonkisiye qimmat qayturmidi")
        self:setValue(resultToken, resultValue)
    end
    self.callStack = {}
end

function executer:runLuaFunc()
    local funcTable = table.remove(self.callStack, 1)
    local result = {funcTable.action(unpack(self.callStack))}
    self.callStack = result
end

function executer:runUyghurFunc()
    -- create local scope
    self:newScope()
    -- read func
    local funcTable = table.remove(self.callStack, 1)
    local funcAst = funcTable.action()
    -- read args
    for i,v in ipairs(funcAst.tokens) do
        if i == 1 then
            assert(v.type == TOKEN_TYPE.NAME)
        else
            assert(v.type == TOKEN_TYPE.NAME)
            local value = table.remove(self.callStack, 1) or TOKEN_VALUES.EMPTY
            self:setValue(v, value, true)
        end
    end
    self.callStack = {}
    -- run body
    for i,v in ipairs(funcAst.children) do
        self:executeAst(v)
    end
    -- exit local scope
    self:exitScope()
end

function executer:callUyghurFunc(funcName, ...)
    -- create call stack
    local nameToken = tokenizer:token(TOKEN_TYPE.NAME, funcName)
    self.callStack = {}
    table.insert(self.callStack, nameToken)
    for i,v in ipairs({...}) do
        table.insert(self.callStack, v)
    end
    -- run func
    self:runUyghurFunc()
    -- get result
    local result = self.callStack
    self.callStack = {}
    return unpack(result)
end

function executer:execute_AST_RESULT(node)
    assert(node.name == AST_TYPE.AST_RESULT)
    local resultToken = node.tokens[1]
    local resultValue = self:getValue(resultToken)
    table.insert(self.callStack, resultValue)
end

function executer:assert(v, token, msg)
    assert(v == true, string.format("%s: xojjet:[%s], qur:[%d], qatar:[%d], soz:[%s]", msg, token.path, token.line, token.column, token.value))
end


-- compiler
local compiler = {}

function compiler:compile(tree)
    if not tree then return "-- empty ..." end
    self.tree = tree
    self.code = ""
    self:compileAst(tree)
    return self.code
end

function compiler:write(code)
    code = code or ""
    self.code = self.code and self.code .. "\n" .. code or code
end

function compiler:convertValue(token)
    local value = token.value
    if token.type == TOKEN_TYPE.NAME then
        return tostring(value)
    elseif token.type == TOKEN_TYPE.STRING then
        return "[[" .. value .. "]]"
    elseif token.type == TOKEN_TYPE.NUMBER then
        return tostring(value)
    elseif token.type == TOKEN_TYPE.BOOL then
        return tostring(value == TOKEN_VALUES.TRUE)
    elseif token.type == TOKEN_TYPE.EMPTY then
        return "nil"
    else
        self:assert(false, token, string.format("invalid token type [%s] for compiler", token.type))
    end
end

function compiler:compileAst(ast)
    if not ast then return end
    local name = ast.name
    local func = self["compile_" .. name]
    assert(func ~= nil, string.format("compile func for [%s] is unimplemented", name))
    func(self, ast)
end

function compiler:compile_AST_PROGRAM(node)
    for i,v in ipairs(node.children) do
       self:compileAst(v)
    end
end

function compiler:compile_AST_OPERATE(node)
    local token = node.tokens[2]
    local operation = node.tokens[3]
    if operation.type == TOKEN_TYPE.OUTPUT then
        self:write("print(" .. self:convertValue(token) .. ")")
    elseif operation.type == TOKEN_TYPE.INPUT then
        self:write(token.value .. " = io.read()")
    end
end

function compiler:compile_AST_VARIABLE(node)
    self:write("local " .. node.tokens[1].value .. " = " .. self:convertValue(node.tokens[2]))
end

function compiler:compile_AST_ASSIGN(node)
    self:write(node.tokens[1].value .. " = " .. self:convertValue(node.tokens[2]))
end

function compiler:compile_AST_EXPRESSION(node)
    local resultToken = node.tokens[1]
    local leftToken = node.tokens[2]
    local operationToken = node.tokens[3]
    local rightToken = node.tokens[4]
    --
    local resultValue = resultToken.value
    local leftVlaue = self:convertValue(leftToken)
    local rightVlaue = self:convertValue(rightToken)
    local operationType = node.tokens[3].type
    local operationValue = node.tokens[3].value
    --
    assert(operationType == TOKEN_TYPE.OPERATION)
    if operationValue == TOKEN_VALUES.EQUAL then
        self:write(resultValue .. " = " .. leftVlaue .. " == " .. rightVlaue)
    elseif operationValue == TOKEN_VALUES.CONCAT then
        self:write(resultValue .. " = tostring(" .. leftVlaue .. ") .. " .. "tostring(" .. rightVlaue .. ")")
    end
end

function compiler:compile_AST_EXPRESSION_NUMBER(node)
    local resultToken = node.tokens[1]
    local leftToken = node.tokens[2]
    local operationToken = node.tokens[3]
    local rightToken = node.tokens[4]
    --
    local resultValue = resultToken.value
    local leftVlaue = self:convertValue(leftToken)
    local rightVlaue = self:convertValue(rightToken)
    local operationType = node.tokens[3].type
    local operationValue = node.tokens[3].value
    --
    assert(operationType == TOKEN_TYPE.OPERATION_NUMBER)
    if operationValue == TOKEN_VALUES.ADD then
        self:write(resultValue .. " = " .. leftVlaue .. " + " .. rightVlaue)
    elseif operationValue == TOKEN_VALUES.SUB then
        self:write(resultValue .. " = " .. leftVlaue .. " - " .. rightVlaue)
    elseif operationValue == TOKEN_VALUES.MUL then
        self:write(resultValue .. " = " .. leftVlaue .. " * " .. rightVlaue)
    elseif operationValue == TOKEN_VALUES.DIV then
        self:write(resultValue .. " = " .. leftVlaue .. " / " .. rightVlaue)
    elseif operationValue == TOKEN_VALUES.LESS then
        self:write(resultValue .. " = " .. leftVlaue .. " < " .. rightVlaue)
    elseif operationValue == TOKEN_VALUES.MORE then
        self:write(resultValue .. " = " .. leftVlaue .. " > " .. rightVlaue)
    end
end

function compiler:compile_AST_EXPRESSION_LOGIC(node)
    local resultToken = node.tokens[1]
    local leftToken = node.tokens[2]
    local operationToken = node.tokens[3]
    local rightToken = node.tokens[4]
    --
    local resultValue = resultToken.value
    local leftVlaue = self:convertValue(leftToken)
    local rightVlaue = rightToken and self:convertValue(rightToken) or "nil"
    local operationType = node.tokens[3].type
    --
    if operationType == TOKEN_TYPE.NOT then
        self:write(resultValue .. " = not " .. leftVlaue)
    elseif operationType == TOKEN_TYPE.AND then
        self:write(resultValue .. " = " .. leftVlaue .. " == true and " .. rightVlaue .. " == true")
    elseif operationType == TOKEN_TYPE.OR then
        self:write(resultValue .. " = " .. leftVlaue .. " == true or " .. rightVlaue .. " == true")
    end
end

function compiler:compile_AST_IF(node)
    self:write()
    local index = 1
    local length = #node.children
    local active = false
    while index <= length do
        local leaf = node.children[index]
        if leaf.name == AST_TYPE.AST_IF_FIRST then
            local leftToken = leaf.tokens[1]
            local rightToken = leaf.tokens[2]
            local leftVlaue = self:convertValue(leftToken)
            local rightVlaue = self:convertValue(rightToken)
            self:write("if " .. leftVlaue .. " == " .. rightVlaue .. " then")
        elseif leaf.name == AST_TYPE.AST_IF_MIDDLE then
            local leftToken = leaf.tokens[1]
            local rightToken = leaf.tokens[2]
            local leftVlaue = self:convertValue(leftToken)
            local rightVlaue = self:convertValue(rightToken)
            self:write("elseif " .. leftVlaue .. " == " .. rightVlaue .. " then")
        elseif leaf.name == AST_TYPE.AST_IF_LAST then
            self:write("else")
        else
            self:compileAst(leaf)
        end
        index = index + 1
    end
    self:write("end")
    self:write()
end

function compiler:compile_AST_WHILE(node)
    local leftToken = node.tokens[1]
    local rightToken = node.tokens[2]
    local leftVlaue = self:convertValue(leftToken)
    local rightVlaue = self:convertValue(rightToken)
    self:write()
    self:write("while " .. leftVlaue .. " == " .. rightVlaue .. " do")
    for i,v in ipairs(node.children) do
       self:compileAst(v)
    end
    self:write("end")
    self:write()
end

function compiler:compile_AST_FUNC(node)
    local funcName = nil
    local variables = ""
    local length = #node.tokens
    for i=1,length do
        local v = node.tokens[i]
        if i == 1 then
            funcName = v.value
        else
            variables = variables .. v.value
            if i ~= length then
                variables = variables .. ", "
            end
        end
    end
    self:write()
    self:write("local function " .. funcName .. "(" .. variables .. ")")
    for i,v in ipairs(node.children) do
        self:compileAst(v)
    end
    self:write("end")
    self:write()
end

function compiler:compile_AST_CALL(node)
    local funcName = nil
    local resultName = nil
    local variables = ""
    local length = #node.tokens
    for i = 1, length do
        local v = node.tokens[i]
        if v.type == TOKEN_TYPE.RESULT then
            local resultToken = node.tokens[i + 1]
            assert(resultToken ~= nil)
            resultName = resultToken.value
            break
        elseif i == 1 then
            local nameToken = v
            funcName = nameToken.value
        else
            local variable = self:convertValue(v)
            variables = variables .. variable
            local nextToken = node.tokens[i + 1]
            if nextToken.type ~= TOKEN_TYPE.RESULT then
                variables = variables .. ", "
            end
        end
    end
    funcName = lua[funcName] or funcName
    if resultName then
        self:write(resultName .. " = " .. funcName .. "(" .. variables .. ")")
    else
        self:write(funcName .. "(" .. variables .. ")")
    end
end

function compiler:compile_AST_RESULT(node)
    assert(node.name == AST_TYPE.AST_RESULT)
    local resultToken = node.tokens[1]
    local resultValue = self:convertValue(resultToken)
    self:write("return " .. resultValue)
end

function compiler:assert(v, token, msg)
    assert(v == true, string.format("%s: xojjet:[%s], qur:[%d], qatar:[%d], soz:[%s]", msg, token.path, token.line, token.column, token.value))
end


-- program

local function importLua(uyghurValue, luaValue)
    assert(is_valid_str(uyghurValue), string.format("invalid uyghur func name [%s]", uyghurValue))
    assert(is_valid_str(luaValue), string.format("invalid lua func name [%s]", luaValue))
    local func = get_lua_func(luaValue)
    assert(func ~= nil, string.format("function [%s] is not found in lua!", luaValue))
    assert(lua[uyghurValue] == nil, string.format("function [%s] is already registered in uyghur!", uyghurValue))
    lua[uyghurValue] = func
end

local function exportLua(uyghurValue, luaValue)
    assert(is_valid_str(uyghurValue), string.format("invalid uyghur func name [%s]", uyghurValue))
    assert(is_valid_str(luaValue), string.format("invalid lua func name [%s]", luaValue))
    set_lua_value(luaValue, function(...)
        return executer:callUyghurFunc(uyghurValue, ...)
    end)
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
    local isCompile = isCompile == true or isCompile == "TRUE" or isCompile == "true"
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
    io.write("\n")
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

local uyghur = {
    version = "0.0.1",
    import = function(uyghurValue, luaValue)
        importLua(uyghurValue, luaValue)
    end,
    export = function(uyghurValue, luaValue)
        exportLua(uyghurValue, luaValue)
    end,
    execute = function(filePath)
        runFile(filePath, false)
    end,
    compile = function(filePath)
        runFile(filePath, true)
    end,
    run = function()
        runInput()
    end,
}

local args = {...}
if not args[1] then
    runInput()
elseif string.match(args[1], "%a+$") == "uyghur" then
    return uyghur
else
    runFile(args[1], args[2])
end
