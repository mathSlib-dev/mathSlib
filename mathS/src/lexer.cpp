#include "lexer.h"
#include <iostream>

void mathS::Lexer::get(mathS::Token& token) {
    token.text = "";
    if (index >= content.length()) {
        token.type = END;
        return;
    }
    char c = content[index++];
    // skip space
    while (((c) == ' ' || (c) == '\n' || (c) == '\t') && (index < content.length())) c = content[index++];
    token.type = checkType(c);
    do {
        token.text += c;
        c = content[index++];
    } while (checkType(c) == token.type && index < content.length());
    index--;
}

mathS::TOKEN_TYPE mathS::Lexer::checkType(char c) {
    if (c >= '0' && c <= '9') return NUMBER;
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_') return SYMBOL;
    return OPERATOR;
}
