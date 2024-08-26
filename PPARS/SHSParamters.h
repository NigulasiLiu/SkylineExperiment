#pragma once
#ifndef SHSPARAMETERS_H
#define SHSPARAMETERS_H

#include <vector>
#include <gmpxx.h>  // 使用 GMP 库的 BigInteger 支持

class SHSParamters {
protected:
    std::vector<mpz_class> param;  // 使用 vector 来替代 Java 的 List

public:
    // 默认构造函数
    SHSParamters();

    // 构造函数
    SHSParamters(const std::vector<mpz_class>& param);

    // 获取参数列表
    const std::vector<mpz_class>& getParams() const;

    // 设置参数列表
    void setParams(const std::vector<mpz_class>& key);
};

#endif // SHSPARAMETERS_H
