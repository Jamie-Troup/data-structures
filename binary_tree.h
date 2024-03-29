#pragma once

#include "stack.h"
#include "queue.h"

#include <SFML/Graphics.hpp>
#include <cmath>
#include <memory>
#include <vector>
#include <type_traits>

template <typename T>
struct BinTreeNode
{
	T 								data;
	int 							id;
	std::shared_ptr<BinTreeNode<T>> left  = nullptr;
	std::shared_ptr<BinTreeNode<T>> right = nullptr;
	
	BinTreeNode<T>(T data_in)
		: data(data_in) {}
};

template <typename T>
class BinTree
{
	int	 m_size = 0;
	int  m_ids = 0;
	int  m_depth = -1;
	bool m_fullscreen = false;
	
public:
	std::shared_ptr<BinTreeNode<T>> root;
	
	BinTree() {}
	
	BinTree(T data)
	{
		root = std::make_shared<BinTreeNode<T>>(data);
		root->id = m_ids;
		m_ids++;
		m_size++;
		m_depth++; 
	}
	
	std::shared_ptr<BinTreeNode<T>> search(int id)
	{
		if (root)
		{
			auto node = root;
			
			if (node->id == id)
			{
				return node;
			}
			else 
			{
				auto stack = Stack<std::shared_ptr<BinTreeNode<T>>>();
				bool not_found = true;
				
				while (not_found)
				{
					node = preorderSuccessor(node, stack);
					
					if (!node)
					{
						not_found = false;
					}
					else
					{
						if (node->id == id)
						{
							not_found = false;
						}
					}
				}
				
				if (!node)
				{
					return nullptr;
				}
				else
				{
					return node;
				}
			}
		}
		
		return nullptr;
	}
	
	std::shared_ptr<BinTreeNode<T>> preorderSuccessor(std::shared_ptr<BinTreeNode<T>> node, Stack<std::shared_ptr<BinTreeNode<T>>>& stack)
	{
		if (node->left)
		{
			stack.push(node);
			
			return node->left;
		}
		
		if (node->right)
		{
			return node->right;
		}
		
		bool stack_not_empty = true;
		
		while (!node->right && stack_not_empty)
		{
			if (!stack.isEmpty())
			{
				node = stack.pop();
			}
			else
			{
				stack_not_empty = false;
			}
		}
		
		if (stack_not_empty == false)
		{
			return nullptr;
		}
		
		return node->right;
	}
	
	void insert(T data)
	{
		auto node = std::make_shared<BinTreeNode<T>>(data);
		node->id = m_ids++;
		
		auto queue = Queue<std::shared_ptr<BinTreeNode<T>>>();
		
		if (root)
		{
			bool cont = true;
			queue.enqueue(root);
			std::shared_ptr<BinTreeNode<T>> target;
			
			while (!queue.isEmpty() && cont)
			{
				auto temp = Queue<std::shared_ptr<BinTreeNode<T>>>();
				
				while(!queue.isEmpty())
				{
					temp.enqueue(queue.dequeue());
				}
				
				while(!temp.isEmpty() && cont)
				{
					auto tempNode = temp.dequeue();
					if (tempNode->left && tempNode->right)
					{
						queue.enqueue(tempNode->left);
						queue.enqueue(tempNode->right);
					}
					else
					{
						cont = false;
						target = tempNode;
					}
				}
			}
			
			if (target->left)
			{
				target->right = node;
			}
			else
			{
				target->left = node;
			}
		}
		else
		{
			root = node;
		}
		
		m_size++;
		m_depth = log2(m_size);
	}
	
	void delete_node(int id)
	{
		auto leaf_node = root;
		auto stack = Stack<std::shared_ptr<BinTreeNode<T>>>();
		
		if (m_size)
		{
			if (m_size == 1)
			{
				if (leaf_node->id == id)
				{
					root == nullptr;
				}
			}
			else if (m_size == 2)
			{
				if (leaf_node->id == id)
				{
					if (root->left)
					{
						root = root->left;
					}
					else
					{
						root = root->right;
					}
				}
				else
				{
					if (leaf_node->left)
					{
						root->left = nullptr;
					}
					else
					{
						root->right = nullptr;
					}
				}
			}
			else
			{
				bool leaf_not_found = true;
				
				while (leaf_not_found)
				{
					if (!leaf_node->left && !leaf_node->right)
					{
						leaf_not_found = false;
					}
					else
					{
						leaf_node = preorderSuccessor(leaf_node, stack);
					}
				}
				
				auto parent = find_parent(leaf_node->id);
				
				if (parent->left)
				{
					if (parent->left->id == leaf_node->id)
					{
						parent->left = nullptr;
					}
				}
				else
				{
					parent->right = nullptr;					
				}
				
				if (leaf_node->id != id)
				{
					if (id != root->id)
					{
						auto parent_del_node = find_parent(id);
						
						if (parent_del_node)
						{
							std::shared_ptr<BinTreeNode<T>> del_node;
							
							if (parent_del_node->left)
							{
								if (parent_del_node->left->id == id)
								{
									del_node = parent_del_node->left;
									parent_del_node->left = leaf_node;
								}	
							}
							else
							{
								del_node = parent_del_node->right;
								parent_del_node->right = leaf_node;
							}
							
							leaf_node->left = del_node->left;
							leaf_node->right = del_node->right;
						}
					}
					else
					{
						leaf_node->left = root->left;
						leaf_node->right = root->right;
						root = leaf_node;
					}
				}
			}
		}
	}
	
	std::shared_ptr<BinTreeNode<T>> find_parent(int id)
	{
		bool parent_found = false;
		bool tree_fully_checked = false;
		
		auto node = root;
		auto stack = Stack<std::shared_ptr<BinTreeNode<T>>>();
		
		while (!parent_found && !tree_fully_checked)
		{
			if (!node)
			{
				tree_fully_checked = true;
			}
			else
			{
				if (node->left || node->right)
				{
					if (node->left)
					{
						if (node->left->id == id)
						{
							parent_found = true;
						}
					}
					if (node->right)
					{
						if (node->right->id == id)
						{
							parent_found = true;
						}
					}
				}

				if (!parent_found)
				{
					node = preorderSuccessor(node, stack);				
				}
			}	
		}
		
		if (parent_found == true)
		{
			return node;
		}
		
		return nullptr;
	}
	
	void print()
	{
		if constexpr (std::is_same<T, char>::value)
		{
			sf::Font font;
			
			if (font.loadFromFile("font.ttf"))
			{
				if (m_depth >= 0)
				{
					sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
					window.setFramerateLimit(60);
					
					std::vector<float> vec(m_depth+1);
					if (m_depth > 0)
					{
						if (m_depth == 1)
						{
							vec[1] = 0.5;
						}
						else
						{
							float max = 0;
							for (int i = 0; i < m_depth-1; i++)
							{
								max += pow(2.0, i);
							}
							
							max += 0.5; // spacing from central axis of parent to edge of child box for deepest level
							vec[1] = max;
							
							for (int i = 2; i <= m_depth; i++)
							{
								vec[i] = (vec[i-1] - pow(2.0, m_depth-i));
							}
						}
					}
					
					// Root node SFML styling
					sf::Vector2<float> size(50.0, 50.0);
					sf::RectangleShape root_sq(size);
					root_sq.setFillColor(sf::Color::Green);
					root_sq.setOrigin(25, 25);
					sf::Vector2<float> root_pos(100, 25);
					root_sq.setPosition(root_pos);
					
					// Root text SFML styling
					sf::Text root_txt;
					root_txt.setFont(font);
					root_txt.setString(root->data);
					root_txt.setCharacterSize(24);
					root_txt.setFillColor(sf::Color::White);
					root_txt.setStyle(sf::Text::Bold);
					root_txt.setPosition(root_pos);
					
					auto queue = Queue<std::shared_ptr<BinTreeNode<T>>>();
					auto temp = Queue<std::shared_ptr<BinTreeNode<T>>>();
					auto parent_pos = Queue<sf::Vector2<float>>();
					auto orientation = Queue<char>();
					
					std::vector<sf::RectangleShape> shape_vec;
					std::vector<sf::Text> text_vec;
					
					shape_vec.push_back(root_sq);
					text_vec.push_back(root_txt);
					
					if (root->left)
					{
						parent_pos.enqueue(root_pos);
						queue.enqueue(root->left);
						orientation.enqueue('l');
					}
					
					if (root->right)
					{
						parent_pos.enqueue(root_pos);
						queue.enqueue(root->right);
						orientation.enqueue('r');
					}
					
					int curr_depth = 1;
					while (!queue.isEmpty())
					{
						while (!queue.isEmpty())
						{
							temp.enqueue(queue.dequeue());
						}

						while (!temp.isEmpty())
						{
							auto node = temp.dequeue();
							
							char orient = orientation.dequeue();
							auto par_pos_val = parent_pos.dequeue();
						
							float spacing = (vec[curr_depth]*50.0f) + 25;
							
							if (orient == 'l')
							{
								spacing = spacing * -1;
							}
							
							// Node SFML styling
							sf::RectangleShape shape(size);
							shape.setFillColor(sf::Color::Green);
							shape.setOrigin(25, 25);
							sf::Vector2<float> shape_pos(par_pos_val.x + spacing, (curr_depth * 50) + 25.0f);
							shape.setPosition(shape_pos);
							shape_vec.push_back(shape);
							
							// Node text styling
							sf::Text text;
							text.setFont(font);
							text.setString(node->data);
							text.setCharacterSize(24);
							text.setFillColor(sf::Color::White);
							text.setStyle(sf::Text::Bold);
							text.setPosition(shape_pos);
							text_vec.push_back(text);

							if (node->left)
							{
								queue.enqueue(node->left);
								orientation.enqueue('l');
								parent_pos.enqueue(shape_pos);
							}
							
							if (node->right)
							{
								queue.enqueue(node->right);
								orientation.enqueue('r');
								parent_pos.enqueue(shape_pos);
							}
						}
						
						if (!queue.isEmpty())
						{
							curr_depth++;
						}
					}

					while (window.isOpen())
					{
						sf::Event event;
						while (window.pollEvent(event))
						{
							if (event.type == sf::Event::Closed)
							{
								window.close();					
							}	
							else if (event.type == sf::Event::KeyPressed)
							{
								switch (event.key.code)
								{
									case sf::Keyboard::Q:
									{
										if (m_fullscreen)
										{
											auto max_size = window.getSize();
											float centre = max_size.x / 2.0f;
											float diff = centre - 100;
											
											window.create(sf::VideoMode(200, 200), "SFML works!");
											
											for (auto& shape: shape_vec)
											{
												shape.setPosition((shape.getPosition()).x - diff, shape.getPosition().y);
											}
											
											for (auto& text: text_vec)
											{
												text.setPosition((text.getPosition()).x - diff, text.getPosition().y);
											}
											
											m_fullscreen = false;
										}
										break;
									}
									case sf::Keyboard::F:
									{
										if (!m_fullscreen)
										{
											window.create(sf::VideoMode(200, 200), "SFML works!", sf::Style::Fullscreen);
											
											auto max_size = window.getSize();
											float centre = max_size.x / 2.0f;
											float diff = centre - 100;
											
											for (auto& shape: shape_vec)
											{
												shape.setPosition((shape.getPosition()).x + diff, shape.getPosition().y);
											}
											
											for (auto& text: text_vec)
											{
												text.setPosition((text.getPosition()).x + diff, text.getPosition().y);
											}
											
											m_fullscreen = true;
										}
										break;
									}
									default: 
									{
										break;
									}
								}
							}
						}
						
						window.clear();
						
						for (auto& shape: shape_vec)
						{
							window.draw(shape);					
						}
						
						for (auto& text: text_vec)
						{
							window.draw(text);
						}
						
						window.display();
					}
				}
			}
		}
	}
	
	int get_depth()
	{
		return m_depth;
	}
	
	void delete_all() {}
};
