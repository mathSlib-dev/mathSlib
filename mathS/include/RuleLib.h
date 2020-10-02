#pragma once

#include <Rule.h>

namespace mathS {
	// 规则库，在这里可以写包括直接生成的规则，或是必须要程序进行的规则
	namespace RuleLib {

		// 对 Item 中的第一个向量乘法展开
		bool VectorMultiply(Ptr<MathObject> obj, Ptr<MathObject>& rst);
		// 按乘法分配律对 Item 中第一个Polynomial加法展开
		bool ExpandDistributive(Ptr<MathObject> obj, Ptr<MathObject>& rst);

		// Item 同底数幂合并
		static Rule Power_reduction_1 = MakeRule(Parse("_a/_a*_RES_"), Parse("_RES_"));
		static Rule Power_reduction_2 = MakeRule(Parse("_a*_a*_RES_"), Parse("_a^2_RES_"));
		static Rule Power_reduction_3 = MakeRule(Parse("_a^_e*_a*_RES_"), Parse("_RES_*_a^(_e+1)"));
		static Rule Power_reduction_4 = MakeRule(Parse("_a^_e/_a^_*_RES_"), Parse("_RES_*_a^(_e-1)"));
		static Rule Power_reduction_5 = MakeRule(Parse("_a^_e1*_a^_e2*_RES_"), Parse("_RES_*_a^(_e1+_e2)"));
		static Rule Power_reduction_6 = MakeRule(Parse("_a^_e1/_a^_e2*_RES_"), Parse("_RES_*_a^(_e1-_e2)"));

		// 常数项乘法计算
		bool ConstantMultiply(Ptr<MathObject> obj, Ptr<MathObject>& rst);
		// 常数项分子分母约分
		bool ConstantFractionReduction(Ptr<MathObject> obj, Ptr<MathObject>& rst);

	};
}