// wy.cpp : Defines the entry point for the console application.
//
#include <iostream>
#include <algorithm>

template <typename T>
struct Node{
	T element;
	Node* pNext;
	Node(T ele, Node* next = NULL):element(ele), pNext(next){}
};

template <typename T>
class SingleList{
public:
	SingleList(){ 
		head = tail = NULL; 
	}

	SingleList(T arr[], int len)
	{
		head = tail = NULL;
		for(int i =0; i < len; ++i)
			pushElement(arr[i]);
	}

	//在链表末端插入元素
	void pushElement(T ele)
	{
		if (head == NULL)
			head = tail = new Node<T>(ele, NULL);
		else
		{
			tail->pNext = new Node<T>(ele, NULL);
			tail = tail->pNext;
		}
	}
	//在链表指定位置pos插入元素
	void insertElement(Node<T>* pos, T ele)
	{
		if(head == NULL)
			return;
		Node<T>* temp = head;
		while (temp-> pNext != pos)
			temp = temp->pNext;

		Node<T>* pCreate = new Node<T>(ele, pos);
		temp->pNext = pCreate;
	}
	//删除有序链表中的重复元素
	void removeDuplicate()
	{
		if(head == NULL)
			return;
		Node<T>* pPre = head;
		Node<T>* pCur = pPre->pNext;
		while (pCur != NULL)
		{
			if(pCur->element == pPre->element)
			{
				pPre->pNext = pCur->pNext;
				pCur = pPre->pNext;
			}
			else
			{
				pPre = pCur;
				pCur = pPre->pNext;
			}
		}
	}
	//查找元素值为ele的结点
	Node<T>* findElement(T ele)
	{
		Node<T>* temp = head;
		while (temp != NULL)
		{
			if(temp->element == ele)
				return temp;
			else
				temp = temp->pNext;
		}
		return NULL;
	}
	//输出单链表
	void printList()
	{
		Node<T>* temp = head;
		using std::cout;
		using std::endl;
		while (temp != NULL)
		{
			cout<<temp->element<<" ";
			temp = temp->pNext;
		}
		cout<<endl;
	}
private:
	Node<T>* head;
	Node<T>* tail;
};

int main(int argc, char* argv[])
{

	int arr[] = {1, 1, 2, 3, 4, 4, 4, 5, 6, 6, 11};
	int len = sizeof(arr)/sizeof(int);
	SingleList<int> slist;
	std::for_each(arr, arr+len, [&](const int& val){ slist.pushElement(val); });
	slist.printList();
	slist.removeDuplicate();
	slist.printList();

	SingleList<int> slist2;
	slist2.printList();
	slist2.removeDuplicate();

	SingleList<int> slist3(arr, len);
	slist3.printList();

	Node<int>* pos = slist3.findElement(11);
	slist3.insertElement(pos, 10);
	slist3.printList();
	return 0;
}
