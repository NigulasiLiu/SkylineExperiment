#include "Data.h"  // �����Լ���ͷ�ļ�

// ���캯������ʼ�����ݵ���ַ�������
Data::Data(const std::vector<mpz_class>& x, int bitlen) : x(x), fx(x.size()) {
    for (std::size_t i = 0; i < x.size(); ++i) {
        StrSet ss(x[i].get_si(), bitlen);  // �� mpz_class ת��Ϊ int
        fx[i] = ss;  // �� StrSet ����洢�� fx ������
    }
}
