#pragma once

#include <Rule.h>

/*
	使用某些策略运用一系列Rule并返回一个MathObject，我们把这称为Evaluate。Evaluate的参数和返回类型与Rule相同，但作用不一样
	
	对于每种MathObject，都有默认的Evaluate操作
	

	如果Evaluate是Ptr<MathObject>(Ptr<MathObject>)的，实现逻辑是：Evaluate返回的表达式一定是不能再Evaluate的表达式。
	这个逻辑很简单，但有可能导致问题：
	例如对于一个Item，应用某些规则可能会导致它不再是Item，所以在Evaluate(Item)内部，发现Rule应用改变了结构，又要重新Evaluate，
	这意味着不同的Evaluate之间的执行关系，不是顺序的，而是在栈中层层调用的。如果一个表达式计算步骤很多，有可能导致stack overflow
	最大计算步骤非常有限，如果某个表达式计算需要非常多的步数，不到1秒程序就会栈溢出。

	如果程序的实现逻辑像下面这样，就不会有栈溢出的问题
	我们令Evaluate 是 bool(Ptr<MathObject> input, Ptr<MathObject>& result)，一个Evaluate只是一步运算或几步运算的尝试，并不能保证表达式已经计算完毕
	如果对象还可能需要继续计算，则返回 true. 如果已经计算完成则返回 false.
	Evaluate的引用参数返回涉及到input中的内容，一定是拷贝。即result和input是独立的，不用担心他们的子表达式之间有耦合。

	Evaluate 的常见用法：
	计算obj,并直接把结果返回给obj，丢弃计算前的obj：Evaluate(obj, obj).
	对于一个对象obj完成全部计算（直至不再修改）：while(Evaluate(obj, obj));
	全部完成计算，并判断是否修改while(Evaluate(obj))flag = true;


	对于一些结构，Evaluate要做的事
	Power：(a*b)^e=a^e*b^e展开，(a^e1)^e2=a^(e1*e2)，常数自动计算，多项式常整数幂展开，化简
	Item: Vec分配率展开(a+b)*c=a*c+b*c，化简：倒数相消a/a=1，常数自动计算
	Polynomial：常数自动计算，合并同类项
	
	Function FunctionalOperator根据函数名称的不同，做不同的计算。未知的函数不做计算。

	Compare 做简单的化简处理，如果全是常数，直接算出结果（True常量或False常量）

	...

*/

namespace mathS{

	/// <summary>
	/// 总 Evaluate
	/// </summary>
	/// <param name="input"></param>
	/// <returns></returns>
	bool Evaluate(Ptr<MathObject> input, Ptr<MathObject>& result);


	// 各个类型的Evaluate
	bool Evaluate(Ptr<Vector>, Ptr<MathObject>& result);
	bool Evaluate(Ptr<Function> input, Ptr<MathObject>& result);
	bool Evaluate(Ptr<FunctionalOperator> input, Ptr<MathObject>& result);
	bool Evaluate(Ptr<Locate> input, Ptr<MathObject>& result);
	bool Evaluate(Ptr<Power> input, Ptr<MathObject>& result);
	bool Evaluate(Ptr<Inverse> input, Ptr<MathObject>& result);
	bool Evaluate(Ptr<Item> input, Ptr<MathObject>& result);
	bool Evaluate(Ptr<Opposite> input, Ptr<MathObject>& result);
	bool Evaluate(Ptr<Polynomial> input, Ptr<MathObject>& result);
	bool Evaluate(Ptr<Map>, Ptr<MathObject>& result);
	bool Evaluate(Ptr<Compare> input, Ptr<MathObject>& result);
	

}