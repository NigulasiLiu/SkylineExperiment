#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <string>
#include <map>
#include <chrono>
#include <gmpxx.h>
#include "SymHomSch.h"
#include "SHSParamters.h"
#include "Data.h"
#include "QToken.h"
#include "BFilter.h"
#include "StrSet.h"
#include "RangeEncoder.h"
#include "GenQueryToken.h"
#include "Server.h"

void runExperiment(const std::string& datasetPath, int repeat, const std::vector<int>& datasetSizes, int dimension) {
    for (int datasetSize : datasetSizes) {
        // 生成结果文件名
        std::string outputFileName = datasetPath.substr(datasetPath.find_last_of("/") + 1) + "_n=" + std::to_string(datasetSize) + "_d=" + std::to_string(dimension) + "_results.txt";

        // 打开文件流
        std::ofstream outFile(outputFileName);
        if (!outFile.is_open()) {
            std::cerr << "Error opening file for writing: " << outputFileName << std::endl;
            return;
        }

        outFile << "Running experiments on dataset: " << datasetPath << std::endl;
        std::cout << "Running experiments on dataset: " << datasetPath << ", n=" << datasetSize << ", d=" << dimension << std::endl;

        for (int r = 0; r < repeat; ++r) {
            // 初始化密钥
            SymHomSch shs;

            int k0 = 4096; // p 和 q, r'
            int k1 = 80;   // 消息长度
            int k2 = 160;  // L 和 r

            shs.KeyGen(k0, k1, k2);

            SHSParamters pp = shs.getPublicParams();
            SHSParamters sp = shs.getSecretParams();

            mpz_class E0 = shs.Enc(mpz_class(0), sp);
            mpz_class E1 = shs.Enc(mpz_class(1), sp);
            mpz_class E2 = shs.Enc(mpz_class(-1), sp);
            std::vector<mpz_class> pk = { E0, E1, E2 };

            outFile << "[+] Dimension=" << dimension << " (Repeat " << (r + 1) << ")" << std::endl;
            outFile << "[+] Set k0=" << k0 << "; k1=" << k1 << "; k2=" << k2 << std::endl;
            outFile << "[+] |p|=" << sp.getParams()[0].get_mpz_t()->_mp_size * sizeof(mp_limb_t) * 8
                    << "; |L|=" << sp.getParams()[1].get_mpz_t()->_mp_size * sizeof(mp_limb_t) * 8
                    << "; |N|=" << k2 << " + " << sp.getParams()[2].get_mpz_t()->_mp_size * sizeof(mp_limb_t) * 8 << std::endl;
            outFile << "[+] Cipher bitLength=" << E0.get_mpz_t()->_mp_size * sizeof(mp_limb_t) * 8
                    << "; Decrypt pk E0=" << shs.Dec(pk[0], sp) << "; E1=" << shs.Dec(pk[1], sp) << std::endl;

            int domain = 256;
            int bitlen = 8;
            int maxElementNum = 2 * bitlen - 2;
            int hashfuncNum = 10;

            double ttoken = 0;
            double tsearch = 0;

            int loop = 1;

            // 准备查询点
            GenQueryToken gqt(sp, dimension, loop);

            // 准备数据库
            Server server(pk, pp, dimension, datasetSize, bitlen, shs, datasetPath);
            std::vector<Data> dataset = server.dataset;

            // 预编码
            std::map<int, StrSet> encodeTable = server.encodeAdvance(dataset);

            for (int l = 0; l < loop; ++l) {
                std::vector<mpz_class> qpoint = gqt.querySet[l];

                // 生成查询令牌
                auto startToken = std::chrono::high_resolution_clock::now();
                std::vector<QToken> QT(dimension);
                for (int i = 0; i < dimension; ++i) {
                    mpz_class qi = qpoint[i];

                    std::set<std::string> sq0 = RangeEncoder::buildRangeSet(0, qi.get_si(), bitlen);
                    std::set<std::string> sqt = RangeEncoder::buildRangeSet(qi.get_si(), domain, bitlen);

                    BFilter bf(maxElementNum, hashfuncNum);
                    bf.insertElement(bf.bf0, sq0);
                    bf.insertElement(bf.bft, sqt);

                    QToken qti(bf.bf0.hashes.size());
                    qti.insert0(bf.bf0.hashes, shs, sp, bf.bf0.prng);
                    qti.insertt(bf.bft.hashes, shs, sp, bf.bft.prng);

                    QT[i] = qti;
                }
                auto endToken = std::chrono::high_resolution_clock::now();
                ttoken += std::chrono::duration<double, std::milli>(endToken - startToken).count();

                // 搜索
                std::map<std::string, mpz_class> resCache;
                auto startSearch = std::chrono::high_resolution_clock::now();
                mpz_class st = pk[0];  // E(0)
                for (size_t i = 0; i < dataset.size(); ++i) {
                    Data& xi = dataset[i];
                    mpz_class fi = pk[1];  // E(1)

                    for (size_t j = 0; j < dataset.size(); ++j) {
                        if (i != j) {
                            Data& xj = dataset[j];
                            mpz_class delta = server.SDRD(xi, xj, QT, encodeTable, resCache);
                            fi = shs.Mul(fi, shs.SubByMul(pk[1], delta, pk[2], sp), sp);
                            fi = server.bootstrapOne(fi);
                        }
                    }

                    st = shs.Add(st, fi, sp);
                }
                auto endSearch = std::chrono::high_resolution_clock::now();
                tsearch += std::chrono::duration<double, std::milli>(endSearch - startSearch).count();
                resCache.clear();
            }

            outFile << "Datasize=" << datasetSize << "; dimens=" << dimension << "; Repeat (query)=" << loop << "; hashfuncNum=" << hashfuncNum
                      << "; Gen token Time cost=" << ttoken / loop
                      << "(ms); Search Time cost=" << tsearch / loop << "(ms)" << std::endl;
        }

        outFile.close();  // 关闭文件流
    }
}

int main() {
    int dimension = 3;  // 固定维度
    std::vector<int> datasetSizes = { 200, 400, 600, 800, 1000, 1200, 1400, 1600, 1800, 2000 };  // 数据集大小
    int repeat = 10;  // 每组实验重复次数

    // 数据集路径列表
    std::vector<std::string> datasets = {
        "/home/pengyubo/wlby/PPARS/sources/eyestate_dataset.csv",
        "/home/pengyubo/wlby/PPARS/sources/house_dataset.csv",
        "/home/pengyubo/wlby/PPARS/sources/uscensus_dataset.csv"
    };

    // 对每个数据集运行实验
    for (const auto& datasetPath : datasets) {
        runExperiment(datasetPath, repeat, datasetSizes, dimension);
    }

    return 0;
}

