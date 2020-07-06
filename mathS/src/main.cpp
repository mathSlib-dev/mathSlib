// mathS.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <lexer.h>

using namespace mathS;

int main() {
    Lexer lexer(" rxt+fd__");
    Token token;
    while (token.type != END) {
        lexer.get(token);
        std::cout << token.text << std::endl;
    }
    return 0;
}