#include "Data.h"  // 包含自己的头文件

// 构造函数，初始化数据点和字符串集合
Data::Data(const std::vector<mpz_class>& x, int bitlen) : x(x), fx(x.size()) {
    for (std::size_t i = 0; i < x.size(); ++i) {
        StrSet ss(x[i].get_si(), bitlen);  // 将 mpz_class 转换为 int
        fx[i] = ss;  // 将 StrSet 对象存储在 fx 向量中
    }
}
