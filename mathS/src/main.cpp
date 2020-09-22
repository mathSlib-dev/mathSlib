// mathS.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <lexer.h>
#include <MathParser.h>
#include<crtdbg.h>

using namespace mathS;

int main() {
    std::string str;

    // 测试 parser
    while (true) {
        std::cin >> str;
        auto m = Parser(str).Parse();
        std::cout << m->GetString() << std::endl;
        _CrtDumpMemoryLeaks();
    }
    return 0;
}