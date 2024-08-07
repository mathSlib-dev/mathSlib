#pragma once

#include "Rule.h"

namespace mathS {
	// ����⣬���������д����ֱ�����ɵĹ��򣬻��Ǳ���Ҫ������еĹ���
	namespace RuleLib {

		// �� Item �еĵ�һ�������˷�չ��
		bool VectorMultiply(Ptr<MathObject> obj, Ptr<MathObject>& rst);
		// ���˷������ɶ� Item �е�һ��Polynomial�ӷ�չ��
		bool ExpandDistributive(Ptr<MathObject> obj, Ptr<MathObject>& rst);

		// ��������
		bool ConstantPlus(Ptr<MathObject> obj, Ptr<MathObject>& rst);
		bool ConstantNegative(Ptr<MathObject> obj, Ptr<MathObject>& rst);
		bool ConstantMultiply(Ptr<MathObject> obj, Ptr<MathObject>& rst);

		// Item ͬ�����ݺϲ�
		static Rule Power_reduction_1 = MakeRule(Parse("_a/_a*_RES_"), Parse("_RES_"));
		static Rule Power_reduction_2 = MakeRule(Parse("_a*_a*_RES_"), Parse("_a^2*_RES_"));
		static Rule Power_reduction_3 = MakeRule(Parse("_a^_e*_a*_RES_"), Parse("_RES_*_a^(_e+1)"));
		static Rule Power_reduction_4 = MakeRule(Parse("_a^_e/_a^_*_RES_"), Parse("_RES_*_a^(_e-1)"));
		static Rule Power_reduction_5 = MakeRule(Parse("_a^_e1*_a^_e2*_RES_"), Parse("_RES_*_a^(_e1+_e2)"));
		static Rule Power_reduction_6 = MakeRule(Parse("_a^_e1/_a^_e2*_RES_"), Parse("_RES_*_a^(_e1-_e2)"));
		// ȥ�� 1
		bool Drop_ones(Ptr<MathObject> obj, Ptr<MathObject>& rst);

		// ������˷�����
//		bool ConstantMultiply(Ptr<MathObject> obj, Ptr<MathObject>& rst);
		// ��������ӷ�ĸԼ��
		bool ConstantFractionReduction(Ptr<MathObject> obj, Ptr<MathObject>& rst);

		static Rule Power_simplify = MakeRule(Parse("(_a^_e1)^_e2"), Parse("_a^(_e1*_e2)"));
		
		// (a*b)^e=a^e*b^e
		bool ExpandItemPower(Ptr<MathObject> obj, Ptr<MathObject>& rst);
		// {a,b}^{c,d}��{a,b}^c��a^{b,c}
		bool VectorPower(Ptr<MathObject> obj, Ptr<MathObject>& rst);

		// ����ʽ����
		static Rule Reduce_opposite_terms = MakeRule(Parse("_a-_a+_RES_"), Parse("_RES_"));
		static Rule Combining_similar_terms = MakeRule(Parse("#a*_res_+#b*_res_+_RES_"), Parse("(#a+#b)*_res_+_RES_"));
		bool Drop_zeros(Ptr<MathObject> obj, Ptr<MathObject>& rst);
		
		// ���Ż���
		static Rule Double_negative = MakeRule(Parse("-(-_a)"), Parse("_a"));

		static Rule Double_inverse_1 = MakeRule(Parse("/(/_a)"), Parse("_a"));
		static Rule Double_inverse_2 = MakeRule(Parse("/(/_a*_RES_)"), Parse("_a/_RES_"));

	};
}