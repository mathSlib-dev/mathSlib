#pragma once

#include <vector>
#include <string>
#include <functional>
#include <Ptr.h>
#include <cmath>

namespace mathS
{
	/*
	The logic of NMathObject:

	NMathObject
	|=>NAtom
	|=>NList
	(The following are not implemented yet)
	|=>//NSparseList
	|=>//NMatrix
	|=>//NSparseMatrix

	NList 和 NAtom 构成广义表，通过类的继承实现。
	与之相对的做法是使用 NNode，但这样的话会浪费一些空间。

	*/


	// Numeric math library
	namespace NMath
	{
		typedef double NValueType;

		class NMathObject
		{
		public:
			enum Type
			{
				ATOM, LIST, SAPRSE_LIST, ERROR
			};
		public:
			virtual ~NMathObject() {};

			virtual bool IsAtom()const = 0;
			virtual Type GetType() const = 0;
			virtual int Size()const = 0;
			virtual NValueType GetValue() const = 0;

			virtual bool IsError() const = 0;
			virtual std::string GetString() const = 0;
			virtual Ptr<NMathObject> DeepCopy() const  = 0;
		};

		// Atom type of numeric math object.
		class NAtom : public NMathObject
		{
		public:
			NValueType value;

			NAtom(NValueType v) :value{ v } {};
			~NAtom() {};

			Type GetType() const { return Type::ATOM; }
			bool IsAtom() const { return true; }
			bool IsError() const { return false; }
			int Size() const { return 1; }
			NValueType GetValue() const { return value; }

			std::string GetString() const;
			Ptr<NMathObject> DeepCopy() const;
		};

		// General list of numeric math object.
		class NList : public NMathObject
		{
		public:
			// 匹配
			/*
			f(_x)
			f(1,2)
			_x -> 1,2
			*/
			std::vector<Ptr<NMathObject>> components;

			NList() {};
			NList(std::initializer_list<Ptr<NMathObject>> _init_list) : components{ _init_list }{}
			NList(std::initializer_list<NValueType> _init_list)
			{
				for (auto it : _init_list)
					components.push_back(New<NAtom>(it));
			}
			~NList() {}

			Type GetType() const { return Type::LIST; }
			bool IsAtom() const { return false; }
			bool IsError() const { return false; }
			int Size() const { return components.size(); };
			NValueType GetValue() const { return components.empty()?0.:components[0]->GetValue(); }

			Ptr<NMathObject> PartLocate(const std::vector<int>& loc)const;
			std::string GetString() const;
			Ptr<NMathObject> DeepCopy() const;
		};

		// Error type object, which represents an error occurred during calculation.
		class NMathError : public NMathObject
		{
		public:
			std::string info;

			NMathError(const std::string info) : info{ info } {};
			~NMathError() {};

			bool IsAtom()const { return false; };
			bool IsError() const { return true; };
			Type GetType() const { return Type::ERROR; };
			int Size()const { return 0; };
			NValueType GetValue() const { return NAN; }

			std::string GetString() const { return info; };
			Ptr<NMathObject> DeepCopy() const;
		};
		/*
		class NMatrix : public NMathObject
		{
		public:
			std::vector<NAtom*> components;

			~NMatrix();
		};
		*/


		// Locate a part of obj and return a reference. Note that this is unsafe because it does not check the indices!
		Ptr<NMathObject>& PartLocate_ref(Ptr<NMathObject> obj, const std::vector<int>& loc);

		// Locate a part of obj. Cannot modify the depth of obj, since leaf nodes - NAtom is not changable.
		Ptr<NMathObject> PartLocate(Ptr<NMathObject> obj, const std::vector<int>& loc);

		// Locate a part of obj. 
		Ptr<NMathObject> PartLocate(Ptr<NMathObject> obj, const int loc);

		Ptr<NMathObject> Concatenate(Ptr<NMathObject> a, Ptr<NMathObject> b);
	}

	/*
	// Sparse MathObject
	class NSparseList :public NMathObject
	{

	};
	class NSparseMatrix : public NMathObject
	{

	};

	class NValuePlusOp
		{
		public:
			inline NValueType operator()(NValueType _Left, NValueType _Right) const
			{
				return _Left + _Right;
			}
		};
	*/
	

}



