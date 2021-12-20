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
const int moniteInterval = 5000;
const int eachRunInterval = 20;//相隔多少时间当做两次运行，单位秒
const int runModeCount = 6;
struct processInfo {

	DWORD   id;//唯一主ID
	//从系统实时获取
	DWORD   size;
	DWORD   usage;
	DWORD   processID;          // this process
	string  defaultHeapID;
	DWORD   moduleID;           // associated exe
	DWORD   threads;
	DWORD   parentProcessID;    // this process's parent process
	LONG    priClassBase;         // Base priority of process's threads
	DWORD   flags;
	//从规则库中获取
	int dbID;  //规则库ID
	int runMode;
	int times;
	string ProgramTitle;
	string ProgramDirectory;
	string RunPath;
	time_t PerPeriodTime;
	time_t Interval;
	time_t TotalTime;
	unsigned char LimitRule;
	string  processName;
	time_t startTime;
	time_t endTime;
	//根据系统信息运算获取
	time_t lastRunTime;
	time_t duration;
	time_t curDuration;
	//time_t timeAfterPrevRun;
	int runTimes;
	bool isRunnig;
	bool isTerminate;
};
struct processes {
	struct processInfo* ProcessInfo;
	struct processes* next;
};

struct processesByRuleList {
	struct processInfo* ProcessInfo;
	struct processesByRuleList* next;
};

struct threadInfo {
	int threadID;
	int ownerProcessID;
	threadInfo* next;
};
static struct processes* moniteProcesses;// = new struct processes;
static struct processesByRuleList* processesByRule[runModeCount];
static int maxMoniteProc = 1;
struct processInfo* findMoniteProc(string procName) {
	struct processes* pointer = moniteProcesses;
	while ((*pointer).ProcessInfo->processName != procName && (*pointer).next) {
		pointer = (*pointer).next;
	}
	if ((*pointer).next) return (*pointer).ProcessInfo;
	else return nullptr;
}


BOOL EnableDebugPrivilege()

{
	HANDLE hToken;
	BOOL fOk = FALSE;
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken))
	{
		TOKEN_PRIVILEGES tp;
		tp.PrivilegeCount = 1;
		LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tp.Privileges[0].Luid);
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

		AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);
		int a = GetLastError();
		cout << "return code:" << a << endl;
		fOk = (a == ERROR_SUCCESS);
		CloseHandle(hToken);
	}
	return fOk;
}

void callb() {

	ofstream logFile(logFilePath, ios::app | ios::_Noreplace);
	bool logFileOpen = logFile.is_open();

	//将被监控程序默认为未运行,不需停止
	struct processes* pointer = moniteProcesses;
	while (pointer) {
		(*pointer).ProcessInfo->isRunnig = false;
		(*pointer).ProcessInfo->isTerminate = false;
		pointer = (*pointer).next;
	}
	time_t now = time(0);
	char* dt = ctime(&now);
	PROCESSENTRY32 pe32;
	// 在使用这个结构之前，先设置它的大小
	pe32.dwSize = sizeof(pe32);
	if (logFileOpen) logFile << "记录时间：" << dt << endl;
	// 给系统内的所有进程拍一个快照
	HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		printf(" CreateToolhelp32Snapshot调用失败！ \n");
		return;
	}
	// 遍历进程快照
	BOOL bMore = ::Process32First(hProcessSnap, &pe32);
	while (bMore)
	{
		if (logFileOpen)
			logFile << pe32.cntThreads << "  " << pe32.cntUsage << "  " << pe32.dwFlags << "   " << pe32.dwSize << "  " << pe32.pcPriClassBase << "  " << pe32.szExeFile << "  " << pe32.th32DefaultHeapID
			<< "  " << pe32.th32ModuleID << "  " << pe32.th32ParentProcessID << "  " << pe32.th32ProcessID << endl;
		//判断是否被监控进程
		struct processInfo* process = findMoniteProc(pe32.szExeFile);
		if (process != NULL) {
			cout << "finded" << endl;
			//更新被监控进程实时信息
			process->defaultHeapID = pe32.th32DefaultHeapID;
			process->flags = pe32.dwFlags;
			if (now - process->lastRunTime > eachRunInterval)
			{
				process->runTimes += 1;
				process->curDuration = 0;
			}
			process->lastRunTime = now;
			process->moduleID = pe32.th32ModuleID;
			process->parentProcessID = pe32.th32ParentProcessID;
			process->priClassBase = pe32.pcPriClassBase;
			process->processID = pe32.th32ProcessID;
			process->duration += moniteInterval / 1000;
			process->curDuration += moniteInterval / 1000;
			process->size = pe32.dwSize;
			process->threads = pe32.cntThreads;
			process->usage = pe32.cntUsage;
			//process->timeAfterPrevRun += moniteInterval/1000;
			process->isRunnig = true;
		}
		//wprintf(L"Process Name is : %ls\n", pe32.szExeFile);

		//printf(" Process ID is：%u \n\n", pe32.th32ProcessID);

		bMore = ::Process32Next(hProcessSnap, &pe32);
	}
	// 释放snapshot对象
	::CloseHandle(hProcessSnap);

	static struct processesByRuleList* scanPtr;
	processInfo* pInfo;
	//判断是否在指定的时间段运行
	scanPtr = processesByRule[0];
	while (scanPtr)
	{
		pInfo = (*scanPtr).ProcessInfo;
		if (pInfo->isRunnig && now<pInfo->startTime || now >pInfo->endTime) pInfo->isTerminate |= true;
		scanPtr = (*scanPtr).next;
	}
	//判断是否超过每天运行的次数
	scanPtr = processesByRule[1];
	while (scanPtr)
	{
		pInfo = (*scanPtr).ProcessInfo;
		if (pInfo->isRunnig && pInfo->runTimes > pInfo->times) pInfo->isTerminate |= true;
		scanPtr = (*scanPtr).next;
	}
	//判断每两次运行间隔是否超过规定时间
	scanPtr = processesByRule[2];
	while (scanPtr)
	{
		pInfo = (*scanPtr).ProcessInfo;
		if (pInfo->isRunnig && now - pInfo->lastRunTime > pInfo->Interval) pInfo->isTerminate |= true;
		scanPtr = (*scanPtr).next;
	}
	//判断每次运行持续是否超过规定时间
	scanPtr = processesByRule[3];
	while (scanPtr)
	{
		pInfo = (*scanPtr).ProcessInfo;
		if (pInfo->curDuration > pInfo->PerPeriodTime) pInfo->isTerminate |= true;
		scanPtr = (*scanPtr).next;
	}
	//判断是否在禁止的时间段运行
	scanPtr = processesByRule[4];
	while (scanPtr)
	{
		pInfo = (*scanPtr).ProcessInfo;
		if (pInfo->isRunnig && now > pInfo->startTime && now < pInfo->endTime) pInfo->isTerminate |= true;
		scanPtr = (*scanPtr).next;
	}
	//判断每天总共运行时长是否超过规定时间
	scanPtr = processesByRule[5];
	while (scanPtr)
	{
		pInfo = (*scanPtr).ProcessInfo;
		if (pInfo->isRunnig && pInfo->duration > pInfo->TotalTime) pInfo->isTerminate |= true;
		scanPtr = (*scanPtr).next;
	}

	cout << "checkpoint 3" << endl;
	//停止触及规定的进程

	HANDLE   hThreadSnap = INVALID_HANDLE_VALUE;
	THREADENTRY32   te32;
	hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	te32.dwSize = sizeof(THREADENTRY32);
	if (!Thread32First(hThreadSnap, &te32))
	{
		int a = GetLastError();
		CloseHandle(hThreadSnap);     // Must clean up the snapshot object!
	}
	else {

		struct threadInfo* threadList=new struct threadInfo;
		struct threadInfo** ptrThreadInfo = &threadList;
		do
		{
			*ptrThreadInfo = new struct threadInfo;
			(**ptrThreadInfo).ownerProcessID = te32.th32OwnerProcessID;
			(**ptrThreadInfo).threadID = te32.th32ThreadID;
			(**ptrThreadInfo).next = nullptr;
			ptrThreadInfo = &(**ptrThreadInfo).next;
		} while (::Thread32Next(hThreadSnap, &te32));
		pointer = moniteProcesses;
		cout << "checkpoint 1" << endl;
		while (pointer) {
			if ((*pointer).ProcessInfo->isTerminate)
			{
				cout << "checkpoint4" << endl;
				int processid = (*pointer).ProcessInfo->processID;
				struct threadInfo* pointer2 = threadList;
				while (pointer2)
				{
					cout << "checkpoint 2" << endl;
					if ((*pointer2).ownerProcessID == processid)
					{
						HANDLE handle = OpenThread(THREAD_TERMINATE | THREAD_QUERY_INFORMATION, FALSE, (*pointer2).threadID);
						BOOL bResult = TerminateThread(handle, 0);
						cout << "Terminate thread result:" << bResult <<"   error:"<<GetLastError() << endl;
						CloseHandle(handle);
					}
					pointer2 = (*pointer2).next;
				}
				HANDLE handle = OpenProcess(PROCESS_TERMINATE | PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processid);
				if (handle != NULL)
				{
					//EnableDebugPrivilege();
					BOOL bResult = TerminateProcess(handle, 0);
					cout << "Terminate result:" << bResult << endl;
					CloseHandle(handle);
				}
			}
			pointer = (*pointer).next;
		}
	}
	std::cout << "test ok" << std::endl;
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
	//maxMoniteProc = DBRS.getRuleCount();
	DB::TimeControllerRule*** rules = new DB::TimeControllerRule**;
	DBRS.getAllRule(rules, maxMoniteProc);
	//moniteProcesses = new  processes ** [maxMoniteProc];
	//int modeCount[runModeCount];
	//processes* prevNodeProcess = nullptr;
	//processesByRuleList* prevNodeByRule[runModeCount] = { nullptr };
	processesByRuleList** ptrNodeByRule[runModeCount];
	for (int i = 0; i < runModeCount; i++)
		ptrNodeByRule[i] = &processesByRule[i];
	processes** ptrNodeProcess = &moniteProcesses;
	for (int i = 0; i < maxMoniteProc; i++) {
		processes* tempProcessNode = new processes;
		(*tempProcessNode).ProcessInfo = new processInfo;
		(*tempProcessNode).next = nullptr;
		if (ptrNodeProcess) (*ptrNodeProcess) = tempProcessNode;
		//生成主ID;
		(**ptrNodeProcess).ProcessInfo->id = i;
		//获取需监视进程规则库中的信息
		(**ptrNodeProcess).ProcessInfo->dbID = (*rules)[i]->GetId();
		(**ptrNodeProcess).ProcessInfo->processName = (*rules)[i]->GetProgramName();
		(**ptrNodeProcess).ProcessInfo->endTime = (*rules)[i]->GetEndTime();
		(**ptrNodeProcess).ProcessInfo->times = (*rules)[i]->GetTimes();
		(**ptrNodeProcess).ProcessInfo->PerPeriodTime = (*rules)[i]->GetPerPeriodTime();
		(**ptrNodeProcess).ProcessInfo->Interval = (*rules)[i]->GetInterval();
		(**ptrNodeProcess).ProcessInfo->ProgramDirectory = (*rules)[i]->GetProgramDirectory();
		(**ptrNodeProcess).ProcessInfo->ProgramTitle = (*rules)[i]->GetProgramTitle();
		(**ptrNodeProcess).ProcessInfo->RunPath = (*rules)[i]->GetRunPath();
		(**ptrNodeProcess).ProcessInfo->runMode = (*rules)[i]->GetRunMode();
		(**ptrNodeProcess).ProcessInfo->TotalTime = (*rules)[i]->GetTotalTime();
		(**ptrNodeProcess).ProcessInfo->startTime = (*rules)[i]->GetStartTime();
		//初始化被监视进程信息
		(**ptrNodeProcess).ProcessInfo->runTimes = 0;
		//(**ptrNodeProcess).ProcessInfo->timeAfterPrevRun = 0;
		(**ptrNodeProcess).ProcessInfo->lastRunTime = -1;
		(**ptrNodeProcess).ProcessInfo->duration = 0;
		(**ptrNodeProcess).ProcessInfo->curDuration = 0;

		for (int j = 0; j < runModeCount; j++) {
			//prevNodeByRule = nullptr;
			//curNodeByRule = processesByRule[j]=new struct processesByRuleList;
			processesByRuleList* tempNodeByRule = new struct processesByRuleList;
			(*tempNodeByRule).next = nullptr;
			//添加进程指针到对应运行模式的监视列表中，模式通过runMode二进制位设置定义，详见common.h文件
			if ((**ptrNodeProcess).ProcessInfo->runMode & (0x1 << j))
			{
				(*tempNodeByRule).ProcessInfo = (**ptrNodeProcess).ProcessInfo;
				//*(curNodeByRule[j]) = temp;
				if (ptrNodeByRule[j])
				{
					(*ptrNodeByRule[j]) = tempNodeByRule;
					ptrNodeByRule[j] = &(**ptrNodeByRule[j]).next;
					//(*prevNodeByRule[j]).next = curNodeByRule[j];
					//prevNodeByRule[j] = curNodeByRule[j];
					//curNodeByRule[j] = new struct processesByRuleList;
				}
			}
		}
		//prevNodeProcess = curNodeProcess;
		ptrNodeProcess = &(**ptrNodeProcess).next;
		//curNodeProcess = new struct processes;
		delete (*rules)[i];

	}
	//for (int i = 0; i < runModeCount; i++)
	//	if (curNodeByRule[i]) delete curNodeByRule[i];
	//if (curNodeProcess) delete curNodeProcess;
	delete* rules;
	delete rules;
	WindowsTimer timer;
	timer.setCallback(callb);
	timer.start(moniteInterval, true);
	OutputDebugString("hello");
	while (true)
	{
		if (KEYDOWN(VK_ESCAPE)) // 按ESC退出,非阻塞模式，每次循环不会停留在这
			return -1;
	}
}