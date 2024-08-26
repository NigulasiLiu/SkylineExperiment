#include "StrSet.h"

// 构造函数，根据输入整数 x 和位长度 bitlen 生成字符串集合
StrSet::StrSet(int x, int bitlen) {
    fx = RangeEncoder::buildValueSet(x, bitlen);  // 调用 RangeEncoder 的静态方法生成字符串集合
}
