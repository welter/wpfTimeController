#include "LogQueueClass.h"
#include <iostream>

using namespace std;
	logQueueClass::logQueueClass() {
		head = nullptr;
	}
	logQueueClass::~logQueueClass() {//����ͷɾ������
		if (!head) return;
		ListNode* now = head;
		ListNode* next = nullptr;
		while (now->next) {
			next = now->next;
			delete now;
			now = next;
		}
		delete now;
		now = nullptr;
	}
	logQueueClass::ListNode* logQueueClass::CreateNode(struLogData* value) {
		ListNode* newNode = new ListNode;
		newNode->logData = value;
		newNode->next = nullptr;

		return newNode;
	}
	//�������ݣ�β�壩
	void logQueueClass::push(struLogData* value) {
		ListNode* newNode = CreateNode(value);
		if (!head) {
			head = newNode;
		}
		else {
			ListNode* temp = head;
			while (temp->next) {
				temp = temp->next;
			}
			temp->next = newNode;
		}
	}
	//�Ƴ����ݣ�ͷɾ��
	struLogData* logQueueClass::pop() {
		if (!head) {
			std::cout << "�˶���Ϊ�գ�������ɾ����" << std::endl;
			return NULL;
		}
		ListNode* now = head;
		struLogData* nowLogData = now->logData;
		ListNode* next = nullptr;
		if (now->next) {
			next = now->next;
			delete now;
			now = next;
			head = next;
			return nowLogData;
		}
		else {
			delete now;
			now = nullptr;
			return nowLogData;
		}
	}
	//��ѯ��ͷ
	struLogData* logQueueClass::front() {
		if (!head) {
			std::cout << "����Ϊ�գ���û�ж�ͷ����" << std::endl;
			return NULL;
		}
		return head->logData;
	}
	//��ѯȫ������
	void logQueueClass::select() {
		if (!head) {
			std::cout << "����Ϊ�գ���" << std::endl;
			return;
		}
		ListNode* temp = head;
		while (temp) {
			std::cout << temp->logData << " ";
			temp = temp->next;
		}
		std::cout << std::endl;
	}