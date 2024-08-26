#include "SHSParamters.h"

// 默认构造函数
SHSParamters::SHSParamters() : param() {}

// 构造函数
SHSParamters::SHSParamters(const std::vector<mpz_class>& param) : param(param) {}

// 获取参数列表
const std::vector<mpz_class>& SHSParamters::getParams() const {
    return param;
}

// 设置参数列表
void SHSParamters::setParams(const std::vector<mpz_class>& key) {
    this->param = key;
}
