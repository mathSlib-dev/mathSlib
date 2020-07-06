#ifndef MATHS_LEXER_H
#define MATHS_LEXER_H

#include <string>

namespace mathS {
    enum TOKEN_TYPE {
        NUMBER, SYMBOL, OPERATOR, END
    };
    struct Token {
        Token() : type(NUMBER) { }
        TOKEN_TYPE type;
        std::string text;
    };
    class Lexer {
    public:
        explicit Lexer(const std::string& c) : content(c) { }
        void get(Token&);
    private:
        TOKEN_TYPE checkType(char);
        const std::string& content;
        int index = 0;
    };
}


#endif //MATHS_LEXER_H
