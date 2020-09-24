#ifndef MATHS_LEXER_H
#define MATHS_LEXER_H

#include <string>

namespace mathS {
    /*
     * TODO 词法分析器的实现
     * 词法分析器主要是拿来分割字符串的，将其分割为一个个的token方便之后语法的处理及表达式树构建
     * */
    
    struct Token {
        enum TYPE {
            NUMORSYMB, STRING, OPERATOR, END
        };
        Token() : type(NUMORSYMB) { }
        TYPE type;
        std::string text;
    };
    class Lexer {
    public:
        explicit Lexer(const std::string& c) : content(c) { }
        void get(Token&);
    private:
        Token::TYPE checkType(char);
        const std::string content;
        int index = 0;
    };
}


#endif //MATHS_LEXER_H
