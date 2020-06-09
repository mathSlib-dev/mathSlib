// mathS.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <NMathObject.h>

using namespace mathS::NMath;

int main()
{
    mathS::NMath::NList* nlist1 = new mathS::NMath::NList({ 3.1, 2.4, 5.6 });
    mathS::NMath::NList* nlist2 = new mathS::NMath::NList({new NAtom(1.1),new NAtom(4.3), new NList({1.1,2.2,3.3})});
    std::cout << Plus(nlist1, nlist2)->GetString() << std::endl;
    std::cout << "Hello World!\n";
}