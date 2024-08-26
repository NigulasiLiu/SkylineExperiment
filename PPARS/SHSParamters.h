#pragma once
#ifndef SHSPARAMETERS_H
#define SHSPARAMETERS_H

#include <vector>
#include <gmpxx.h>  // ʹ�� GMP ��� BigInteger ֧��

class SHSParamters {
protected:
    std::vector<mpz_class> param;  // ʹ�� vector ����� Java �� List

public:
    // Ĭ�Ϲ��캯��
    SHSParamters();

    // ���캯��
    SHSParamters(const std::vector<mpz_class>& param);

    // ��ȡ�����б�
    const std::vector<mpz_class>& getParams() const;

    // ���ò����б�
    void setParams(const std::vector<mpz_class>& key);
};

#endif // SHSPARAMETERS_H
