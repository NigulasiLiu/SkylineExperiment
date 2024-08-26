#pragma once
#ifndef STRSET_H
#define STRSET_H

#include <set>
#include <string>
#include "RangeEncoder.h"  // ���� RangeEncoder ����ʵ��

/**
 * StrSet �࣬��ʾһ���ַ�������
 */
class StrSet {
public:
    std::set<std::string> fx;  // �ַ�������

    StrSet() = default;  // Ĭ�Ϲ��캯��

    // ���캯���������������� x ��λ���� bitlen �����ַ�������
    StrSet(int x, int bitlen);
};

#endif // STRSET_H
