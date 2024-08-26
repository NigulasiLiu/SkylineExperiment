#include "QToken.h"

// ���캯������ʼ�����ܲ�¡�������ĳ���
QToken::QToken(int len)
    : eBF0(len), eBFt(len), prng0(0, 0), prngt(0, 0) {}

// ����Ԫ�ص����ܲ�¡������ 0
void QToken::insert0(const std::vector<bool>& bfset, SymHomSch& shs, SHSParamters& sp, BloomFilter::RandomInRange& prng) {
    for (std::size_t i = 0; i < eBF0.size(); ++i) {
        mpz_class c;
        if (bfset[i]) {
            c = shs.Enc(1, sp);
        }
        else {
            c = shs.Enc(0, sp);
        }
        eBF0[i] = c;
    }
    prng0 = prng;
}

// ����Ԫ�ص����ܲ�¡������ t
void QToken::insertt(const std::vector<bool>& bfset, SymHomSch& shs, SHSParamters& sp, BloomFilter::RandomInRange& prng) {
    for (std::size_t i = 0; i < eBFt.size(); ++i) {
        mpz_class c;
        if (bfset[i]) {
            c = shs.Enc(1, sp);
        }
        else {
            c = shs.Enc(0, sp);
        }
        eBFt[i] = c;
    }
    prngt = prng;
}
