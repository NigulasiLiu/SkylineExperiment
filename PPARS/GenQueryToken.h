#pragma once
#ifndef GENQUERYTOKEN_H
#define GENQUERYTOKEN_H

#include <vector>
#include <gmpxx.h>
#include "SymHomSch.h"  // 假设 SymHomSch 类已实现
#include "SHSParamters.h" // 假设 SHSParamters 类已实现

class GenQueryToken {
public:
    SymHomSch encryptor;  // 加密器实例
    SHSParamters sp;      // 私钥参数
    int dimens;           // 数据维度
    std::vector<std::vector<mpz_class>> querySet;  // 存储查询点的数组

    // 构造函数，初始化私钥参数和数据维度，并读取查询点
    GenQueryToken(const SHSParamters& sp, const SHSParamters& pp, int dimens, int qs);
    GenQueryToken(const SHSParamters& sp, int dimens, int qs);

    // 从文件中读取查询点
    void readQuery(int dimens, int qs);

    // 使用 SHE 加密明文
    std::vector<std::vector<mpz_class>> vectorEnc(const std::vector<std::vector<mpz_class>>& plainSet);
};

#endif // GENQUERYTOKEN_H
