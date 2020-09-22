#pragma once

#include <vector>
#include <string>
#include <functional>


namespace mathS
{
	class MathObject;
	class ListObject;
	class Atom;
	class Vector;
	class Function;
	class FunctionalOperator;
	class Locate;
	class Power;
	class Inverse;
	class Item;
	class Opposite;
	class Polynomial;
	class Map;
	class Compare;
	class List;
	class ErrorObject;
	class EmptyObject;

	template<class T>
	class Ptr :std::shared_ptr<T> {
	public:
		template<class... _Types>
		static Ptr<T> New(_Types&& ..._Args) {
			return std::make_shared<T>(_Args);
		}
		template<class T2>
		static Ptr<T2> Dynamic_cast(const Ptr<T> p) {
			return std::dynamic_pointer_cast<T2>(p);
		}
	};

	class MathObject
	{
	public:
		enum Type
		{
			// The structure of math objects is something like an expression tree.
			// We assign different levels to each type of math objects.
			// The levels indicate the priority of operations,
			// also the implicit structures of of text form expression input.
			// In contrast, brackets indicate the explicit structures.

			// Level 1: Atom element. 
			// NUMBER,VARIABLE,STRING are deprecated!!! Now we use ATOM.
			// NUMBER:		Starts with a figure from '0'-'9', maybe containing a point, end with space, any operator or bracket.
			//				NOTE: complex are not actually supported YET. But we allow number text with suffix 'i', 
			//				which represents unit complex.
			//
			// VARIABLE:	Usually starts with an aphabet, end with any operator or bracket. 
			//				When starts with _, it acts as an OBJECT PLACEHOLDER, which matches any math object.
			//				When starts with @, it acts as a NAME PLACEHOLDER, which matches any name variable.
			//				NOTE: Its use is more than a variable. For example, it can also represent a name of a function.
			//
			// STRING:		Enclosed with "". The Char \ has the same meaning in C++ string. 
			//
			// ATOM:		Basic element, including number, variable, string. Since all of them can be represented by std::string.
			//				Specific type of ATOM are distinguished only when it is needed.
			//
			// VECTOR:		Enclosed with {}. Inside a vector, there is a list of all elements.
			ATOM, VECTOR, EMPTY, NUMBER, STRING, VARIABLE,
			// Level 2: _FUNCTION(_PARAMETER)
			//		  : _FOPERATOR<<@VAR1,@VAR2,...|_FPARAMETER>>(_PARAMETER)
			FUNCTION, FUNCOPERATOR,
			// Level 3: _OBJECT[_LOC]
			LOCATE,
			// Level 4: _BASE^_EXPONENT
			POWER,
			// Level 5: /_COMPONENT
			INVERSE, 
			// Level 6: _A*_B/_C*...*_Z
			ITEM, 
			// Level 7: -_COMPONENT
			OPPOSITE,
			// Level 8: _A+_B-_C...+_Z
			POLYNOMIAL,
			// Level 9: _KEY->_VALUE
			MAP, 
			// Level 10: _LEFT==_RIGHT; _LEFT<=_RIGHT; _LEFT>=_RIGHT; _LEFT<_RIGHT; _LEFT>_RIGHT ...
			COMPARE, 
			// Level 11:  _A;_B;_C;
			// Not implemented yet.
			// SENTENCE_LIST
			
			// Level 12: _A,_B,_C,...,_Z
			LIST,

			// Level -1: 
			ERROR
		};
		static const int LEVEL_EMPTY = 1;
		static const int LEVEL_ATOM = 1;
		static const int LEVEL_VECTOR = 1;
		static const int LEVEL_FUNCTION = 2;
		static const int LEVEL_FUNCOPERATOR = 2;
		static const int LEVEL_LOCATE = 3;
		static const int LEVEL_POWER = 4;
		static const int LEVEL_INVERSE= 5;
		static const int LEVEL_ITEM = 6;
		static const int LEVEL_OPPOSITE = 7;
		static const int LEVEL_POLYNOMIAL = 8;
		static const int LEVEL_MAP = 9;
		static const int LEVEL_COMPARE = 10;
		static const int LEVEL_LIST = 12;
		static const int LEVEL_CLOSED = 99;
		static const int LEVEL_ERROR = -1;
	public:

		//MathObject() {};
		virtual ~MathObject() {};

		virtual Type GetType() const = 0;
		virtual std::string GetString() const = 0;
		virtual int Level() const = 0;


		virtual MathObject* DeepCopy() const = 0;
	};

	// list type math object. 
	// Elements are separated by comma ",".
	class ListObject : public MathObject
	{
	public:
		std::vector<MathObject*> components;

		ListObject() {}
		~ListObject()
		{
			for (auto it : components)
				delete it;
		}

		Type GetType() const { return Type::LIST; };
		int Level() const { return LEVEL_LIST; };
		std::string GetString() const;

		MathObject* DeepCopy() const;

		void push_back(MathObject* const obj) { components.push_back(obj); }
	};

	class Atom : public MathObject
	{
		// 2020-7-3 架构调整. 原先的Number, Variable, String被全部当做了Atom.
		// Atom.str 表示其内容：
		// 如果是一个number,那么str就是一个数字对应的字符串；如果是一个variable，就是以字母开头的字符串（或特殊修饰符）；如果是string，那一定是""包括的字符串.
		// 这个调整的理由是：Number, Variable, String本来就是只用std::string来存储的，其Level(),GetString(),DeepCopy()等方法一模一样
		//					并且在一般的表达式变形中，不必对这三者作区分。只需要在化简、求值等涉及到其具体内容时，需要区分。
		//					因此采用Atom统一表示，语法分析器会更简单。而涉及到其内容时，再作识别Atom是数字、变量还是字符串.
	public:
		std::string str;

		Atom() {};
		Atom(const std::string name_str) :str{ name_str } {};
		~Atom() {}

		Type GetType() const { return Type::ATOM; };

		Type AtomType() const;
		// 返回数值. 暂时只支持double. 如果以后支持了复数，则需要修改此处.
		double NumberValue() const;

		int Level() const { return LEVEL_ATOM; };
		std::string GetString() const;

		MathObject* DeepCopy() const;
	};

	class Vector : public MathObject
	{
		// eg. {7,x,y}
	public:
		Vector() {}
		~Vector() { delete list; }
	public:
		ListObject* list = nullptr;

		Type GetType() const { return Type::VECTOR; };
		int Level() const { return LEVEL_VECTOR; };
		std::string GetString() const;

		MathObject* DeepCopy() const;
	};

	class Function : public MathObject
	{
		// eg. f[x], Sin[u]
	public:
		Function() {}
		~Function() { delete function; delete parameter; }
	public:
		MathObject* function = nullptr;
		MathObject* parameter = nullptr;

		Type GetType() const { return Type::FUNCTION; };
		int Level() const { return LEVEL_FUNCTION; };
		std::string GetString() const;

		MathObject* DeepCopy() const;
	};
	
	class FunctionalOperator : public MathObject
	{
	public:
		FunctionalOperator() {};
		~FunctionalOperator() { delete function; delete fparameter; delete parameter; for (auto itv : variables) delete itv; };
	public:
		MathObject* function = nullptr;
		std::vector<Atom*> variables;
		MathObject* fparameter = nullptr;
		MathObject* parameter = nullptr;

		Type GetType() const { return Type::FUNCOPERATOR; };
		int Level() const { return LEVEL_FUNCTION; };
		std::string GetString() const;
		
		MathObject* DeepCopy() const;
	};

	class Locate : public MathObject
	{
		// eg: arr[3], {a,b,3}[2]
	public:
		Locate() {}
		~Locate() { delete object; delete location; };
	public:
		MathObject* object = nullptr;
		MathObject* location = nullptr;

		Type GetType() const { return Type::LOCATE; };
		int Level() const { return LEVEL_LOCATE; };
		std::string GetString() const;

		MathObject* DeepCopy() const;
	};
	class Power : public MathObject
	{
		// eg. a^5, x^y, y^p^q (this is equivalent to y^(p^q))
	public:
		Power() {}
		~Power() { delete base; delete exponent; }
	public:
		MathObject* base = nullptr;;
		MathObject* exponent = nullptr;

		Type GetType() const { return Type::POWER; };
		int Level() const { return LEVEL_POWER; };
		std::string GetString() const;

		MathObject* DeepCopy() const;
	};

	class Inverse : public MathObject
	{
		// eg. /y   /(x+y*z)
	public:
		Inverse() {}
        Inverse(MathObject* const c) : component(c) {}
		~Inverse() { delete component; }
	public:
		MathObject* component = nullptr;

		Type GetType() const { return Type::INVERSE; };
		int Level() const { return LEVEL_INVERSE; };
		std::string GetString() const;

		MathObject* DeepCopy() const;
	};

	class Item : public MathObject
	{
	public:
		Item() {}
		~Item() { 
			for (auto it : factors) 
				delete it;
		}
        void push_back(MathObject* const f) { factors.push_back(f); }
		// eg. x1*x2   x/y
	public:
		std::vector<MathObject*> factors;

		Type GetType() const { return Type::ITEM; };
		int Level() const { return LEVEL_ITEM; };
		std::string GetString() const;

		MathObject* DeepCopy() const;
	};

	class Opposite : public MathObject 
	{
	public:
		Opposite() {};
		Opposite(MathObject* const component) :component{ component } {};
		~Opposite() { delete component; }
		// -x, -y*z, -{1,2}*u/p
	public:
		MathObject* component;
		
		Type GetType() const { return Type::OPPOSITE; };
		int Level() const { return LEVEL_OPPOSITE; };
		std::string GetString() const;

		MathObject* DeepCopy() const;
	};
	class Polynomial : public MathObject
	{
		// eg. 3.2+4*y+6/(x+1)
	public:
		Polynomial() {}
		~Polynomial() {
			for (auto it : items)
				delete it;
		}
	public:
		std::vector<MathObject*> items;

		Type GetType() const { return Type::POLYNOMIAL; };
		int Level() const { return LEVEL_POLYNOMIAL; };
		std::string GetString() const;

		MathObject* DeepCopy() const;

		void push_back(MathObject* const itm) { items.push_back(itm); };
	};

	class Map : public MathObject
	{
		// eg. attr -> 1
	public:
		Map() {}
		Map(MathObject* const a, MathObject* const b) :key{ a }, value{ b }{};
		~Map() { delete key; delete value; }
	public:
		MathObject* key;
		MathObject* value;

		Type GetType() const { return Type::MAP; };
		int Level() const { return LEVEL_MAP; };
		std::string GetString() const ;

		MathObject* DeepCopy() const;
	};

	class Compare : public MathObject
	{
		// eg. a==b, u<v,
	public:
		Compare() {}
		Compare(MathObject* const a, MathObject* const b) :left{ a }, right{ b }{};
		~Compare() { delete left; delete right; }
	public:
		std::string op;
		MathObject* left;
		MathObject* right;

		Type GetType() const { return Type::COMPARE; };
		int Level() const { return LEVEL_COMPARE; };
		std::string GetString() const ;

		MathObject* DeepCopy() const;
	};

	class ErrorObject : public MathObject
	{
	public:
		std::string info;

		ErrorObject() {}
		ErrorObject(const std::string info) :info{ info } {};
		~ErrorObject() {};

		Type GetType() const { return Type::ERROR; };
		int Level() const { return LEVEL_ERROR; };
		std::string GetString() const { return info; };

		MathObject* DeepCopy() const;
	};

	class EmptyObject : public MathObject
	{
	public:
		EmptyObject(){}
		~EmptyObject() {}

	public:

		Type GetType() const { return Type::EMPTY; };
		int Level() const { return LEVEL_EMPTY; };
		std::string GetString() const { return std::string(); };

		MathObject* DeepCopy() const;
	};
	/*
	class Matrix : public MathObject
	{
	public:
		std::vector<Vector*> rowVectors;
	};
	
	*/




	// Math Function Type : MathObject -> MathObject
	typedef std::function<MathObject*(MathObject*)> MathFunction;
	
	class Rule
	{
	public:
		MathObject* fromForm;
		MathObject* toForm;
		
		static void Apply()
		{

		}
	};
	
	
}