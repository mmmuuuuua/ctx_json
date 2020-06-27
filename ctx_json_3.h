#ifndef LEPTJSON_H__
#define LEPTJSON_H__

#include <stddef.h> 
#include <assert.h>  
#include <errno.h>   
#include <math.h>   
#include <stdlib.h>  
#include <cstring>  
#include <string>  
#include <memory>
#include "streams.h"
#include "allocator.h"
#include "util.h"

#ifndef LEPT_PARSE_STACK_INIT_SIZE
#define LEPT_PARSE_STACK_INIT_SIZE 256
#endif

typedef enum { LEPT_NULL, LEPT_FALSE, LEPT_TRUE, LEPT_NUMBER, LEPT_STRING, LEPT_ARRAY, LEPT_OBJECT, LEPT_CTX} lept_type;

enum {
    LEPT_PARSE_OK = 0,
    LEPT_PARSE_EXPECT_VALUE,
    LEPT_PARSE_INVALID_VALUE,
    LEPT_PARSE_ROOT_NOT_SINGULAR,
    LEPT_PARSE_NUMBER_TOO_BIG,
    LEPT_PARSE_MISS_QUOTATION_MARK,
    LEPT_PARSE_INVALID_STRING_ESCAPE,
    LEPT_PARSE_INVALID_STRING_CHAR,
    LEPT_PARSE_INVALID_UNICODE_HEX,
    LEPT_PARSE_INVALID_UNICODE_SURROGATE,
    LEPT_PARSE_MISS_COMMA_OR_SQUARE_BRACKET,
    LEPT_PARSE_MISS_KEY,
    LEPT_PARSE_MISS_COLON,
    LEPT_PARSE_MISS_COMMA_OR_CURLY_BRACKET
};


typedef struct {
    //const char* json;
    char* stack;
    size_t size, top;
}lept_context;


class lept_value;
class lept_member;

class lept_value {
    template <typename, typename> friend class JsonReader;
    public:
        //默认构造函数
        lept_value():data_()
        {
            //allocator_ = new Allocator();
            allocator_ = std::make_shared<Allocator>();
            type_ = LEPT_CTX; 
            parseResult_ = LEPT_PARSE_OK;
        }

        lept_value(std::shared_ptr<Allocator> allocator):data_()
        {
            allocator_ = allocator;
            type_ = LEPT_CTX;
            parseResult_ = LEPT_PARSE_OK;
        }

        //拷贝构造函数
        /*
        lept_value(const lept_value& rhs){
            switch(rhs.type_){
                case LEPT_STRING:
                    std::memcpy(data_.s.s, rhs.data_.s.s, rhs.data_.s.len);
                    type_ = LEPT_STRING;
                    break;
                case LEPT_ARRAY: {
                        size_t count = rhs.data_.a.size;
                        lept_value* lb = reinterpret_cast<lept_value*>(allocator_->Alloc(count * sizeof(lept_value)));
                        lept_value* rb = rhs.GetArrayPointer();
                        for(size_t i = 0; i < count; i++)
                            new (&lb[i]) lept_value(rb[i]);
                        data_.a.size = count;
                        SetArrayPointer(lb);
                        type_ = LEPT_ARRAY;
                    }
                    break;
                case LEPT_OBJECT: {
                        size_t count = rhs.data_.o.size;
                        lept_member* lb = reinterpret_cast<lept_member*>(allocator_->Alloc(count * sizeof(lept_member)));
                        lept_member* rb = rhs.GetObjectPointer();
                        for(size_t i = 0; i < count; i++){
                            new (lb[i].v) lept_value(rb[i].v);
                            std::memcpy(lb[i].k, rb[i].k, rb[i].klen);
                            lb[i].klen = rb[i].klen;
                        }
                        data_.o.size = count;
                        SetObjectPointer(lb);
                        type_ = LEPT_OBJECT;
                    }
                    break;
                case LEPT_NUMBER:
                    data_.n = rhs.data_.n;
                    type_ = LEPT_NUMBER;
                    break;
                default:
                    type_ = rhs.type_;
                    break;
            }
            parseResult_ = LEPT_PARSE_OK;
        }
        */
        //移动构造函数
        lept_value(lept_value&& rhs):allocator_(rhs.allocator_), data_(rhs.data_)
        {
            rhs.allocator_ = NULL;
            rhs.type_ = LEPT_CTX;
            parseResult_ = LEPT_PARSE_OK;
        }

        //拷贝赋值运算符
        /*
        lept_value& operator=(lept_value& rhs){
            if(this != &rhs){
                switch(rhs.type_){
                    case LEPT_STRING:
                        std::memcpy(data_.s.s, rhs.data_.s.s, rhs.data_.s.len);
                        type_ = LEPT_STRING;
                        break;
                    case LEPT_ARRAY: {
                            size_t count = rhs.data_.a.size;
                            lept_value* lb = reinterpret_cast<lept_value*>(allocator_->Alloc(count * sizeof(lept_value)));
                            lept_value* rb = rhs.GetArrayPointer();
                            for(size_t i = 0; i < count; i++)
                                new (&lb[i]) lept_value(rb[i]);
                            data_.a.size = count;
                            SetArrayPointer(lb);
                            type_ = LEPT_ARRAY;
                        }
                        break;
                    case LEPT_OBJECT: {
                            size_t count = rhs.data_.o.size;
                            lept_member* lb = reinterpret_cast<lept_member*>(allocator_->Alloc(count * sizeof(lept_member)));
                            lept_member* rb = rhs.GetObjectPointer();
                            for(size_t i = 0; i < count; i++){
                                new (lb[i].v) lept_value(rb[i].v);
                                std::memcpy(lb[i].k, rb[i].k, rb[i].klen);
                                lb[i].klen = rb[i].klen;
                            }
                            data_.o.size = count;
                            SetObjectPointer(lb);
                            type_ = LEPT_OBJECT;
                        }
                        break;
                    case LEPT_NUMBER:
                        data_.n = rhs.data_.n;
                        type_ = LEPT_NUMBER;
                        break;
                    default:
                        type_ = rhs.type_;
                        break;
                }
            }
            parseResult_ = rhs.parseResult_;
            return *this;
        }
        */

        //移动赋值运算符
        lept_value& operator=(lept_value&& rhs){
            //if(this != &rhs){
            //delete allocator_;
            allocator_ = rhs.allocator_;
            rhs.allocator_ = NULL;
            rhs.type_ = LEPT_CTX;
            data_ = rhs.data_;
            //}
            parseResult_ = rhs.parseResult_;
            return *this;
        }

        //析构函数
        ~lept_value(){
            //delete allocator_;
        }

        //交换
        lept_value& swap(lept_value& rhs){
            Swap(allocator_, rhs.allocator_);
            Swap(data_, rhs.data_);
            Swap(parseResult_, rhs.parseResult_);
            return *this;
        }

        lept_value* get_lept_value(){
            lept_value* res = static_cast<lept_value*>(allocator_->Alloc(1 * sizeof(lept_value)));
            new (res) lept_value(allocator_);
            return res;
        }


        void lept_init()
        {
            type_ = LEPT_CTX;
            //parseResult_ = LEPT_PARSE_OK;
        }

        //void lept_free();

        lept_type lept_get_type();

        void lept_set_null()
        {
            //lept_free();
            type_ = LEPT_NULL;
        }

        int lept_get_boolean();
        void lept_set_boolean(int b);

        double lept_get_number();
        void lept_set_number(double n);

        const char* lept_get_string();
        size_t lept_get_string_length();
        void lept_set_string(const char* s, size_t len);

        size_t lept_get_array_size();
        lept_value* lept_get_array_element(size_t index);

        size_t lept_get_object_size();
        const char* lept_get_object_key(size_t index);
        size_t lept_get_object_key_length(size_t index);
        lept_value* lept_get_object_value(size_t index);

        //查询接口


        /*
        //重载的parse接口，处理不同情况
        template <typename UtfEncodingType>
        int parse(const char* ch)
        {
            StringStream stream(ch);
            //JsonReader<UtfEncodingType,StringStream> reader;
            //parseResult_ = reader.lept_parse(*this,stream);
            //return parseResult_;
            return 1;
        }

        template <typename UtfEncodingType>
        int parse(std::string str)
        {
            char* ch = str.c_str();
            StringStream stream(ch);
            //JsonReader<UtfEncodingType,StringStream> reader;
            //parseResult_ = reader.lept_parse(*this,stream);
            //return parseResult_;
            return 1;
        }

        template <typename UtfEncodingType>
        int parse(FILE* fp)
        {
            char readBuffer[65536];
            FileStream stream(fp, readBuffer, sizeof(readBuffer)); 
            //JsonReader<UtfEncodingType,FileStream> reader;
            //parseResult_ = reader.lept_parse(*this,stream);
            //return parseResult_;
            return 1;
        }
        */
        
        void SetStringPointer(char* str){
            data_.s.s = str;
        }

        void SetArrayPointer(lept_value* values){
            data_.a.e = values;
        }

        void SetObjectPointer(lept_member* members){
            data_.o.m = members;
        }

        char* GetStringPointer() const{
            return data_.s.s;
        }
        
        lept_value* GetArrayPointer() const{
            return data_.a.e;
        }

        lept_member* GetObjectPointer() const{
            return data_.o.m;
        }

        void SetString(char* str, size_t len)
        {
            type_ = LEPT_STRING;
            if(len!=0){
                char* s = static_cast<char*>(allocator_->Alloc(len));
                SetStringPointer(s);
                std::memcpy(static_cast<void*>(s), str, len);
            }
            else
                SetStringPointer(0);
            data_.s.len = len;
        }

        void SetArray(lept_value* values, size_t count)
        {
            type_ = LEPT_ARRAY;
            if(count){
                lept_value* v = static_cast<lept_value*>(allocator_->Alloc(count * sizeof(lept_value)));
                SetArrayPointer(v);
                std::memcpy(static_cast<void*>(v), values, count * sizeof(lept_value));
            }
            else
                SetArrayPointer(0);
            data_.a.size = count;
        }

        //这里的count是指总的 字节数
        void SetObject(lept_member* members, size_t count, size_t total)
        {
            type_ = LEPT_OBJECT;
            if(count){
                //lept_member* m = static_cast<lept_member*>(allocator_->Alloc(count * sizeof(lept_member)));
                //SetObjectPointer(m);
                //std::memcpy(static_cast<void*>(m), members, count * sizeof(lept_member));

                lept_member* m = static_cast<lept_member*>(allocator_->Alloc(total));
                SetObjectPointer(m);
                std::memcpy(static_cast<void*>(m), members, total);
            }
            else
                SetObjectPointer(0);
            data_.o.size = count;
        }

        bool HasParseError() const { 
            return parseResult_ != LEPT_PARSE_OK; 
        }

        int GetParseError() const {
            return parseResult_;
        }
    private:
        union Data{
            struct { lept_member* m; size_t size; }o;   /* object: members, member count */
            struct { lept_value* e; size_t size; }a;    /* array:  elements, element count */
            struct { char* s; size_t len; }s;           /* string: null-terminated string, string length */
            double n;                                   /* number */
        };  // u->data_;

        Data data_;
        lept_type type_;
        //Allocator* allocator_;
        std::shared_ptr<Allocator> allocator_;
        int parseResult_;
};

class lept_member {
    public:
        lept_member():v(), k(NULL), klen(0)
        {

        }
        
        lept_member(std::shared_ptr<Allocator> allocator):v(allocator), k(NULL), klen(0)
        {

        }
        
        char* k; size_t klen;   /* member key string, key string length */
        lept_value v;           /* member value */
};

template <typename UtfEncodingType,typename StreamType = StringStream>
class JsonReader
{
    public:
        int lept_parse(lept_value& v, StreamType& stream);
    private:
        void* lept_context_push(size_t size);
        void* lept_context_pop(size_t size);
        void lept_parse_whitespace(StreamType& stream);

        int lept_parse_true(lept_value& v, StreamType& stream);
        int lept_parse_false(lept_value& v, StreamType& stream);
        int lept_parse_null(lept_value& v, StreamType& stream);
        int lept_parse_number(lept_value& v, StreamType& stream);
        bool lept_parse_hex4(StreamType& stream, unsigned* u);
        void lept_encode_utf8(unsigned u);
        int lept_parse_string(lept_value& v, StreamType& stream);
        int lept_parse_array(lept_value& v, StreamType& stream);
        int lept_parse_object(lept_value& v, StreamType& stream);
        int lept_parse_value(lept_value& v, StreamType& stream);

        int string_error(int ret,size_t head)
        {
            c.top = head;
            return ret;
        }
        void put_char(char ch)
        {
            *(char*)lept_context_push(sizeof(char)) = ch;
        }
        void put_string(char* s,int len)
        {
            memcpy(lept_context_push(len), s, len);
        }
        void expect(char ch, StreamType& stream)
        {
            assert(stream.Next() == ch);
        }

        bool consume(char ch, StreamType& stream){
            if(JSON_LIKELY(stream.Peek() == ch)){
                stream.Next();
                return true;        
            }
            else
                return false;
        }

        lept_context c;
};


template <typename UtfEncodingType,typename StreamType>
void* JsonReader<UtfEncodingType, StreamType>::lept_context_push(size_t size) {
    void* ret;
    assert(size > 0);
    if (c.top + size >= c.size) {
        if (c.size == 0)
            c.size = LEPT_PARSE_STACK_INIT_SIZE;
        while (c.top + size >= c.size)
            c.size += c.size >> 1;  /* c->size * 1.5 */
        c.stack = (char*)realloc(c.stack, c.size);
    }
    ret = c.stack + c.top;
    c.top += size;
    return ret;
}

template <typename UtfEncodingType,typename StreamType>
void* JsonReader<UtfEncodingType, StreamType>::lept_context_pop(size_t size) {
    assert(c.top >= size);
    return c.stack + (c.top -= size);
}

template <typename UtfEncodingType,typename StreamType>
void JsonReader<UtfEncodingType, StreamType>::lept_parse_whitespace(StreamType& stream) {
    char ch = stream.Peek();
    while (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r'){
        stream.Next();
        ch = stream.Peek();
    }
}

template <typename UtfEncodingType,typename StreamType>
int JsonReader<UtfEncodingType, StreamType>::lept_parse_true(lept_value& v, StreamType& stream) {
    expect('t', stream);
    if(JSON_LIKELY(consume('r', stream) && consume('u', stream) && consume('e', stream))){
        v.type_ = LEPT_TRUE;
        return LEPT_PARSE_OK;
    }
    else
        return LEPT_PARSE_INVALID_VALUE;

}

template <typename UtfEncodingType,typename StreamType>
int JsonReader<UtfEncodingType, StreamType>::lept_parse_false(lept_value& v, StreamType& stream) {
    expect('f', stream);
    if(JSON_LIKELY(consume('a', stream) && consume('l', stream) && consume('s', stream) && consume('e', stream))){
        v.type_ = LEPT_FALSE;
        return LEPT_PARSE_OK;
    }
    else
        return LEPT_PARSE_INVALID_VALUE;
}

template <typename UtfEncodingType,typename StreamType>
int JsonReader<UtfEncodingType, StreamType>::lept_parse_null(lept_value& v, StreamType& stream) {
    expect('n', stream);
    if(JSON_LIKELY(consume('u', stream) && consume('l', stream) && consume('l', stream))){
        v.type_ = LEPT_NULL;
        return LEPT_PARSE_OK;
    }
    else
        return LEPT_PARSE_INVALID_VALUE;
}

template <typename UtfEncodingType,typename StreamType>
int JsonReader<UtfEncodingType, StreamType>::lept_parse_number(lept_value& v, StreamType& stream) {
    const char* tmp = stream.Current();
    if (stream.Peek() == '-') 
        stream.Next();
    if (stream.Peek() == '0') 
        stream.Next();
    else {
        if (!(stream.Peek() >= '1' && stream.Peek() <= '9')) return LEPT_PARSE_INVALID_VALUE;
        for (stream.Next(); stream.Peek() >= '0' && stream.Peek() <= '9'; stream.Next());
    }
    if (stream.Peek() == '.') {
        stream.Next();
        if (!(stream.Peek() >= '0' && stream.Peek() <= '9')) return LEPT_PARSE_INVALID_VALUE;
        for (stream.Next(); stream.Peek() >= '0' && stream.Peek() <= '9'; stream.Next());
    }
    if (stream.Peek() == 'e' || stream.Peek() == 'E') {
        stream.Next();
        if (stream.Peek() == '+' || stream.Peek() == '-') stream.Next();
        if (!(stream.Peek() >= '0' && stream.Peek() <= '9')) return LEPT_PARSE_INVALID_VALUE;
        for (stream.Next(); stream.Peek() >= '0' && stream.Peek() <= '9'; stream.Next());
    }
    errno = 0;
    v.data_.n = strtod(tmp, NULL);
    if (errno == ERANGE && (v.data_.n == HUGE_VAL || v.data_.n == -HUGE_VAL))
        return LEPT_PARSE_NUMBER_TOO_BIG;
    v.type_ = LEPT_NUMBER;
    //c.json = p;
    return LEPT_PARSE_OK;
}

template <typename UtfEncodingType,typename StreamType>
bool JsonReader<UtfEncodingType, StreamType>::lept_parse_hex4(StreamType& stream, unsigned* u) {
    int i;
    *u = 0;
    for (i = 0; i < 4; i++) {
        char ch = stream.Peek();
        stream.Next();
        *u <<= 4;
        if      (ch >= '0' && ch <= '9')  *u |= ch - '0';
        else if (ch >= 'A' && ch <= 'F')  *u |= ch - ('A' - 10);
        else if (ch >= 'a' && ch <= 'f')  *u |= ch - ('a' - 10);
        else return false;
    }
    return true;
}

template <typename UtfEncodingType,typename StreamType>
void JsonReader<UtfEncodingType, StreamType>::lept_encode_utf8(unsigned u) {
    if (u <= 0x7F) 
        put_char(u & 0xFF);
    else if (u <= 0x7FF) {
        put_char(0xC0 | ((u >> 6) & 0xFF));
        put_char(0x80 | ( u       & 0x3F));
    }
    else if (u <= 0xFFFF) {
        put_char(0xE0 | ((u >> 12) & 0xFF));
        put_char(0x80 | ((u >>  6) & 0x3F));
        put_char(0x80 | ( u        & 0x3F));
    }
    else {
        assert(u <= 0x10FFFF);
        put_char(0xF0 | ((u >> 18) & 0xFF));
        put_char(0x80 | ((u >> 12) & 0x3F));
        put_char(0x80 | ((u >>  6) & 0x3F));
        put_char(0x80 | ( u        & 0x3F));
    }
}

template <typename UtfEncodingType,typename StreamType>
int JsonReader<UtfEncodingType, StreamType>::lept_parse_string(lept_value& v, StreamType& stream) {
    size_t head = c.top, len;
    char* tmp = static_cast<char*>(v.allocator_->Alloc(4));
    //char* tmp = (char*)(malloc(sizeof(char)*4));
    int tempLen_ = 0;
    unsigned u, u2;
    //const char* p;
    expect('\"', stream);
    //p = c.json;
    for (;;) {
        char ch = stream.Next();

        if(JSON_UNLIKELY(ch == '\"')){
            len = c.top - head;
            v.SetString((char*)lept_context_pop(len), len);
            //c.json = p;
            return LEPT_PARSE_OK;
        }
        else if(JSON_UNLIKELY(ch == '\\')){
            switch (stream.Next()) {
                case '\"': put_char('\"'); break;
                case '\\': put_char('\\'); break;
                case '/':  put_char('/' ); break;
                case 'b':  put_char('\b'); break;
                case 'f':  put_char('\f'); break;
                case 'n':  put_char('\n'); break;
                case 'r':  put_char('\r'); break;
                case 't':  put_char('\t'); break;
                case 'u':
                    if (!(lept_parse_hex4(stream, &u)))
                        return string_error(LEPT_PARSE_INVALID_UNICODE_HEX,head);
                    if (u >= 0xD800 && u <= 0xDBFF) { /* surrogate pair */
                        if (stream.Next() != '\\')
                            return string_error(LEPT_PARSE_INVALID_UNICODE_SURROGATE,head);
                        if (stream.Next() != 'u')
                            return string_error(LEPT_PARSE_INVALID_UNICODE_SURROGATE,head);
                        if (!(lept_parse_hex4(stream, &u2)))
                            return string_error(LEPT_PARSE_INVALID_UNICODE_HEX,head);
                        if (u2 < 0xDC00 || u2 > 0xDFFF)
                            return string_error(LEPT_PARSE_INVALID_UNICODE_SURROGATE,head);
                        u = (((u - 0xD800) << 10) | (u2 - 0xDC00)) + 0x10000;
                    }
                    UtfEncodingType::encode(tmp,tempLen_,u);
                    put_string(tmp,tempLen_);
                    //lept_encode_utf8(u);
                    break;
                default:
                    return string_error(LEPT_PARSE_INVALID_STRING_ESCAPE,head);
            }
        }
        else if(JSON_UNLIKELY(ch == '\0')){
            return string_error(LEPT_PARSE_MISS_QUOTATION_MARK,head);
        }
        else{
            if ((unsigned char)ch < 0x20)
                return string_error(LEPT_PARSE_INVALID_STRING_CHAR,head);
            put_char(ch);
        }
    }
    //free(tmp);
}

template <typename UtfEncodingType,typename StreamType>
int JsonReader<UtfEncodingType, StreamType>::lept_parse_array(lept_value& v, StreamType& stream) {
    size_t i, size = 0;
    int ret;
    expect('[', stream);
    lept_parse_whitespace(stream);
    if (stream.Peek() == ']') {
        stream.Next();
        v.type_ = LEPT_ARRAY;
        v.data_.a.size = 0;
        v.data_.a.e = NULL;
        return LEPT_PARSE_OK;
    }
    for (;;) {
        //lept_value e(allocator_);
        //e.lept_init();
        if ((ret = lept_parse_value(v, stream)) != LEPT_PARSE_OK)
            break;
        memcpy(lept_context_push(sizeof(lept_value)), &v, sizeof(lept_value));
        size++;
        lept_parse_whitespace(stream);
        if (stream.Peek() == ',') {
            stream.Next();
            lept_parse_whitespace(stream);
        }
        else if (stream.Peek() == ']') {
            stream.Next();
            v.SetArray ((lept_value*)(lept_context_pop(size * sizeof(lept_value))), size);
            return LEPT_PARSE_OK;
        }
        else {
            ret = LEPT_PARSE_MISS_COMMA_OR_SQUARE_BRACKET;
            break;
        }
    }

    for (i = 0; i < size; i++)
        (lept_value*)lept_context_pop(sizeof(lept_value));
    return ret;
}

template <typename UtfEncodingType,typename StreamType>
int JsonReader<UtfEncodingType, StreamType>::lept_parse_object(lept_value& v, StreamType& stream) {
    size_t i,size;
    lept_member m(v.allocator_);
    int ret;
    expect('{', stream);
    lept_parse_whitespace(stream);
    if (stream.Peek() == '}') {
        stream.Next();
        v.type_ = LEPT_OBJECT;
        v.data_.o.m = 0;
        v.data_.o.size = 0;
        return LEPT_PARSE_OK;
    }
    m.k = NULL;
    size = 0;
    for (;;) {
        //m.v = v.get_lept_value();
        m.v.lept_init();
        lept_parse_whitespace(stream);
        lept_value key_(v.allocator_);
        key_.lept_init();
        if ((ret = lept_parse_value(key_, stream)) != LEPT_PARSE_OK)
        {
            ret = LEPT_PARSE_MISS_KEY;
            break;
        }
        if(key_.lept_get_type()!=LEPT_STRING)
        {
            ret = LEPT_PARSE_MISS_KEY;
            break;
        }
        lept_parse_whitespace(stream);
        if(stream.Peek()!=':')
        {
            ret = LEPT_PARSE_MISS_COLON;
            break;
        }
        expect(':', stream);
        lept_parse_whitespace(stream);
        if ((ret = lept_parse_value(m.v, stream)) != LEPT_PARSE_OK)
        {
            break;
        }
        m.k = const_cast<char *>(key_.lept_get_string());
        m.klen = key_.lept_get_string_length();
        memcpy(lept_context_push(sizeof(lept_member)), &m, sizeof(lept_member));

        key_.data_.s.s = NULL;
        size++;
        m.k = NULL; /* ownership is transferred to member on stack */
        /* \todo parse ws [comma | right-curly-brace] ws */

        lept_parse_whitespace(stream);
        if(stream.Peek()==',')
        {
            stream.Next();
            lept_parse_whitespace(stream);  
        }
        else if(stream.Peek()=='}')
        { 
            stream.Next();
            v.SetObject((lept_member*)(lept_context_pop(size * sizeof(lept_member))), size, size * sizeof(lept_member));
            return LEPT_PARSE_OK;
        }
        else{
            ret = LEPT_PARSE_MISS_COMMA_OR_CURLY_BRACKET;
            break;
        }
    }
    for(i=0;i<size;i++)
        (lept_member*)lept_context_pop(sizeof(lept_member));
    
    return ret;
}

template <typename UtfEncodingType,typename StreamType>
int JsonReader<UtfEncodingType, StreamType>::lept_parse_value(lept_value& v, StreamType& stream) {
    switch (stream.Peek()) {
        case 't':  return lept_parse_true(v, stream);
        case 'f':  return lept_parse_false(v, stream);
        case 'n':  return lept_parse_null(v, stream);
        default:   return lept_parse_number(v, stream);
        case '"':  return lept_parse_string(v, stream);
        case '[':  return lept_parse_array(v, stream);
        case '{':  return lept_parse_object(v, stream);
        case '\0': return LEPT_PARSE_EXPECT_VALUE;
    }
}

template <typename UtfEncodingType,typename StreamType>
int JsonReader<UtfEncodingType, StreamType>::lept_parse(lept_value& v, StreamType& stream) {
    int ret;
    //assert(v != NULL);
    //c.json = json;
    c.stack = NULL;
    c.size = c.top = 0;
    v.lept_init();
    lept_parse_whitespace(stream);
    if ((ret = lept_parse_value(v, stream)) == LEPT_PARSE_OK) {
        lept_parse_whitespace(stream);
        if (stream.Peek() != '\0') {
            v.type_ = LEPT_CTX;
            ret = LEPT_PARSE_ROOT_NOT_SINGULAR;
        }
    }
    else
        v.type_ = LEPT_CTX;
    assert(c.top == 0);
    free(c.stack);
    return ret;
}

/*
template <typename UtfEncodingType>
class JsonWriter
{
    public:
        void lept_stringify_string(const char* s, size_t len) {
            int i=0;
            static const char hex_digits[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
            PUTC(c,'\"');
            for(i=0;i<len;i++)
            {
                unsigned char ch = (unsigned char)s[i];
                switch(ch)
                {
                    case '\"': PUTC(c,'\\'); PUTC(c,'\"'); break;
                    case '\\': PUTC(c,'\\'); PUTC(c,'\\'); break;
                    //case '/': PUTC(c,'\\'); PUTC(c,'/'); break;
                    case '\b': PUTC(c,'\\');PUTC(c,'b'); break;
                    case '\f': PUTC(c,'\\'); PUTC(c,'f'); break;
                    case '\n': PUTC(c,'\\'); PUTC(c,'n'); break;
                    case '\r': PUTC(c,'\\'); PUTC(c,'r'); break;
                    case '\t': PUTC(c,'\\'); PUTC(c,'t'); break;
                    default:                
                        if (ch < 0x20) {
                            PUTC(c,'\\');
                            PUTC(c,'u');
                            PUTC(c,'0');
                            PUTC(c,'0');
                            PUTC(c,hex_digits[ch >> 4]);
                            PUTC(c,hex_digits[ch & 15]);
                        }
                        else
                            PUTC(c,s[i]);

                }
            }
            PUTC(c,'\"');
        }        
        void lept_stringify_value(const lept_value* v) {
            int i=0;
            switch (v->type) {
                case LEPT_NULL:   PUTS(c, "null",  4); break;
                case LEPT_FALSE:  PUTS(c, "false", 5); break;
                case LEPT_TRUE:   PUTS(c, "true",  4); break;
                case LEPT_NUMBER: c->top -= 32 - sprintf(lept_context_push(c, 32), "%.17g", v->u.n); break;
                case LEPT_STRING: lept_stringify_string(c, v->u.s.s, v->u.s.len); break;
                case LEPT_ARRAY:
                    PUTC(c,'[');
                    for(i=0;i<(v->u).a.size;i++)
                    {
                        if(i!=0)
                            PUTC(c,',');
                        lept_stringify_value(c,&((v->u).a.e[i]));
                    }
                    PUTC(c,']');
                    break;
                case LEPT_OBJECT:
                    PUTC(c,'{');
                    for(i=0;i<(v->u).o.size;i++)
                    {
                        if(i!=0)
                            PUTC(c,',');
                        lept_stringify_string(c,(v->u).o.m[i].k,(v->u).o.m[i].klen);
                        PUTC(c,':');
                        lept_stringify_value(c,&((v->u).o.m[i].v));
                    }
                    PUTC(c,'}');
                    break;
                default: assert(0 && "invalid type");
            }
        }

        char* lept_stringify(const lept_value* v, size_t* length) {
            assert(v != NULL);
            c.stack = (char*)malloc(c.size = LEPT_PARSE_STRINGIFY_INIT_SIZE);
            c.top = 0;
            lept_stringify_value(v);
            if (length)
                *length = c.top;
            PUTC(&c, '\0');
            return c.stack;
        }
    private:
        lept_context c;
};
*/

#endif /* LEPTJSON_H__ */
