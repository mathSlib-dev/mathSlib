#include "Rule.h"

using namespace mathS;

Match mathS::MakeMatch(Ptr<MathObject> pattern)
{
	return [pattern](Ptr<MathObject> obj) {
		std::map<std::string, Ptr<MathObject>> table;
		std::list<std::string> table_list;
		return DoMatch(pattern, obj, table, table_list);
	};
}

Rule mathS::MakeRule(Ptr<MathObject> src_pattern, Ptr<MathObject> tar_pattern)
{
	return [src_pattern, tar_pattern](Ptr<MathObject> obj, Ptr<MathObject>& rst) {
		std::map<std::string, Ptr<MathObject>> table;
		std::list<std::string> table_list;
		if (!DoMatch(src_pattern, obj, table, table_list))
			return false;
		rst = DoReplace(tar_pattern, table);
		return true;
	};
}

bool mathS::DoMatch(Ptr<MathObject> pattern, Ptr<MathObject> obj, std::map<std::string, Ptr<MathObject>>& table, std::list<std::string>& table_list)
{
	// 通配符匹配表
	MathObject::Type mtype = pattern->GetType();
	
	switch (mtype) {
	case MathObject::ATOM: {
		// 如果是Atom, 考虑是匹配变量名还是
		Ptr<Atom> atom_pattern = Dynamic_cast<Atom>(pattern);
		switch (atom_pattern->str[0]) {
		case '_': {
			auto it = table.find(atom_pattern->str);
			if (it == table.end()) {
				table.insert(std::make_pair(atom_pattern->str, obj));
				table_list.push_back(atom_pattern->str);
			}
			else {
				if (!FullCompare(it->second, obj))
					return false;
			}
			break;
		}
		case '@': {
			if (obj->GetType() != MathObject::ATOM)
				return false;
			auto it = table.find(atom_pattern->str);
			auto atom_obj = Dynamic_cast<Atom>(obj);
			if (it == table.end()) {
				if (atom_obj->AtomType() != MathObject::VARIABLE)
					return false;
				table.insert(std::make_pair(atom_pattern->str, obj));
				table_list.push_back(atom_pattern->str);
			}
			else {
				if (it->second->GetString() != atom_obj->str)
					return false;
			}
			break;
		}
		case '#': {
			if (obj->GetType() != MathObject::ATOM)
				return false;
			auto it = table.find(atom_pattern->str);
			auto atom_obj = Dynamic_cast<Atom>(obj);
			if (it == table.end()) {
				if (atom_obj->AtomType() != MathObject::NUMBER)
					return false;
				table.insert(std::make_pair(atom_pattern->str, obj));
				table_list.push_back(atom_pattern->str);
			}
			else {
				if (it->second->GetString() != atom_obj->str)
					return false;
			}
			break;
		}
		case '$': {
			if (obj->GetType() != MathObject::ATOM)
				return false;
			auto it = table.find(atom_pattern->str);
			auto atom_obj = Dynamic_cast<Atom>(obj);
			if (it == table.end()) {
				if (atom_obj->AtomType() != MathObject::STRING)
					return false;
				table.insert(std::make_pair(atom_pattern->str, obj));
				table_list.push_back(atom_pattern->str);
			}
			else {
				if (it->second->GetString() != atom_obj->str)
					return false;
			}
			break;
		}
		default: {
			// 是变量名，要求obj对应也是完全相同的变量名
			if (obj->GetType() != MathObject::ATOM)
				return false;
			if (atom_pattern->str != obj->GetString())
				return false;
			break;
		}
		}
		break;
	}
	case MathObject::VECTOR: {
		if (obj->GetType() != MathObject::VECTOR)		// 确认类型
			return false;
		Ptr<Vector> vec_pattern = Dynamic_cast<Vector>(pattern);
		Ptr<Vector> vec_obj = Dynamic_cast<Vector>(obj);
		// 分别比较子表达式
		if (vec_pattern->components.size() != vec_obj->components.size())
			return false;
		for(int i = 0; i<vec_pattern->components.size();i++)
			if (!DoMatch(vec_pattern->components[i], vec_obj->components[i], table, table_list)) return false;
		break;
	}
	case MathObject::FUNCTION: {
		// 匹配函数
		if (obj->GetType() != MathObject::FUNCTION)		// 确认类型
			return false;
		Ptr<Function> func_pattern = Dynamic_cast<Function>(pattern);
		Ptr<Function> func_obj = Dynamic_cast<Function>(obj);
		// 分别比较子表达式
		if (func_pattern->parameter.size() != func_obj->parameter.size())
			return false;
		if (!DoMatch(func_pattern->function, func_obj->function, table, table_list))
			return false;
		for (int i = 0; i < func_pattern->parameter.size(); i++) {
			if (!DoMatch(func_pattern->parameter[i], func_obj->parameter[i], table, table_list))
				return false;
		}
		break;
	}
	case MathObject::FUNCOPERATOR: {
		if (obj->GetType() != MathObject::FUNCOPERATOR)
			return false;
		Ptr<FunctionalOperator> fop_pattern = Dynamic_cast<FunctionalOperator>(pattern);
		Ptr<FunctionalOperator> fop_obj = Dynamic_cast<FunctionalOperator>(obj);
		if (fop_pattern->parameter.size() != fop_obj->parameter.size() || fop_pattern->variables.size() != fop_obj->variables.size() || fop_pattern->fparameter.size() != fop_obj->fparameter.size())
			return false;
		if (!DoMatch(fop_pattern->function, fop_obj->function, table, table_list)) 
			return false;
		for (int i = 0; i < fop_pattern->variables.size(); i++) 
			if (!DoMatch(fop_pattern->variables[i], fop_obj->variables[i], table, table_list)) return false;
		for (int i = 0; i < fop_pattern->fparameter.size(); i++) 
			if (!DoMatch(fop_pattern->fparameter[i], fop_obj->fparameter[i], table, table_list)) return false;
		for (int i = 0; i < fop_pattern->parameter.size(); i++) 
			if (!DoMatch(fop_pattern->parameter[i], fop_obj->parameter[i], table, table_list)) return false;
		break;
	}
	case  MathObject::POWER: {
		if (obj->GetType() != MathObject::POWER)
			return false;
		Ptr<Power> pow_pattern = Dynamic_cast<Power>(pattern);
		Ptr<Power> pow_obj = Dynamic_cast<Power>(obj);
		if (!DoMatch(pow_pattern->base, pow_obj->base, table, table_list))
			return false;
		if (!DoMatch(pow_pattern->exponent, pow_obj->exponent, table, table_list))
			return false;
		break;
	}
	case MathObject::INVERSE: {
		if (obj->GetType() != MathObject::INVERSE)
			return false;
		Ptr<Inverse> inv_pattern = Dynamic_cast<Inverse>(pattern);
		Ptr<Inverse> inv_obj = Dynamic_cast<Inverse>(obj);
		if (!DoMatch(inv_pattern->component, inv_obj->component, table, table_list))
			return false;
		break;
	}
	case MathObject::ITEM: {
		if (obj->GetType() != MathObject::ITEM)		// 确认类型
			return false;
		auto& pattern_factors = Dynamic_cast<Item>(pattern)->factors;
		auto& obj_factors = Dynamic_cast<Item>(obj)->factors;
		
		// 判断是否有残项
		std::string residual;
		bool with_residual = false;
		if (pattern_factors.back()->GetType() == MathObject::ATOM) {
			 residual = pattern_factors.back()->GetString();
			if (residual.size() > 2 && residual[0] == '_' && residual[residual.size() - 1] == '_') {
				with_residual = true;
			}
		}
		// 要匹配 pattern 中项的个数。没有残项就全部，有残项，不匹配残项。
		int matchsize = with_residual ? pattern_factors.size() - 1 : pattern_factors.size();
		if (matchsize > obj_factors.size())	// 判断长短
			return false;

		std::vector<bool> md(obj_factors.size(), false);	// 互斥约束
		std::vector<std::list<int>> feasible; // 直接约束的可行集合
		feasible.resize(matchsize);
		int table_size = table_list.size();
		// 先得到直接约束的可行集合
		for (int i = 0; i < matchsize; i++) {
			for (int j = 0; j < obj_factors.size(); j++) {
				if (DoMatch(pattern_factors[i], obj_factors[j], table, table_list))
					feasible[i].push_back(j);
				// 恢复匹配表
				while (table.size() > table_size) {
					table.erase(table_list.back());
					table_list.pop_back();
				}
			}
			if (feasible[i].empty())
				return false;
		}
		// 对互斥约束前向检验，枚举剩下的情况
		std::vector<std::list<int>::iterator> it;
		for (auto& jt : feasible)
			it.push_back(jt.begin());
		std::vector<int> table_sizes;
		table_sizes.resize(matchsize);
		table_sizes[0] = table_list.size();
		for (int i = 0; i < matchsize;) {
			// 恢复匹配表至 i 未匹配的状态
			while (table_list.size() > table_sizes[i]) {
				table.erase(table_list.back());
				table_list.pop_back();
			}
			// 本层要赋值的变量 pattern_factor[i]， 值域为其可行的匹配 *it[i] : feasible[i]
			while (it[i] != feasible[i].end()) {
				// 必须满足互斥约束，且匹配成功，则选中，跳出选下一层。否则枚举本层的下一个
				if ((!md[*it[i]]) && DoMatch(pattern_factors[i], obj_factors[*it[i]], table, table_list))
					break;
				it[i]++;
				// 本次匹配尝试失败也要恢复匹配表
				while (table_list.size() > table_sizes[i]) {
					table.erase(table_list.back());
					table_list.pop_back();
				}
			}
			if (it[i] != feasible[i].end()) {
				// 当前变量已经赋值，对下一个变量进行赋值
				md[*it[i]] = true;
				if (i == matchsize - 1) break;		// 已经全部赋值
				table_sizes[i + 1] = table_list.size();	
				i++;
			}
			else {
				// 没有可选的，返回上一个变量
				if (i == 0) return false;
				it[i] = feasible[i].begin();
				i--;
				md[*it[i]] = false;
				it[i]++;
			}
		}
		// 处理残项
		if (with_residual) {
			Ptr<MathObject> factors_residual ;
			if (matchsize == obj_factors.size()) {
				// residual 若为空，至少给一个1
				factors_residual = New<Atom>("1");
			}
			else {
				Ptr<Item> item_res = New<Item>();
				for (int j = 0; j < obj_factors.size(); j++) {
					if (!md[j])
						item_res->factors.push_back(obj_factors[j]);
				}
				factors_residual = item_res;
			}
			// 比较table中的项
			auto itres = table.find(residual);
			if (itres == table.end()) {
				table[residual] = factors_residual;
				table_list.push_back(residual);
			}
			else {
				if (!FullCompare(itres->second, factors_residual))
					return false;
			}
		}
		break;
	}
	case MathObject::OPPOSITE: {
		if (obj->GetType() != MathObject::OPPOSITE)
			return false;
		Ptr<Opposite> oppo_pattern = Dynamic_cast<Opposite>(pattern);
		Ptr<Opposite> oppo_obj = Dynamic_cast<Opposite>(obj);
		if (!DoMatch(oppo_pattern->component, oppo_obj->component, table, table_list))
			return false;
		break;
	}
	case MathObject::POLYNOMIAL: {
		if (obj->GetType() != MathObject::POLYNOMIAL)		// 确认类型
			return false;
		auto& pattern_items = Dynamic_cast<Polynomial>(pattern)->items;
		auto& obj_items = Dynamic_cast<Polynomial>(obj)->items;
		// 判断是否有残项
		std::string residual;
		bool with_residual = false;
		if (pattern_items.back()->GetType() == MathObject::ATOM) {
			residual = pattern_items.back()->GetString();
			if (residual.size() > 2 && residual[0] == '_' && residual[residual.size() - 1] == '_') {
				with_residual = true;
			}
		}
		// 要匹配 pattern 中项的个数。没有残项就全部，有残项，不匹配残项。
		int matchsize = with_residual ? pattern_items.size() - 1 : pattern_items.size();
		if (matchsize > obj_items.size())	// 判断长短
			return false;

		std::vector<bool> md(obj_items.size(), false);	// 是否已经配过
		std::vector<std::list<int>> feasible; // 直接约束的可行集合
		feasible.resize(matchsize);
		int table_size = table_list.size();
		// 先得到直接约束的可行集合
		for (int i = 0; i < matchsize; i++) {
			for (int j = 0; j < obj_items.size(); j++) {
				if (DoMatch(pattern_items[i], obj_items[j], table, table_list))
					feasible[i].push_back(j);
				// 恢复匹配表
				while (table.size() > table_size) {
					table.erase(table_list.back());
					table_list.pop_back();
				}
			}
			if (feasible[i].empty())
				return false;
		}
		// 对互斥约束前向检验，枚举剩下的情况
		std::vector<std::list<int>::iterator> it;
		for (auto& jt : feasible)
			it.push_back(jt.begin());
		std::vector<int> table_sizes;
		table_sizes.resize(matchsize);
		table_sizes[0] = table_list.size();
		for (int i = 0; i < matchsize;) {
			// 恢复匹配表至 i 未匹配的状态
			while (table_list.size() > table_sizes[i]) {
				table.erase(table_list.back());
				table_list.pop_back();
			}
			while (it[i] != feasible[i].end()) {
				// 必须满足互斥约束，并且能Match，则认为选中，跳出选下一层。否则尝试本层的下一个
				if ((!md[*it[i]]) && DoMatch(pattern_items[i], obj_items[*it[i]], table, table_list)) 
					break;
				it[i]++;
				// 本次匹配尝试失败也要恢复匹配表
				while (table_list.size() > table_sizes[i]) {
					table.erase(table_list.back());
					table_list.pop_back();
				}
			}
			if (it[i] != feasible[i].end()) {
				// 赋值可行
				md[*it[i]] = true;
				// 全部赋值完毕，得到可行解
				if (i == matchsize - 1) break;
				table_sizes[i + 1] = table_list.size();
				i++;
			}
			else {
				// 没有可选的，返回上一个变量
				if (i == 0) return false;
				it[i] = feasible[i].begin();
				i--;
				md[*it[i]] = false;
				it[i]++;
			}
		}
		// 处理残项
		if (with_residual) {
			Ptr<MathObject> items_residual;
			if (matchsize == obj_items.size()) {
				// items_residual若为空，则至少给一个0
				items_residual = New<Atom>("0");
			}
			else {
				Ptr<Polynomial> poly_res = New<Polynomial>();
				for (int j = 0; j < obj_items.size(); j++) {
					if (!md[j])
						poly_res->items.push_back(obj_items[j]);
				}
				items_residual = poly_res;
			}
			// 比较table中的项
			auto itres = table.find(residual);
			if (itres == table.end()) {
				table[residual] = items_residual;
				table_list.push_back(residual);
			}
			else {
				if(!FullCompare(itres->second, items_residual))
					return false;
			}
		}
		break;
	}
	case MathObject::MAP: {
		if (obj->GetType() != MathObject::MAP)
			return false;
		Ptr<Map> map_pattern = Dynamic_cast<Map>(pattern);
		Ptr<Map> map_obj = Dynamic_cast<Map>(obj);
		if (!DoMatch(map_pattern->key, map_obj->key, table, table_list))
			return false;
		if (!DoMatch(map_pattern->value, map_obj->value, table, table_list))
			return false;
		break;
	}
	case MathObject::COMPARE: {
		if (obj->GetType() != MathObject::COMPARE)
			return false;
		Ptr<Compare> cmp_pattern = Dynamic_cast<Compare>(pattern);
		Ptr<Compare> cmp_obj = Dynamic_cast<Compare>(obj);
		if (cmp_pattern->op != cmp_obj->op)
			return false;
		if (!DoMatch(cmp_pattern->left, cmp_obj->left, table, table_list))
			return false;
		if (!DoMatch(cmp_pattern->right, cmp_obj->right, table, table_list))
			return false;
		break;
	}
	default:
		// 不支持的类型，放弃匹配，直接返回false
		return false;
		break;
	}
	return true;
}

Ptr<MathObject> mathS::DoReplace(Ptr<MathObject> pattern, std::map<std::string, Ptr<MathObject>>& table)
{
	switch (pattern->GetType()) {
	case MathObject::ATOM: {
		// 如果是Atom, 考虑替换（不检查是通配符等，直接在table里查找，只要找到就替换）
		Ptr<Atom> atom_pattern = Dynamic_cast<Atom>(pattern);
		auto it = table.find(atom_pattern->str);
		if (it == table.end())
			return pattern->DeepCopy();
		else
			return it->second->DeepCopy();
	}
	case MathObject::VECTOR: {
		Ptr<Vector> vec_pattern = Dynamic_cast<Vector>(pattern);
		Ptr<Vector> vec_ret = New<Vector>();
		vec_ret->components.reserve(vec_pattern->components.size());
		// 对子表达式应用替换，返回拷贝
		for (int i = 0; i < vec_pattern->components.size(); i++)
			vec_ret->components.push_back(DoReplace(vec_pattern->components[i], table));
		return vec_ret;
	}
	case MathObject::FUNCTION: {
		Ptr<Function> func_pattern = Dynamic_cast<Function>(pattern);
		Ptr<Function> func_ret = New<Function>();
		func_ret->function = DoReplace(func_pattern->function, table);
		func_ret->parameter.reserve(func_pattern->parameter.size());
		// 对子表达式应用替换，返回拷贝
		for (int i = 0; i < func_pattern->parameter.size(); i++) 
			func_ret->parameter.push_back(DoReplace(func_pattern->parameter[i], table));
		return func_ret;
	}
	case MathObject::FUNCOPERATOR: {
		Ptr<FunctionalOperator> fop_pattern = Dynamic_cast<FunctionalOperator>(pattern);
		Ptr<FunctionalOperator> fop_ret = New<FunctionalOperator>();
		fop_ret->function = DoReplace(fop_pattern->function, table);
		fop_ret->variables.reserve(fop_pattern->variables.size());
		fop_ret->fparameter.reserve(fop_pattern->fparameter.size());
		fop_ret->parameter.reserve(fop_pattern->parameter.size());
		// 对子表达式应用替换，返回拷贝
		for (int i = 0; i < fop_pattern->variables.size(); i++) {
			auto var = DoReplace(fop_pattern->variables[i], table);
			if (var->GetType() != MathObject::ATOM)
				return New<ErrorObject>("Rule: (Unexpected) Cannot replace Functional Operator variable with non-atom object. You may have defined illegal rules. ");
			fop_ret->variables.push_back(Dynamic_cast<Atom>(var));
		}	
		for (int i = 0; i < fop_pattern->fparameter.size(); i++)
			fop_ret->fparameter.push_back(DoReplace(fop_pattern->fparameter[i], table));
		for (int i = 0; i < fop_pattern->parameter.size(); i++)
			fop_ret->parameter.push_back(DoReplace(fop_pattern->parameter[i], table));
		return fop_ret;
	}
	case  MathObject::POWER: {
		Ptr<Power> pow_pattern = Dynamic_cast<Power>(pattern);
		return New<Power>(DoReplace(pow_pattern->base, table), DoReplace(pow_pattern->exponent, table));
	}
	case MathObject::INVERSE: {
		Ptr<Inverse> inv_pattern = Dynamic_cast<Inverse>(pattern);
		return New<Inverse>(DoReplace(inv_pattern->component, table));
	}
	case MathObject::ITEM: {
		Ptr<Item> itm_pattern = Dynamic_cast<Item>(pattern);
		Ptr<Item> itm_ret = New<Item>();
		itm_ret->factors.reserve(itm_pattern->factors.size());
		// 对子表达式应用替换，返回拷贝
		for (int i = 0; i < itm_pattern->factors.size(); i++)
			itm_ret->push_back(DoReplace(itm_pattern->factors[i], table));
		return itm_ret;
	}
	case MathObject::OPPOSITE: {
		Ptr<Opposite> oppo_pattern = Dynamic_cast<Opposite>(pattern);
		return New<Opposite>(DoReplace(oppo_pattern->component, table));
	}
	case MathObject::POLYNOMIAL: {
		Ptr<Polynomial> poly_pattern = Dynamic_cast<Polynomial>(pattern);
		Ptr<Polynomial> poly_ret = New<Polynomial>();
		poly_ret->items.reserve(poly_pattern->items.size());
		// 对子表达式应用替换，返回拷贝
		for (int i = 0; i < poly_pattern->items.size(); i++)
			poly_ret->push_back(DoReplace(poly_pattern->items[i], table));
		return poly_ret;
	}
	case MathObject::MAP: {
		Ptr<Map> map_pattern = Dynamic_cast<Map>(pattern);
		return New<Map>(DoReplace(map_pattern->key, table), DoReplace(map_pattern->value, table));
	}
	case MathObject::COMPARE: {
		Ptr<Compare> cmp_pattern = Dynamic_cast<Compare>(pattern);
		return New<Compare>(DoReplace(cmp_pattern->left, table), cmp_pattern->op, DoReplace(cmp_pattern->right, table));
	}
	default:
		return New<ErrorObject>("Rule: (Unexpected) Unkown MathObject type. This should not happen.");
	}
}

bool mathS::FullCompare(Ptr<MathObject> a, Ptr<MathObject> b)
{
	// 通配符匹配表
	MathObject::Type mtype = a->GetType();
	if (b->GetType() != mtype)
		return false;

	switch (mtype) {
	case MathObject::ATOM: {
		// 如果是Atom, 考虑是匹配变量名还是
		if (b->GetType() != MathObject::ATOM)
			return false;
		Ptr<Atom> atom_a = Dynamic_cast<Atom>(a);
		if (atom_a->str != b->GetString())
			return false;
		break;
	}
	case MathObject::VECTOR: {
		if (b->GetType() != MathObject::VECTOR)		// 确认类型
			return false;
		Ptr<Vector> vec_a = Dynamic_cast<Vector>(a);
		Ptr<Vector> vec_obj = Dynamic_cast<Vector>(b);
		// 分别比较子表达式
		if (vec_a->components.size() != vec_obj->components.size())
			return false;
		for (int i = 0; i < vec_a->components.size(); i++)
			if (!FullCompare(vec_a->components[i], vec_obj->components[i])) return false;
		break;
	}
	case MathObject::FUNCTION: {
		// 匹配函数
		if (b->GetType() != MathObject::FUNCTION)		// 确认类型
			return false;
		Ptr<Function> func_a = Dynamic_cast<Function>(a);
		Ptr<Function> func_obj = Dynamic_cast<Function>(b);
		// 分别比较子表达式
		if (func_a->parameter.size() != func_obj->parameter.size())
			return false;
		if (!FullCompare(func_a->function, func_obj->function))
			return false;
		for (int i = 0; i < func_a->parameter.size(); i++) {
			if (!FullCompare(func_a->parameter[i], func_obj->parameter[i]))
				return false;
		}
		break;
	}
	case MathObject::FUNCOPERATOR: {
		if (b->GetType() != MathObject::FUNCOPERATOR)
			return false;
		Ptr<FunctionalOperator> fop_a = Dynamic_cast<FunctionalOperator>(a);
		Ptr<FunctionalOperator> fop_obj = Dynamic_cast<FunctionalOperator>(b);
		if (fop_a->parameter.size() != fop_obj->parameter.size() || fop_a->variables.size() != fop_obj->variables.size() || fop_a->fparameter.size() != fop_obj->fparameter.size())
			return false;
		if (!FullCompare(fop_a->function, fop_obj->function))
			return false;
		for (int i = 0; i < fop_a->variables.size(); i++)
			if (!FullCompare(fop_a->variables[i], fop_obj->variables[i])) return false;
		for (int i = 0; i < fop_a->fparameter.size(); i++)
			if (!FullCompare(fop_a->fparameter[i], fop_obj->fparameter[i])) return false;
		for (int i = 0; i < fop_a->parameter.size(); i++)
			if (!FullCompare(fop_a->parameter[i], fop_obj->parameter[i])) return false;
		break;
	}
	case  MathObject::POWER: {
		if (b->GetType() != MathObject::POWER)
			return false;
		Ptr<Power> pow_a = Dynamic_cast<Power>(a);
		Ptr<Power> pow_obj = Dynamic_cast<Power>(b);
		if (!FullCompare(pow_a->base, pow_obj->base))
			return false;
		if (!FullCompare(pow_a->exponent, pow_obj->exponent))
			return false;
		break;
	}
	case MathObject::INVERSE: {
		if (b->GetType() != MathObject::INVERSE)
			return false;
		Ptr<Inverse> inv_a = Dynamic_cast<Inverse>(a);
		Ptr<Inverse> inv_obj = Dynamic_cast<Inverse>(b);
		if (!FullCompare(inv_a->component, inv_obj->component))
			return false;
		break;
	}
	case MathObject::ITEM: {
		if (b->GetType() != MathObject::ITEM)		// 确认类型
			return false;
		Ptr<Item> itm_a = Dynamic_cast<Item>(a);
		Ptr<Item> itm_b = Dynamic_cast<Item>(b);
		if (itm_a->factors.size() != itm_b->factors.size())	// 判断长短
			return false;
		auto& a_factors = itm_a->factors;
		auto& b_factors = itm_b->factors;

		std::vector<bool> md(b_factors.size(), false);	// 是否已经配过
		for (int i = 0; i < a_factors.size(); i++) {
			bool flag = false;
			for (int j = 0; j < b_factors.size(); j++) {
				if (md[j]) continue;	// 不匹配已经匹配过的项
				if (FullCompare(a_factors[i], b_factors[j])) {
					flag = true;
					md[j] = true;
					break;
				}
			}
			if (!flag)
				return false;
		}
		break;
	}
	case MathObject::OPPOSITE: {
		if (b->GetType() != MathObject::OPPOSITE)
			return false;
		Ptr<Opposite> oppo_a = Dynamic_cast<Opposite>(a);
		Ptr<Opposite> oppo_obj = Dynamic_cast<Opposite>(b);
		if (!FullCompare(oppo_a->component, oppo_obj->component))
			return false;
		break;
	}
	case MathObject::POLYNOMIAL: {
		if (b->GetType() != MathObject::POLYNOMIAL)		// 确认类型
			return false;
		Ptr<Polynomial> poly_a = Dynamic_cast<Polynomial>(a);
		Ptr<Polynomial> poly_b = Dynamic_cast<Polynomial>(b);
		if (poly_a->items.size() != poly_b->items.size())	// 判断长短
			return false;
		auto& a_items = poly_a->items;
		auto& b_items = poly_b->items;
		std::vector<bool> md(b_items.size(), false);	// 是否已经配过
		for (int i = 0; i < a_items.size(); i++) {
			bool flag = false;
			for (int j = 0; j < b_items.size(); j++) {
				if (md[j]) continue;	// 不匹配已经匹配过的项
				if (FullCompare(a_items[i], b_items[j])) {
					flag = true;
					md[j] = true;
					break;
				}
			}
			if (!flag)
				return false;
		}
		break;
	}
	case MathObject::MAP: {
		if (b->GetType() != MathObject::MAP)
			return false;
		Ptr<Map> map_a = Dynamic_cast<Map>(a);
		Ptr<Map> map_obj = Dynamic_cast<Map>(b);
		if (!FullCompare(map_a->key, map_obj->key))
			return false;
		if (!FullCompare(map_a->value, map_obj->value))
			return false;
		break;
	}
	case MathObject::COMPARE: {
		if (b->GetType() != MathObject::COMPARE)
			return false;
		Ptr<Compare> cmp_a = Dynamic_cast<Compare>(a);
		Ptr<Compare> cmp_b = Dynamic_cast<Compare>(b);
		if (cmp_a->op != cmp_b->op)
			return false;
		if (!FullCompare(cmp_a->left, cmp_b->left))
			return false;
		if (!FullCompare(cmp_a->right, cmp_b->right))
			return false;
		break;
	}
	default:
		return false;
		break;
	}
	return true;
}


