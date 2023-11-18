#include "../DB/common.h"
class logQueueClass {
private:
	struct ListNode;
	
	ListNode* head;
	ListNode* tail;

public:	
	struct ListNode 
	{
		struLogData* logData;
		ListNode* next;
	};
	logQueueClass();
	~logQueueClass();
	void Clear();//清除所有数据
	ListNode* CreateNode(struLogData* value);
	//推入数据（尾插）
	void* push(struLogData* value);
	//推出数据（头删）
	struLogData* pop();
	//查询队头
	ListNode* front();
	//查询全部数据
	void select();
};