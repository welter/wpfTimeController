#pragma once
#include <cstdint>
#include <wtypes.h>
#include <xstring>
#include "../Timer/pscmd.h"
#include <Psapi.h>
//����˽ӿڲ���ָ��
    const unsigned char MP_TIMERCONTROLER_STOP = 1;
    const unsigned char MP_TIMERCONTROLER_RESUME = 2;
    const unsigned char MP_TIMERCONTROLER_RESET = 3;
    const unsigned char MP_TIMERCONTROLER_TERMINATEPROC = 4;
    const unsigned char MP_TIMERCONTROLER_LOGON = 5;
    const unsigned char MP_TIMERCONTROLER_LOGOFF = 6;
    const unsigned char MP_TIMERCONTROLER_LOADSETTING = 7;
    const unsigned char MP_TIMERCONTROLER_QUERYPROCESSINFO = 8;//��ѯ������Ϣ
    const unsigned char MP_TIMERCONTROLER_GetPROCESSES = 9;//��ȡ��ǰ���н�����Ϣ
    const unsigned char MP_TIMERCONTROLER_RETURN_PROCESSINFORMATION = 10;//���ؽ�����Ϣ
    const unsigned char MP_TIMERCONTROLER_RETURN_PROCESSINFORMATIONS = 11;//���ض��������Ϣ

    const std::string DES_KEY = "asdfasdfsadfasdf";

    typedef struct processInformation {
		PROCESS_MEMORY_COUNTERS* pmc;
        
        PCMDBUFFER_T commandLine;
        int commandLineSize;
    };
    typedef struct exchangeMessage {
        std::string header = "WPFTIMER";
        std::string USERNAME;
        std::string USER_TOKEN;
        int cmd;
        int contextLength;
        std::string context;
    };
namespace DB {
    const unsigned char rmSpecifiedTime = 0b00000001;//��ָ����ʱ�������
    const unsigned char rmTimes = 0b00000010;    //ÿ�����еĴ���
    const unsigned char rmTimeinterval = 0b00000100;//ÿ�������б�������ʱ��
    const unsigned char rmDuration = 0b00001000;//ÿ�����������ʱ��
    const unsigned char rmProhibitSpecifiedTime = 0b00010000;//��ֹ��ָ����ʱ�������
    const unsigned char rmTotalTime = 0b00100000;//ÿ���ܹ�����ʱ��
	enum LimitRule {f,g,h,i,j};

#define enum_to_string(x) #x

    typedef struct timeval
    {
        long tv_sec; /*��*/
        long tv_usec; /*΢��*/
    };
    class TimeControllerRule;

    

}
