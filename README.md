# mathS

A library for symbolic mathematic computing and scientific computing.

We start from an imitation of Mathematica. We try to make it more connected to C++, and allow people to enjoy its feature conveniently in C++ programs. For example, parse a text input "sin(1/x)+2*y^2" and instantly get an `std::function `.

*mathS=math+Science|math+Symbol*



## Features

* Symbolic expression processing is under developing. We have already implemented `MathParser`. It converts text expressions to varied `MathObject`. Our next step is to implement symbolic derivative, 
* **MathS** grammar is designed to state an expression clearly and without those ambiguous meanings usually in writing. Vector, matrix, any shape of tensors or general list are supported.  Functional operators and some advanced operators like $\nabla,\int,\sum,\prod$  are supported. Equations and inequations are supported in expression. 
* `NMath` module offers better performance on numerical tasks. Numerical linear algebra will be strongly supported. It is under developing. We have already implemented `Assembler`. `MathObejct` can be converted to `NFunction` which takes the variables in the expression as parameters. This would support those cases like plotting, numerical integrating  when massive computing is needed. 
* We are planning to make it possible to convert `MathObject` to LaTex format string.
* As an C++ library, you can enjoy its features in C++ freely. `NFunction` has nothing different from `std::function`. 



## A powerful calculator in 9 line

The follow program with **mathS** is a calculator that supports various and vector and matrix. 

```c++
#include <iostream>
#include <MathParser.h>
#include <LBAssembler.h>

using namespace mathS;

int main() {    
	std::string str;
    Assembler assembler;
    
 	std::cin >> str;
    auto mobj = Parser(str).Parse();			// Get MathObject
        
    if (mobj->GetType() == MathObject::ERROR) 	// Check sytanx
        std::cout << mobj->GetString() << std::endl;

    std::vector<std::string> params = {};		// No variables in the expression in a calculator program.

    auto f = assembler.Assemble(mobj, params);	// Get NFunction
    std::cout << "Answer = " << f({})->GetString() << std::endl;
    
    return 0;
}
```

Sample input and output

```
E^3+9.1*(3-2.1)
Answer = 28.275537

floor(1.34+3)
Answer = 4.000000

{3,6,9}/3
Answer = {1.000000,2.000000,3.000000}

{3.1,3.2,{6,2,1}}*{1.1,2.2,{3.1,3.2,4.3}}
Answer = {3.410000,7.040000,{18.600000,6.400000,4.300000}}

sin({1,PI/6,PI/2})
Answer = {0.841471,0.500000,1.000000}
```

## What's to be done

This is our timeline.

* *Step 1*: Basic part

  * **Parser**: Convert text expressions to `MathObject`
  * **Assembler**: Convert `MathObject` to `NMathFuncition`(numeric math function), also C++ std function. 

  When *Step 1* is done, mathS is already quite a useful library. It will allow programmers  get a function instantly by text expressions.

  Maybe it will go like this:

  ```C++
  std::functional<std::vector<double>(std::vector<double>)> f = ToStdFunction<double, double>(assembler.assemble(parser.parse("Sin[1/x]+x^2","x")));
  
  std::cout << "f(3.1)=" << f(3.1) << std::endl;
  // It will output the result of f(3.1), which is just Sin(1/3.1)+3.1^2
  ```

  The most simply usage with mathS - implementation of a powerful calculator:

  ```C++
  
  using namespace mathS;
  using namespace mathS::NMath;
  
  int main()
  {
      Assember assembler;
      Parser parser;
      while(true)
      {
          std::string expr;
          std::cin >> expr;
          auto f = ToStdFunction<double,void>(assembler.assemble(parser.parse(expr)));
          std::cout << "=" << f() << std::endl;
      }
      return 0;
  }
  
  /*
  This calculator supports vector, matrix and list.
  Input examples:
  
  log(7.2)
  4*(3+1)/5+8^0.5
  dot[{1,3,2},{7,6,5}]
  MatrixInverse[{{1,3,4},{2,5,6},{0,-1,3}}]
  Average[{1,2,3,4,6}]
  */
  ```

* *Step 2*: Essential part

  * **Rule**: Use a chain of rules to transform `MathObject`, so that we can perform symbolic integral, differential, simplify any expression.
  * **NAlgorithms**: Adopt numeric computing algorithms. Performing interpolation, numeric integral, matrix decomposition, numeric optimization, linear programming, solving linear and non-linear equations, statistics, etc. . Also many other interesting utilities.(This will take a very long time)
  * **Interpreter**: More controlling functions, and allow user to execute a mathS script. Similar to a Mathematica program.

* *Step 3*: Advanced part

  Many of the following could be just dreams. 

  * Even  more diverse objects. 

    * **SparseList**: For large sparse matrix and algorithms.
    * **Graph**: Build a graph, display and perform graph algorithms.
    * **Graphics**: Allow users to plot and draw with simple commands. (Very useful, but to make it great is not that easy)
    * **Image**:  Read, process and show images.
    * ...

  * Extensions

    (This is cool but maybe not practical. We are not supposed to repeat useless work, which has been done by others perfectly.)

    * **MLlib**: Simple implementation of machine learning algorithms



## Documentation

TODO.



# Development

* See in `doc/dev-log-***.md`. 