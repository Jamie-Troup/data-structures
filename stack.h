#pragma once

#include <memory>

template <class T>
class StackNode
{
public:
	T data{};
	std::shared_ptr<StackNode<T>> next = nullptr;
	
	StackNode<T>(T data_in)
		: data(data_in) {}
};

template <class T>
class Stack
{
	int						 		m_size = 0;
	std::shared_ptr<StackNode<T>> 	m_root = nullptr;
	std::shared_ptr<StackNode<T>>   m_top  = nullptr;

public:
	Stack() {};
	
	Stack(T data)
	{
		m_root = std::make_shared<StackNode<T>>(data);
		m_top = m_root;
		m_size++;
	}
	
	void push(T data)
	{	
		if (m_top)
		{
			m_top->next = std::make_shared<StackNode<T>>(data);
			m_top = m_top->next;
		}
		else
		{
			m_root = std::make_shared<StackNode<T>>(data);
			m_top = m_root;
		}
		
		m_size++;
	}
	
	T pop()
	{	
		if (m_size == 1)
		{
			auto root_cpy = m_root;
			m_root = nullptr;
			m_top = nullptr;
			m_size--;
			return root_cpy->data;
		}
		else
		{
			auto curr_node = m_root;
			
			for (int i = 0; i < (m_size - 2); i++)
			{
				curr_node = curr_node->next;
			}
			
			auto node_cpy = curr_node->next;
			curr_node->next = nullptr;
			m_top = curr_node;
			m_size--;
			return node_cpy->data;
		}
	}
	
	T& top()
	{
		if (m_top)
		{
			return m_top->data;		
		}
		
		return m_top;
	}
	
	bool isEmpty()
	{
		return m_size == 0;
	}
};
