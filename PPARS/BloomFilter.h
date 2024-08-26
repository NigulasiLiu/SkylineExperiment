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
    // Ƕ���࣬�������ɷ�Χ�������
    class RandomInRange {
    public:
        std::mt19937 prng; // �����������
        int max; // ���ص����ֵ + 1
        int count; // Ҫ���ɵ����Ԫ������
        int i = 0; // �����ɵ�Ԫ������
        int value; // ��ǰ���ɵ�ֵ

        // Ĭ�Ϲ��캯��
        RandomInRange()
            : prng(std::random_device{}()), max(0), count(0), i(0), value(0) {}

        // ���캯������ʼ��������������Ͳ���
        RandomInRange(int maximum, int k);
        // ��ʼ�������������������
        void init(const std::size_t& seed);

        // ������һ������������µ�ǰֵ
        RandomInRange& next();

        // ����Ƿ��и�����������Ҫ����
        bool hasNext() const;

        // �������ɼ�����
        void reset();
    };

    RandomInRange prng; // �������������Ϊ��ĳ�Ա����

    std::vector<bool> hashes; // ���ڴ洢��¡��������λ����
    int totalsize; // ������������С���Ա���Ϊ��λ
    int k; // ����ϣ��������
    static constexpr double LN2 = 0.6931471805599453; // ��Ȼ������ln(2)�ĳ���ֵ

    // ���캯������ʼ����¡�������Ĵ�С�͹�ϣ��������
    BloomFilter(int n, int k);
    // ��Ԫ����ӵ���������
    void add(const std::size_t& hash_value);

    // ���Ԫ���Ƿ���ܴ����ڹ�������
    bool contains(const std::size_t& hash_value);

    // ��չ���������������λ
    void clear();

    // ���Ƶ�ǰ������
    BloomFilter clone() const;

    // ����һ��Ψһ�Ĺ�ϣֵ��ʾ������
    std::size_t hashCode() const;

    // ����������������λ�����Ƿ����
    bool equals(const BloomFilter& other) const;

    // ����һ����¡�������ϲ�����ǰ��������
    void merge(const BloomFilter& other);
};

#endif // BLOOMFILTER_H
