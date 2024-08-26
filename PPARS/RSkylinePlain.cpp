#include "RSkylinePlain.h"
#include <gmp.h>

// 构造函数，初始化维度
RSkylinePlain::RSkylinePlain(int dimens) : dimens(dimens) {}

// 在明文数据集中搜索反向天际线
std::vector<std::vector<mpz_class>> RSkylinePlain::searchReversePlain(const std::vector<mpz_class>& qc, const std::vector<Data>& plainSet) {
    int n = plainSet.size();
    std::vector<std::vector<mpz_class>> reverseSkyline;

    for (int i = 0; i < n; ++i) {
        std::vector<mpz_class> p = plainSet[i].x;
        std::vector<std::vector<mpz_class>> plainAbs;

        std::vector<mpz_class> qAbs = getVectorAbs(qc, p);
        plainAbs.push_back(qAbs);

        for (int j = 0; j < n; ++j) {
            if (i == j) {
                continue;  // 相同值，跳过
            }
            std::vector<mpz_class> tmp = plainSet[j].x;
            plainAbs.push_back(getVectorAbs(tmp, p));
        }

        // 检查第一个与其他的支配关系
        bool flag = false;
        for (int j = 1; j < n; ++j) {
            std::vector<mpz_class> testAbs = plainAbs[j];
            flag = dominateCheckPlain(testAbs, qAbs);  // true：支配；false：不支配
            if (flag) {
                break;
            }
        }

        // 如果没有支配qc的点
        if (!flag) {
            reverseSkyline.push_back(p);
        }
    }

    return reverseSkyline;
}

// 检查 p1 是否支配 p2
bool RSkylinePlain::dominateCheckPlain(const std::vector<mpz_class>& p1, const std::vector<mpz_class>& p2) {
    mpz_class sumP1 = 0;
    mpz_class sumP2 = 0;

    for (int i = 0; i < this->dimens; ++i) {
        if (p1[i] > p2[i]) {
            return false;  // p1 不支配 p2
        }
        sumP1 += p1[i];
        sumP2 += p2[i];
    }

    // sumP1 必须小于 sumP2
    if (sumP1 >= sumP2) {
        return false;
    }

    return true;  // 支配关系成立
}

// 计算两个向量之间的绝对距离
std::vector<mpz_class> RSkylinePlain::getVectorAbs(const std::vector<mpz_class>& x, const std::vector<mpz_class>& y) {
    std::vector<mpz_class> tc(dimens);
    for (int i = 0; i < dimens; ++i) {
        mpz_class diff = x[i] - y[i];
        mpz_pow_ui(tc[i].get_mpz_t(), diff.get_mpz_t(), 2);  // 计算平方差
    }
    return tc;
}
