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
#include <zip.h>
using namespace std;
#define KEYDOWN( vk ) ( 0x8000 & ::GetAsyncKeyState( vk ) ) 

const char* logFilePath = "test.log";
const int moniteInterval = 5000;
const int intervalAsNextRun = 20;//�������ʱ�䵱���������У���λ��
const int runModeCount = 6;
//ͬ�����̼��ṹ
struct processesID {
	DWORD processID; //����id
	string processName; //��������
	processesID* next;
};
struct processInfo {

	DWORD   id;//Ψһ��ID
	//��ϵͳʵʱ��ȡ
	DWORD   size;
	DWORD   usage;
	//DWORD   processID;          // this process
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
	//struct processesID* processes;
	byte* processesID = new byte[32]{ 0 };
	byte* ptrLastProcID = &(processesID[0]);
	byte countOfProcessID = 0;
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
	while (pointer) {
		if ((*pointer).ProcessInfo->processName == procName) return (*pointer).ProcessInfo;
		else
			pointer = (*pointer).next;
	}
	return nullptr;
}


// WCHAR ת��Ϊ std::string
string WCHAR2String(LPCWSTR pwszSrc)
{
	int nLen = WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, NULL, 0, NULL, NULL);
	if (nLen <= 0)
		return std::string("");

	char* pszDst = new char[nLen];
	if (NULL == pszDst)
		return string("");

	WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, pszDst, nLen, NULL, NULL);
	pszDst[nLen - 1] = 0;

	std::string strTmp(pszDst);
	delete[] pszDst;

	return strTmp;
}


DWORD getProcessID(byte* const processIDGroup, byte& num,byte count)
{
	if (count>num) {
		byte* p = new byte[8];
		DWORD offset = num<<3;
		//DWORD result;
		//p =(byte*) &result;
		*p = processIDGroup[offset];
		offset++;
		p[1] = processIDGroup[offset];
		offset++;
		p[2] = processIDGroup[offset];
		offset++;
		p[3] = processIDGroup[offset];
		offset++;
		p[4] = processIDGroup[offset];
		offset++;
		p[5] = processIDGroup[offset];
		offset++;
		p[6] = processIDGroup[offset];
		offset++;
		p[7] = processIDGroup[offset];
		offset++;
		//byte* rest = new byte[(--restLength) * 8];
		//strncpy((char*)rest, (char*)(*processIDGroup + 8), restLength * 8);
		//delete[](*processIDGroup);
		//*processIDGroup = rest;
		DWORD result = *((DWORD *) p);
		delete[] p;
		num++;
		return result;
	}
	else {
//		*processIDGroup = nullptr;
		return 0;
	}
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

	ofstream logFile(logFilePath, ios::app);
	bool logFileOpen = logFile.is_open();

	//������س���Ĭ��Ϊδ���С�����ֹͣ�����processid��
	struct processes* pointer = moniteProcesses;
	while (pointer) {
		(*pointer).ProcessInfo->isRunnig = false;
		(*pointer).ProcessInfo->isTerminate = false;
		delete[](*pointer).ProcessInfo->processesID;
		(*pointer).ProcessInfo->processesID = new byte[32];
		(*pointer).ProcessInfo->ptrLastProcID = (*pointer).ProcessInfo->processesID;
		(*pointer).ProcessInfo->countOfProcessID = 0;
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
			if (now - process->lastRunTime > intervalAsNextRun)
			{
				process->runTimes += 1;
				process->curDuration = 0;
			}
			process->lastRunTime = now;
			process->moduleID = pe32.th32ModuleID;
			process->parentProcessID = pe32.th32ParentProcessID;
			process->priClassBase = pe32.pcPriClassBase;

			/*process->processes->processID = pe32.th32ProcessID;
			process->processes->processName = pe32.szExeFile;
			process->processes->next = new struct processesID;
			process->processes->next->processName = "---null";
			process->processes->next->next = nullptr;*/

			byte* p = (byte*)&(pe32.th32ProcessID);
			byte c = process->countOfProcessID;
			//char* p2 = strchr(process->processesID, '\0');
			if ((c>0) && process->ptrLastProcID >= ((byte*)(process->processesID) + ((((c-1) >> 2)+1)<<5)))
			{
				DWORD oldLength = c * 8;
				//process->countOfProcessID += 32;
				byte* newProcessesID = new byte[oldLength + 32];
				if (memcpy(newProcessesID, process->processesID, oldLength))
				{
					DWORD oldOffset = process->ptrLastProcID - process->processesID;
					delete[] process->processesID;
					process->processesID = newProcessesID;
					process->ptrLastProcID = process->processesID + oldOffset;

				}
				else;
			}
			*(process->ptrLastProcID) = p[0];
			process->ptrLastProcID++;
			*(process->ptrLastProcID) = p[1];
			process->ptrLastProcID++;
			*(process->ptrLastProcID) = p[2];
			process->ptrLastProcID++;
			*(process->ptrLastProcID) = p[3];
			process->ptrLastProcID++;
			*(process->ptrLastProcID) = p[4];
			process->ptrLastProcID++;
			*(process->ptrLastProcID) = p[5];
			process->ptrLastProcID++;
			*(process->ptrLastProcID) = p[6];
			process->ptrLastProcID++;
			*(process->ptrLastProcID) = p[7];
			//cout << "ProcessID:" <<(byte) p[0]<< (byte)p[1] << (byte)p[2] << (byte)p[3] << (byte)p[4] << (byte)p[5] << (byte)p[6] << (byte) p[7]<<endl;  //����

			printf("ProcessID:%u%u%u%u%u%u%u%u\n", (byte)p[0], (byte)p[1], (byte)p[2], (byte)p[3], (byte)p[4], (byte)p[5], (byte)p[6], (byte)p[7]);  //����
			process->ptrLastProcID++;
			process->countOfProcessID++;
			process->duration += moniteInterval / 1000;
			process->curDuration += moniteInterval / 1000;
			process->size = pe32.dwSize;
			process->threads = pe32.cntThreads;
			process->usage = pe32.cntUsage;
			//process->timeAfterPrevRun += moniteInterval/1000;
			process->isRunnig = true;
		}
		//wprintf(L"Process Name is : %ls\n", pe32.szExeFile);

		//printf(" Process ID is��%u \n\n", pe32.th32ProcessID);

		bMore = ::Process32Next(hProcessSnap, &pe32);
	}
	// �ͷ�snapshot����
	::CloseHandle(hProcessSnap);


	//����
	processes* pointert = moniteProcesses;
	for (int i = 0; i < maxMoniteProc; i++) {
		if (pointert->ProcessInfo->processName == "msedge.exe") {
			cout << "ProcessID2:  ";
			byte* pp1 = pointert->ProcessInfo->processesID;
			for (int j = 0; j < (pointert->ProcessInfo->countOfProcessID); j++) {
				printf("ProcessID2   :%u%u%u%u%u%u%u%u\n", (byte)pp1[0], (byte)pp1[1], (byte)pp1[2], (byte)pp1[3], (byte)pp1[4], (byte)pp1[5], (byte)pp1[6], (byte)pp1[7]);  //����
				pp1 += 8;
			}
			cout << endl;
		}
		pointert = pointert->next;
	}

	//���Խ���

	static struct processesByRuleList* scanPtr;
	processInfo* pInfo;
	//�ж��Ƿ���ָ����ʱ�������
	scanPtr = processesByRule[0];
	while (scanPtr)
	{
		pInfo = (*scanPtr).ProcessInfo;
		if (pInfo->isRunnig && now<pInfo->startTime || now >pInfo->endTime) pInfo->isTerminate |= true;
		scanPtr = (*scanPtr).next;
	}
	//�ж��Ƿ񳬹�ÿ�����еĴ���
	scanPtr = processesByRule[1];
	while (scanPtr)
	{
		pInfo = (*scanPtr).ProcessInfo;
		if (pInfo->isRunnig && pInfo->runTimes > pInfo->times) pInfo->isTerminate |= true;
		scanPtr = (*scanPtr).next;
	}
	//�ж�ÿ�������м���Ƿ񳬹��涨ʱ��
	scanPtr = processesByRule[2];
	while (scanPtr)
	{
		pInfo = (*scanPtr).ProcessInfo;
		if (pInfo->isRunnig && now - pInfo->lastRunTime > pInfo->Interval) pInfo->isTerminate |= true;
		scanPtr = (*scanPtr).next;
	}
	//�ж�ÿ�����г����Ƿ񳬹��涨ʱ��
	scanPtr = processesByRule[3];
	while (scanPtr)
	{
		pInfo = (*scanPtr).ProcessInfo;
		if (pInfo->curDuration > pInfo->PerPeriodTime) pInfo->isTerminate |= true;
		scanPtr = (*scanPtr).next;
	}
	//�ж��Ƿ��ڽ�ֹ��ʱ�������
	scanPtr = processesByRule[4];
	while (scanPtr)
	{
		pInfo = (*scanPtr).ProcessInfo;
		if (pInfo->isRunnig && now > pInfo->startTime && now < pInfo->endTime) pInfo->isTerminate |= true;
		scanPtr = (*scanPtr).next;
	}
	//�ж�ÿ���ܹ�����ʱ���Ƿ񳬹��涨ʱ��
	scanPtr = processesByRule[5];
	while (scanPtr)
	{
		pInfo = (*scanPtr).ProcessInfo;
		if (pInfo->isRunnig && pInfo->duration > pInfo->TotalTime) pInfo->isTerminate |= true;
		scanPtr = (*scanPtr).next;
	}

	cout << "checkpoint 3" << endl;
	//ֹͣ�����涨�Ľ���

	//HANDLE   hThreadSnap = INVALID_HANDLE_VALUE;
	//THREADENTRY32   te32;
	//hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	//te32.dwSize = sizeof(THREADENTRY32);
	//if (!Thread32First(hThreadSnap, &te32))
	//{
	//	int a = GetLastError();
	//	CloseHandle(hThreadSnap);     // Must clean up the snapshot object!
	//}
	//else {

	//	struct threadInfo* threadList = new struct threadInfo;
	//	struct threadInfo** ptrThreadInfo = &threadList;
	//	do
	//	{
	//		*ptrThreadInfo = new struct threadInfo;
	//		(**ptrThreadInfo).ownerProcessID = te32.th32OwnerProcessID;
	//		(**ptrThreadInfo).threadID = te32.th32ThreadID;
	//		(**ptrThreadInfo).next = nullptr;
	//		ptrThreadInfo = &(**ptrThreadInfo).next;
	//	} while (::Thread32Next(hThreadSnap, &te32));
	pointer = moniteProcesses;
	cout << "checkpoint 1" << endl;
	while (pointer) {
		//if ((*pointer).ProcessInfo->isTerminate)

		//����
		if ((*pointer).ProcessInfo->processName == "msedge.exe")
			//���Խ���

		{
			//cout << "checkpoint4" << endl;
			//int processid = (*pointer).ProcessInfo->processID;
			//struct threadInfo* pointer2 = threadList;
			//while (pointer2)
			//{
			//	cout << "checkpoint 2" << endl;
			//	if ((*pointer2).ownerProcessID == processid)
			//	{
			//		HANDLE handle = OpenThread(THREAD_TERMINATE | THREAD_QUERY_INFORMATION, FALSE, (*pointer2).threadID);
			//		BOOL bResult = TerminateThread(handle, 0);
			//		cout << "Terminate thread result:" << bResult << "   error:" << GetLastError() << endl;
			//		CloseHandle(handle);
			//	}
			//	pointer2 = (*pointer2).next;
			//}
			//struct processesID* processIDPointer = (*pointer).ProcessInfo->processes;
			//while (processIDPointer && (*processIDPointer).processName != "---null")
			DWORD id;
			byte order=0;
			byte count = (*pointer).ProcessInfo->countOfProcessID;
			
			while (id = getProcessID(pointer->ProcessInfo->processesID, order,count))
			{
				cout << "checkpoint 2" << endl;

				HANDLE handle = OpenProcess(PROCESS_TERMINATE | PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, id);
				if (handle != NULL)
				{
					//EnableDebugPrivilege();
					BOOL bResult = TerminateProcess(handle, 0);
					cout << "Terminate result:" << bResult << endl;
					CloseHandle(handle);
				}
			}

		}
		pointer = (*pointer).next;
	}
	//	}
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
		//��ʼ�������ӽ�����Ϣ
		(**ptrNodeProcess).ProcessInfo->runTimes = 0;
		//(**ptrNodeProcess).ProcessInfo->processes = new struct processesID;
		//(**ptrNodeProcess).ProcessInfo->processes->processName = "---null";
		//(**ptrNodeProcess).ProcessInfo->processes->next = nullptr;
		//(**ptrNodeProcess).ProcessInfo->timeAfterPrevRun = 0;
		(**ptrNodeProcess).ProcessInfo->lastRunTime = -1;
		(**ptrNodeProcess).ProcessInfo->duration = 0;
		(**ptrNodeProcess).ProcessInfo->curDuration = 0;

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
	//timer.start(moniteInterval, true);
	callb();//��һ���Ե��ò���
	OutputDebugString( "hello");
	while (true)
	{
		if (KEYDOWN(VK_ESCAPE)) // ��ESC�˳�,������ģʽ��ÿ��ѭ������ͣ������
			return -1;
	}
}