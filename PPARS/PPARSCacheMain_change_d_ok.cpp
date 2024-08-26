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

void runExperiment(const std::string& datasetPath, int repeat, const std::vector<int>& dimensions, int datasetSize) {
    // 生成结果文件名
    std::string outputFileName = datasetPath.substr(datasetPath.find_last_of("/") + 1) + "_results.txt";
    
    // 打开文件流
    std::ofstream outFile(outputFileName);
    if (!outFile.is_open()) {
        std::cerr << "Error opening file for writing: " << outputFileName << std::endl;
        return;
    }

    outFile << "Dimension,DatasetSize,GenTokenTime(ms),SearchTime(ms),Repeat" << std::endl;

    for (int r = 0; r < repeat; ++r) {
    std::cout << "Dimension,DatasetSize,GenTokenTime(ms),SearchTime(ms),Repeat:" << r+1 <<std::endl;
        for (int dimension : dimensions) {
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

            // 记录每次的实验结果
            outFile << dimension << ","
                    << datasetSize << ","
                    << ttoken / loop << ","
                    << tsearch / loop << ","
                    << (r + 1) << std::endl;
        }
    }

    outFile.close();  // 关闭文件流
}

int main() {
    //std::vector<int> dimensions = { 2, 3, 4, 5, 6, 7, 8 };  // 设置需要测试的维度
    std::vector<int> dimensions = { 8 };  // 设置需要测试的维度
    int datasetSize = 1000;  // 数据集大小
    int repeat = 1;  // 每组实验重复次数

    // 数据集路径列表
    std::vector<std::string> datasets = {
        "/home/pengyubo/wlby/PPARS/sources/eyestate_dataset.csv",
       // "/home/pengyubo/wlby/PPARS/sources/house_dataset.csv",
       //"/home/pengyubo/wlby/PPARS/sources/uscensus_dataset.csv"
    };

    // 对每个数据集运行实验
    for (const auto& datasetPath : datasets) {
        runExperiment(datasetPath, repeat, dimensions, datasetSize);
    }

    return 0;
}

