#pragma once
#ifndef SYMHOMSCH_H
#define SYMHOMSCH_H

#include <gmpxx.h>
#include <vector>
#include <random>
#include "SHSParamters.h"  // �ٶ� SHSParamters ��ʵ��

class SymHomSch {
private:
    gmp_randclass rnd;        // ��������������� GMP ��
    int k0 = 0, k1 = 0, k2 = 0;  // ��Կ����
    SHSParamters secretParam;  // ˽Կ���� (sp)
    SHSParamters publicParam;  // ��Կ���� (pp)
    mpz_class faker = 0;
    mpz_class fakerp = 0;

public:
    // Ĭ�Ϲ��캯��
    SymHomSch();

    // ���������캯������ʼ�������������
    SymHomSch(const SHSParamters& sp, const SHSParamters& pp);

    // ���ƹ��캯��
    SymHomSch(const SymHomSch& other);

    // ��Կ���ɺ���
    void KeyGen(int param_k0, int param_k1, int param_k2);

    // ���ܺ���
    mpz_class Enc(const mpz_class& message, const SHSParamters& Param);

    // α���ܺ��� (���������������ǰ����)
    mpz_class FakeEnc(const mpz_class& message, const SHSParamters& Param) const;

    // ���ܺ���
    mpz_class Dec(const mpz_class& cipher, const SHSParamters& Param) const;

    // ��ȡ��Կ����
    SHSParamters getPublicParams() const;

    // ��ȡ˽Կ����
    SHSParamters getSecretParams() const;

    // ̬ͬ�ӷ�
    static mpz_class Add(const mpz_class& cipher1, const mpz_class& cipher2, const SHSParamters& Param);

    // ̬ͬ�˷�
    static mpz_class Mul(const mpz_class& cipher1, const mpz_class& cipher2, const SHSParamters& Param);

    // ̬ͬ���������ļ����ģ�
    static mpz_class Sub(const mpz_class& cipher1, const mpz_class& cipher2, const SHSParamters& Param);

    // ̬ͬ�˷��Ӽ�����ϣ����ڼ�������һ��ʵ�֣�
    static mpz_class SubByMul(const mpz_class& cipher1, const mpz_class& cipher2, const mpz_class& inv1, const SHSParamters& Param);
};

#endif // SYMHOMSCH_H
