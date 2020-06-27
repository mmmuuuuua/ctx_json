



template <typename UtfEncodingType,typename StreamType = StringStream>
int JsonReader<UtfEncodingType, StreamType>::lept_parse(lept_value& v, StreamType& stream) {
    int ret;
    //assert(v != NULL);
    //c.json = json;
    c.stack = NULL;
    c.size = c.top = 0;
    v->lept_init();
    lept_parse_whitespace(stream);
    if ((ret = lept_parse_value(v, stream)) == LEPT_PARSE_OK) {
        lept_parse_whitespace(stream);
        if (stream.Peek() != '\0') {
            v->type = LEPT_NULL;
            ret = LEPT_PARSE_ROOT_NOT_SINGULAR;
        }
    }
    assert(c.top == 0);
    free(c.stack);
    return ret;
}

template <typename UtfEncodingType,typename StreamType = StringStream>
int JsonReader<UtfEncodingType, StreamType>::lept_parse_value(lept_value* v, StreamType& stream) {
    switch (stream.Peek()) {
        case 't':  return lept_parse_literal(v, "true", LEPT_TRUE, stream);
        case 'f':  return lept_parse_literal(v, "false", LEPT_FALSE, stream);
        case 'n':  return lept_parse_literal(v, "null", LEPT_NULL, stream);
        default:   return lept_parse_number(v, stream);
        case '"':  return lept_parse_string(v, stream);
        case '[':  return lept_parse_array(v, stream);
        case '{':  return lept_parse_object(v, stream);
        case '\0': return LEPT_PARSE_EXPECT_VALUE;
    }
}