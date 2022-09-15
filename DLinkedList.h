#pragma once

#include <iostream>
#include <Windows.h>
using namespace std;

void SetColorAndBackground(int ForgC, int BackC)
{
	WORD wColor = ((BackC & 0x0F) << 4) + (ForgC & 0x0F);;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wColor);
	return;
}

template <typename T>
class Node {
public:
	T data;
	Node<T>* next;
	Node<T>* prev;
};

template <typename T>
class DLinkedList {

private:
	Node<T>* head;

public:
	class literator {
	protected:
		friend class DLinkedList<T>;
		Node<T>* current;

	public:
		literator(Node<T>* start = NULL) {
			current = start;
		}

		T& operator*() const {
			return current->data;
		}

		literator& operator++() {
			if (current != NULL)
				current = current->next;
			return *this;
		}

		literator operator++(int) {
			literator l1(current);
			if (current != NULL)
				current = current->next;
			return l1;
		}

		literator& operator--() {
			if (current != NULL)
				current = current->prev;
			return *this;
		}

		literator operator--(int) {
			literator l1(current);
			if (current != NULL)
				current = current->prev;
			return l1;
		}

		bool operator == (literator& rhs) {
			if (current == rhs.current) return true;
			return false;
		}

		bool operator != (literator& rhs) {
			if (current != rhs.current) return true;
			return false;
		}
	};

public:
	DLinkedList();
	void Insert_at_Head(T const element);
	void Delete_from_Head();
	void Delete_All(T const element);
	void Replace_All(T const element1, T const element2);
	void Replace_All(T const* element1, int size1, T const* element2, int size2);
	void Swap(Node<T>* st1, Node<T>* st2);

	literator begin() {
		return head;
	}

	literator end() {
		return NULL;
	}

	void Insert(literator l, T const element) {
		literator temp = end();
		if (l != temp) {
			Node<T>* curr = head;
			literator currIterator(curr);

			while (l != currIterator) {
				curr = curr->next;
				currIterator++;
			}

			Node<T>* newcurr = new Node<T>;

			newcurr->data = element;
			newcurr->next = curr->next;
			newcurr->prev = curr;

			if (curr->next)
				curr->next->prev = newcurr;
			curr->next = newcurr;
		}
	}

	void Insert_Multiple(literator l, T const* element, int n) {
		literator temp = end();

		if (l != temp) {
			Node<T>* curr = head;
			literator currIterator(curr);

			while (l != currIterator) {
				curr = curr->next;
				currIterator++;
			}

			int count = n;
			Node<T>* newcurr;
			while (count > 0) {
				newcurr = new Node<T>;
				newcurr->data = element[n - count];
				newcurr->prev = curr;
				newcurr->next = curr->next;

				if (curr->next)
					curr->next->prev = newcurr;
				curr->next = newcurr;

				curr = curr->next;
				count--;
			}
		}
	}

	void Delete(literator& l, T& value) {
		literator temp = end();
		literator Head = begin();
		if (l != temp && l != Head) {
			Node<T>* curr = head;
			while (Head != l) {
				curr = curr->next;
				Head++;
			}
			value = curr->data;

			Node<T>* bond = curr->prev;
			bond->next = curr->next;
			if (curr->next) curr->next->prev = bond;
			l++;
			delete curr;

		}
	}

	void Delete_Multiple(literator& l, T const* element, int n) {
		literator check = begin();
		literator remember = check;
		if (l != check) {
			check++;
			int count = 0, i = 0;
			T value;
			l++;
			while (check != l) {
				if (*check == element[i]) {
					remember = check;
					while (*check == element[i] && i < n && check != l) {
						count++;
						i++;
						check++;
					}
					if (count == n) {
						while (i > 0) {
							Delete(remember, value);
							i--;
						}
					}
					else {
						check = remember;
						check++;
					}
				}
				else check++;
				i = 0;
				count = 0;
			}
			//l--;
		}
	}

	void Delete_in_Range(literator& l1, literator& l2) {
		literator l3 = end();
		if (l1 != l3) {
			T value;
			while (l1 != l2)
				Delete(l1, value);
			l2++;
			Delete(l1, value);
		}
	}

	literator Search_First(T const* element, int n) {
		literator l1 = begin(), l2 = end();
		l1++;
		literator remember = l1;

		int i = 0;

		while (l1 != l2) {
			if (*l1 == element[i]) {
				remember = l1;
				while (l1 != l2 && *l1 == element[i] && i < n) {
					i++;
					l1++;
				}
				if (i == n) return remember;

				l1 = remember;
				l1++;
			}
			else l1++;
			i = 0;
		}
		return NULL;
	}

	literator* Search_All(const T* element, int n, int& occurences) {
		literator* l = NULL;
		literator* temp = NULL;
		int size = 0;

		literator l1 = begin(), l2 = end();
		l1++;
		literator remember = l1;

		int i = 0;

		while (l1 != l2) {
			if (*l1 == element[i]) {
				remember = l1;
				while (l1 != l2 && *l1 == element[i] && i < n) {
					i++;
					l1++;
				}
				if (i == n) {
					occurences++;
					temp = new literator[occurences];
					temp[occurences - 1] = remember;
					if (l) {
						for (int i = 0; i < occurences - 1; i++) temp[i] = l[i];
						delete[]l;
					}
					l = temp;
					temp = NULL;
				}
				else {
					l1 = remember;
					l1++;
				}
			}
			else l1++;
			i = 0;
		}

		return l;
	}

	void UpdateEntries(int array[]);
	void print() const;
	void reprint() const;
	void reprintHighlight(literator* l, int occurences, int wordSize) {

		SetColorAndBackground(0, 15);

		//SetColorAndBackground(6, 15);  for highlighting..
		literator match = begin();
		match++;

		Node<T>* curr = head->next;
		int count = 0, i = 0, limit = 0;
		while (curr != NULL) {
			if (i < occurences && match == l[i]) {
				SetColorAndBackground(15, 6);
				while (limit < wordSize) {
					if (count == 100) {
						cout << endl;
						count = 0;
					}
					if (curr->data == 13) {
						count = 0;
						cout << endl;
						curr = curr->next;
						match++;
						continue;
					}
					cout << curr->data;
					++count;
					curr = curr->next;
					match++;

					limit++;
				}
				limit = 0;
				i++;
				SetColorAndBackground(0, 15);
				continue;
			}
			if (count == 100) {
				cout << endl;
				count = 0;
			}
			if (curr->data == 13) {
				count = 0;
				cout << endl;
				curr = curr->next;
				match++;
				continue;
			}
			cout << curr->data;
			++count;
			curr = curr->next;
			match++;
		}
		SetColorAndBackground(15, 0);
	}

	void setCoordinates(literator current, int& x, int& y) {
		literator l = begin();
		x = 0;
		y = 0;
		int count = 0;

		while (l != current) {
			if (count == 100) {
				count = 0;
				y++;
				x = 0;
			}
			if (*l == 13) {
				count = 0;
				y++;
				x = 0;
				l++;
				++x;
				continue;
			}
			++x;
			++count;
			l++;
		}
	}

	~DLinkedList();
};

template <typename T>
DLinkedList<T>::DLinkedList() {
	head = new Node<T>;
	head->next = NULL;
	head->prev = NULL;

}

template <typename T>
void DLinkedList<T>::Insert_at_Head(T const element) {
	Node<T>* curr = new Node<T>;
	curr->data = element;

	if (head->next == NULL) {
		curr->next = head->next;
		curr->prev = head;
		head->next = curr;
	}
	else {
		curr->next = head->next;
		curr->next->prev = curr;
		curr->prev = head;
		head->next = curr;
	}
}

template <typename T>
void DLinkedList<T>::Delete_from_Head() {
	if (head->next) {
		Node<T>* curr = head->next;
		head->next = head->next->next;
		if (head->next)
			head->next->prev = head;
		delete curr;
	}
}

template <typename T>
void DLinkedList<T>::Delete_All(T const element) {
	if (head->next) {
		Node<T>* curr = head->next;
		Node<T>* temp;
		while (curr != NULL) {
			if (curr->data == element) {
				temp = curr;
				curr->prev->next = curr->next;
				if (curr->next != NULL)
					curr->next->prev = curr->prev;
				curr = curr->next;
				delete temp;
				continue;
			}
			curr = curr->next;
		}
	}
}

template <typename T>
void DLinkedList<T>::Replace_All(T const element1, T const element2) {
	if (head->next) {
		Node<T>* curr = head->next;
		while (curr != NULL) {
			if (curr->data == element1) {
				curr->data = element2;
			}
			curr = curr->next;
		}
	}
}

template <typename T>
void DLinkedList<T>::Replace_All(T const* element1, int size1, T const* element2, int size2) {
	if (head->next) {
		Node<T>* curr = head->next;
		Node<T>* remember;
		Node<T>* bond;
		int i = 0, count = 0;
		while (curr != NULL) {
			if (curr->data == element1[i]) {
				remember = curr;
				count = 0;
				while (curr != NULL && curr->data == element1[i] && count < size1) {
					i++;
					count++;
					curr = curr->next;
				}
				i = 0;
				if (count == size1) {
					bond = remember->prev;
					while (count > 0) {
						bond->next = remember->next;
						if (bond->next) bond->next->prev = bond;
						remember = bond->next;
						count--;
					}
					while (count < size2) {
						remember = new Node<T>;
						remember->data = element2[count];
						remember->prev = bond;
						remember->next = bond->next;
						if (remember->next) remember->next->prev = remember;
						bond->next = remember;
						bond = bond->next;
						count++;
					}
				}
				curr = remember;
			}
			curr = curr->next;
		}
	}
}

template <typename T>
void DLinkedList<T>::Swap(Node<T>* st1, Node<T>* st2) {
	if (st1 && st2) {
		Node<T>* temp1 = st2->next;
		Node<T>* temp2 = st2->prev;

		st2->next = st1->next;
		st2->prev = st1->prev;
		st1->next = temp1;
		st1->prev = temp2;

		if (st2->next) st2->next->prev = st2;
		st2->prev->next = st2;
		if (st1->next) st1->next->prev = st1;
		st1->prev->next = st1;
	}
}

template <typename T>
void DLinkedList<T>::print() const {
	Node<T>* curr = head->next;
	while (curr->next != NULL) {
		cout << curr->data << " ";
		curr = curr->next;
	}
	cout << curr->data << endl;

	//reverse printing
	/*while (curr->prev != null) {
		cout << curr->data << "";
		curr = curr->prev;
	}
	cout << endl << endl;*/

}

template <typename T>
void DLinkedList<T>::reprint() const {
	SetColorAndBackground(0, 15);
	Node<T>* curr = head->next;
	int count = 0;
	while (curr != NULL) {
		if (count == 100) {
			cout << endl;
			count = 0;
		}
		if (curr->data == 13) {
			count = 0;
			cout << endl;
			curr = curr->next;
			continue;
		}
		cout << curr->data;
		++count;
		curr = curr->next;
	}
	SetColorAndBackground(15, 0);
}

template <typename T>
void DLinkedList<T>::UpdateEntries(int array[]) {
	Node<T>* curr = head->next;
	int count = 0, i = 0;
	for (int j = 0; j < 51; j++) array[j] = 0;
	while (curr != NULL) {
		if (count == 100) {
			array[i]++;
			i++;
			count = 0;
		}
		if (curr->data == 13) {
			array[i]++;
			count = 0;
			i++;
			curr = curr->next;
			continue;
		}
		++count;
		curr = curr->next;
		array[i] = count;
	}
}

template <typename T>
DLinkedList<T>::~DLinkedList() {
	Node<T>* curr = head;
	while (head) {
		head = head->next;
		delete curr;
		curr = head;
	}
}