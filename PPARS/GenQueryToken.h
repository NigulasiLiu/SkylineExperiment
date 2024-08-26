#pragma once
#ifndef GENQUERYTOKEN_H
#define GENQUERYTOKEN_H

#include <vector>
#include <gmpxx.h>
#include "SymHomSch.h"  // ���� SymHomSch ����ʵ��
#include "SHSParamters.h" // ���� SHSParamters ����ʵ��

class GenQueryToken {
public:
    SymHomSch encryptor;  // ������ʵ��
    SHSParamters sp;      // ˽Կ����
    int dimens;           // ����ά��
    std::vector<std::vector<mpz_class>> querySet;  // �洢��ѯ�������

    // ���캯������ʼ��˽Կ����������ά�ȣ�����ȡ��ѯ��
    GenQueryToken(const SHSParamters& sp, const SHSParamters& pp, int dimens, int qs);
    GenQueryToken(const SHSParamters& sp, int dimens, int qs);

    // ���ļ��ж�ȡ��ѯ��
    void readQuery(int dimens, int qs);

    // ʹ�� SHE ��������
    std::vector<std::vector<mpz_class>> vectorEnc(const std::vector<std::vector<mpz_class>>& plainSet);
};

#endif // GENQUERYTOKEN_H
