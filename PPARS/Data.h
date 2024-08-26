#pragma once
#ifndef DATA_H
#define DATA_H

#include <vector>
#include <gmpxx.h>  // ʹ�� GMP �⴦�������
#include "StrSet.h"  // ���� StrSet ���ͷ�ļ�

class Data {
public:
    std::vector<mpz_class> x;  // dά���ݵ㣬����
    std::vector<StrSet> fx;    // ÿ��ά�ȶ�Ӧ���ַ������ϣ�������ֵ����

    Data() = default;  // Ĭ�Ϲ��캯��
    // ���캯������ʼ�����ݵ���ַ�������
    Data(const std::vector<mpz_class>& x, int bitlen);
};

#endif // DATA_H

