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
const int runModeCount = 6;
struct processInfo {
	
	DWORD   id;//Ψһ��ID
	//��ϵͳʵʱ��ȡ
	DWORD   size;
	DWORD   usage;
	DWORD   processID;          // this process
	string  defaultHeapID;
	DWORD   moduleID;           // associated exe
	DWORD   threads;
	DWORD   parentProcessID;    // this process's parent process
	LONG    priClassBase;         // Base priority of process's threads
	DWORD   flags;
	//�ӹ�����л�ȡ
	int dbID;  //�����ID
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
	//����ϵͳ��Ϣ�����ȡ
	time_t lastRunTime;
	time_t duration;
	time_t timeAfterPrevRun;
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
static struct processes* moniteProcesses;// = new struct processes;
static struct processesByRuleList* processesByRule[runModeCount];
static int maxMoniteProc = 1;
struct processInfo* findMoniteProc(string procName) {
	struct processes* pointer = moniteProcesses;
	while ((*pointer).ProcessInfo->processName != procName && (*pointer).next) {
		pointer = (*pointer).next;
	}
	if (pointer) return (*pointer).ProcessInfo;
	else return nullptr;
}

void callb() {

	ofstream logFile(logFilePath, ios::app | ios::_Noreplace);
	bool logFileOpen = logFile.is_open();

	//������س���Ĭ��Ϊδ����,����ֹͣ
	struct processes* pointer = moniteProcesses;
	while (pointer) {
		(*pointer).ProcessInfo->isRunnig = false;
		(*pointer).ProcessInfo->isTerminate = false;
		pointer = (*pointer).next;
	}
	time_t now = time(0);
	char* dt = ctime(&now);
	PROCESSENTRY32 pe32;
	// ��ʹ������ṹ֮ǰ�����������Ĵ�С
	pe32.dwSize = sizeof(pe32);
	if (logFileOpen) logFile << "��¼ʱ�䣺" << dt << endl;
	// ��ϵͳ�ڵ����н�����һ������
	HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		printf(" CreateToolhelp32Snapshot����ʧ�ܣ� \n");
		return;
	}
	// �������̿���
	BOOL bMore = ::Process32First(hProcessSnap, &pe32);
	while (bMore)
	{
		if (logFileOpen)
			logFile << pe32.cntThreads << "  " << pe32.cntUsage << "  " << pe32.dwFlags << "   " << pe32.dwSize << "  " << pe32.pcPriClassBase << "  " << pe32.szExeFile << "  " << pe32.th32DefaultHeapID
			<< "  " << pe32.th32ModuleID << "  " << pe32.th32ParentProcessID << "  " << pe32.th32ProcessID << endl;
		//�ж��Ƿ񱻼�ؽ���
		struct processInfo* process = findMoniteProc(pe32.szExeFile);
		if (process != NULL) {
			cout << "finded" << endl;
			//���±���ؽ���ʵʱ��Ϣ
			process->defaultHeapID = pe32.th32DefaultHeapID;
			process->flags = pe32.dwFlags;
			if (now - process->lastRunTime > eachRunInterval) process->runTimes += 1;
			process->lastRunTime = now;
			process->moduleID = pe32.th32ModuleID;
			process->parentProcessID = pe32.th32ParentProcessID;
			process->priClassBase = pe32.pcPriClassBase;
			process->processID = pe32.th32ProcessID;
			process->duration += moniteInterval;
			process->size = pe32.dwSize;
			process->threads = pe32.cntThreads;
			process->usage = pe32.cntUsage;
			process->timeAfterPrevRun += moniteInterval;
			process->isRunnig = true;
		}
		wprintf(L"Process Name is : %ls\n", pe32.szExeFile);
		
		printf(" Process ID is��%u \n\n", pe32.th32ProcessID);

		bMore = ::Process32Next(hProcessSnap, &pe32);
	}
	// �ͷ�snapshot����
	::CloseHandle(hProcessSnap);
	
	static struct processesByRuleList* scanPtr;	
	processInfo* pInfo;
	//�ж��Ƿ���ָ����ʱ�������
	scanPtr = processesByRule[0];
	while (scanPtr)
	{
		pInfo = (*scanPtr).ProcessInfo;
		if (pInfo->isRunnig && now<pInfo->startTime || now >pInfo->endTime) pInfo->isTerminate |= true;
	}
	//�ж��Ƿ񳬹�ÿ�����еĴ���
	scanPtr = processesByRule[1];
	while (scanPtr)
	{
		pInfo = (*scanPtr).ProcessInfo;
		if (pInfo->isRunnig && pInfo->runTimes > pInfo->times) pInfo->isTerminate |= true;
	}
	//�ж�ÿ�������м���Ƿ񳬹��涨ʱ��
	scanPtr = processesByRule[2];
	while (scanPtr)
	{
		pInfo = (*scanPtr).ProcessInfo;
		if (pInfo->isRunnig && now-pInfo->lastRunTime > pInfo->Interval) pInfo->isTerminate |= true;
	}
	//�ж�ÿ�����г����Ƿ񳬹��涨ʱ��
	scanPtr = processesByRule[3];
	while (scanPtr)
	{
		pInfo = (*scanPtr).ProcessInfo;
		if (pInfo->isRunnig && now - pInfo->lastRunTime < pInfo->PerPeriodTime) pInfo->isTerminate |= true;
	}
	//�ж��Ƿ��ڽ�ֹ��ʱ�������
	scanPtr = processesByRule[4];
	while (scanPtr)
	{
		pInfo = (*scanPtr).ProcessInfo;
		if (pInfo->isRunnig && now>pInfo->startTime && now <pInfo->endTime  ) pInfo->isTerminate |= true;
	}
	//�ж�ÿ���ܹ�����ʱ���Ƿ񳬹��涨ʱ��
	scanPtr = processesByRule[5];
	while (scanPtr)
	{
		pInfo = (*scanPtr).ProcessInfo;
		if (pInfo->isRunnig && pInfo->duration > pInfo->TotalTime) pInfo->isTerminate |= true;
	}
	//ֹͣ�����涨�Ľ���
	struct processes* pointer = moniteProcesses;
	while (pointer) {
		if ((*pointer).ProcessInfo->isTerminate)
		{
			HANDLE handle = OpenProcess(PROCESS_TERMINATE, FALSE,(*pointer).ProcessInfo->processID);
			if (handle != NULL)
			{
				BOOL bResult = TerminateProcess(handle, 0);
				CloseHandle(handle);
			}
		}
		pointer = (*pointer).next;
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
	processesByRuleList** ptrNodeByRule[runModeCount] ;
	for (int i = 0; i < runModeCount; i++)
		ptrNodeByRule[i] = &processesByRule[i] ;
	processes** ptrNodeProcess = &moniteProcesses;
	for (int i = 0; i < maxMoniteProc; i++) {
		processes* tempProcessNode = new processes;
		(*tempProcessNode).ProcessInfo = new processInfo;
		(*tempProcessNode).next = nullptr;
		if (ptrNodeProcess) (*ptrNodeProcess)= tempProcessNode;
		//������ID;
		(**ptrNodeProcess).ProcessInfo->id = i;
		//��ȡ����ӽ��̹�����е���Ϣ
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


		for (int j = 0; j < runModeCount; j++) {
			//prevNodeByRule = nullptr;
			//curNodeByRule = processesByRule[j]=new struct processesByRuleList;
			processesByRuleList* tempNodeByRule = new struct processesByRuleList;
			(*tempNodeByRule).next = nullptr;
            //��ӽ���ָ�뵽��Ӧ����ģʽ�ļ����б��У�ģʽͨ��runMode������λ���ö��壬���common.h�ļ�
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
	timer.start(5000, true);
	OutputDebugString("hello");
	while (true)
	{
		if (KEYDOWN(VK_ESCAPE)) // ��ESC�˳�,������ģʽ��ÿ��ѭ������ͣ������
			return -1;
	}
}