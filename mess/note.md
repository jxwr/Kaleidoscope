## notes
- seq的attribute为fusion::tuple，注意这里不能用"boost/tuple/tuple.hpp"，需要用"boost/fusion/include/boost_tuple.hpp"
- alternative的attr为variant
- visitor需要定义result_type类型
- 如果不写grammar，就得写action, 也就是grammar=rule+actino
- ConstantFP::get(getGlobalContext(), APFloat(Val)) 生成浮点数
- make_tuple也要使用fusion版本，`#include boost/fusion/tuple/make_tuple.hpp`
- 使用stack变量的方式可以避免phi-node，但是会造成大量内存读写，mem2reg-pass可以优化
- http://llvm.org/docs/tutorial/LangImpl7.html#id4
- 

## 2014 8,31

似乎boost::spirit还挺容易使用的，但是是我不喜欢fusion的结构定义宏，用variant,tuple,vector似乎也足够了，外面套一层struct，定义一些方法操作各项的字段即可。Grammar部分到底是不是要搞很多grammar出来，似乎不必要，重点还是要事先定义好BNF。从spirit到ast似乎是个很直接也很容易的过程，问题是需要定一个好用一点的visitor模式，用于prettyprint和codegen。剩下的就是玩llvm了，llvm的文档不是很好用，得仔细看看SSA，phi-node。




