## 2020-7-6

**\[任务\]**这个阶段我们有一下几个独立的开发任务

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

