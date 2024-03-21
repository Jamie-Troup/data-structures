#pragma once

#include <memory>

template <typename T>
struct QueueNode
{
	T data{};
	std::shared_ptr<QueueNode<T>> next = nullptr;
	
	QueueNode<T>(T data_in)
		: data(data_in) {}
};

template <typename T>
class Queue
{
	int 							m_size  = 0;
	std::shared_ptr<QueueNode<T>> 	m_first = nullptr;
	std::shared_ptr<QueueNode<T>> 	m_top   = nullptr;
	
public:
	Queue() {}
	
	Queue(T data)
	{
		m_first = std::make_shared<QueueNode<T>>(data);
		m_top = m_first;
		m_size++;
	}
	
	T dequeue()
	{
		T val = m_first->data;
		
		if (m_size == 1)
		{
			m_first = nullptr;
			m_top = m_first;
		}
		else
		{
			m_first = m_first->next;
		}
		
		m_size--;
		return val;
	}
	
	void enqueue(T data)
	{
		auto node = std::make_shared<QueueNode<T>>(data);
		
		if (m_size == 0)
		{
			m_first = node;
			m_top = m_first;
		}
		else
		{
			m_top->next = node;
			m_top = node;		
		}
		
		m_size++;
	}
	
	bool isEmpty()
	{
		return m_size == 0;
	}
};
