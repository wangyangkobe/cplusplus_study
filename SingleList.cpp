// wy.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <algorithm>

template <typename T>
struct Node{
	T elemet;
	Node* pNext;
	Node(T ele, Node* next = NULL):elemet(ele), pNext(next){}
};

template <typename T>
class SingleList{
public:
	SingleList(){ 
		head = tail = NULL; 
	}

	SingleList(T arr[], int len){
		head = tail = NULL;
		for(int i =0; i < len; ++i)
			pushElement(arr[i]);
	}

	void pushElement(T ele){
		if (head == NULL)
			head = tail = new Node<T>(ele, NULL);
		else{
			tail->pNext = new Node<T>(ele, NULL);
			tail = tail->pNext;
		}
	}

	void removeDuplicate()
	{
		if(head == NULL)
			return;
		Node<T>* pPre = head;
		Node<T>* pCur = pPre->pNext;
		while (pCur != NULL)
		{
			if(pCur->elemet == pPre->elemet)
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
	void printList(){
		Node<T>* temp = head;
		using std::cout;
		using std::endl;
		while (temp != NULL)
		{
			cout<<temp->elemet<<" ";
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

	int arr[] = {1, 1, 2, 3, 4, 4, 4, 5, 6, 6};
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
	return 0;
}
