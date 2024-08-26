#ifndef BFILTER_H
#define BFILTER_H

#include <set>
#include <string>
#include "BloomFilter.h"  // 包含 BloomFilter 类的头文件

/**
 * 将元素映射到布隆过滤器
 */
class BFilter {
public:
    BloomFilter bf0;  // 第一个布隆过滤器实例
    BloomFilter bft;  // 第二个布隆过滤器实例

    BFilter() = default;  // 默认构造函数
    // 构造函数，初始化布隆过滤器
    BFilter(int maxElementNum, int hashfuncNum);

    // 将元素插入到布隆过滤器中
    void insertElement(BloomFilter& bf, const std::set<std::string>& elements);
};

#endif // BFILTER_H
