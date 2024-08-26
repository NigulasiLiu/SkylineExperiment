#include "DataBases.h"
#include <iostream>
#include <fstream>
#include <sstream>

DataBases::DataBases(const std::vector<mpz_class>& publicKey, const SHSParamters& publicParam, int dimens, int datasize, int bitlen, const std::string& filePath)
    : pk(publicKey), pp(publicParam), dimens(dimens), bitlen(bitlen) {
    readData(dimens, datasize, filePath);  // 从文件中读取数据
}

void DataBases::readData(int dimens, int ds, const std::string& filePath) {
    int cnt = 0;

    std::ifstream file(filePath);

    if (!file) {
        std::cerr << "Error, file " << filePath << " not exist!" << std::endl;
        exit(1);
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream lineStream(line);
        std::string item;
        std::vector<mpz_class> arr(dimens);

        if (cnt >= ds) {
            break;
        }

        for (int i = 0; i < dimens && std::getline(lineStream, item, ','); ++i) {
            mpf_class tmp(item);
            if (tmp < 0) {
                std::cerr << "Data has negative numbers" << std::endl;
                exit(0);
            }
            arr[i] = tmp.get_si();
        }

        Data data(arr, bitlen);
        plainSet.push_back(data);
        cnt++;
    }

    file.close();
}

