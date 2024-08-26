#include "RSkylinePlain.h"
#include <gmp.h>

// ���캯������ʼ��ά��
RSkylinePlain::RSkylinePlain(int dimens) : dimens(dimens) {}

// ���������ݼ����������������
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
                continue;  // ��ֵͬ������
            }
            std::vector<mpz_class> tmp = plainSet[j].x;
            plainAbs.push_back(getVectorAbs(tmp, p));
        }

        // ����һ����������֧���ϵ
        bool flag = false;
        for (int j = 1; j < n; ++j) {
            std::vector<mpz_class> testAbs = plainAbs[j];
            flag = dominateCheckPlain(testAbs, qAbs);  // true��֧�䣻false����֧��
            if (flag) {
                break;
            }
        }

        // ���û��֧��qc�ĵ�
        if (!flag) {
            reverseSkyline.push_back(p);
        }
    }

    return reverseSkyline;
}

// ��� p1 �Ƿ�֧�� p2
bool RSkylinePlain::dominateCheckPlain(const std::vector<mpz_class>& p1, const std::vector<mpz_class>& p2) {
    mpz_class sumP1 = 0;
    mpz_class sumP2 = 0;

    for (int i = 0; i < this->dimens; ++i) {
        if (p1[i] > p2[i]) {
            return false;  // p1 ��֧�� p2
        }
        sumP1 += p1[i];
        sumP2 += p2[i];
    }

    // sumP1 ����С�� sumP2
    if (sumP1 >= sumP2) {
        return false;
    }

    return true;  // ֧���ϵ����
}

// ������������֮��ľ��Ծ���
std::vector<mpz_class> RSkylinePlain::getVectorAbs(const std::vector<mpz_class>& x, const std::vector<mpz_class>& y) {
    std::vector<mpz_class> tc(dimens);
    for (int i = 0; i < dimens; ++i) {
        mpz_class diff = x[i] - y[i];
        mpz_pow_ui(tc[i].get_mpz_t(), diff.get_mpz_t(), 2);  // ����ƽ����
    }
    return tc;
}
