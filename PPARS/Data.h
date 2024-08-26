#pragma once
#ifndef DATA_H
#define DATA_H

#include <vector>
#include <gmpxx.h>  // 使用 GMP 库处理大整数
#include "StrSet.h"  // 包含 StrSet 类的头文件

class Data {
public:
    std::vector<mpz_class> x;  // d维数据点，明文
    std::vector<StrSet> fx;    // 每个维度对应的字符串集合，编码后的值集合

    Data() = default;  // 默认构造函数
    // 构造函数，初始化数据点和字符串集合
    Data(const std::vector<mpz_class>& x, int bitlen);
};

#endif // DATA_H

