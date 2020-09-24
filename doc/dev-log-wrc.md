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

* 完成LBAssemble，从符号对象转为数值计算对象，并具有初步的数值计算功能。(wrc)
* 建立在MathObject上的运算库，库将具有初步的符号计算功能：(lv, jky)
  * 首先需要基本的匹配源模式->变换为目标模式的模块
  * 手动编写不能使用匹配变换的方法进行变形的规则



## 2020-9-23

 **[工作]** LBAssembler 大部分开发完成。