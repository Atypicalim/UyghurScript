// cipher

#include "../uyghur.c"

void native_cipher_md5(Bridge *bridge) {
    CString data = Bridge_receiveString(bridge);
    int successful;
    unsigned char _hash[MD5_SIZE];
    char hash[33];
    md5(_hash, data, strlen(data));
    md5_format(hash, sizeof(hash), _hash);
    Bridge_returnString(bridge, hash);
}

/////////////////////////////////////////////////////////////////////

void native_cipher_base64_encode(Bridge *bridge) {
    CString data = Bridge_receiveString(bridge);
    CString encoded = base64_encode(data);
    Bridge_returnString(bridge, encoded);
}

void native_cipher_base64_decode(Bridge *bridge) {
    CString data = Bridge_receiveString(bridge);
    CString decoded = base64_decode(data);
    Bridge_returnString(bridge, decoded);
}

/////////////////////////////////////////////////////////////////////

JValue *__json_encode_value(Value *);

void __json_encode_map(Hashkey *hashkey, void *jObject) {
    String *key = hashkey->key;
    Value *val = hashkey->value;
    CString _key = String_get(key);
    JValue *_val = __json_encode_value(val);
    json_object_add_member(jObject, _key, _val);
}

void __json_encode_arr(int index, Value *val, void *jArray) {
    JValue *_val = __json_encode_value(val);
    json_array_add_element(jArray, _val);
}

JValue *__json_encode_value(Value *uVal) {
    JValue *jVal = (JValue*)malloc(sizeof(JValue));
    json_init(jVal);
    if (Value_isEmpty(uVal)) {
        json_set_null(jVal);
    } else if (Value_isBoolean(uVal)) {
        json_set_boolean(jVal, uVal->boolean);
    } else if (Value_isNumber(uVal)) {
        json_set_number(jVal, uVal->number);
    } else if (Value_isString(uVal)) {
        json_set_string(jVal, uVal->string, strlen(uVal->string));
    } else if (Dictable_isDct(uVal)) {
        json_set_object(jVal, 10);
        Hashmap_foreachItem(uVal->map, __json_encode_map, jVal);
    } else if (Listable_isLst(uVal)) {
        json_set_array(jVal, 10);
        Array_foreachItem(uVal->arr, __json_encode_arr, jVal);
    }
    return jVal;
}

void native_cipher_json_encode(Bridge *bridge) {
    Value *value = Bridge_receiveValue(bridge, UG_TYPE_NON);
    JValue *_value = __json_encode_value(value);
    CString json;
    json_encode(&json, _value);
    json_free(_value);
    Bridge_returnString(bridge, json);
}

/////////////////////////////////////////////////////////////////////

Value *__json_decode_value(JValue *);

void __json_decode_map(JMember *member, void *uDict) {
    char *_key = member->k;
    Value *_val = __json_decode_value(&member->v);
    Dictable_setLocation(uDict, _key, _val);
}

void __json_decode_arr( JElement *element, void *uList) {
    Value *_val = __json_decode_value(element);
    Listable_push(uList, _val);
}

Value *__json_decode_value(JValue *jVal) {
    JType jTyp = jVal->type;
    Value *uVal = NULL;
    if (jTyp == JSON_NULL) {
        uVal = Value_newEmpty(NULL);
    } else if (jTyp == JSON_BOOLEAN) {
        uVal = Value_newBoolean(jVal->u.b, NULL);
    } else if (jTyp == JSON_NUMBER) {
        uVal = Value_newNumber(jVal->u.n, NULL);
    } else if (jTyp == JSON_STRING) {
        uVal = Value_newString(jVal->u.s.s, NULL);
    } else if (jTyp == JSON_OBJECT) {
        uVal = Dictable_newDct(NULL);
        int size = json_object_get_size(jVal);
        for (size_t i = 0; i < size; i++) {
            JMember *member = json_object_get_index(jVal, i);
            __json_decode_map(member, uVal);
        }
    } else if (jTyp == JSON_ARRAY) {
        uVal = Listable_newLst(NULL);
        int size = json_array_get_size(jVal);
        for (size_t i = 0; i < size; i++) {
            JElement *element = json_array_get_index(jVal, i);
            __json_decode_arr(element, uVal);
        }
    }
    return uVal;
}

void native_cipher_json_decode(Bridge *bridge) {
    CString json = Bridge_receiveString(bridge);
    JValue _value;
    json_decode(&_value, json);
    Value *value = __json_decode_value(&_value);
    json_free(&_value);
    Bridge_returnValue(bridge, value);
}

/////////////////////////////////////////////////////////////////////
