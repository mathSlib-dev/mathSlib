#include "lexer.h"
#include <iostream>

#define SPACE(c) ((c) == ' ' || (c) == '\n' || (c) == '\t')
#define _END (index >= content.length())
#define NOT_END (index < content.length())

void mathS::Lexer::get(mathS::Token& token) {
    token.text = "";
    if (_END) {
        token.type = END;
        return;
    }
    char c = content[index++];
    // skip space
    while (SPACE(c) && NOT_END) c = content[index++];
    token.type = checkType(c);
    do {
        token.text += c;
        c = content[index++];
    } while (checkType(c) == token.type);
    index--;
}

mathS::TOKEN_TYPE mathS::Lexer::checkType(char c) {
    if (c >= '0' && c <= '9') return NUMBER;
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_') return SYMBOL;
    return OPERATOR;
}
