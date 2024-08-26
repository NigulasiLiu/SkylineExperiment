#ifndef DATABASES_H
#define DATABASES_H

#include <vector>
#include <gmpxx.h>
#include "SymHomSch.h"
#include "SHSParamters.h"
#include "Data.h"

class DataBases {
public:
    std::vector<mpz_class> pk;     // Public key
    SHSParamters pp;               // Public parameters
    int dimens;                    // Dimensions
    int bitlen;                    // Bit length
    std::vector<Data> plainSet;    // Plain data set

    DataBases(const std::vector<mpz_class>& publicKey, const SHSParamters& publicParam, int dimens, int datasize, int bitlen, const std::string& filePath);
    void readData(int dimens, int ds, const std::string& filePath);
};

#endif // DATABASES_H

