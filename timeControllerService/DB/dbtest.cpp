#include <windows.h>
#include <TlHelp32.h>
#include "common.h"
#include <conio.h>
#include <iostream>
#include <fstream>
#include <Psapi.h>
#pragma comment(lib,"Psapi.lib")
#include "RuleModel.h"
#include "../Timer/Timer.h"
#include "DBRuleService.h"
using namespace std;
#define KEYDOWN( vk ) ( 0x8000 & ::GetAsyncKeyState( vk ) ) 

const char* logFilePath = "test.log";
const int moniteInterval = 1000;
const int eachRunInterval = 600000;
const int ruleTypeCount = 6;
struct processesInfo {
    DWORD   size;
    DWORD   usage;
    DWORD   processID;          // this process
    string  defaultHeapID;
    DWORD   moduleID;           // associated exe
    DWORD   threads;
    DWORD   parentProcessID;    // this process's parent process
    LONG    priClassBase;         // Base priority of process's threads
    DWORD   flags;
    string  processName;
    time_t startTime;
    time_t lastRunTime;
    time_t runningTime;
    int runTimes;
    time_t timeAfterPrevRun;
};
static processesInfo* *moniteProcesses=new processesInfo*[10];
static int** processesByRule = new int* [ruleTypeCount-1];
static int maxMoniteProc=1;
int findMoniteProc(string procName) {
    for (int i = 0; i < maxMoniteProc; i++) {
        if (moniteProcesses[i]->processName == procName) return i;
    }
    return -1;
}

void callb() {

    ofstream logFile(logFilePath, ios::app|ios::_Noreplace);
    bool logFileOpen = logFile.is_open();
    PROCESSENTRY32 pe32;
    // 在使用这个结构之前，先设置它的大小
    pe32.dwSize = sizeof(pe32);
    // 给系统内的所有进程拍一个快照
    time_t now = time(0);
    char* dt = ctime(&now);
    if (logFileOpen) logFile << "记录时间：" << dt << endl;
    HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE)
    {
        printf(" CreateToolhelp32Snapshot调用失败！ \n");
        return ;
    }
    // 遍历进程快照，轮流显示每个进程的信息
    BOOL bMore = ::Process32First(hProcessSnap, &pe32);
    while (bMore)
    {
        if (logFileOpen)
        logFile << pe32.cntThreads << "  " << pe32.cntUsage << "  " << pe32.dwFlags << "   " << pe32.dwSize << "  " << pe32.pcPriClassBase << "  " << pe32.szExeFile << "  " << pe32.th32DefaultHeapID
            << "  " << pe32.th32ModuleID << "  " << pe32.th32ParentProcessID << "  " << pe32.th32ProcessID << endl;
        if (int pi=findMoniteProc(pe32.szExeFile)!=-1) {
            cout << "finded" << endl;
            moniteProcesses[pi]->defaultHeapID = pe32.th32DefaultHeapID;
            moniteProcesses[pi]->flags = pe32.dwFlags;
            if (now - moniteProcesses[pi]->lastRunTime > eachRunInterval) moniteProcesses[pi]->runTimes += 1;
            moniteProcesses[pi]->lastRunTime = now;
            moniteProcesses[pi]->moduleID = pe32.th32ModuleID;
            moniteProcesses[pi]->parentProcessID=pe32.th32ParentProcessID;
            moniteProcesses[pi]->priClassBase=pe32.pcPriClassBase;
            moniteProcesses[pi]->processID=pe32.th32ProcessID;
            moniteProcesses[pi]->runningTime+=moniteInterval;
            moniteProcesses[pi]->size=pe32.dwSize;
            moniteProcesses[pi]->threads = pe32.cntThreads;
            moniteProcesses[pi]->usage = pe32.cntUsage;
            moniteProcesses[pi]->timeAfterPrevRun += moniteInterval;            
        }
        wprintf(L"Process Name is : %ls\n", pe32.szExeFile);
        
        printf(" Process ID is：%u \n\n", pe32.th32ProcessID);

        bMore = ::Process32Next(hProcessSnap, &pe32);
    }

    // 释放snapshot对象
    ::CloseHandle(hProcessSnap);
    std::cout << "test ok"<<std::endl;
    if (logFileOpen) logFile.close();
    return;
    };
int main(void)
{
    std::cout << "hello0" << std::endl;
    printf("hello00");
    DB::DBRuleService DBRS;
    DB::TimeControllerRule* rule = new DB::TimeControllerRule();
    DBRS.getRule(rule, 1);
    rule->SetRuleName("2");
    rule->SetProgramName("3");
    rule->SetProgramTitle("3");
    rule->SetProgramDirectory("4");
    rule->SetRunPath("5");
    rule->SetStartTime(10);
    rule->SetEndTime(20);
    rule->SetPerPeriodTime(15);
    rule->SetTimes(16);
    rule->SetRunMode(DB::rmTimes);
    rule->SetLimitRule(DB::LimitRule::g);
    rule->SetTotalTime(30);
    std::cout << "hello1" << std::endl;
    maxMoniteProc = DBRS.getRuleCount();
    DB::TimeControllerRule*** rules;
    DBRS.getAllRule(rules, maxMoniteProc);
    moniteProcesses = new  processesInfo* [maxMoniteProc];   
        for (int i = 0; i < ruleTypeCount; i++) {
        processesByRule[i] = new int[maxMoniteProc - 1];
        for (int j = 0; j < maxMoniteProc; j++) {
            if (i == 0) {
                moniteProcesses[j] = new processesInfo;
                moniteProcesses[j]->processName = "notepad.exe";
            }
            processesByRule[i][j] = 1;
        }
    }
    WindowsTimer timer;
    timer.setCallback(callb);
    timer.start(5000, true);
    OutputDebugString("hello");
    while (true)
    {
        if (KEYDOWN(VK_ESCAPE)) // 按ESC退出,非阻塞模式，每次循环不会停留在这
            return -1;
    }
}