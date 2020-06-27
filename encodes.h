#ifndef ENCONDES_H__
#define ENCONDES_H__
#include <assert.h>  /* assert() */

struct UTF8
{
    static void encode(char* os,int& len,unsigned u) {
        char* ret = os;
        if (u <= 0x7F) 
            *os++ = (u & 0xFF);
            //put_char(u & 0xFF);
        else if (u <= 0x7FF) {
            *os++ = (0xC0 | ((u >> 6) & 0xFF));
            *os++ = (0x80 | ( u       & 0x3F));
            //put_char(0xC0 | ((u >> 6) & 0xFF));
            //put_char(0x80 | ( u       & 0x3F));
        }
        else if (u <= 0xFFFF) {
            *os++ = (0xE0 | ((u >> 12) & 0xFF));
            *os++ = (0x80 | ((u >>  6) & 0x3F));
            *os++ = (0x80 | ( u        & 0x3F));
            //put_char(0xE0 | ((u >> 12) & 0xFF));
            //put_char(0x80 | ((u >>  6) & 0x3F));
            //put_char(0x80 | ( u        & 0x3F));
        }
        else {
            assert(u <= 0x10FFFF);
            *os++ = (0xF0 | ((u >> 18) & 0xFF));
            *os++ = (0x80 | ((u >> 12) & 0x3F));
            *os++ = (0x80 | ((u >>  6) & 0x3F));
            *os++ = (0x80 | ( u        & 0x3F));
            //put_char(0xF0 | ((u >> 18) & 0xFF));
            //put_char(0x80 | ((u >> 12) & 0x3F));
            //put_char(0x80 | ((u >>  6) & 0x3F));
            //put_char(0x80 | ( u        & 0x3F));
        }
        len = os - ret;
        os = ret;
    }
};

struct UTF16
{
    static void encode(char* os,int& len,unsigned u)
    {
        char* ret = os;
        if(u <= 0xFFFF)
        {
            *os++ = ((u >> 8) & 0xFF);
            *os++ = (u & 0xFF);
            //put_char((c >> 8) & 0xFF);
            //put_char(c & 0xFF);
        }
        else{
            //put_char((v >> 10) | 0xD800));
            //put_char((v & 0x3FF) | 0xDC00));
            *os++ = (0xD8 | ((u >> 18) & 0x03));
            *os++ = ((u >> 10) & 0xFF);
            *os++ = (0xDC | ((u >> 8) & 0x03));
            *os++ = (u & 0xFF);
            //put_char(0xD8 | ((c >> 18) & 0x03));
            //put_char((c >> 10) & 0xFF);
            //put_char(0xDC | ((c >> 8) & 0x03));
            //put_char(c & 0xFF);
        }
        len = os - ret;
        os = ret;
    }

    /*
    static void lept_encode_utf16be(unsigned u)
    {
        if(u <= 0xFFFF)
        {
            put_char((c >> 8) & 0xFFu));
            put_char(c & 0xFFu));
        }
        else{
            put_char((v & 0x3FF) | 0xDC00));
            put_char((v >> 10) | 0xD800));
        }
    }
    */
};

struct UTF32
{
    static void encode(char* os,int& len,unsigned u)
    {
        char* ret = os;
        assert(u<=0x10FFFF);
        *os++ = ((u >> 24) & 0xFFu);
        *os++ = ((u >> 16) & 0xFFu);
        *os++ = ((u >> 8) & 0xFFu);
        *os++ = (u & 0xFFu);
        //put_char((c >> 24) & 0xFFu);
        //put_char((c >> 16) & 0xFFu);
        //put_char((c >> 8) & 0xFFu);
        //put_char(c & 0xFFu);
        len = os - ret;
        os = ret;
    }

    /*
    static void lept_encode_utf32le(unsigned u)
    {
        assert(u<=0x10FFFF);
        put_char(c & 0xFFu));
        put_char((c >> 8) & 0xFFu));
        put_char((c >> 16) & 0xFFu));
        put_char((c >> 24) & 0xFFu));
    }
    */
    
};


#endif


