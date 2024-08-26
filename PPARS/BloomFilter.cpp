#include "BloomFilter.h"

BloomFilter::RandomInRange::RandomInRange(int maximum, int k)
    : max(maximum), count(k), prng(std::random_device{}()) {}

void BloomFilter::RandomInRange::init(const std::size_t& seed) {
    prng.seed(static_cast<unsigned int>(seed));
}

BloomFilter::RandomInRange& BloomFilter::RandomInRange::next() {
    i++;
    value = prng() % max; // 生成范围内的随机值
    if (value < 0) value = -value; // 确保值为正数
    return *this;
}

bool BloomFilter::RandomInRange::hasNext() const {
    return i < count;
}

void BloomFilter::RandomInRange::reset() {
    i = 0;
}

BloomFilter::BloomFilter(int n, int k) : k(k), prng(n, k) {
    int m = static_cast<int>(std::round(k * n / LN2)); // 计算容器的大小
    totalsize = m;
    hashes.resize(m, false); // 初始化位数组
}

void BloomFilter::add(const std::size_t& hash_value) {
    prng.init(hash_value); // 使用元素的哈希值初始化随机数生成器
    while (prng.hasNext()) {
        prng.next(); // 生成下一个随机位置
        hashes[prng.value] = true; // 将该位置设置为true
    }
    prng.reset(); // 重置生成器状态
}

bool BloomFilter::contains(const std::size_t& hash_value) {
    prng.init(hash_value); // 使用元素的哈希值初始化随机数生成器
    while (prng.hasNext()) {
        prng.next(); // 生成下一个随机位置
        if (!hashes[prng.value])
            return false; // 如果任意位置为false，说明元素不在过滤器中
    }
    prng.reset(); // 重置生成器状态
    return true; // 所有位置均为true，元素可能在过滤器中
}

void BloomFilter::clear() {
    std::fill(hashes.begin(), hashes.end(), false); // 将位数组中所有位设置为false
}

BloomFilter BloomFilter::clone() const {
    return *this;
}

std::size_t BloomFilter::hashCode() const {
    std::size_t seed = 0;
    for (bool bit : hashes) {
        seed ^= std::hash<bool>()(bit) + 0x9e3779b9 + (seed << 6) + (seed >> 2); // 混合哈希值
    }
    return seed ^ k;
}

bool BloomFilter::equals(const BloomFilter& other) const {
    return this->hashes == other.hashes && this->k == other.k;
}

void BloomFilter::merge(const BloomFilter& other) {
    if (other.k != this->k || other.hashes.size() != this->hashes.size()) {
        throw std::invalid_argument("Incompatible bloom filters"); // 如果过滤器不兼容，抛出异常
    }
    for (std::size_t i = 0; i < hashes.size(); ++i) {
        hashes[i] = hashes[i] || other.hashes[i]; // 合并位数组
    }
}
