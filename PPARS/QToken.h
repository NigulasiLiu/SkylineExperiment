#pragma once
#ifndef QTOKEN_H
#define QTOKEN_H

#include <vector>
#include <gmpxx.h>
#include <bitset>
#include "SymHomSch.h"   // 假设 SymHomSch 类已实现
#include "SHSParamters.h" // 假设 SHSParamters 类已实现
#include "BloomFilter.h"  // 假设 BloomFilter 和 RandomInRange 类在 BloomFilter 中已实现

/**
 * 将布隆过滤器转换为查询令牌
 */
class QToken {
public:
    std::vector<mpz_class> eBF0;  // 加密后的布隆过滤器 0
    std::vector<mpz_class> eBFt;  // 加密后的布隆过滤器 t

    BloomFilter::RandomInRange prng0; // 随机数生成器 0
    BloomFilter::RandomInRange prngt; // 随机数生成器 t


    // 默认构造函数
    QToken() : prng0(0, 0) {} // 使用适当的默认值初始化 prng0
    // 自定义构造函数，确保成员变量被正确初始化
    QToken(int seed, int max_range, int num_elements)
        : prng0(max_range, num_elements) { // 这里的参数应与 RandomInRange 构造函数匹配
        prng0.init(seed); // 初始化随机数生成器的种子
    }

    // 构造函数，初始化加密布隆过滤器的长度
    QToken(int len);

    // 插入元素到加密布隆过滤器 0
    void insert0(const std::vector<bool>& bfset, SymHomSch& shs, SHSParamters& sp, BloomFilter::RandomInRange& prng);

    // 插入元素到加密布隆过滤器 t
    void insertt(const std::vector<bool>& bfset, SymHomSch& shs, SHSParamters& sp, BloomFilter::RandomInRange& prng);

};

#endif // QTOKEN_H
