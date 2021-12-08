#include <windows.h>
#include <TlHelp32.h>
#include "common.h"
#include <conio.h>
#include <iostream>
#include "RuleModel.h"
#include "../Timer/Timer.h"
#include "DBRuleService.h"
#define KEYDOWN( vk ) ( 0x8000 & ::GetAsyncKeyState( vk ) ) 

void callb() {

    PROCESSENTRY32 pe32;
    // ��ʹ������ṹ֮ǰ�����������Ĵ�С
    pe32.dwSize = sizeof(pe32);

    // ��ϵͳ�ڵ����н�����һ������
    HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE)
    {
        printf(" CreateToolhelp32Snapshot����ʧ�ܣ� \n");
        return ;
    }

    // �������̿��գ�������ʾÿ�����̵���Ϣ
    BOOL bMore = ::Process32First(hProcessSnap, &pe32);
    while (bMore)
    {
        //printf(" �������ƣ�%s \n", pe32.szExeFile);
        //CHAR tt[] = TEXT("haha");
        //printf(" �������ƣ�%s \n", tt);
        //cout << pe32.szExeFile << endl; 
        //CHAR cFileName[9]= "ATUO.BAT";
        //printf("File name is %s\n",cFileName);
        //wchar_t buf[100] = L"123";
        //wprintf(L"%ls\n", buf);
        wprintf(L"Process Name is : %ls\n", pe32.szExeFile);
        printf(" Process ID is��%u \n\n", pe32.th32ProcessID);

        bMore = ::Process32Next(hProcessSnap, &pe32);
    }

    // ��Ҫ���������snapshot����
    ::CloseHandle(hProcessSnap);
    std::cout << "test ok"<<std::endl;
    return;
    };
int main(void)
{
    std::cout << "hello0" << std::endl;
    printf("hello00");
    DB::DBRuleService DBRS;
    DB::TimeControllerRule* rule=new DB::TimeControllerRule();
    rule->SetRuleName("2");
    rule->SetProgramName("3");
    rule->SetProgramTitle("3");
    rule->SetProgramDirectory("4");
    rule->SetRunPath("5");
    rule->SetStartTime(DB::timeval{  15, 10 });
    rule->SetEndTime(DB::timeval{ 20,20 });
    rule->SetPerPeriodTime(15);
    rule->SetTimes(16);
    rule->SetRunMode(DB::rmTimes);
    rule->SetLimitRule(DB::LimitRule::g);
    rule->SetTotalTime(DB::timeval{ 30,40 });
    std::cout << "hello1" << std::endl;
    WindowsTimer timer;
    timer.setCallback(callb);
    timer.start(1, true);
    OutputDebugString( "hello");
    while (true)
    {
        if (KEYDOWN(VK_ESCAPE)) // ��ESC�˳�,������ģʽ��ÿ��ѭ������ͣ������
            break;
        //OutputDebugString ("1");
    }
    return 0;

    /*DBRS.addRule(rule); */

    //DBRS.setRule("2",rule);
    
}