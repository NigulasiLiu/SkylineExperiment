#pragma once
#ifndef RSKYLINEPLAIN_H
#define RSKYLINEPLAIN_H

#include <vector>
#include <gmpxx.h>
#include "Data.h"  // 假设 Data 类已实现

class RSkylinePlain {
public:
    int dimens;  // 数据维度

    // 构造函数，初始化维度
    RSkylinePlain(int dimens);

    // 在明文数据集中搜索反向天际线
    std::vector<std::vector<mpz_class>> searchReversePlain(const std::vector<mpz_class>& qc, const std::vector<Data>& plainSet);

private:
    // 检查 p1 是否支配 p2
    bool dominateCheckPlain(const std::vector<mpz_class>& p1, const std::vector<mpz_class>& p2);

    // 计算两个向量之间的绝对距离
    std::vector<mpz_class> getVectorAbs(const std::vector<mpz_class>& x, const std::vector<mpz_class>& y);
};

#endif // RSKYLINEPLAIN_H
