# mathS

A library for symbolic mathematic computing and scientific computing.

We start from an imitation of Mathematica. We try to make it more connected to C++, and allow people to enjoy its feature conveniently in C++ programs. For example, parse a text input "Sin[1/x]+2*y^2" and instantly get a std function !

*mathS=math+Science|math+Symbol*

# Introduction

## What's done

* Basic definitions of `MathObject` 
* Basic definitions of `NMathObject`
* Documentations of above and basic ideas.
* A previous half-finished C# project [MathiS](https://github.com/EasternJournalist/MathiS) , based on string matching and lambda expression, which has already successfully assembled functions and plot graphics, but less extensible.
* Nothing else.

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
  
  Log[7.2]
  4*(3+1)/5+8^0.5
  Dot[{1,3,2},{7,6,5}]
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
    * **Graph**: Build a graph, show it and perform graph algorithms.
    * **Graphics**: Allow users to plot and draws with simple commands. (Very useful, but to make it great is not that easy)
    * **Image**:  Read, process and show images.
    * ...

  * Extensions

    (This is cool but maybe not practical. We are not supposed to repeat useless work, which has been done by others perfectly.)

    * **MLlib**: Simple implementation of some machine learning algorithms