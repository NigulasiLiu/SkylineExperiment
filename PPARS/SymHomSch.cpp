#include "SymHomSch.h"

// 默认构造函数
SymHomSch::SymHomSch() : rnd(gmp_randinit_default), faker(0), fakerp(0) {}

// 参数化构造函数，初始化随机数生成器
SymHomSch::SymHomSch(const SHSParamters& sp, const SHSParamters& pp)
    : rnd(gmp_randinit_default), secretParam(sp), publicParam(pp) {
    rnd.seed(time(nullptr)); // 使用当前时间作为种子
}

// 复制构造函数
SymHomSch::SymHomSch(const SymHomSch& other)
    : rnd(gmp_randinit_default), k0(other.k0), k1(other.k1), k2(other.k2),
    secretParam(other.secretParam), publicParam(other.publicParam),
    faker(other.faker), fakerp(other.fakerp) {
    rnd.seed(time(nullptr));
}

// 密钥生成函数
void SymHomSch::KeyGen(int param_k0, int param_k1, int param_k2) {
    k0 = param_k0;
    k1 = param_k1;
    k2 = param_k2;

    mpz_class p = rnd.get_z_bits(k0);
    mpz_class q = rnd.get_z_bits(k0);
    mpz_class N = p * q;

    mpz_class L = rnd.get_z_bits(k2) + 1;

    secretParam = SHSParamters({ p, L, N, q });
    publicParam = SHSParamters({ N });

    faker = rnd.get_z_bits(k2) + 1;
    fakerp = (rnd.get_z_bits(k0 * 2) + 1) % N;
}

// 加密函数
mpz_class SymHomSch::Enc(const mpz_class& message, const SHSParamters& Param) {
    mpz_class p = Param.getParams()[0];
    mpz_class L = Param.getParams()[1];
    mpz_class N = Param.getParams()[2];

    mpz_class r = rnd.get_z_bits(k2) + 1;
    mpz_class rp = (rnd.get_z_bits(k0 * 2) + 1) % N;
    return (((r * L) + message) * (1 + rp * p)) % N;
}

// 伪加密函数 (假设所有随机数提前计算)
mpz_class SymHomSch::FakeEnc(const mpz_class& message, const SHSParamters& Param) const {
    mpz_class p = Param.getParams()[0];
    mpz_class L = Param.getParams()[1];
    mpz_class N = Param.getParams()[2];

    return (((faker * L) + message) * (1 + fakerp * p)) % N;
}

// 解密函数
mpz_class SymHomSch::Dec(const mpz_class& cipher, const SHSParamters& Param) const {
    mpz_class p = Param.getParams()[0];
    mpz_class L = Param.getParams()[1];
    mpz_class N = Param.getParams()[2];

    mpz_class res = cipher % p;
    res = res % L;
    return res;
}

// 获取公钥参数
SHSParamters SymHomSch::getPublicParams() const {
    return publicParam;
}

// 获取私钥参数
SHSParamters SymHomSch::getSecretParams() const {
    return secretParam;
}

// 同态加法
mpz_class SymHomSch::Add(const mpz_class& cipher1, const mpz_class& cipher2, const SHSParamters& Param) {
    return (cipher1 + cipher2) % Param.getParams()[0];
}

// 同态乘法
mpz_class SymHomSch::Mul(const mpz_class& cipher1, const mpz_class& cipher2, const SHSParamters& Param) {
    return (cipher1 * cipher2) % Param.getParams()[0];
}

// 同态减法（密文减明文）
mpz_class SymHomSch::Sub(const mpz_class& cipher1, const mpz_class& cipher2, const SHSParamters& Param) {
    return (cipher1 - cipher2) % Param.getParams()[0];
}

// 同态乘法加减法组合（用于减法的另一种实现）
mpz_class SymHomSch::SubByMul(const mpz_class& cipher1, const mpz_class& cipher2, const mpz_class& inv1, const SHSParamters& Param) {
    return (cipher1 + Mul(cipher2, inv1, Param)) % Param.getParams()[0];
}
