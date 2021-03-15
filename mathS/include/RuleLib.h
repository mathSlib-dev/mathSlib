#pragma once

#include <Rule.h>

namespace mathS {
	// 规则库，在这里可以写包括直接生成的规则，或是必须要程序进行的规则
	namespace RuleLib {

		// 对 Item 中的第一个向量乘法展开
		bool VectorMultiply(Ptr<MathObject> obj, Ptr<MathObject>& rst);
		// 按乘法分配律对 Item 中第一个Polynomial加法展开
		bool ExpandDistributive(Ptr<MathObject> obj, Ptr<MathObject>& rst);

		// 常量化简
		bool ConstantPlus(Ptr<MathObject> obj, Ptr<MathObject>& rst);
		bool ConstantNegative(Ptr<MathObject> obj, Ptr<MathObject>& rst);
		bool ConstantMultiply(Ptr<MathObject> obj, Ptr<MathObject>& rst);

		// Item 同底数幂合并
		static Rule Power_reduction_1 = MakeRule(Parse("_a/_a*_RES_"), Parse("_RES_"));
		static Rule Power_reduction_2 = MakeRule(Parse("_a*_a*_RES_"), Parse("_a^2*_RES_"));
		static Rule Power_reduction_3 = MakeRule(Parse("_a^_e*_a*_RES_"), Parse("_RES_*_a^(_e+1)"));
		static Rule Power_reduction_4 = MakeRule(Parse("_a^_e/_a^_*_RES_"), Parse("_RES_*_a^(_e-1)"));
		static Rule Power_reduction_5 = MakeRule(Parse("_a^_e1*_a^_e2*_RES_"), Parse("_RES_*_a^(_e1+_e2)"));
		static Rule Power_reduction_6 = MakeRule(Parse("_a^_e1/_a^_e2*_RES_"), Parse("_RES_*_a^(_e1-_e2)"));
		// 去掉 1
		bool Drop_ones(Ptr<MathObject> obj, Ptr<MathObject>& rst);

		// 常数项乘法计算
//		bool ConstantMultiply(Ptr<MathObject> obj, Ptr<MathObject>& rst);
		// 常数项分子分母约分
		bool ConstantFractionReduction(Ptr<MathObject> obj, Ptr<MathObject>& rst);

		static Rule Power_simplify = MakeRule(Parse("(_a^_e1)^_e2"), Parse("_a^(_e1*_e2)"));
		
		// (a*b)^e=a^e*b^e
		bool ExpandItemPower(Ptr<MathObject> obj, Ptr<MathObject>& rst);
		// {a,b}^{c,d}或{a,b}^c或a^{b,c}
		bool VectorPower(Ptr<MathObject> obj, Ptr<MathObject>& rst);

		// 多项式化简
		static Rule Reduce_opposite_terms = MakeRule(Parse("_a-_a+_RES_"), Parse("_RES_"));
		static Rule Combining_similar_terms = MakeRule(Parse("#a*_res_+#b*_res_+_RES_"), Parse("(#a+#b)*_res_+_RES_"));
		bool Drop_zeros(Ptr<MathObject> obj, Ptr<MathObject>& rst);
		
		// 负号化简
		static Rule Double_negative = MakeRule(Parse("-(-_a)"), Parse("_a"));

		static Rule Double_inverse_1 = MakeRule(Parse("/(/_a)"), Parse("_a"));
		static Rule Double_inverse_2 = MakeRule(Parse("/(/_a*_RES_)"), Parse("_a/_RES_"));

	};
}