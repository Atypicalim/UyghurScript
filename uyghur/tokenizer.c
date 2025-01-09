// tokenizer

#include "others/header.h"

void Tokenizer_reset(Tokenizer *this)
{
    this->line = 1;
    this->column = 1;
    this->head = NULL;
    this->tail = NULL;
    this->path = NULL;
    this->code = NULL;
}

Tokenizer *Tokenizer_new(Uyghur *uyghur)
{
    // init
    Tokenizer *tokenizer = malloc(sizeof(Tokenizer));
    tokenizer->uyghur = uyghur;
    // iters
    tokenizer->iterStatic = malloc(sizeof(UTF8Iter));
    tokenizer->iterDynamic = malloc(sizeof(UTF8Iter));
    // reset
    Tokenizer_reset(tokenizer);
    return tokenizer;
}

Token *Tokenizer_parseLetter(Tokenizer *this, String *letter, bool isGetName)
{
    // 
    CString _letter =  String_get(letter);
    String *trans = NULL;
    if (trans == NULL) trans = Hashmap_get(this->uyghur->lettersMap, _letter);
    if (trans == NULL) trans = Hashmap_get(this->uyghur->aliasesMap, _letter);
    if (trans != NULL) _letter = String_get(trans);
    // name 
    String *word = Hashmap_get(this->uyghur->wordsMap, _letter);
    if (word == NULL) {
        Token *token = Token_new(UG_TTYPE_NAM, _letter);
        token->extra = "extra...";
        return token;
    }
    // word
    CString _word = String_get(word);
    char *_type = is_eq_string(_letter, _word) ? UG_TTYPE_WRD : _word;
    Token *token = Token_new(_type, _letter);
    token->extra = "extra...";
    return token;
}

UTFCHAR Tokenizer_getchar(Tokenizer *this, int indent)
{
    this->iterDynamic->codepoint = this->iterStatic->codepoint;
    this->iterDynamic->size = this->iterStatic->size;
    this->iterDynamic->position = this->iterStatic->position;
    this->iterDynamic->next = this->iterStatic->next;
    this->iterDynamic->count = this->iterStatic->count;
    for (size_t i = 0; i < abs(indent); i++)
    {
        if (indent > 0) utf8iter_next(this->iterDynamic);
        if (indent < 0) utf8iter_previous(this->iterDynamic);
    }
    return utf8iter_getchar(this->iterDynamic);
}

bool Tokenizer_isChar(Tokenizer *this, int indent, UTFCHAR c) {
    UTFCHAR ch = Tokenizer_getchar(this, indent);
    return is_uchar_eq_uchar(ch, c);
}

// UTFCHAR Tokenizer_getValidChar(Tokenizer *, int)
UTFCHAR Tokenizer_getValidChar(Tokenizer *this, int indent)
{
    UTFCHAR c = Tokenizer_getchar(this, indent);
    if (is_space(c))
    {
        return Tokenizer_getValidChar(this, indent + (indent < 0 ? -1 : 1));
    }
    return c;
}

UTFCHAR Tokenizer_skipN(Tokenizer *this, int n)
{
    this->column = this->column + n;
    for (size_t i = 0; i < n; i++) {
        utf8iter_next(this->iterStatic);
    }
}

void Tokenizer_error(Tokenizer *this, char *msg) {
    UTFCHAR c = Tokenizer_getchar(this, 0);
    char *m = msg != NULL ? msg : LANG_ERR_TOKENIZER_EXCEPTION;
    char *s = tools_format(LANG_ERR_SIGN_PLACE, this->path, this->line, this->column, c);
    log_error("Tokenizer: %s, %s", m, s);
    exit(1);
}

void Tokenizer_assert(Tokenizer *this, bool value, char *msg)
{
    if (!value) Tokenizer_error(this, msg);
}

void Token_addToken(Tokenizer *this, Token *token) {
    log_debug("tokenizer.token: %s->%s", token->type, escape_cstring(token->value));
    Token_bindInfo(token, this->path, this->line, this->column);
    //
    if (this->head == NULL)
    {
        this->head = token;
        this->tail = token;
    }
    else
    {
        this->tail->next = token;
        token->last = this->tail;
        this->tail = token;
    }
}

String *Tokenizer_readLetter(Tokenizer *this) {
    String *str = String_new();
    UTFCHAR _c = Tokenizer_getchar(this, 0);
    UTFCHAR c = clone_uchar(_c);
    UTFCHAR _n = Tokenizer_getchar(this, 1);
    UTFCHAR n = clone_uchar(_n);
    //
    Tokenizer_assert(this, is_letter_begin(c, n), LANG_ERR_INVALID_LTTR);
    String_appendStr(str, c);
    //
    free_uchar(c);
    free_uchar(n);
    //
    Tokenizer_skipN(this, 1);
    c = Tokenizer_getchar(this, 0);
    //
    while (is_letter_body(c)) {
        String_appendStr(str, c);
        Tokenizer_skipN(this, 1);
        c = Tokenizer_getchar(this, 0);
    }
    return str;
}

int _tokenizer_readNumber(Tokenizer *this, String *str, UTFCHAR *c) {
    bool dotCount = false;
    Tokenizer_skipN(this, 1);
    *c = Tokenizer_getchar(this, 0);
    while (is_digit(*c) || is_uchar_eq_uchar(*c, SIGN_DOT)) {
        if (is_uchar_eq_uchar(*c, SIGN_DOT)) {
            dotCount++;
        }
        String_appendStr(str, *c);
        Tokenizer_skipN(this, 1);
        *c = Tokenizer_getchar(this, 0);
    }
    return dotCount;
}

String *Tokenizer_readNumber(Tokenizer *this) {
    String *str = String_new();
    bool isZero = Tokenizer_isChar(this, 0, "0");
    UTFCHAR n = Tokenizer_getchar(this, 1);
    // hex
    if (isZero && is_uchar_eq_uchar(n, "x")) {
        String_appendStr(str, "0x");
        Tokenizer_skipN(this, 2);
        UTFCHAR _c = Tokenizer_getchar(this, 0);
        while (isxdigit(_c[0])) {
            String_appendStr(str, _c);
            Tokenizer_skipN(this, 1);
            _c = Tokenizer_getchar(this, 0);
        }
        return str;
    }
    // bin
    if (isZero && is_uchar_eq_uchar(n, "b")) {
        String_appendStr(str, "0b");
        Tokenizer_skipN(this, 2);
        UTFCHAR _c = Tokenizer_getchar(this, 0);
        while (_c[0] == '0' || _c[0] == '1') {
            String_appendStr(str, _c);
            Tokenizer_skipN(this, 1);
            _c = Tokenizer_getchar(this, 0);
        }
        return str;
    }
    // 
    UTFCHAR c = Tokenizer_getchar(this, 0);
    if (is_uchar_eq_uchar(c, SIGN_ADD) || is_uchar_eq_uchar(c, SIGN_SUB)) {
        String_appendStr(str, c);
        Tokenizer_skipN(this, 1);
        c = Tokenizer_getchar(this, 0);
        Tokenizer_assert(this, is_digit(c), LANG_ERR_INVALID_NMBR);
        String_appendStr(str, c);
    } else if (is_digit(c)) {
        String_appendStr(str, SIGN_ADD);
        String_appendStr(str, c);
    } else {
        Tokenizer_error(this, LANG_ERR_INVALID_NMBR);
    }
    bool dotCount = _tokenizer_readNumber(this, str, &c);
    Tokenizer_assert(this, dotCount <= 1, LANG_ERR_INVALID_NMBR);
    return str;
}

String *Tokenizer_readNumKey(Tokenizer *this) {
    String *str = String_new();
    UTFCHAR c = Tokenizer_getchar(this, 0);
    //
    if (is_uchar_eq_uchar(c, SIGN_ADD) || is_uchar_eq_uchar(c, SIGN_SUB)) {
        Tokenizer_error(this, LANG_ERR_INVALID_NMBR);
    } else if (is_digit(c)) {
        String_appendStr(str, SIGN_ADD);
        String_appendStr(str, c);
    } else {
        Tokenizer_error(this, LANG_ERR_INVALID_NMBR);
    }
    //
    bool dotCount = _tokenizer_readNumber(this, str, &c);
    Tokenizer_assert(this, dotCount <= 0, LANG_ERR_CANNOT_BE_FLOAT);
    return str;
}

void _tokenizer_readString(Tokenizer *this, String *str, UTFCHAR *c) {
    if (is_uchar_eq_uchar(*c, SIGN_ESCAPE)) {
        Tokenizer_skipN(this, 1);
        UTFCHAR _escp = Tokenizer_getchar(this, 0);
        char escp = CHAR_QUESSION;
        if (strlen(_escp) == 1) {
            escp = decode_escape(_escp[0]);
        }
        String_appendChar(str, escp);
    } else {
        String_appendStr(str, *c);
    }
    Tokenizer_skipN(this, 1);
    *c = Tokenizer_getchar(this, 0);
}

String *Tokenizer_readString(Tokenizer *this) {
    String *str = String_new();
    UTFCHAR c = Tokenizer_getchar(this, 0);
    while (true) {
        if (!is_string_body(c)) {
            break;
        }
        _tokenizer_readString(this, str, &c);
    }
    return str;
}

String *Tokenizer_readStrKey(Tokenizer *this) {
    String *str = String_new();
    UTFCHAR c = Tokenizer_getchar(this, 0);
    while (true) {
        if (!is_string_body(c) || is_special(c) || is_eq_strings(c, 2, "(", ")")) {
            break;
        }
        _tokenizer_readString(this, str, &c);
    }
    return str;
}

Token *Tokenizer_parseCode(Tokenizer *this, const char *path, const char *code)
{
    Tokenizer_reset(this);
    this->path = path;
    this->code = code;
    UTFCHAR tempChar = NULL;
    UTFCHAR currChar = NULL;
    int numMapGenerator = 0;
    int numArrGenerator = 0;
    bool isGenerator = false;
    bool isCalculator = false;
    String *scopeObject = NULL;

    UTF8Iter ITER;
    utf8iter_init(this->iterStatic, code);
    utf8iter_init(this->iterDynamic, code);
    utf8iter_next(this->iterStatic);
    utf8iter_next(this->iterDynamic);
    while (this->iterStatic->next < this->iterStatic->length) {
        // 
        tempChar = Tokenizer_getchar(this, 0);
        if (!currChar) free_uchar(currChar);
        currChar = clone_uchar(tempChar);
        // 
        // log_debug(
        //     "tokenizer.next: [%i.%i] %u [%s]",
        //     this->iterStatic->length, this->iterStatic->position,
        //     this->iterStatic->codepoint, currChar
        // );
        // end
        if (is_empty(currChar)) {
            // log_debug("tokenizer.end");
            break;
        }
        // line
        if (is_line(currChar))
        {
            // log_debug("tokenizer.line");
            this->line++;
            this->column = 1;
            Tokenizer_skipN(this, 1);
            isCalculator = false;
            isGenerator = numMapGenerator > 0 || numArrGenerator > 0;
            continue;
        }
        // empty
        if (is_space(currChar) || is_cntrl(currChar))
        {
            // log_debug("tokenizer.empty");
            Tokenizer_skipN(this, 1);
            continue;
        }
        // comment
        if (is_uchar_eq_uchar(currChar, SIGN_LOCK))
        {
            Tokenizer_skipN(this, 1);
            UTFCHAR c = Tokenizer_getchar(this, 0);
            while(!is_line(c) && !is_empty(c)) {
                Tokenizer_skipN(this, 1);
                c = Tokenizer_getchar(this, 0);
            }
            continue;
        }
        // scope
        if (is_uchar_eq_uchar(currChar, SIGN_AT))
        {
            Tokenizer_skipN(this, 1);
            UTFCHAR c = Tokenizer_getchar(this, 0);
            if (is_scope(c)) {
                Tokenizer_skipN(this, 1);
                scopeObject = String_format("%s", c);
            } else {
                scopeObject = Tokenizer_readLetter(this);
            }
            UTFCHAR t = Tokenizer_getchar(this, 0);
            Tokenizer_assert(this, is_scope_open(t), LANG_ERR_TOKENIZER_INVALID_BOX);
            continue;
        }
        // key
        if (scopeObject != NULL) {
            Tokenizer_assert(this, is_scope_open(currChar), LANG_ERR_TOKENIZER_KEY_START_ERROR);
            UTFCHAR openChar = currChar;
            UTFCHAR closeChar = NULL;
            // not empty
            Tokenizer_skipN(this, 1);
            UTFCHAR c = Tokenizer_getchar(this, 0);
            // get key
            String *txt = NULL;
            char *typ = NULL;
            if (is_uchar_eq_uchar(openChar, TVALUE_DOT)) {
                if (is_digit(c)) {
                    txt = Tokenizer_readNumKey(this);
                    typ = UG_TTYPE_NUM;
                } else {
                    txt = Tokenizer_readStrKey(this);
                    typ = UG_TTYPE_STR;
                }
            } else if (is_uchar_eq_uchar(openChar, TVALUE_COLON)) {
                if (is_digit(c)) {
                    txt = Tokenizer_readNumKey(this);
                    typ = UG_TTYPE_NUM;
                } else {
                    txt = Tokenizer_readStrKey(this);
                    typ = UG_TTYPE_STR;
                }
            } else if (is_uchar_eq_uchar(openChar, SIGN_OPEN_MIDDLE)) {
                closeChar = SIGN_CLOSE_MIDDLE;
                txt = Tokenizer_readLetter(this);
                Token *_tkn = Tokenizer_parseLetter(this, txt, false);
                Tokenizer_assert(this, _tkn == NULL || Token_isName(_tkn), LANG_ERR_TOKENIZER_INVALID_KEY);
                typ = UG_TTYPE_NAM;
            }
            // normal close
            UTFCHAR temp = Tokenizer_getchar(this, 0);
            if (closeChar) {
                bool isClosed = Tokenizer_isChar(this, 0, closeChar);
                Tokenizer_assert(this, isClosed, LANG_ERR_TOKENIZER_KEY_END_ERROR);
                Tokenizer_skipN(this, 1);
            }
            // scoped key
            Tokenizer_assert(this, is_eq_strings(typ, TTYPES_GROUP_KEYS), LANG_ERR_INVALID_TYPE);
            Tokenizer_assert(this, scopeObject != NULL, NULL);
            //
            CString _keyText = String_get(txt);
            if (is_eq_string(typ, UG_TTYPE_STR)) {
                String *trans = Hashmap_get(this->uyghur->aliasesMap, _keyText);
                if (trans != NULL) {
                    _keyText = String_get(trans);
                }
            }
            //
            Token *_parsedScope = Tokenizer_parseLetter(this, scopeObject, false);
            Token *token = Token_key(typ, _keyText, _parsedScope->value);
            scopeObject = NULL;
            // insert token
            Token_addToken(this, token);
            continue;
        }
        // string
        if (is_string_open(currChar))
        {
            Tokenizer_skipN(this, 1);
            //
            String *str = Tokenizer_readString(this);
            Token *tkn = Token_new(UG_TTYPE_STR, String_get(str));
            Token_addToken(this, tkn);
            // 
            UTFCHAR c = Tokenizer_getchar(this, 0);
            Tokenizer_assert(this, is_string_close(c), LANG_ERR_TOKENIZER_STRING_END_ERROR);
            Tokenizer_skipN(this, 1);
            continue;
        }
        // number
        if (is_number_begin(currChar, Tokenizer_getchar(this, 1)))
        {
            String *num = Tokenizer_readNumber(this);
            Token *tkn = Token_new(UG_TTYPE_NUM, String_get(num));
            Token_addToken(this, tkn);
            continue; 
        }
        // letter
        if (is_letter_begin(currChar, Tokenizer_getchar(this, 1)))
        {
            String *ltr = Tokenizer_readLetter(this);
            Token *tkn = Tokenizer_parseLetter(this, ltr, true);
            Token_addToken(this, tkn);
            continue; 
        }
        // equaling
        if (is_equaling(currChar))
        {
            Token *tkn = Token_new(UG_TTYPE_WRD, TVALUE_EQUALING);
            Token_addToken(this, tkn);
            UTFCHAR c = Tokenizer_getValidChar(this, 1);
            Tokenizer_skipN(this, 1);
            if (is_uchar_eq_uchar(c, SIGN_OPEN_MIDDLE) || is_uchar_eq_uchar(c, SIGN_OPEN_BIG)) {
                isGenerator = true;
            } else {
                isCalculator = true;
            }
            continue;
        }
        // generation
        bool consumeGenerator = false;
        if (isGenerator && is_uchar_eq_uchar(currChar, SIGN_OPEN_MIDDLE)) {
            numMapGenerator++;
            consumeGenerator = true;
        } else if (isGenerator && is_uchar_eq_uchar(currChar, SIGN_OPEN_BIG)) {
            numArrGenerator++;
            consumeGenerator = true;
        }
        if (isGenerator && is_uchar_eq_uchar(currChar, SIGN_CLOSE_MIDDLE)) {
            numMapGenerator--;
            consumeGenerator = true;
        } else if (isGenerator && is_uchar_eq_uchar(currChar, SIGN_CLOSE_BIG)) {
            numArrGenerator--;
            consumeGenerator = true;
        }
        if (isGenerator && (is_uchar_eq_uchar(currChar, TVALUE_COLON) || is_uchar_eq_uchar(currChar, TVALUE_COMMA))) {
            consumeGenerator = true;
        }
        if (consumeGenerator) {
            Token *tkn = Token_new(UG_TTYPE_WRD, currChar);
            Token_addToken(this, tkn);
            Tokenizer_skipN(this, 1);
            continue;
        }
        // calculation
        if (isCalculator && is_calculation_char(currChar))
        {
            UTFCHAR lastC = Tokenizer_getValidChar(this, -1);
            UTFCHAR nextC = Tokenizer_getValidChar(this, 1);
            if (
                !is_calculation_char(lastC)
                && !is_calculation_char(nextC)
                && !is_uchar_eq_uchar(lastC, SIGN_EQ)
                && !is_uchar_eq_uchar(lastC, SIGN_OPEN_SMALL)
            ) {
                Token *tkn = Token_new(UG_TTYPE_CLC, currChar);
                Token_addToken(this, tkn);
                Tokenizer_skipN(this, 1);
                continue;
            }
        }
        // open
        if (is_uchar_eq_uchar(currChar, SIGN_OPEN_SMALL))
        {
            Token *tkn = Token_new(UG_TTYPE_WRD, TVALUE_OPEN);
            Token_addToken(this, tkn);
            Tokenizer_skipN(this, 1);
            continue;
        }
        // close
        if (is_uchar_eq_uchar(currChar, SIGN_CLOSE_SMALL))
        {
            Token *tkn = Token_new(UG_TTYPE_WRD, TVALUE_CLOSE);
            Token_addToken(this, tkn);
            Tokenizer_skipN(this, 1);
            continue;
        }
        // calculation
        if (is_calculation_logicals(currChar))
        {
            Token *tkn = Token_new(UG_TTYPE_CLC, currChar);
            Token_addToken(this, tkn);
            Tokenizer_skipN(this, 1);
            continue;
        }
        // unsupported
        log_error("tokenizer.error: %s", currChar);
        Tokenizer_error(this, LANG_ERR_TOKENIZER_INVALID_SIGN);
        break;
    }
    if (!currChar) free_uchar(currChar);
    return this->head;
}

void Tokenizer_free(Tokenizer *this)
{
    //
    Token *current = this->head;
    Token *temp;
    while (current != NULL)
    {
        temp = current->next;
        Token_free(current);
        current = temp;
    }
    //
    if (this->iterStatic) {
        this->iterStatic->ptr = NULL;
        free(this->iterStatic);
    }
    if (this->iterDynamic) {
        this->iterDynamic->ptr = NULL;
        free(this->iterDynamic);
    }
    //
    this->uyghur = NULL;
    Tokenizer_reset(this);
    free(this);
}
