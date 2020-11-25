## 2020-7-6

 **\[任务\]** 这个阶段我们有一下几个独立的开发任务

* Parser. 对输入的表达式文本解析，返回一个MathObject.

* Match. 对两个MathObject`pattern`与`target`进行匹配：

  * 初始化一个**匹配表**`std::sting`->`MathObject`，用于特殊标识符的匹配
  * 递归地对当前MathObject进行下面的算法
    * 如果`pattern`中的当前MathObject是Atom并且以特殊标识符'_'修饰，就是要匹配任意MathObject. 首先检查匹配表中，是否已经有这个特殊表示符修饰的串，如果有的话，就把匹配表中对应的MathObject与当前`target`的MathObject进行匹配，并返回匹配结果；如果匹配表中没有，也就是未出现过的特殊标识，就将这个映射添加到匹配表中，并返回匹配成功。
    * 如果`pattern`中的当前MathObject是Atom并且以特殊标识符''@'修饰，就是要匹配一个名称，要求`target`对应的MathObject必须是Atom，并且是变量名称而不是数字和字符串。如果是以特殊标识符`$s`修饰，就是要匹配字符串，以特殊标识符`$n`修饰，就是匹配数字。**同样也要先去对照匹配表。**
    * 如果`pattern`当前的MathObject是Atom，并且没有特殊标识符修饰，就要求`target`与之对应的必须完全一模一样。
    * 如果`pattern`当前MathObject是更高级的结构，要求`target`的MathObject类型与之相同（如果是ListObject/Polynomial/Item这些还要求size相同），并且递归地匹配每个子MathObject。

  这里的Match，是成对的`pattern`与`target`匹配。有时，一些匹配不方便用pattern来表示，需要用一段代码，一个函数来进行匹配。这些要另外讨论，如何把两种匹配统一起来，以后再想。我们就先把`pattern`对`target`的匹配做好把。

* LBAssembler：给一个MathObject和参数名称列表，返回一个NMathFunction，即一个输入NMathObject返回NMathObject的函数。其原理是利用LBAssembler，在NMath体系下，组装出一个函数。可以附加一个功能ToStdFunction，把NMathFunction变为std::function，更方便对函数的使用。

## 2020-7-7

**[工作]** LBAssembler进度推进。

* 介绍LBAssemble的原理：所谓Assemble，即把一个表达式的基本元素变成NFunction，根据表达式的结构，一层一层组装成更复杂运算的NFunction。例如已有两个NFunction，`f1,f2`，则他们的和就是下面的lambda表达式：

  ```C++
  [f1,f2](NMathObject* param)
  {
  	return f1(param)+f2(param);
  }
  ```

  例如`2*x+sin(y)`是一个表达式，要组装出函数`f(x,y)`，需要把每个Atom看做是关于参数`x,y`的NFunction，然后再一层一层组装.

* 本次工作，先实现了基本Atom的转换，也就是Number和Variable的组装。String不支持转换，因为没有对应的NMathObject。

## 2020-9-22

**[工作]** 

* MathParser已经开发完成。它没有使用栈结构，而是逐层嵌套地构造表达式。
* 所有对象(MathObject, NMathObject)的指针使用改为使用STL的智能指针。为了简化名称和体现程序逻辑，在Ptr.h中有简单封装。这将更方便内存管理。我认为在目前阶段广泛使用`std::shared_ptr`不会对程序的性能有显著影响。

**[约定]**

* 对MathObject的表示框架作了修改，增加FunctionalOperator类型，这用来表示积分、求和、累乘、傅里叶变换、卷积、导数算子等这些特殊的函数。我们把这些函数与普通的函数区分开，因为它们是*函数到函数的映射*。我们当然可以把函数和数字都看做等价的对象，但对于表达式来说，想要表达一个对象是函数还是数是比较困难而且容易产生歧义的，尤其是对于函数对象，我们需要明确函数的参数是什么。为此引入的FunctionalOperator的书写格式为：

* ```
  _FUNCOP<<@VAR1,@VAR2,...|_FUNC>>(_PARA)
  ```

**[任务]**

* 完成LBAssemble，从符号对象转为数值计算对象，并具有初步的数值计算功能。
* 建立在MathObject上的运算库，库将具有初步的符号计算功能：
  * 首先需要基本的匹配源模式->变换为目标模式的模块
  * 手动编写不能使用匹配变换的方法进行变形的规则



## 2020-9-23

 **[工作]** LBAssembler 大部分开发完成。



## 2020-9-23

**[工作]** 实现了FucntionalOperator，并用数值求和Sum测试，LBAssembler开发完成。对于每次运行独立的单条命令，不需要考虑更多的上下文，已经够用。

**[约定]** MathObject框架变动，废弃了ListObject。本来ListObject的引入，就是为了方便统一处理函数的参数列表、向量的元素列表、索引定位的列表，但这样会导致其他的麻烦：

1. 每次对函数的参数都要作类型检查是否为ListObject，ListObject中套了一个MathObject， 和直接一个MathObject，应当是等价的，而他们能表示为不同的结构，会增加处理的麻烦；
2. 转为NMath时，由于NList不像Vector那样，是List套一层Vector，所以会混淆函数多元函数和向量作为参数的函数，即f(x,y)和f({x,y})。如果要区分，则不得不强制给参数套一层List，即参数的第一层一定是List，而这与MathObject的Function表示逻辑不同，Function是允许不套List的
3. ListObject会导致语法过于灵活，可能出现(1,2)\*3之类的写法，而这是没有意义的，应当表达成{1,2}\*3的向量形式。

因此我们放弃ListObject作为中间对象，这并不影响MathObject的表达能力。凡是允许多元组的地方，直接在相应的MathObject中内置std::vector。他们的处理方法，如Parse， Assemble是类似的。

为了废弃ListObject，框架从上至下进行了很多变动，因为原本ListObject的地位很重要。但改动后，能感觉到代码更优美了，少了很多的检查，例如我们不再需要EmptyObject，因为std::vector为空时，直接表达了函数参数为空，或者索引列表为空这些情况，而不必需要EmptyObject。其他地方则用不到EmptyObject。

原本ListObject的代码并不能算删掉，因为对于List的处理方法是不变的，这些代码被修改后重新用上了。

**[任务]** 总结一下各个模块

* Symbolic
  * MathObject
  * Rule=Match&Replace
  * RuleLibrary：Grad，Derivative，Simplify，Integrate，Evaluate...
* LBAssembler：MathObject->NFunction
* Numeric
  * NMathObject
  * NFunction
  * NFunctionLibrary
  * NFuncOperatorLibrary

## 2020-9-30

**[工作]** 

* 完成Match，Rule模块。

* Match和Rule是做什么的？

  * Match和Rule是用于符号表达式计算的基础模块，它们都是输入一个`Ptr<MathObject>`返回一个`bool`，表示是否匹配成功的函数。Match只会作匹配尝试，而Rule则会做匹配尝试后进行Rule应用的结果，并通过参数中的引用返回。所以Rule其实就是Match & Replace。（Match单独使用的情况应该很少，绝大多数是直接用Rule）

* 如何定义Match和Rule？

  * 1 给定含有特殊匹配符号修饰的变量的表达式， Source Pattern和Target Pattern，直接通过MakeMatch或MakeRule定义。（通过这种方式定义的Rule或Match必须在程序初始化时就MakeMatch或MakeRule完成。如果在其他地方用MakeRule或MakeMatch来定义，要用static关键字，避免每次调用到这个函数都会重新MakeMatch或MakeRule）

    通配符的含义：\_xxx，表示匹配任意MathObject；@xxx表示匹配Atom变量名称；#xxx表示匹配Atom数字，$xxx表示匹配Atom字符串。在Item或Polynomial的*最后*一项，\_xxx\_表示匹配剩下所有的项。

  * 2 手写`Ptr<MathOBject>(Ptr<MathObject>)`或`Ptr<MathObject>(Ptr<MathObject>, Ptr<MathObject>&)`型函数。因为不是所有的Match和Rule都是可以用一个Source pattern到Target Pattern就能表示的，有些如不定长的向量就不能表示。这时需要手写函数。

* 程序可能要定义大量的Rule，将这些Rule定义在`RuleLib`命名空间下。

* Match和Rule的定义和使用，可以参考程序中示例。在现在的`main.cpp`中，有对单个Rule应用的测试。

## 2020-10-2

**[工作]**

* 完备的Match。现在是基于CSP的前向检验求解的，不会漏匹配情况了。

* Evaluate初步，后面继续开发Evaluate可以参考。现在main.cpp中可以测试Evaluate。

**[任务]** 继续完善RuleLib：

* 常数值计算的Rule，包括乘法
* Function和FunctionalOperator计算的Rule



## 2020-11-25

**[算法]**

符号不定积分

* 约定

  * 在符号积分、符号求导中，为了方便符合习惯，特别地，我们允许省略积分/求导后的函数的变量符号。实际上，一般我们要求外部符号和内部符号不冲突，即`D<<x|f(x)>>(t)​`表示积分后函数为`F(t)`，但对于符号积分求导而言，变量 `t` 只在最后结果代换，因此采用`D<<x|f(x)>>(x)`这种写法不会由于歧义导致错误的结果。那么我们允许省略掉圆括号中的内容，写成`D<<x|f(x)>>`，就表示积分/求导后的函数，仍然使用符号`x`。这实际上是一个危险的、容易产生歧义的，原则上内部变量和外部变量是不能混淆的，例如在可能`x`预先有个赋“值”`x=2`或者`x=a+b`，这时便会产生歧义和错误，应该禁止省略写法。而**在符号`x`未被占用、赋值的情况下，可以允许这样写**。这对于用户更方便，也符合一般的书写习惯。但我们自己必须十分清楚两种写法的区别。
  * 简而言之：省略写法`D<<x|f(x)>>`等价`D<<x|f(x)>>(x)`。在脚本运行的上下文中，会检查符号`x`的占用情况，如果`x`被占用，它会报错，并提示使用不冲突的局部变量名称如`D<<t|f(t)>>(x)`。
  * 为了支持省略写法，1. 需要修改Parse，在FOP<<VAR|FUNC>>缺少圆括号时，不是报错，而是自动认为是空的参数列表。 2. 在求导、积分算法中，如果参数列表是空，则自动补上和VAR名称相同的变量。之后的处理完全照旧。

* 关于符号积分的算法，国内外论文肯定是比较丰富，体系完善的。简单翻了几篇中文的文章，没有很多奇妙之处。现在先提一个平凡的，但已经能解决不少问题的算法：

* 基本要素：

  * 基本积分形式表，是符号积分算法的知识库；除了基本积分，还可以包含常见的困难积分。
  * 积分失败表，即常见的没有初等表达的积分形式，用于算法明智地终止返回失败。

* 算法流程：

  * 化简

    * 化简被积表达式。如果化简后是多项式，则将积分相应地展开，使得每个部分被积式是因式，将常数因子拿到积分外面。对每个部分分别积分，进行下面的步骤：

  * 直接匹配基本积分表、积分失败表

  * 尝试换元积分

    * 设积分表达式形如`Integrate<<x|P>>`，对于每个基本形式积分`u(_t)`，将`P`尝试匹配到`u(_v)*_Res_`
    * 对匹配到的`_v`求导`D<<x|_v>>`，用求导结果匹配`_Res_`
      * 若匹配成功，则积分成功，返回结果
      * 若匹配失败，则换元积分失败

  * 尝试分部积分

    这部分比较困难，因为分部积分的情况非常多，并且对于分部积分的情况也比较难很快确定一种做法是好的，通常还要递归地算下去，那么就容易出现尝试次数爆炸的情形，需要解决。

    * 对于被积因式表达式`P`，列出全部划分为两个部分相乘的情况`_u*_dv`。若`P`有m个因子，则共有$2^m-2$种划分。（`_dv`为1的情况不算，`u`为1的情况不算）。
    * 启发方法：还没有计算`_dv`的积分，对于每个划分，使用某个标准，对分部后表达式的是否容易积分给出一个评价，按评价结果排序，优先尝试“更容易积分”的分部方法。
    * 对于每个划分方法做尝试：求`_dv`的积分`Integrate<<x|_dv>>`，如果积分失败，则这个划分是失败的。如果成功，则求`_u`的导数`D<<x|u_>>`，再求二者乘积的积分，如果失败，则这个划分是失败的。如果成功，则积分成功，返回相应的结果。

