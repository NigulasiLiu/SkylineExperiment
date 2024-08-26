#pragma once
#ifndef QTOKEN_H
#define QTOKEN_H

#include <vector>
#include <gmpxx.h>
#include <bitset>
#include "SymHomSch.h"   // ���� SymHomSch ����ʵ��
#include "SHSParamters.h" // ���� SHSParamters ����ʵ��
#include "BloomFilter.h"  // ���� BloomFilter �� RandomInRange ���� BloomFilter ����ʵ��

/**
 * ����¡������ת��Ϊ��ѯ����
 */
class QToken {
public:
    std::vector<mpz_class> eBF0;  // ���ܺ�Ĳ�¡������ 0
    std::vector<mpz_class> eBFt;  // ���ܺ�Ĳ�¡������ t

    BloomFilter::RandomInRange prng0; // ����������� 0
    BloomFilter::RandomInRange prngt; // ����������� t


    // Ĭ�Ϲ��캯��
    QToken() : prng0(0, 0) {} // ʹ���ʵ���Ĭ��ֵ��ʼ�� prng0
    // �Զ��幹�캯����ȷ����Ա��������ȷ��ʼ��
    QToken(int seed, int max_range, int num_elements)
        : prng0(max_range, num_elements) { // ����Ĳ���Ӧ�� RandomInRange ���캯��ƥ��
        prng0.init(seed); // ��ʼ�������������������
    }

    // ���캯������ʼ�����ܲ�¡�������ĳ���
    QToken(int len);

    // ����Ԫ�ص����ܲ�¡������ 0
    void insert0(const std::vector<bool>& bfset, SymHomSch& shs, SHSParamters& sp, BloomFilter::RandomInRange& prng);

    // ����Ԫ�ص����ܲ�¡������ t
    void insertt(const std::vector<bool>& bfset, SymHomSch& shs, SHSParamters& sp, BloomFilter::RandomInRange& prng);

};

#endif // QTOKEN_H
