// mathS.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <lexer.h>
#include <MathParser.h>

using namespace mathS;

int main() {
    std::string str;
    std::cin >> str;
    Lexer lexer(str);
    Token token;
    while (true) {
        lexer.get(token);
        if (token.type == Token::END)
            break;
        std::cout << token.text << std::endl;
    }
    return 0;
}