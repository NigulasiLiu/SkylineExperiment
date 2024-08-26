#include "BFilter.h"  // 包含自己的头文件

// 构造函数，初始化布隆过滤器
BFilter::BFilter(int maxElementNum, int hashfuncNum)
    : bf0(maxElementNum, hashfuncNum), bft(maxElementNum, hashfuncNum) {}

// 将元素插入到布隆过滤器中
void BFilter::insertElement(BloomFilter& bf, const std::set<std::string>& elements) {
    for (const auto& e : elements) {
        std::size_t hash_value = std::hash<std::string>{}(e);  // 计算字符串的哈希值
        bf.add(hash_value);  // 将哈希值添加到布隆过滤器
    }
}
