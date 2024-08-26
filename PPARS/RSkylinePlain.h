#pragma once
#ifndef RSKYLINEPLAIN_H
#define RSKYLINEPLAIN_H

#include <vector>
#include <gmpxx.h>
#include "Data.h"  // ���� Data ����ʵ��

class RSkylinePlain {
public:
    int dimens;  // ����ά��

    // ���캯������ʼ��ά��
    RSkylinePlain(int dimens);

    // ���������ݼ����������������
    std::vector<std::vector<mpz_class>> searchReversePlain(const std::vector<mpz_class>& qc, const std::vector<Data>& plainSet);

private:
    // ��� p1 �Ƿ�֧�� p2
    bool dominateCheckPlain(const std::vector<mpz_class>& p1, const std::vector<mpz_class>& p2);

    // ������������֮��ľ��Ծ���
    std::vector<mpz_class> getVectorAbs(const std::vector<mpz_class>& x, const std::vector<mpz_class>& y);
};

#endif // RSKYLINEPLAIN_H
