#include "../DB/common.h"
class logQueueClass {
private:
	struct ListNode {
		struLogData* logData;
		ListNode* next;
	};
	ListNode* head;

public:
	logQueueClass();
	~logQueueClass();
	ListNode* CreateNode(struLogData* value);
	//推入数据（尾插）
	void push(struLogData* value);
	//推出数据（头删）
	struLogData* pop();
	//查询队头
	struLogData* front();
	//查询全部数据
	void select();
};