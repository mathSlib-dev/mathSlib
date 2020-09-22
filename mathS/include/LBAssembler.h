#pragma once

#include<MathObject.h>
#include<NMathObject.h>
#include<NFunction.h>
#include<map>
/*
	In this file, we introduce methods that use lambda expression to
	assemble a NMathFunction(numeric math function) with given MathObject.
	Also, the NMathFunction can be converted into a cpp function if you wish.
*/

namespace mathS
{

	class Assembler
	{

	public:

	
	private:
		
		// 考虑到将来可能要在一个脚本运行时组装函数，也许，不是从function_table, constant_table中来查找，
		// 而是从运行时的环境中查找所需要的函数、常量、变量等，会更合适，因为运行脚本时，用户可能自定义了的数值函数。
		// 但是由于这样的脚本执行器还没有搭建起来， 就先在Assembler里存储function_table和constant_table。

		// 函数表。组装时需要的标准函数直接根据函数名称从函数表里取
		std::map<std::string, NMath::NFunction> function_table;
		// 常量表。组装时遇到常量符号，会从常量表里取。
		std::map<std::string, NMath::NMathObject> constant_table;

		// 根据表达式expr，组装一个以expr中的出现的变量为参数的函数。参数的顺序是param_str所给出的。
		// 如 expr = sin(u)*v, params={u, v}，那么返回的函数就是f(x1,x2)=sin(x1)*x2.
		NMath::NFunction Assemble(Ptr<MathObject> expr, const std::vector<std::string>& params);
		
	};
}