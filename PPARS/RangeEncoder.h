#ifndef RANGEENCODER_H
#define RANGEENCODER_H

#include <string>
#include <set>
#include <bitset>
#include <cmath>
#include <algorithm>

class RangeEncoder {
public:
    // ��̬���������ɷ�Χ����
    static std::set<std::string> buildRangeSet(float v, float v1, int bitlen);

    // ��̬����������ֵ����
    static std::set<std::string> buildValueSet(float value, int bitlen);

private:
    // ��̬�����������󲿷ֵ��ַ�������
    static std::set<std::string> buildLeftPart(int left, int m, int commonPrefix);

    // ��̬�����������Ҳ��ֵ��ַ�������
    static std::set<std::string> buildRightPart(int right, int m, int commonPrefix);

    // ��̬������������ת��Ϊ�������ַ���
    static std::string toBinaryString(int n);

    // ��̬��������ȡ���λΪ1��λ��
    static int highestOneBitPosition(int x);
};

#endif // RANGEENCODER_H
