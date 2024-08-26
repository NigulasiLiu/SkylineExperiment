#include "RangeEncoder.h"
#include <future>  // 引入用于多线程处理的头文件

std::set<std::string> RangeEncoder::buildRangeSet(float v, float v1, int bitlen) {
    int left = static_cast<int>(v) + (1 << bitlen);
    int right = static_cast<int>(v1) + (1 << bitlen);
    std::set<std::string> ret;

    int commonPrefix = 0;
    while (left > 0) {
        int highestOneBit = highestOneBitPosition(left);
        if (highestOneBit == highestOneBitPosition(right)) {
            left -= (1 << highestOneBit);
            right -= (1 << highestOneBit);
            commonPrefix += (1 << highestOneBit);
        }
        else {
            break;
        }
    }
    int m = highestOneBitPosition(right);

    // 使用类名代替 this 指针来调用静态成员函数
    auto leftPartFuture = std::async(std::launch::async, &RangeEncoder::buildLeftPart, left, m, commonPrefix);
    auto rightPartFuture = std::async(std::launch::async, &RangeEncoder::buildRightPart, right, m, commonPrefix);

    // 获取任务结果
    auto leftPartSet = leftPartFuture.get();
    auto rightPartSet = rightPartFuture.get();

    // 将结果插入到最终集合中
    ret.insert(leftPartSet.begin(), leftPartSet.end());
    ret.insert(rightPartSet.begin(), rightPartSet.end());

    return ret;
}

std::set<std::string> RangeEncoder::buildValueSet(float value, int bitlen) {
    int c = static_cast<int>(value) + (1 << bitlen);
    std::string cStr = std::bitset<32>(c).to_string();  // 将整数转换为二进制字符串
    cStr.erase(0, cStr.find_first_not_of('0'));  // 删除前导0

    std::set<std::string> resultSet;
    for (size_t j = 1; j <= cStr.size(); ++j) {
        resultSet.insert(cStr.substr(0, j));
    }
    return resultSet;
}

std::set<std::string> RangeEncoder::buildLeftPart(int left, int m, int commonPrefix) {
    std::set<std::string> ret;
    while (left % 2 == 0 && m > left + 1) {
        left >>= 1;
        m >>= 1;
        commonPrefix >>= 1;
    }
    ret.insert(toBinaryString(commonPrefix + left));

    while (m > left + 1) {
        left >>= 1;
        m >>= 1;
        commonPrefix >>= 1;
        if (left % 2 == 0) {
            left += 1;
            ret.insert(toBinaryString(commonPrefix + left));
        }
    }

    return ret;
}

std::set<std::string> RangeEncoder::buildRightPart(int right, int m, int commonPrefix) {
    std::set<std::string> ret;
    while (right % 2 == 1 && m > 1 && m < right) {
        right >>= 1;
        m >>= 1;
        commonPrefix >>= 1;
    }
    ret.insert(toBinaryString(commonPrefix + right));

    while (m > 1 && m < right) {
        right >>= 1;
        m >>= 1;
        commonPrefix >>= 1;
        if (right % 2 == 1) {
            right -= 1;
            ret.insert(toBinaryString(commonPrefix + right));
        }
    }

    return ret;
}

std::string RangeEncoder::toBinaryString(int n) {
    return std::bitset<32>(n).to_string().substr(32 - static_cast<size_t>(std::log2(n + 1)));
}

int RangeEncoder::highestOneBitPosition(int x) {
    return x > 0 ? static_cast<int>(std::log2(x)) : 0;
}

