#include "SHSParamters.h"

// Ĭ�Ϲ��캯��
SHSParamters::SHSParamters() : param() {}

// ���캯��
SHSParamters::SHSParamters(const std::vector<mpz_class>& param) : param(param) {}

// ��ȡ�����б�
const std::vector<mpz_class>& SHSParamters::getParams() const {
    return param;
}

// ���ò����б�
void SHSParamters::setParams(const std::vector<mpz_class>& key) {
    this->param = key;
}
