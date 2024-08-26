#ifndef BLOOMFILTER_H
#define BLOOMFILTER_H

#include <vector>
#include <random>
#include <stdexcept>
#include <algorithm>
#include <functional>
#include <cmath>

class BloomFilter {
public:
    // 嵌套类，用于生成范围内随机数
    class RandomInRange {
    public:
        std::mt19937 prng; // 随机数生成器
        int max; // 返回的最大值 + 1
        int count; // 要生成的随机元素数量
        int i = 0; // 已生成的元素数量
        int value; // 当前生成的值

        // 默认构造函数
        RandomInRange()
            : prng(std::random_device{}()), max(0), count(0), i(0), value(0) {}

        // 构造函数，初始化随机数生成器和参数
        RandomInRange(int maximum, int k);
        // 初始化随机数生成器的种子
        void init(const std::size_t& seed);

        // 生成下一个随机数并更新当前值
        RandomInRange& next();

        // 检查是否还有更多的随机数需要生成
        bool hasNext() const;

        // 重置生成计数器
        void reset();
    };

    RandomInRange prng; // 随机数生成器作为类的成员变量

    std::vector<bool> hashes; // 用于存储布隆过滤器的位数组
    int totalsize; // 容器的期望大小，以比特为单位
    int k; // 最大哈希函数数量
    static constexpr double LN2 = 0.6931471805599453; // 自然对数的ln(2)的常数值

    // 构造函数，初始化布隆过滤器的大小和哈希函数数量
    BloomFilter(int n, int k);
    // 将元素添加到过滤器中
    void add(const std::size_t& hash_value);

    // 检查元素是否可能存在于过滤器中
    bool contains(const std::size_t& hash_value);

    // 清空过滤器，重置所有位
    void clear();

    // 复制当前过滤器
    BloomFilter clone() const;

    // 生成一个唯一的哈希值表示过滤器
    std::size_t hashCode() const;

    // 测试两个过滤器的位数组是否相等
    bool equals(const BloomFilter& other) const;

    // 将另一个布隆过滤器合并到当前过滤器中
    void merge(const BloomFilter& other);
};

#endif // BLOOMFILTER_H
