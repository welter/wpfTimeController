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
	void Clear();//�����������
	ListNode* CreateNode(struLogData* value);
	//�������ݣ�β�壩
	void* push(struLogData* value);
	//�Ƴ����ݣ�ͷɾ��
	struLogData* pop();
	//��ѯ��ͷ
	ListNode* front();
	//��ѯȫ������
	void select();
};