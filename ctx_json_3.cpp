#ifdef _WINDOWS
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif
#include "ctx_json_3.h"

/*
void lept_value::lept_free() {
    size_t i;
    switch (type) {
        case LEPT_STRING:
            free(u.s.s);
            break;
        case LEPT_ARRAY:
            for (i = 0; i < u.a.size; i++)
                u.a.e[i].lept_free();
            free(u.a.e);
            break;
        case LEPT_OBJECT:
            for (i = 0; i < u.o.size; i++)
            {
                u.o.m[i].v.lept_free();
                free(u.o.m[i].k);
            }
            free(u.o.m);
            break;
        default: break;
    }
    type = LEPT_NULL;
}*/

lept_type lept_value::lept_get_type() {
    return type_;
}

int lept_value::lept_get_boolean() {
    assert(type_ == LEPT_TRUE || type_ == LEPT_FALSE);
    return type_ == LEPT_TRUE;
}

void lept_value::lept_set_boolean(int b) {
    //lept_free();
    type_ = b ? LEPT_TRUE : LEPT_FALSE;
}

double lept_value::lept_get_number() {
    assert(type_ == LEPT_NUMBER);
    return data_.n;
}

void lept_value::lept_set_number(double n) {
    //lept_free();
    data_.n = n;
    type_ = LEPT_NUMBER;
}

const char* lept_value::lept_get_string() {
    assert(type_ == LEPT_STRING);
    return data_.s.s;
}

size_t lept_value::lept_get_string_length() {
    assert(type_ == LEPT_STRING);
    return data_.s.len;
}

void lept_value::lept_set_string(const char* s, size_t len) {
    assert(s != NULL || len == 0);
    //lept_free();
    //u.s.s = (char*)malloc(len + 1);
    data_.s.s = reinterpret_cast<char*>(allocator_->Alloc(len + 1));
    memcpy(data_.s.s, s, len);
    data_.s.s[len] = '\0';
    data_.s.len = len;
    type_ = LEPT_STRING;
}

size_t lept_value::lept_get_array_size() {
    assert(type_ == LEPT_ARRAY);
    return data_.a.size;
}

lept_value* lept_value::lept_get_array_element(size_t index) {
    assert(type_ == LEPT_ARRAY);
    assert(index < data_.a.size);
    return &data_.a.e[index];
}

size_t lept_value::lept_get_object_size() {
    assert(type_ == LEPT_OBJECT);
    return data_.o.size;
}

const char* lept_value::lept_get_object_key(size_t index) {
    assert(type_ == LEPT_OBJECT);
    assert(index < data_.o.size);
    return data_.o.m[index].k;
}

size_t lept_value::lept_get_object_key_length(size_t index) {
    assert(type_ == LEPT_OBJECT);
    assert(index < data_.o.size);
    return data_.o.m[index].klen;
}

lept_value* lept_value::lept_get_object_value(size_t index) {
    assert(type_ == LEPT_OBJECT);
    assert(index < data_.o.size);
    return &(data_.o.m[index].v);
}
