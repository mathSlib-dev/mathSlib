#include <RuleLib.h>
#include <ConstantEvaluate.h>

bool mathS::RuleLib::VectorMultiply(Ptr<MathObject> obj, Ptr<MathObject>& rst)
{
	// 判断类型，找出向量
	if (obj->GetType() != MathObject::ITEM)
		return false;
	auto& obj_factors = Dynamic_cast<Item>(obj)->factors;
	if (obj_factors.size() <= 1)
		return false;
	std::vector<ptrdiff_t> vec_idx;
	std::vector<Ptr<Vector>> vecs;
	ptrdiff_t vecshape;
	for (ptrdiff_t i = 0; i < obj_factors.size(); i++)
		if (obj_factors[i]->GetType() == MathObject::VECTOR) {
			auto v = Dynamic_cast<Vector>(obj_factors[i]);
			// 检查形状
			if (vec_idx.empty())
				vecshape = v->components.size();
			else
				if (v->components.size() != vecshape) return false;	// 形状不一致，不能乘开
			vec_idx.push_back(i);
			vecs.push_back(v);
		}
	if (vecs.size() == 0)
		return false;
	// 计算展开结果
	auto vec_rst = New<Vector>();
	vec_rst->components.reserve(vecshape);
	for (ptrdiff_t i = 0; i < vecshape; i++) {
		Ptr<Item> itm = New<Item>();
		for (ptrdiff_t k = 0; k < vec_idx[0]; k++)
			itm->push_back(obj_factors[k]->DeepCopy());
		itm->push_back(vecs[0]->components[i]->DeepCopy());
		for (ptrdiff_t j = 1; j < vec_idx.size(); j++) {
			for (ptrdiff_t k = vec_idx[j - 1] + 1; k < vec_idx[j]; k++)
				itm->push_back(obj_factors[k]);
			itm->push_back(vecs[j]->components[i]->DeepCopy());
		}
		for (ptrdiff_t k = vec_idx.back() + 1; k < obj_factors.size(); k++)
			itm->push_back(obj_factors[k]);

		vec_rst->push_back(itm);
	}
	rst = vec_rst;
	return true;
}

bool mathS::RuleLib::ExpandDistributive(Ptr<MathObject> obj, Ptr<MathObject>& rst)
{
	// 找到第一个多项式
	if (obj->GetType() != MathObject::ITEM)
		return false;
	auto& obj_factors = Dynamic_cast<Item>(obj)->factors;
	if (obj_factors.size() <= 1)
		return false;
	ptrdiff_t poly_idx = 0;
	while (poly_idx < obj_factors.size() && obj_factors[poly_idx]->GetType() != MathObject::POLYNOMIAL)
		poly_idx++;
	if (poly_idx >= obj_factors.size())
		return false;

	// 计算展开结果
	auto& poly_items = Dynamic_cast<Polynomial>(obj_factors[poly_idx])->items;
	auto poly_rst = New<Polynomial>();
	poly_rst->items.reserve(poly_items.size());
	for (ptrdiff_t i = 0; i < poly_items.size(); i++) {
		Ptr<Item> itm = New<Item>();
		for (ptrdiff_t j = 0; j < poly_idx; j++)
			itm->push_back(obj_factors[j]->DeepCopy());
		itm->push_back(poly_items[i]->DeepCopy());
		for (ptrdiff_t j = poly_idx + 1; j < obj_factors.size(); j++)
			itm->push_back(obj_factors[j]->DeepCopy());

		poly_rst->push_back(itm);
	}
	rst = poly_rst;
	return true;
}

bool mathS::RuleLib::ConstantPlus(Ptr<MathObject> obj, Ptr<MathObject>& rst)
{
	static auto src_pattern = Parse("#a+#b+_RES_");
	std::map<std::string, Ptr<MathObject>> table;
	std::list<std::string> table_list;
	if (!DoMatch(src_pattern, obj, table, table_list))
		return false;
	Ptr<Polynomial> poly_rst = New<Polynomial>();
	auto c = ConstantEvaluate::Plus(Dynamic_cast<Atom, MathObject>(table["#a"]), Dynamic_cast<Atom, MathObject>(table["#b"]));
	if (!c->IsZero())
		poly_rst->push_back(c);
	if (!table["_RES_"]->IsZero())
		poly_rst->push_back(table["_RES_"]->DeepCopy());
	rst = ReducePolynomial(poly_rst);
	return true;
}

bool mathS::RuleLib::ConstantNegative(Ptr<MathObject> obj, Ptr<MathObject>& rst)
{
	if (obj->GetType() != MathObject::Type::OPPOSITE)
		return false;
	auto ops_obj = Dynamic_cast<Opposite, MathObject>(obj);
	if (ops_obj->component->GetType() != MathObject::Type::ATOM)
		return false;
	auto atom_obj = Dynamic_cast<Atom, MathObject>(ops_obj->component);
	if (atom_obj->AtomType() != MathObject::NUMBER)
		return false;
	rst = ConstantEvaluate::Negative(atom_obj);
	return true;
}

bool mathS::RuleLib::ConstantMultiply(Ptr<MathObject> obj, Ptr<MathObject>& rst)
{
	static auto src_pattern = Parse("#a*#b*_RES_");
	std::map<std::string, Ptr<MathObject>> table;
	std::list<std::string> table_list;
	if (!DoMatch(src_pattern, obj, table, table_list))
		return false;
	Ptr<Item> itm_rst = New<Item>();
	auto c = ConstantEvaluate::Multyply(Dynamic_cast<Atom, MathObject>(table["#a"]), Dynamic_cast<Atom, MathObject>(table["#b"]));
	if (!c->IsZero())
		itm_rst->push_back(c);
	if (!table["_RES_"]->IsZero())
		itm_rst->push_back(table["_RES_"]->DeepCopy());
	rst = ReduceItem(itm_rst);
	return true;
}

bool mathS::RuleLib::Drop_ones(Ptr<MathObject> obj, Ptr<MathObject>& rst)
{
	if (obj->GetType() != MathObject::Type::ITEM)
		return false;
	auto itm_obj = Dynamic_cast<Item, MathObject>(obj);
	if (itm_obj->factors.size() == 1)
		return false;
	bool flag = false;
	for (size_t i = 0; i < itm_obj->factors.size(); i++) {
		if (itm_obj->factors[i]->IsOne()) {
			flag = true;
			break;
		}
	}
	if (flag) {
		Ptr<Item> itm_rst = New<Item>();
		for (size_t i = 0; i < itm_obj->factors.size(); i++) {
			if (!itm_obj->factors[i]->IsOne())
				itm_rst->push_back(itm_obj->factors[i]);
		}
		rst = ReduceItem(itm_rst);
		return true;
	}
	else {
		return false;
	}
}

bool mathS::RuleLib::ExpandItemPower(Ptr<MathObject> obj, Ptr<MathObject>& rst)
{
	if (obj->GetType() != MathObject::POWER)
		return false;
	auto pow_obj = Dynamic_cast<Power>(obj);
	if (pow_obj->base->GetType() != MathObject::ITEM)
		return false;
	auto itm_base = Dynamic_cast<Item>(pow_obj->base);
	auto itm_rst = New<Item>();
	for (auto it : itm_base->factors) {
		itm_rst->push_back(New<Power>(it->DeepCopy(), pow_obj->exponent->DeepCopy()));
	}
	rst = itm_rst;
	return true;
}

bool mathS::RuleLib::VectorPower(Ptr<MathObject> obj, Ptr<MathObject>& rst)
{
	if (obj->GetType() != MathObject::POWER)
		return false;
	auto pow_obj = Dynamic_cast<Power>(obj);
	
	if (pow_obj->base->GetType() == MathObject::VECTOR) {
		auto vec_rst = New<Vector>();
		auto& base_components = Dynamic_cast<Vector>(pow_obj->base)->components;
		if (pow_obj->exponent->GetType() == MathObject::VECTOR) {
			auto& exp_components = Dynamic_cast<Vector>(pow_obj->exponent)->components;
			if (base_components.size() != exp_components.size())
				return false;
			vec_rst->components.reserve(base_components.size());
			for (ptrdiff_t i = 0; i < base_components.size(); i++) {
				vec_rst->push_back(New<Power>(base_components[i]->DeepCopy(), exp_components[i]->DeepCopy()));
			}
		}
		else {
			for (ptrdiff_t i = 0; i < base_components.size(); i++) {
				vec_rst->push_back(New<Power>(base_components[i]->DeepCopy(), pow_obj->exponent->DeepCopy()));
			}
		}
		rst = vec_rst;
	}
	else {
		if (pow_obj->exponent->GetType() == MathObject::VECTOR) {
			auto vec_rst = New<Vector>();
			auto& exp_components = Dynamic_cast<Vector>(pow_obj->exponent)->components;
			for (ptrdiff_t i = 0; i < exp_components.size(); i++) {
				vec_rst->push_back(New<Power>(pow_obj->base->DeepCopy(), exp_components[i]->DeepCopy()));
			}
			rst = vec_rst;
		}
		else {
			return false;
		}
	}
	
}

bool mathS::RuleLib::Drop_zeros(Ptr<MathObject> obj, Ptr<MathObject>& rst)
{
	if (obj->GetType() != MathObject::Type::POLYNOMIAL)
		return false;
	auto poly_obj = Dynamic_cast<Polynomial, MathObject>(obj);
	if (poly_obj->items.size() == 1)
		return false;
	bool flag = false;
	for (size_t i = 0; i < poly_obj->items.size(); i++) {
		if (poly_obj->items[i]->IsZero()) {
			flag = true;
			break;
		}
	}
	if (flag) {
		Ptr<Polynomial> poly_rst = New<Polynomial>();
		for (size_t i = 0; i < poly_obj->items.size(); i++) {
			if (!poly_obj->items[i]->IsZero())
				poly_rst->push_back(poly_obj->items[i]);
		}
		rst = ReducePolynomial(poly_rst);
		return true;
	}
	else {
		return false;
	}
	
}
