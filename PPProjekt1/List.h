#pragma once

template<typename T>
struct ListElement
{
	ListElement<T>* next = nullptr;
	T value;
};

template<typename T>
class List
{
	int size = 0;
	ListElement<T>* first = nullptr;
	ListElement<T>* last = nullptr;
public:
	List()
	{
	}
	~List()
	{
		clear();
	}

	int getSize()
	{
		return size;
	}
	void add(T value)
	{
		size++;
		if(first == nullptr)
		{
			first = new ListElement<T>;
			first->value = value;
			last = first;
		}
		else
		{
			last->next = new ListElement<T>;
			last->next->value = value;
			last = last->next;
		}
	}
	ListElement<T>* get(int index)
	{
		int i = 0;
		ListElement<T>* current = first;
		while(i < index)
		{
			current = current->next;
			i++;
		}
		return current;
	}
	void clear()
	{
		size = 0;
		if (first == nullptr)
			return;
		ListElement<T>* current = first;
		ListElement<T>* next;
		while ((next = current->next) != nullptr)
		{
			delete current;
			current = next;
		}
		if (current != nullptr)
			delete current;
		first = nullptr;
		last = nullptr;
	}

	
};

