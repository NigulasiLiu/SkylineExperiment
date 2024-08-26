#pragma once
#ifndef STRSET_H
#define STRSET_H

#include <set>
#include <string>
#include "RangeEncoder.h"  // 假设 RangeEncoder 类已实现

/**
 * StrSet 类，表示一个字符串集合
 */
class StrSet {
public:
    std::set<std::string> fx;  // 字符串集合

    StrSet() = default;  // 默认构造函数

    // 构造函数，根据输入整数 x 和位长度 bitlen 生成字符串集合
    StrSet(int x, int bitlen);
};

#endif // STRSET_H
