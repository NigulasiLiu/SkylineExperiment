#ifndef BFILTER_H
#define BFILTER_H

#include <set>
#include <string>
#include "BloomFilter.h"  // ���� BloomFilter ���ͷ�ļ�

/**
 * ��Ԫ��ӳ�䵽��¡������
 */
class BFilter {
public:
    BloomFilter bf0;  // ��һ����¡������ʵ��
    BloomFilter bft;  // �ڶ�����¡������ʵ��

    BFilter() = default;  // Ĭ�Ϲ��캯��
    // ���캯������ʼ����¡������
    BFilter(int maxElementNum, int hashfuncNum);

    // ��Ԫ�ز��뵽��¡��������
    void insertElement(BloomFilter& bf, const std::set<std::string>& elements);
};

#endif // BFILTER_H
