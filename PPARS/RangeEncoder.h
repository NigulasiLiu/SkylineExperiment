#ifndef RANGEENCODER_H
#define RANGEENCODER_H

#include <string>
#include <set>
#include <bitset>
#include <cmath>
#include <algorithm>

class RangeEncoder {
public:
    // 静态方法：生成范围集合
    static std::set<std::string> buildRangeSet(float v, float v1, int bitlen);

    // 静态方法：生成值集合
    static std::set<std::string> buildValueSet(float value, int bitlen);

private:
    // 静态方法：构建左部分的字符串集合
    static std::set<std::string> buildLeftPart(int left, int m, int commonPrefix);

    // 静态方法：构建右部分的字符串集合
    static std::set<std::string> buildRightPart(int right, int m, int commonPrefix);

    // 静态方法：将整数转换为二进制字符串
    static std::string toBinaryString(int n);

    // 静态方法：获取最高位为1的位置
    static int highestOneBitPosition(int x);
};

#endif // RANGEENCODER_H
