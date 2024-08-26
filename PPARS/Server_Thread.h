#pragma once
#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <gmpxx.h>
#include <future>  // 用于多线程处理
#include "SymHomSch.h"
#include "SHSParamters.h"
#include "Data.h"
#include "QToken.h"
#include "StrSet.h"

class Server {
public:
    std::vector<mpz_class> pk;  // 公钥
    SymHomSch encrypto;          // 同态加密系统
    SHSParamters pp;             // 公钥参数
    std::vector<Data> dataset;   // 数据集
    int dimens = 0;              // 数据维度
    int bitlen = 0;              // 比特长度
    SymHomSch shs;               // 同态加密方案
    int cnt = 0;                 // 计数器
    int total = 0;               // 总计数

    Server() = default;  // 默认构造函数

    Server(const std::vector<mpz_class>& publicKey, const SHSParamters& publicParam, int dimens,
        int datasize, int bitlen, const SymHomSch& shs, const std::string& filePath);

    mpz_class SDRD(const Data& x1, const Data& x2, std::vector<QToken>& qt, std::map<int, StrSet>& encodeTable,
        std::map<std::string, mpz_class>& resCache);

    void checkCiphers(const std::string& label, int dimens, const std::vector<mpz_class>& ciphers);

    void checkCipher(const std::string& label, const mpz_class& cipher);

    std::string toBinary(int x, int len);

    std::map<int, StrSet> encodeAdvance(const std::vector<Data>& dataset);

    mpz_class bootstrapOne(const mpz_class& cipher);

private:

    mpz_class CalaWithCache(const std::vector<mpz_class>& ebf, const std::set<std::string>& fv, BloomFilter::RandomInRange& prng,
        const std::string& str, std::map<std::string, mpz_class>& resCache);

    mpz_class Cala(const std::vector<mpz_class>& ebf, const std::set<std::string>& fv, BloomFilter::RandomInRange& prng);

    mpz_class bfContains(const std::string& o, const std::vector<mpz_class>& ebf, mpz_class e1, BloomFilter::RandomInRange& prng);

    mpz_class SOR(mpz_class ea, mpz_class eb);
};

#endif // SERVER_H

