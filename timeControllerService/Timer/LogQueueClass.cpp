#include "LogQueueClass.h"
#include <iostream>

using namespace std;
	logQueueClass::logQueueClass() {
		head = nullptr;
	}
	logQueueClass::~logQueueClass() {//采用头删法析构
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
	//推入数据（尾插）
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
	//推出数据（头删）
	struLogData* logQueueClass::pop() {
		if (!head) {
			std::cout << "此队列为空，不可再删！！" << std::endl;
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
	//查询队头
	struLogData* logQueueClass::front() {
		if (!head) {
			std::cout << "队列为空！！没有队头！！" << std::endl;
			return NULL;
		}
		return head->logData;
	}
	//查询全部数据
	void logQueueClass::select() {
		if (!head) {
			std::cout << "队列为空！！" << std::endl;
			return;
		}
		ListNode* temp = head;
		while (temp) {
			std::cout << temp->logData << " ";
			temp = temp->next;
		}
		std::cout << std::endl;
	}