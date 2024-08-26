#include "BFilter.h"  // �����Լ���ͷ�ļ�

// ���캯������ʼ����¡������
BFilter::BFilter(int maxElementNum, int hashfuncNum)
    : bf0(maxElementNum, hashfuncNum), bft(maxElementNum, hashfuncNum) {}

// ��Ԫ�ز��뵽��¡��������
void BFilter::insertElement(BloomFilter& bf, const std::set<std::string>& elements) {
    for (const auto& e : elements) {
        std::size_t hash_value = std::hash<std::string>{}(e);  // �����ַ����Ĺ�ϣֵ
        bf.add(hash_value);  // ����ϣֵ��ӵ���¡������
    }
}
