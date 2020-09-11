# DFA_C++
该项目提供 中英文敏感词检测、屏蔽功能，分别使用了两种算法实现：  
1. DFA算法
C++实现的DFA算法，支持中英文、最大匹配、处理大小写、处理标点和特殊字符。提供iOS、Android平台的Demo，在移动平台虽然效率较高，但占用内存较大。  
2. 单层树算法
单层树算法实现敏感词检测、屏蔽功能，分别用Objective-C 和 C++实现，iPhone6真机测试结果， 内存对比DFA算法大大降低，初始化建树时间大大缩短，查询时间在优化算法后也大大提升。

## 测试介绍：

1. DFA算法  
demo里面词库大约7万个左右，建树会产生大约305000个节点，在iphone 6真机上，内存占用28M左右，建树时间2.6s左右，50字内单次查询大约1ms左右。
查询时间确实很快，但是占用内存太大，不建议移动端使用，服务器可以考虑使用。当然本demo还有优化空间，因为直接用了宽字符，会有内存占用浪费的情况。

2. 单层树算法  
实际开发中，整理词库后大约14000 +词，iPhone6真机：
- C++实现，初始化建树时间170ms左右，内存3M左右，120个字有特殊符号单次查询时间3.5ms，120个无特殊符号单次查询时间2.9ms，50个有特殊符号单次查询时间2ms。
- Objective实现，初始化建树时间50ms左右，内存3M左右，120个字有特殊符号单次查询时间14ms，120个无特殊符号单次查询时间8.5ms，50个有特殊符号单次查询时间8ms。

## 工程介绍：

1. SheildDemoCPlusPlus 为iOS demo工程，SheildDemoCPlusPlus/SheildDemoCPlusPlus/DFilter.cpp为算法类

2. CPlusDemo 为Android demo工程

3. FilterDemo 为生成android 各个平台的.so工程

4. adtest 为新算法iOS demo工程 ，包含 adtest/adtest/Filter/FilterManager.mm （objective-c语言）实现的算法， 以及adtest/adtest/Filter/DFilter.cpp（C++）实现的算法。

## 后续：

后续，采用了单层树的方式，取每个词的首字母作为key，首字符后面的按照最长的排在前面作为value，且该value是数组。 
