## 2020-7-6
**[任务]** 编写语法解析方面的内容

## 2020-11-15

**[工作]**
* 数值求和与求积

## 2020-11-18

**[工作]**
* 完成化简过程中常量加减法的运算
注: 添加了新的MakeRule函数：`Rule mathS::MakeRule(const Ptr<MathObject>& src_pattern, const std::function<Ptr<MathObject>(std::map<std::string, Ptr<MathObject>>&, bool&)>& do_replace)`
它接受一个用户自定义的do replace并且此do replace有一个flag参数可以在do replace发现已经完成后但仍然让它进行do replace时拒绝继续化简并将flag设为true

**[任务]** 编写高精度乘除法算法：
