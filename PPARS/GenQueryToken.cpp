#include "GenQueryToken.h"
#include <iostream>
#include <fstream>
#include <sstream>

// 构造函数，初始化私钥参数和数据维度，并读取查询点
GenQueryToken::GenQueryToken(const SHSParamters& sp, const SHSParamters& pp, int dimens, int qs)
    : encryptor(sp, pp), sp(sp), dimens(dimens) {
    readQuery(dimens, qs);  // 从文件中读取查询点
}

GenQueryToken::GenQueryToken(const SHSParamters& sp, int dimens, int qs)
    : sp(sp), dimens(dimens) {
    readQuery(dimens, qs);
}

// 从文件中读取查询点
void GenQueryToken::readQuery(int dimens, int qs) {
    int cnt = 0;

    std::string filePath = "/home/pengyubo/wlby/PPARS/sources/query-point.csv";
    std::ifstream file(filePath);

    if (!file) {
        std::cerr << "Error, file " << filePath << " not exist!" << std::endl;
        exit(1);
    }

    std::string line;
    while (std::getline(file, line) && cnt < qs) {
        std::istringstream lineStream(line);
        std::string item;
        std::vector<mpz_class> arr(dimens);

        for (int i = 0; i < dimens && std::getline(lineStream, item, ','); ++i) {
            arr[i] = mpz_class(item);
        }

        querySet.push_back(arr);
        cnt++;
    }

    file.close();
}

// 使用 SHE 加密明文
std::vector<std::vector<mpz_class>> GenQueryToken::vectorEnc(const std::vector<std::vector<mpz_class>>& plainSet) {
    std::vector<std::vector<mpz_class>> cipherSet;

    for (const auto& pitem : plainSet) {
        std::vector<mpz_class> citem(dimens);

        for (int j = 0; j < dimens; ++j) {
            mpz_class c = encryptor.Enc(pitem[j], sp);
            citem[j] = c;
        }

        cipherSet.push_back(citem);
    }

    return cipherSet;
}
