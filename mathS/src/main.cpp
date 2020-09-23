// mathS.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <lexer.h>
#include <MathParser.h>
#include <LBAssembler.h>

using namespace mathS;

int main() {
    std::string str;

    Assembler assembler;
    
    while (true) {
        std::cin >> str;
        auto m = Parser(str).Parse();

        // 检查表达式是否合法，并输出错误信息
        if (m->GetType() == MathObject::ERROR) {
            std::cout << m->GetString() << std::endl;
            continue;
        }
            
        std::vector<std::string> params;
        std::cout << assembler.Assemble(m, params)({})->GetString() << std::endl;
    }
    return 0;
}