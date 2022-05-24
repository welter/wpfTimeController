#pragma once
#include <cstdint>
const unsigned char WM_TIMERCONTROLER_STOP = 1;
namespace DB {
    const unsigned char rmSpecifiedTime = 0b00000001;//��ָ����ʱ�������
    const unsigned char rmTimes = 0b00000010;    //ÿ�����еĴ���
    const unsigned char rmTimeinterval = 0b00000100;//ÿ�������б�������ʱ��
    const unsigned char rmDuration = 0b00001000;//ÿ�����������ʱ��
    const unsigned char rmProhibitSpecifiedTime = 0b00010000;//��ֹ��ָ����ʱ�������
    const unsigned char rmTotalTime = 0b00100000;//ÿ���ܹ�����ʱ��
	enum LimitRule {f,g,h,i,j};

#define enum_to_string(x) #x

    struct timeval
    {
        long tv_sec; /*��*/
        long tv_usec; /*΢��*/
    };
    class TimeControllerRule;

    

}
