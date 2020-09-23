#include "lexer.h"
#include <iostream>

void mathS::Lexer::get(mathS::Token& token) {
    token.text = "";
    if (index >= content.length()) {
        token.type = Token::END;
        return;
    }
    char c = content[index++];
    // skip space
    while ((c == ' ' || c == '\n' || c == '\t') && (index < content.length()))
        c = content[index++];
    token.type = checkType(c);
    token.text += c;
    switch (token.type){
    case Token::OPERATOR:
        if ((c == '=' || c == '<' || c == '>') && index < content.length()) {
            if (content[index] == '=' || content[index] == c)
                token.text += content[index++];
        }
        if (c == '-' && index < content.length()) {
            if (content[index] == '>') {
                token.text += '>';
                index++;
            }
        }
        break;
    case Token::NUMORSYMB:
        while (index < content.length()) {
            if (checkType(content[index]) != Token::NUMORSYMB) break;
            token.text += content[index++];
        } 
        break;
    case Token::STRING:
        while (index < content.length()) {
            c = content[index++];
            token.text += c;
            if (c == '"') break;
            if (c == '\\' && index < content.length()) {
                token.text += content[index++];
            }
        };
        if (token.text.length() == 1 || token.text.back() != '"' || token.text[token.text.length() - 2] == '\\')
            token.text += '"';
        break;
    default:
        break;
    }
}

mathS::Token::TYPE mathS::Lexer::checkType(char c) {
    // ÅÐ¶Ï×Ö·ûÀàÐÍ
    if ((c >= '0' && c <= '9'||c=='.')
        ||(c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') 
        || c == '_'|| c == '@' || c == '#' || c == '$') 
        return Token::NUMORSYMB;
    if (c == '"')
        return Token::STRING;
    return Token::OPERATOR;
}
