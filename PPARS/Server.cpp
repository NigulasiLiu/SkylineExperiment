#include "Server.h"
#include "DataBases.h"  // 包含 DataBases 类的定义

Server::Server(const std::vector<mpz_class>& publicKey, const SHSParamters& publicParam, int dimens,
    int datasize, int bitlen, const SymHomSch& shs, const std::string& filePath)
    : pk(publicKey), pp(publicParam), dimens(dimens), bitlen(bitlen), shs(shs) {
    DataBases db(pk, pp, dimens, datasize, bitlen, filePath);
    dataset = db.plainSet;
}

mpz_class Server::bootstrapOne(const mpz_class& cipher) {
    SHSParamters sp = shs.getSecretParams();
    mpz_class plain = shs.Dec(cipher, sp);
    return shs.FakeEnc(plain, sp);
}

mpz_class Server::SDRD(const Data& x1, const Data& x2, std::vector<QToken>& qt, std::map<int, StrSet>& encodeTable,
    std::map<std::string, mpz_class>& resCache) {
    mpz_class totalAlpha = pk[1];
    mpz_class totalBeta = pk[1];

    for (int i = 0; i < dimens; ++i) {
        QToken& qti = qt[i];
        mpz_class ealpha, ebeta;

        if (x2.x[i] >= 2 * x1.x[i]) { // case 1
            const std::set<std::string>& fx2i = x2.fx[i].fx;

            std::string astr = std::to_string(x2.x[i].get_si()) + '|' + std::to_string(i) + '|' + '0';
            ealpha = CalaWithCache(qti.eBF0, fx2i, qti.prng0, astr, resCache);

            std::string bstr = std::to_string(x2.x[i].get_si()) + '|' + std::to_string(i) + '|' + 't';
            ebeta = CalaWithCache(qti.eBFt, fx2i, qti.prngt, bstr, resCache);
        }
        else {  // case 2
            const std::set<std::string>& fx2i = x2.fx[i].fx;
            int tmp = 2 * x1.x[i].get_si() - x2.x[i].get_si();
            auto it = encodeTable.find(tmp);
            if (it == encodeTable.end()) {
                StrSet newValue(tmp, bitlen);
                it = encodeTable.insert({ tmp, newValue }).first;
            }
            const std::set<std::string>& fx12i = it->second.fx;

            if (x2.x[i].get_si() >= tmp) {
                std::string a1str = std::to_string(x2.x[i].get_si()) + '|' + std::to_string(i) + '|' + '0';
                mpz_class a1 = CalaWithCache(qti.eBF0, fx2i, qti.prng0, a1str, resCache);

                std::string a2str = std::to_string(tmp) + '|' + std::to_string(i) + '|' + 't';
                mpz_class a2 = CalaWithCache(qti.eBFt, fx12i, qti.prngt, a2str, resCache);

                ealpha = SOR(a1, a2);

                std::string b1str = std::to_string(x2.x[i].get_si()) + '|' + std::to_string(i) + '|' + 't';
                mpz_class b1 = CalaWithCache(qti.eBFt, fx2i, qti.prngt, b1str, resCache);

                std::string b2str = std::to_string(tmp) + '|' + std::to_string(i) + '|' + '0';
                mpz_class b2 = CalaWithCache(qti.eBF0, fx12i, qti.prng0, b2str, resCache);

                ebeta = encrypto.Mul(b1, b2, pp);
            }
        }

        totalAlpha = encrypto.Mul(totalAlpha, ealpha, pp);
        totalBeta = encrypto.Mul(totalBeta, ebeta, pp);

        totalAlpha = bootstrapOne(totalAlpha);
        totalBeta = bootstrapOne(totalBeta);
    }

    mpz_class delta = encrypto.Mul(totalAlpha, encrypto.SubByMul(pk[1], totalBeta, pk[2], pp), pp);
    delta = bootstrapOne(delta);

    return delta;
}

mpz_class Server::CalaWithCache(const std::vector<mpz_class>& ebf, const std::set<std::string>& fv, BloomFilter::RandomInRange& prng,
    const std::string& str, std::map<std::string, mpz_class>& resCache) {
    mpz_class res;
    auto it = resCache.find(str);
    if (it == resCache.end()) {
        res = Cala(ebf, fv, prng);
        resCache[str] = res;
    }
    else {
        res = it->second;
        ++cnt;
    }

    ++total;
    return res;
}

mpz_class Server::Cala(const std::vector<mpz_class>& ebf, const std::set<std::string>& fv, BloomFilter::RandomInRange& prng) {
    auto it = fv.begin();
    mpz_class res = pk[0];
    for (int i = 0; i < fv.size(); ++i) {
        mpz_class ethetai = bfContains(*it++, ebf, pk[1], prng);
        res = SOR(res, ethetai);
        res = bootstrapOne(res);
    }

    return res;
}

mpz_class Server::bfContains(const std::string& o, const std::vector<mpz_class>& ebf, mpz_class e1, BloomFilter::RandomInRange& prng) {
    std::hash<std::string> hasher;
    size_t seed = hasher(o);
    prng.init(seed);

    while (prng.hasNext()) {
        prng.next();
        e1 = encrypto.Mul(e1, ebf[prng.value], pp);
    }

    return bootstrapOne(e1);
}

mpz_class Server::SOR(mpz_class ea, mpz_class eb) {
    mpz_class ec = encrypto.Add(ea, eb, pp);
    return encrypto.SubByMul(ec, encrypto.Mul(ea, eb, pp), pk[2], pp);
}

void Server::checkCiphers(const std::string& label, int dimens, const std::vector<mpz_class>& ciphers) {
    auto sp = shs.getSecretParams();
    std::cout << label << "=>";
    for (int j = 0; j < dimens; ++j) {
        std::cout << shs.Dec(ciphers[j], sp) << ",";
    }
    std::cout << std::endl;
}

void Server::checkCipher(const std::string& label, const mpz_class& cipher) {
    auto sp = shs.getSecretParams();
    std::cout << label << "=>" << shs.Dec(cipher, sp) << std::endl;
}

std::string Server::toBinary(int x, int len) {
    std::string result;
    for (int i = len - 1; i >= 0; --i) {
        int mask = 1 << i;
        result.append((x & mask) != 0 ? "1" : "0");
    }
    return result;
}

std::map<int, StrSet> Server::encodeAdvance(const std::vector<Data>& dataset) {
    std::map<int, StrSet> encodeTable;
    for (int i = 0; i < dataset.size(); ++i) {
        const auto& xi = dataset[i].x;
        for (int j = 0; j < dataset.size(); ++j) {
            if (i != j) {
                const auto& xj = dataset[j].x;
                for (int k = 0; k < dimens; ++k) {
                    int key = 2 * xi[k].get_si() - xj[k].get_si();
                    auto it = encodeTable.find(key);
                    if (it == encodeTable.end()) {
                        StrSet newValue(key, bitlen);
                        encodeTable[key] = newValue;
                    }
                }
            }
        }
    }
    return encodeTable;
}

