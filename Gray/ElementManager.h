#pragma once
#ifndef ELEMENT_MANAGER_H
#define ELEMENT_MANAGER_H

#include <iostream>
#include <vector>

using namespace std;

/*
This class is a container based on std::vector
With it elements from middle can be removed and next elements can take empty spaces
*/

template <class T>
class ElementManager
{
public:
	vector<T> elements;
	vector<int> freeIds;
	vector<int> toDelete;
	int addElement(T element)
	{
		int id;
		if (this->freeIds.empty())
		{
			id = this->nextId;
			nextId++;
			this->numOfElements++;
			this->elements.push_back(element);
		}
		else
		{
			id = this->freeIds.back();
			this->freeIds.pop_back();
			this->elements[id] = element;
		}

		return id;
	}
	void removeElement(T element)
	{
		for (int i = 0; i < this->elements.size(); i++)
		{
			if (element == this->elements.at(i))
			{
				this->toDelete.push_back(i);
			}
		}
	}
	void removeElement(int id) //This function adds the id to the toDelete array, it doesnt delete them right away
	{
		bool elementExists = false;
		//Does this id exists in elements?

		for (int i = 0; i < this->elements.size(); i++)
		{
			if (this->elements.at(i) != nullptr)
			{
				if (this->elements.at(i)->id == id) {
					elementExists = true;
				}
			}

		}
		cout << endl;
		if (elementExists)
		{
			//Does this element already marked for removal?
			for (int i = 0; i < this->toDelete.size(); i++)
			{
				if (this->toDelete[i] == id)
				{
					//Don't add it a second time
					cout << "Already marked for delete";
					return;
				}
			}
			this->toDelete.push_back(id);

		}
		else
		{
			cout << "Trying to remove unexisting elements" << endl;
		}
	}
	void clearElements()
	{
		while (this->toDelete.size() > 0)
		{
			int id = toDelete.back();
			toDelete.pop_back();
			if (this->elements[id] != nullptr)
			{
				this->freeIds.push_back(id);
				this->elements[id]->cleanup();
				delete this->elements[id];
				this->elements[id] = nullptr;
				numOfElements--;
			}
			else
			{
				//This element already cleaned
			}
		}
	}
	void cleanup()
	{
		for (vector<T>::iterator it = this->elements.begin(); it != this->elements.end(); it++)
		{
			if ((*it) != nullptr)
			{
				(*it)->cleanup();
				delete (*it);
			}

		}
		this->numOfElements = 0;
		this->freeIds.clear();
		this->toDelete.clear();
		this->elements.clear();
		this->nextId = 0;
	}
	int getNumOfElements()
	{
		return this->numOfElements;
	}
	T* getElement(int id)
	{
		return this->elements[id];
	}
	ElementManager()
	{
		this->nextId = 0;
		this->numOfElements = 0;
	}
	~ElementManager()
	{

	}
private:
	int numOfElements = 0;
	int maxNumOfElements = 999;
	int nextId = 0;
};

#endif // !ELEMENT_MANAGER_H
