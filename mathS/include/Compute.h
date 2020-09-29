#pragma once

#include <Rule.h>

/*
	使用某些策略运用一系列Rule并返回一个MathObject，我们把这称为Compute。Compute的参数和返回类型与Rule相同，但作用不一样
	
	对于每种MathObject，都有默认的Compute操作
	
	Power：(a*b)^e=a^e*b^e展开，(a^e1)^e2=a^(e1*e2)，常数自动计算，多项式常整数幂展开，化简
	Item: Vec分配率展开(a+b)*c=a*c+b*c，化简：倒数相消a/a=1，常数自动计算
	Polynomial：常数自动计算，合并同类项
	
	Function FunctionalOperator根据函数名称的不同，做不同的计算。未知的函数不做计算。

	Compare 做简单的化简处理，如果全是常数，直接算出结果



*/

namespace mathS{

	/// <summary>
	/// 总 Compute， 在这里分类型
	/// </summary>
	/// <param name="input"></param>
	/// <returns></returns>
	Ptr<MathObject> Compute(const Ptr<MathObject>& input);

	Ptr<MathObject> Compute(const Ptr<Item>& input);


}