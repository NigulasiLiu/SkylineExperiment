#pragma once
#ifndef SYMHOMSCH_H
#define SYMHOMSCH_H

#include <gmpxx.h>
#include <vector>
#include <random>
#include "SHSParamters.h"  // 假定 SHSParamters 已实现

class SymHomSch {
private:
    gmp_randclass rnd;        // 用于生成随机数的 GMP 类
    int k0 = 0, k1 = 0, k2 = 0;  // 密钥参数
    SHSParamters secretParam;  // 私钥参数 (sp)
    SHSParamters publicParam;  // 公钥参数 (pp)
    mpz_class faker = 0;
    mpz_class fakerp = 0;

public:
    // 默认构造函数
    SymHomSch();

    // 参数化构造函数，初始化随机数生成器
    SymHomSch(const SHSParamters& sp, const SHSParamters& pp);

    // 复制构造函数
    SymHomSch(const SymHomSch& other);

    // 密钥生成函数
    void KeyGen(int param_k0, int param_k1, int param_k2);

    // 加密函数
    mpz_class Enc(const mpz_class& message, const SHSParamters& Param);

    // 伪加密函数 (假设所有随机数提前计算)
    mpz_class FakeEnc(const mpz_class& message, const SHSParamters& Param) const;

    // 解密函数
    mpz_class Dec(const mpz_class& cipher, const SHSParamters& Param) const;

    // 获取公钥参数
    SHSParamters getPublicParams() const;

    // 获取私钥参数
    SHSParamters getSecretParams() const;

    // 同态加法
    static mpz_class Add(const mpz_class& cipher1, const mpz_class& cipher2, const SHSParamters& Param);

    // 同态乘法
    static mpz_class Mul(const mpz_class& cipher1, const mpz_class& cipher2, const SHSParamters& Param);

    // 同态减法（密文减明文）
    static mpz_class Sub(const mpz_class& cipher1, const mpz_class& cipher2, const SHSParamters& Param);

    // 同态乘法加减法组合（用于减法的另一种实现）
    static mpz_class SubByMul(const mpz_class& cipher1, const mpz_class& cipher2, const mpz_class& inv1, const SHSParamters& Param);
};

#endif // SYMHOMSCH_H
