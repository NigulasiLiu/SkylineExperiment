#include "StrSet.h"

// ���캯���������������� x ��λ���� bitlen �����ַ�������
StrSet::StrSet(int x, int bitlen) {
    fx = RangeEncoder::buildValueSet(x, bitlen);  // ���� RangeEncoder �ľ�̬���������ַ�������
}
