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
    const char testToken[16] = { '\x30','\x92','\x73','\xc4','\x06','\x8b','\x8d','\xeb','\x52','\x74','\x0c','\x6c','\xeb','\x28','\x8f','\x85'};
    const std::string DES_KEY = "asdfasdf";

    typedef struct processInformation {
		PROCESS_MEMORY_COUNTERS* pmc;
        
        PCMDBUFFER_T commandLine;
        int commandLineSize;
    };
    typedef struct struLogData {
        time_t logTime;
        DWORD cntThreads;
        DWORD cntUsage;
        DWORD dwFlags;
        DWORD dwSize;
        DWORD pcPriClassBase;
        std::string szExeFile;
        ULONG_PTR th32DefaultHeapID;
        DWORD th32ModuleID;
        DWORD th32ParentProcessID;
        DWORD th32ProcessID;
    };


    typedef struct exchangeMessage {
        PCHAR  header = "WPFTIMER";
        PCHAR USERNAME;
        int LEN_USER_TOKEN;
        PCHAR USER_TOKEN;
        int cmd;
        int LEN_CONTEXT;
        PCHAR context;
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
