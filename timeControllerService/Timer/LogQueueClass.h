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
	//�������ݣ�β�壩
	void push(struLogData* value);
	//�Ƴ����ݣ�ͷɾ��
	struLogData* pop();
	//��ѯ��ͷ
	struLogData* front();
	//��ѯȫ������
	void select();
};