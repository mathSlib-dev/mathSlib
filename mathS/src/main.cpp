// mathS.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <lexer.h>
#include <MathParser.h>

using namespace mathS;

int main() {
    Parser parser("a*b*c*d");
    auto obj = parser.Parse();
    std::cout << obj->GetString() << std::endl;
    delete obj;
    return 0;
}