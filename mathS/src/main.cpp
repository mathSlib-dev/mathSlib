// mathS.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <MathParser.h>
#include <LBAssembler.h>

using namespace mathS;

int main() {
    // 一个计算器

    std::string str;
    Assembler assembler;
    
    while (true) {
        std::cin >> str;
        auto mobj = Parser(str).Parse();

        // 检查表达式是否合法，并输出错误信息
        if (mobj->GetType() == MathObject::ERROR) {
            std::cout << mobj->GetString() << std::endl;
            continue;
        }
        // 计算器程序不需要变量参数，空的参数表
        std::vector<std::string> params = {};
        // 组装
        auto f = assembler.Assemble(mobj, params);
        // 输出结果
        std::cout << "Answer = " << f({})->GetString() << std::endl;
    }
    return 0;
}