#include "BloomFilter.h"

BloomFilter::RandomInRange::RandomInRange(int maximum, int k)
    : max(maximum), count(k), prng(std::random_device{}()) {}

void BloomFilter::RandomInRange::init(const std::size_t& seed) {
    prng.seed(static_cast<unsigned int>(seed));
}

BloomFilter::RandomInRange& BloomFilter::RandomInRange::next() {
    i++;
    value = prng() % max; // ���ɷ�Χ�ڵ����ֵ
    if (value < 0) value = -value; // ȷ��ֵΪ����
    return *this;
}

bool BloomFilter::RandomInRange::hasNext() const {
    return i < count;
}

void BloomFilter::RandomInRange::reset() {
    i = 0;
}

BloomFilter::BloomFilter(int n, int k) : k(k), prng(n, k) {
    int m = static_cast<int>(std::round(k * n / LN2)); // ���������Ĵ�С
    totalsize = m;
    hashes.resize(m, false); // ��ʼ��λ����
}

void BloomFilter::add(const std::size_t& hash_value) {
    prng.init(hash_value); // ʹ��Ԫ�صĹ�ϣֵ��ʼ�������������
    while (prng.hasNext()) {
        prng.next(); // ������һ�����λ��
        hashes[prng.value] = true; // ����λ������Ϊtrue
    }
    prng.reset(); // ����������״̬
}

bool BloomFilter::contains(const std::size_t& hash_value) {
    prng.init(hash_value); // ʹ��Ԫ�صĹ�ϣֵ��ʼ�������������
    while (prng.hasNext()) {
        prng.next(); // ������һ�����λ��
        if (!hashes[prng.value])
            return false; // �������λ��Ϊfalse��˵��Ԫ�ز��ڹ�������
    }
    prng.reset(); // ����������״̬
    return true; // ����λ�þ�Ϊtrue��Ԫ�ؿ����ڹ�������
}

void BloomFilter::clear() {
    std::fill(hashes.begin(), hashes.end(), false); // ��λ����������λ����Ϊfalse
}

BloomFilter BloomFilter::clone() const {
    return *this;
}

std::size_t BloomFilter::hashCode() const {
    std::size_t seed = 0;
    for (bool bit : hashes) {
        seed ^= std::hash<bool>()(bit) + 0x9e3779b9 + (seed << 6) + (seed >> 2); // ��Ϲ�ϣֵ
    }
    return seed ^ k;
}

bool BloomFilter::equals(const BloomFilter& other) const {
    return this->hashes == other.hashes && this->k == other.k;
}

void BloomFilter::merge(const BloomFilter& other) {
    if (other.k != this->k || other.hashes.size() != this->hashes.size()) {
        throw std::invalid_argument("Incompatible bloom filters"); // ��������������ݣ��׳��쳣
    }
    for (std::size_t i = 0; i < hashes.size(); ++i) {
        hashes[i] = hashes[i] || other.hashes[i]; // �ϲ�λ����
    }
}
