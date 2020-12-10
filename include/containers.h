#pragma once

//Linked List

namespace linked {

	template<class T>
	struct Node
	{
		T data;
		Node* next=nullptr;
		Node* pre=nullptr;
	};

	template<class T>
	class LinkedList {
	public:
		void addElement(T element)
		{
			Node<T>* node = new Node<T>();
			node->data = element;
			
			if (this->tail != nullptr && this->head != nullptr)
			{
				//List isn't empty
				this->tail->next = node;
				node->pre = this->tail;
				this->tail = node;
			}
			else if(this->tail == nullptr && this->head != nullptr){
				this->tail = node;
				this->head->next = node;
				this->tail->pre = this->head;
			}
			else {
				//List is empty
				this->tail = nullptr;
				this->head = node;
				this->head->next = nullptr;
				this->head->pre = nullptr;
			}
		}
		T popElement(Node<T>* node)
		{
			T element = (T) node->data;
			if (node == this->head)
			{
				this->head = node->next;
			}
			else {
				if (node->pre != nullptr && node->next != nullptr)
				{
					node->pre->next = node->next;
					node->next->pre = node->pre;
				}
				else if (node->pre == nullptr && node->next == nullptr) {
					this->head = nullptr;
					this->tail = nullptr;
				}
				else if(node->pre == nullptr && node->next != nullptr){
					this->head = node->next;
					this->head->pre = nullptr;
				}
				else {
					this->tail = node->pre;
					this->tail->next = nullptr;
				}

			}
			delete node;
			return element;
		}
		Node<T>* head = nullptr;
		Node<T>* tail = nullptr;
	};

}