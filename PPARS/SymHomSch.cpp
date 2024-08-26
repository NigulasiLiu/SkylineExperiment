#include "SymHomSch.h"

// Ĭ�Ϲ��캯��
SymHomSch::SymHomSch() : rnd(gmp_randinit_default), faker(0), fakerp(0) {}

// ���������캯������ʼ�������������
SymHomSch::SymHomSch(const SHSParamters& sp, const SHSParamters& pp)
    : rnd(gmp_randinit_default), secretParam(sp), publicParam(pp) {
    rnd.seed(time(nullptr)); // ʹ�õ�ǰʱ����Ϊ����
}

// ���ƹ��캯��
SymHomSch::SymHomSch(const SymHomSch& other)
    : rnd(gmp_randinit_default), k0(other.k0), k1(other.k1), k2(other.k2),
    secretParam(other.secretParam), publicParam(other.publicParam),
    faker(other.faker), fakerp(other.fakerp) {
    rnd.seed(time(nullptr));
}

// ��Կ���ɺ���
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

// ���ܺ���
mpz_class SymHomSch::Enc(const mpz_class& message, const SHSParamters& Param) {
    mpz_class p = Param.getParams()[0];
    mpz_class L = Param.getParams()[1];
    mpz_class N = Param.getParams()[2];

    mpz_class r = rnd.get_z_bits(k2) + 1;
    mpz_class rp = (rnd.get_z_bits(k0 * 2) + 1) % N;
    return (((r * L) + message) * (1 + rp * p)) % N;
}

// α���ܺ��� (���������������ǰ����)
mpz_class SymHomSch::FakeEnc(const mpz_class& message, const SHSParamters& Param) const {
    mpz_class p = Param.getParams()[0];
    mpz_class L = Param.getParams()[1];
    mpz_class N = Param.getParams()[2];

    return (((faker * L) + message) * (1 + fakerp * p)) % N;
}

// ���ܺ���
mpz_class SymHomSch::Dec(const mpz_class& cipher, const SHSParamters& Param) const {
    mpz_class p = Param.getParams()[0];
    mpz_class L = Param.getParams()[1];
    mpz_class N = Param.getParams()[2];

    mpz_class res = cipher % p;
    res = res % L;
    return res;
}

// ��ȡ��Կ����
SHSParamters SymHomSch::getPublicParams() const {
    return publicParam;
}

// ��ȡ˽Կ����
SHSParamters SymHomSch::getSecretParams() const {
    return secretParam;
}

// ̬ͬ�ӷ�
mpz_class SymHomSch::Add(const mpz_class& cipher1, const mpz_class& cipher2, const SHSParamters& Param) {
    return (cipher1 + cipher2) % Param.getParams()[0];
}

// ̬ͬ�˷�
mpz_class SymHomSch::Mul(const mpz_class& cipher1, const mpz_class& cipher2, const SHSParamters& Param) {
    return (cipher1 * cipher2) % Param.getParams()[0];
}

// ̬ͬ���������ļ����ģ�
mpz_class SymHomSch::Sub(const mpz_class& cipher1, const mpz_class& cipher2, const SHSParamters& Param) {
    return (cipher1 - cipher2) % Param.getParams()[0];
}

// ̬ͬ�˷��Ӽ�����ϣ����ڼ�������һ��ʵ�֣�
mpz_class SymHomSch::SubByMul(const mpz_class& cipher1, const mpz_class& cipher2, const mpz_class& inv1, const SHSParamters& Param) {
    return (cipher1 + Mul(cipher2, inv1, Param)) % Param.getParams()[0];
}
