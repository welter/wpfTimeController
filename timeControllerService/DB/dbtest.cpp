#include <windows.h>
#include <WinUser.h>
#include <TlHelp32.h>
#include "common.h"
#include <conio.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <Psapi.h>
#pragma comment(lib,"Psapi.lib")
#include <WinBase.h>
#include "RuleModel.h"
#include "../Timer/Timer.h"
#include "DBRuleService.h"
#include <zip.h>
#include <tchar.h>
using namespace std;
#define KEYDOWN( vk ) ( 0x8000 & ::GetAsyncKeyState( vk ) ) 
const int WM_TIMECONTROLLER = RegisterWindowMessage(_T("TIMECONTROLLER"));

const string logFileName = "test";
const string procInfoDatFileName = "procinfo";
const char* logFilePath = "/";
const int moniteInterval = 5000;
const int intervalAsNextRun = 20;//�������ʱ�䵱���������У���λ��
const int runModeCount = 6;
const int maxLogDataLen = 6000;
const int logDateLong = 1;

//����״̬
struct struServiceState {
	bool bRunning;  //�Ƿ�����
	struct struLoggedUser {
		string sUsername;
		DWORD dwUserID;
		DWORD dwUserUUID;
		struLoggedUser* next;
	};
};
static struServiceState* serviceState=new struServiceState;

//ͬ�����̼��ṹ
struct processesID {
	DWORD processID; //����id
	string processName; //��������
	processesID* next;
};
struct struLogData {
	DWORD cntThreads;
	DWORD cntUsage;
	DWORD dwFlags;
	DWORD dwSize;
	DWORD pcPriClassBase;
	string szExeFile;
	ULONG_PTR th32DefaultHeapID;
	DWORD th32ModuleID;
	DWORD th32ParentProcessID;
	DWORD th32ProcessID;
};

//������Ϣ�ṹ
struct processInfo {

	DWORD   id;//Ψһ��ID


	//��ϵͳʵʱ��ȡ
	DWORD   size;
	DWORD   usage;
	//DWORD   processID;          // this process
	ULONG_PTR  defaultHeapID;
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
	DWORD* processesID = new DWORD{ 0 };
	DWORD* ptrLastProcID = &(processesID[0]);
	byte countOfProcessID = 0;
	time_t lastRunTime;
	time_t duration;
	time_t curDuration;
	//time_t timeAfterPrevRun;
	int runTimes;
	bool isRunnig;
	byte resetMode;
};
struct processes {
	struct processInfo* ProcessInfo;
	struct processes* next;
};

//������ģʽ���������ӽ��̼���
struct processesByRuleList { 
	int runMode;  //����ģʽ
	struct processInfo* ProcessInfo;
	struct processesByRuleList* next;
};

struct threadInfo {
	int threadID;
	int ownerProcessID;
	threadInfo* next;
};
static struct processes* moniteProcesses;// = new struct processes;  //����ӵĽ���
static struct processesByRuleList* processesByRule[runModeCount];
static int maxMoniteProc = 1;
static struct struLogData* logData = new struLogData[maxLogDataLen];
static int logDataLen = 0;
static WindowsTimer timerMoniteTimer, timerlogTimer;

//�Խ������ڼ�ض����в��ҽ���
//������proceName��������
//���أ��ҵ��Ľ����ڼ�ض�����ָ�룬��û�ҵ����ؿ�ָ�롣
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
//������pwszSrc,WCHAR������ֵ
//���أ���Ӧ��std::string������ֵ
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


DWORD getProcessID(DWORD* const processIDGroup, byte& num, byte count)
{
	if (count > num) {
		byte* p = new byte[8];
		DWORD offset = num << 3;
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
		DWORD result = *((DWORD*)p);
		delete[] p;
		num++;
		return result;
	}
	else {
		//		*processIDGroup = nullptr;
		return 0;
	}
}

//���÷�ʽ
const byte rsNone = 0;  //������
const byte rsAll =1;  //����ȫ��
const byte rsRuntimes = 2;  //�������д���
const byte rsDuration = 4;  //���������г���ʱ��
const byte rsCurDuration = 8;  //���ñ��γ���ʱ��
const byte rsTerminate = 16;  //�˳�

void resetProc(processInfo* proc,byte mode)
{
	if (proc) {
		if (mode == rsAll)
		{
			proc->resetMode = rsNone;
			proc->runTimes = 0;
			//(**ptrNodeProcess).ProcessInfo->processes = new struct processesID;
			//(**ptrNodeProcess).ProcessInfo->processes->processName = "---null";
			//(**ptrNodeProcess).ProcessInfo->processes->next = nullptr;
			//(**ptrNodeProcess).ProcessInfo->timeAfterPrevRun = 0;
			proc->lastRunTime = -1;
			proc->duration = 0;
			proc->curDuration = 0;
		}
		if(mode==rsRuntimes)
			proc->runTimes = 0;
		if (mode == rsDuration)
			proc->duration = 0;
		if (mode == rsCurDuration)
			proc->curDuration = 0;
		if (mode == rsTerminate)
			proc->resetMode = rsNone;
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

//�ռǼ�¼�߳�
void logThread() {
	time_t now = time(0);
	char* dt = ctime(&now);
	string s = logFileName + ".log";
	ofstream logFile(s, ios::app);
	bool logFileOpen = logFile.is_open();
	s = procInfoDatFileName + ".dat";
	ofstream datFile(s, ios::trunc|ios::binary);
	bool datFileOpen = datFile.is_open();
	int a = GetLastError();
	HANDLE hMutex = CreateMutex(nullptr, FALSE, "canLog");
	BOOL canLog = (GetLastError() != ERROR_ALREADY_EXISTS); //
	if (logFileOpen && canLog)
	{
		logFile << endl << "**************************��¼ʱ�䣺" << dt << endl;
		for (int i = 0; i < logDataLen; i++)
		{

			if (logFileOpen)
				logFile << logData[i].cntThreads << "  " << logData[i].cntUsage << "  " << logData[i].dwFlags << "   " << logData[i].dwSize << "  " << logData[i].pcPriClassBase
				<< "  " << logData[i].szExeFile << "  " << logData[i].th32DefaultHeapID
				<< "  " << logData[i].th32ModuleID << "  " << logData[i].th32ParentProcessID << "  " << logData[i].th32ProcessID << endl;
		}
		logFile << "**************-----------------------**************" << endl;
	}
	if (logFileOpen) logFile.close();
	

	if (datFileOpen && canLog)
	{
		datFile << maxMoniteProc << endl;
		processes* pointer=moniteProcesses;
		if (pointer && pointer->ProcessInfo) {
			datFile << endl;
			datFile.write(pointer->ProcessInfo->processName.c_str(), 255);
				//datFile<<setw(255) << pointer->ProcessInfo->processName << endl;
			datFile.write((char*) &(pointer->ProcessInfo->startTime), 8);
			//datFile <<setw(8)<< pointer->ProcessInfo->startTime<<endl;
			datFile.write((char*)&(pointer->ProcessInfo->lastRunTime), 8);
			//datFile << setw(8) << pointer->ProcessInfo->lastRunTime << endl;
			datFile.write((char*)&(pointer->ProcessInfo->duration), 8);
			//datFile << setw(8) << pointer->ProcessInfo->duration << endl;
			datFile.write((char*)&(pointer->ProcessInfo->curDuration), 8);
			//datFile << setw(8) << pointer->ProcessInfo->curDuration << endl;
			datFile.write((char*)&(pointer->ProcessInfo->runTimes), 8);
			//datFile << setw(8) << pointer->ProcessInfo->runTimes << endl;
			datFile.write((char*)&(pointer->ProcessInfo->isRunnig), 1);
			//datFile << setw(1) << pointer->ProcessInfo->isRunnig << endl;
			datFile.write((char*)&(pointer->ProcessInfo->resetMode), 1);
			//datFile << setw(1) << pointer->ProcessInfo->resetMode << endl;
			datFile.write("**!!**!!**!!\n",13);
			pointer = pointer->next;
		}
	}
	if (datFileOpen) datFile.close();
	CloseHandle(hMutex);
	hMutex = NULL;
	return;
}

DWORD static WINAPI mainThread(_In_ LPVOID lpParameter) {
	MSG msg;
	PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);//ʹ�̲߳�����һ����Ϣ����
	//if (!SetEvent(hStartEvent))//�������̵߳ĵȴ��¼��������̸߳����̷߳�����Ϣ
	//{
	//	printf("set event error,%d\n", GetLastError());
	//	return 1;
	//}
	while (true)
	{
		if (GetMessage(&msg, 0, 0, 0)) //û����Ϣ�����������ȴ���ֱ���յ���Ϣ
		{
			if (msg.message==WM_TIMECONTROLLER)
			{
				switch (msg.wParam)
				{
				case MP_TIMERCONTROLER_STOP:  //ֹͣTimerController
					serviceState->bRunning = false;
					break;
				case MP_TIMERCONTROLER_RESUME:  //����TimerController
					serviceState->bRunning = true;
					break;
				case MP_TIMERCONTROLER_RESET:  //����TimerController
					initService();
					break;
				case MP_TIMERCONTROLER_TERMINATEPROC:  //��������
					//msg.lParam
					break;
				case MP_TIMERCONTROLER_QUERYPROCESSINFO: //��ѯ������Ϣ
					break;
				case MP_TIMERCONTROLER_GetPROCESSES://��ȡ��ǰ���н�����Ϣ
					break;
				case MP_TIMERCONTROLER_LOGON:  //��¼TimerController
					break;
				case MP_TIMERCONTROLER_LOGOFF:  //�ǳ�
					break;
				case MP_TIMERCONTROLER_LOADSETTING:  //��������
					break;
				}
				printf("okk");
				//break;
			}
		}
	}
	return 1;
}

//ö��ָ��PID����ӵ�е��߳�
//������dwOwnerPID����PID
//����: byte* ptrThID,ӵ�е��߳�ID����
vector <DWORD > ListProcessThreads(DWORD dwOwnerPID)
{
	vector <DWORD> result;
	
	HANDLE hThreadSnap = INVALID_HANDLE_VALUE;
	THREADENTRY32 te32;

	// Take a snapshot of all running threads  
	hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (hThreadSnap == INVALID_HANDLE_VALUE)
//		return(FALSE);

	// Fill in the size of the structure before using it. 
	te32.dwSize = sizeof(THREADENTRY32);

	// Retrieve information about the first thread,
	// and exit if unsuccessful
	if (!Thread32First(hThreadSnap, &te32))
	{
		//printError(TEXT("Thread32First")); // show cause of failure
		CloseHandle(hThreadSnap);          // clean the snapshot object
//		return(FALSE);
	}

	// Now walk the thread list of the system,
	// and display information about each thread
	// associated with the specified process
	do
	{
		if (te32.th32OwnerProcessID == dwOwnerPID)
		{
			//_tprintf(TEXT("\n\n     THREAD ID      = 0x%08X"), te32.th32ThreadID);
			//_tprintf(TEXT("\n     Base priority  = %d"), te32.tpBasePri);
			//_tprintf(TEXT("\n     Delta priority = %d"), te32.tpDeltaPri);
			//_tprintf(TEXT("\n"));
			result.push_back(te32.th32ThreadID);
		}
	} while (Thread32Next(hThreadSnap, &te32));

	CloseHandle(hThreadSnap);
	return(result);
}

void moniteThread() {
	if (serviceState->bRunning)
	{
		//������س�����ϢĬ��Ϊδ���С�����ֹͣ�����processid��
		struct processes* pointer = moniteProcesses;
		while (pointer) {
			(*pointer).ProcessInfo->isRunnig = false;
			(*pointer).ProcessInfo->resetMode = rsNone;
			delete[](*pointer).ProcessInfo->processesID;
			(*pointer).ProcessInfo->processesID = new DWORD;
			(*pointer).ProcessInfo->ptrLastProcID = (*pointer).ProcessInfo->processesID;
			(*pointer).ProcessInfo->countOfProcessID = 0;
			pointer = (*pointer).next;
		}
		time_t now = time(0);
		char* dt = ctime(&now);
		PROCESSENTRY32 pe32;
		// ��ʹ������ṹ֮ǰ�����������Ĵ�С
		pe32.dwSize = sizeof(pe32);

		// ��ϵͳ�ڵ����н�����һ������
		HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (hProcessSnap == INVALID_HANDLE_VALUE)
		{
			printf(" CreateToolhelp32Snapshot����ʧ�ܣ� \n");
			return;
		}
		//�ܷ��¼log
		HANDLE hMutex = CreateMutex(nullptr, FALSE, "canLog");
		BOOL canLog = (GetLastError() != ERROR_ALREADY_EXISTS); //
		if (canLog)
			logDataLen = 0;
		else
		{
			CloseHandle(hMutex);
			hMutex = NULL;
		}
		// �������̿���
		BOOL bMore = ::Process32First(hProcessSnap, &pe32);
		while (bMore)
		{
			if (canLog)
			{
				logData[logDataLen].cntThreads = pe32.cntThreads;
				logData[logDataLen].cntUsage = pe32.cntUsage;
				logData[logDataLen].dwFlags = pe32.dwFlags;
				logData[logDataLen].dwSize = pe32.dwSize;
				logData[logDataLen].pcPriClassBase = pe32.pcPriClassBase;
				logData[logDataLen].szExeFile = pe32.szExeFile;
				logData[logDataLen].th32DefaultHeapID = pe32.th32DefaultHeapID;
				logData[logDataLen].th32ModuleID = pe32.th32ModuleID;
				logData[logDataLen].th32ParentProcessID = pe32.th32ParentProcessID;
				logData[logDataLen].th32ProcessID = pe32.th32ProcessID;
				if (++logDataLen == maxLogDataLen) logDataLen = 0;
			}
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


				//��¼�ý���ӵ�е��߳�

				vector <DWORD> p2;
				p2 = ListProcessThreads(pe32.th32ProcessID);

				byte* p = (byte*)&(pe32.th32ProcessID);
				byte c = process->countOfProcessID;
				byte c2 = p2.size();
				//char* p2 = strchr(process->processesID, '\0');



				//�жϸý���ӵ�е��߳����Ƿ񳬹�ԭ�����ռ䣨��8���߳�Ϊ��λ����8�����������߳���Ϣ��¼�ռ䣩
				if ((c-c2>7) || c2>(round((c2 + 7) / 8) * 8))
				{
					DWORD oldLength = c * 8;
					//process->countOfProcessID += 32;
					DWORD* newProcessesID = (DWORD*)malloc(round((c2 + 7) / 8) * 32);
					free(process->processesID);
					process->processesID =  newProcessesID;

				}

				vector<DWORD>::iterator it;
				for (it = p2.begin(); it != p2.end(); it++) {
					*(process->ptrLastProcID) = (*it);
					process->ptrLastProcID++;
				}
				process->countOfProcessID = c2;
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
		if (hMutex) {
			CloseHandle(hMutex);
			hMutex = NULL;
		}
		// �ͷ�snapshot����
		::CloseHandle(hProcessSnap);


		//����
		processes* pointert = moniteProcesses;
		for (int i = 0; i < maxMoniteProc; i++) {
			if (pointert->ProcessInfo->processName == "msedge.exe") {
				cout << "ProcessID2:  ";
				DWORD* pp1 = pointert->ProcessInfo->processesID;
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
			if (pInfo->isRunnig && now<pInfo->startTime || now >pInfo->endTime) pInfo->resetMode |= rsTerminate;
			scanPtr = (*scanPtr).next;
		}
		//�ж��Ƿ񳬹�ÿ�����еĴ���
		scanPtr = processesByRule[1];
		while (scanPtr)
		{
			pInfo = (*scanPtr).ProcessInfo;
			if (pInfo->isRunnig && pInfo->runTimes > pInfo->times) pInfo->resetMode |= rsTerminate;
			scanPtr = (*scanPtr).next;
		}
		//�ж�ÿ�������м���Ƿ񳬹��涨ʱ��
		scanPtr = processesByRule[2];
		while (scanPtr)
		{
			pInfo = (*scanPtr).ProcessInfo;
			if (pInfo->isRunnig && now - pInfo->lastRunTime > pInfo->Interval) pInfo->resetMode |= rsTerminate;
			scanPtr = (*scanPtr).next;
		}
		//�ж�ÿ�����г����Ƿ񳬹��涨ʱ��
		scanPtr = processesByRule[3];
		while (scanPtr)
		{
			pInfo = (*scanPtr).ProcessInfo;
			if (pInfo->curDuration > pInfo->PerPeriodTime) pInfo->resetMode |= rsCurDuration | rsTerminate;
			scanPtr = (*scanPtr).next;
		}
		//�ж��Ƿ��ڽ�ֹ��ʱ�������
		scanPtr = processesByRule[4];
		while (scanPtr)
		{
			pInfo = (*scanPtr).ProcessInfo;
			if (pInfo->isRunnig && now > pInfo->startTime && now < pInfo->endTime) pInfo->resetMode |= rsTerminate;
			scanPtr = (*scanPtr).next;
		}
		//�ж�ÿ���ܹ�����ʱ���Ƿ񳬹��涨ʱ��
		scanPtr = processesByRule[5];
		while (scanPtr)
		{
			pInfo = (*scanPtr).ProcessInfo;
			if (pInfo->isRunnig && pInfo->duration > pInfo->TotalTime) pInfo->resetMode |= rsTerminate;
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
			//if ((*pointer).ProcessInfo->resetMode)

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
				byte order = 0;
				byte count = (*pointer).ProcessInfo->countOfProcessID;

				while (id = getProcessID(pointer->ProcessInfo->processesID, order, count))
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
				resetProc(pointer->ProcessInfo, pointer->ProcessInfo->resetMode);
				//pointer->ProcessInfo->resetMode = false;
			}
			pointer = (*pointer).next;
		}
		//	}
		std::cout << "test ok" << std::endl;

		return;
	}
};

void initService()
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
				(*tempNodeByRule).runMode = j;
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
	//if (curNodeProcess) delete curNodeProcess; ��
	delete* rules;
	delete rules;
	time_t now = time(0);
	tm* tmThatTime = _localtime64(&now);
	char cThatTime[30];
	memset(cThatTime, 0, 30);
	sprintf(cThatTime, "%02d%02d%02d_%02dh%02dm%02ds", tmThatTime->tm_year - 100, tmThatTime->tm_mon + 1,
		tmThatTime->tm_mday, tmThatTime->tm_hour, tmThatTime->tm_min, tmThatTime->tm_sec);
	int error;
	string archiveFileName = (string)logFileName + ".zip";
	zip_t* archive = zip_open(archiveFileName.c_str(), ZIP_CREATE, &error);
	int l = zip_get_num_files(archive);
	for (int i = 0; i < l; i++)
	{
		string s = zip_get_name(archive, i, ZIP_FL_ENC_GUESS);
		if (stoi(s.substr(4, 6), nullptr, 10) - stoi(((string)cThatTime).substr(0, 6), nullptr, 10) > logDateLong) {
			zip_delete(archive, i);
		}

	}
	WIN32_FIND_DATA* fd = new WIN32_FIND_DATA;
	HANDLE fh;
	string oldName = (string)logFileName + ".log";
	if ((fh = FindFirstFile(oldName.c_str(), fd)) != INVALID_HANDLE_VALUE)
	{

		string newName = (string)logFileName + (string)cThatTime + ".log";
		rename(oldName.c_str(), newName.c_str());
		zip_source_t* s;
		zip_error_t* zerror = new zip_error_t;
		if ((s = zip_source_file_create(newName.c_str(), 0, -1, zerror)) == NULL ||
			zip_file_add(archive, newName.c_str(), s, ZIP_FL_ENC_UTF_8) < 0) {
			zip_source_free(s);
			printf("error adding file: %s\n", zip_strerror(archive));
		}
		if (zip_close(archive) == 0) DeleteFile(newName.c_str());
		//CloseHandle(fh);
	}
	else	zip_close(archive);

	string s = procInfoDatFileName + ".dat";
	fstream datFile(s, ios::in);
	int a = GetLastError();
	processInfo* p;
	if (datFile.is_open()) {
		int i;
		char s[256] = { 0 };
		char s2[8] = { 0 };
		char s3[1] = { 0 };
		datFile.getline(s, 255, '\n');
		i = stoi(s);
		long long d = 0;
		byte b = 0;
		long long* dp = &d;
		byte* bp = &b;
		for (int j = 0; j < i; j++)
		{
			datFile.getline(s, 255, '\n');
			datFile.read(s, 255);
			s[255] = '\0';
			p = findMoniteProc((string)s);
			if (p) {
				p->processName = s;
				datFile.read((char*)&d, 8);
				p->startTime = d;
				datFile.read((char*)&d, 8);
				p->lastRunTime = d;
				datFile.read((char*)&d, 8);
				p->duration = d;
				datFile.read((char*)&d, 8);
				p->curDuration = d;
				datFile.read((char*)&d, 8);
				p->runTimes = d;
				datFile.read((char*)&b, 1);
				p->isRunnig = b;
				datFile.read((char*)&b, 1);
				p->resetMode = b;
				while (strlen(s) != 0 && strcmp(s, "**!!**!!**!!"))
				{
					datFile.getline(s, 255);
					s[255] = '\0';
				}
			}
		}
		datFile.close();
	}
	timerMoniteTimer.setCallback(moniteThread);
	timerlogTimer.setCallback(logThread);
	timerMoniteTimer.start(moniteInterval, true);
	timerlogTimer.start(10000, true);
}


int main(void)
{
	initService();

	//moniteThread();//��һ���Ե��ò���
	//logThread();//��һ���Ե��ò���
	OutputDebugString("hello");

	//PMYDATA pDataArray;
	DWORD   threadId;
	HANDLE  threadHandle;

		//pDataArray = (PMYDATA)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
		//	sizeof(MYDATA));

		//if (pDataArray== NULL)
		//{
		//	ExitProcess(2);
		//}

		// Generate unique data for each thread to work with.

		//pDataArray->val1 = 1;
		//pDataArray->val2 = 101;

		// Create the thread to begin execution on its own.

	    threadHandle = CreateThread(
			NULL,                   // default security attributes
			0,                      // use default stack size  
			mainThread,       // thread function name
			NULL,          // argument to thread function 
			0,                      // use default creation flags 
			&threadId);   // returns the thread identifier 

		if (threadHandle == NULL)
		{
			//ErrorHandler(TEXT("CreateThread"));
			ExitProcess(3);
		}
		serviceState->bRunning = true;
	while (true)
	{
		bool isIdle = true;
		if (KEYDOWN(VK_ESCAPE)) // ��ESC�˳�,������ģʽ��ÿ��ѭ������ͣ������
			return -1;
		if (isIdle && KEYDOWN(0x41))  //����A����
		{
			isIdle = false;
			MSG msg;
			msg.message = WM_TIMECONTROLLER;
			WPARAM wParam = NULL;
			LPARAM lParam = NULL;
			PostThreadMessage(threadId, WM_TIMECONTROLLER, wParam, lParam);
			isIdle = true;
		}
		if (isIdle && KEYDOWN(0x42))  //����B����
		{
			isIdle = false;
			MSG msg;
			msg.message = WM_TIMECONTROLLER;
			WPARAM wParam = NULL;
			LPARAM lParam = NULL;
			PostThreadMessage(threadId, WM_TIMECONTROLLER, wParam, lParam);
			isIdle = true;
		}
		if (isIdle && KEYDOWN(0x43))  //����C����
		{
			isIdle = false;
			MSG msg;
			msg.message = WM_TIMECONTROLLER;
			WPARAM wParam = NULL;
			LPARAM lParam = NULL;
			PostThreadMessage(threadId, WM_TIMECONTROLLER, wParam, lParam);
			isIdle = true;
		}
		::Sleep(1000);
	}
}