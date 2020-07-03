#pragma once

#include <vector>
#include <string>
#include <functional>

namespace mathS
{


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
			ATOM, VECTOR, EMPTY,
			// Level 2: _OBJECT(_PARAMETER)
			FUNCTION,
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
		static const int LEVEL_LOCATE = 3;
		static const int LEVEL_POWER = 4;
		static const int LEVEL_INVERSE= 5;
		static const int LEVEL_ITEM = 6;
		static const int LEVEL_OPPOSITE = 7;
		static const int LEVEL_POLYNOMIAL = 8;
		static const int LEVEL_MAP = 9;
		static const int LEVEL_COMPARE = 10;
		static const int LEVEL_LIST = 12;
		static const int LEVEL_ERROR = -1;
	public:

		//MathObject() {};
		virtual ~MathObject() {};

		virtual Type GetType() = 0;
		virtual std::string GetString() = 0;
		virtual int Level() const = 0;


		virtual MathObject* DeepCopy() = 0;
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

		Type GetType() { return Type::LIST; };
		int Level() const { return LEVEL_LIST; };
		std::string GetString();

		MathObject* DeepCopy();
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

		Type GetType() { return Type::ATOM; };
		int Level() const { return LEVEL_ATOM; };
		std::string GetString();

		MathObject* DeepCopy();
	};

	class Vector : public MathObject
	{
		// eg. {7,x,y}
	public:
		Vector() {}
		~Vector() { delete list; }
	public:
		ListObject* list;

		Type GetType() { return Type::VECTOR; };
		int Level() const { return LEVEL_VECTOR; };
		std::string GetString();

		MathObject* DeepCopy();
	};

	class Function : public MathObject
	{
		// eg. f[x], Sin[u]
	public:
		Function() {}
		~Function() { delete function; delete parameter; }
	public:
		MathObject* function;
		MathObject* parameter;

		Type GetType() { return Type::FUNCTION; };
		int Level() const { return LEVEL_FUNCTION; };
		std::string GetString();

		MathObject* DeepCopy();
	};

	class Locate : public MathObject
	{
		// eg: arr[3], {a,b,3}[2]
	public:
		Locate() {}
		~Locate() { delete object; delete location; };
	public:
		MathObject* object;
		MathObject* location;

		Type GetType() { return Type::LOCATE; };
		int Level() const { return LEVEL_LOCATE; };
		std::string GetString();

		MathObject* DeepCopy();
	};
	class Power : public MathObject
	{
		// eg. a^5, x^y, y^p^q (this is equivalent to y^(p^q))
	public:
		Power() {}
		~Power() { delete base; delete exponent; }
	public:
		MathObject* base;
		MathObject* exponent;

		Type GetType() { return Type::POWER; };
		int Level() const { return LEVEL_POWER; };
		std::string GetString();

		MathObject* DeepCopy();
	};

	class Inverse : public MathObject
	{
		// eg. /y   /(x+y*z)
	public:
		Inverse() {}
		~Inverse() { delete component; }
	public:
		MathObject* component;

		Type GetType() { return Type::INVERSE; };
		int Level() const { return LEVEL_INVERSE; };
		std::string GetString();

		MathObject* DeepCopy();
	};

	class Item : public MathObject
	{
	public:
		Item() {}
		~Item() { 
			for (auto it : factors) 
				delete it;
		}
		// eg. x1*x2   x/y
	public:
		std::vector<MathObject*> factors;

		Type GetType() { return Type::ITEM; };
		int Level() const { return LEVEL_ITEM; };
		std::string GetString();

		MathObject* DeepCopy();
	};

	class Opposite : public MathObject 
	{
	public:
		Opposite() {};
		~Opposite() { delete component; }
		// -x, -y*z, -{1,2}*u/p
	public:
		MathObject* component;
		
		Type GetType() { return Type::OPPOSITE; };
		int Level() const { return LEVEL_OPPOSITE; };
		std::string GetString();

		MathObject* DeepCopy();
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

		Type GetType() { return Type::POLYNOMIAL; };
		int Level() const { return LEVEL_POLYNOMIAL; };
		std::string GetString();

		MathObject* DeepCopy();
	};

	class Map : public MathObject
	{
		// eg. attr -> 1
	public:
		Map() {}
		~Map() { delete key; delete value; }
	public:
		MathObject* key;
		MathObject* value;

		Type GetType() { return Type::MAP; };
		int Level() const { return LEVEL_MAP; };
		std::string GetString();

		MathObject* DeepCopy();
	};

	class Compare : public MathObject
	{
		// eg. a==b, u<v,
	public:
		Compare() {}
		~Compare() { delete left; delete right; }
	public:
		std::string op;
		MathObject* left;
		MathObject* right;

		Type GetType() { return Type::COMPARE; };
		int Level() const { return LEVEL_COMPARE; };
		std::string GetString();

		MathObject* DeepCopy();
	};

	class ErrorObject : public MathObject
	{
	public:
		std::string info;

		ErrorObject() {}
		ErrorObject(const std::string info) :info{ info } {};
		~ErrorObject() {};

		Type GetType() { return Type::ERROR; };
		int Level() const { return LEVEL_ERROR; };
		std::string GetString() { return info; };

		MathObject* DeepCopy();
	};

	class EmptyObject : public MathObject
	{
	public:
		EmptyObject(){}
		~EmptyObject() {}

	public:

		Type GetType() { return Type::EMPTY; };
		int Level() const { return LEVEL_EMPTY; };
		std::string GetString() { return std::string(); };

		MathObject* DeepCopy();
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