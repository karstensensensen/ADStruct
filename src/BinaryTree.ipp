#pragma once

#include "BinaryTree.h"

#include <stack>

namespace ADS
{
	namespace Bases
	{
		// definition of toString for node base

		template<typename T, template<typename> class TNode>
		std::string NodeBase<T, TNode>::toString() const
		{
			std::string result;
			toStringHelper(result, "", "", this);
			return result;
		}

		template<typename T, template<typename> class TNode>
		void NodeBase<T, TNode>::toStringHelper(std::string& str, std::string padding, std::string pointer, const NodeBase<T, TNode>* node) const
		{
			if (!node) return;

			str += padding + pointer + std::to_string(node->val) + '\n';

			padding += "\xB3 ";

			std::string pointerLeft = node->right ? "\xC3l\xC4" : "\xC0l\xC4";
			std::string pointerRight = "\xC0r\xC4";

			toStringHelper(str, padding, pointerLeft, node->left);
			toStringHelper(str, padding, pointerRight, node->right);
		}
	}

	// standard binary tree node definitions

	template<typename T>
	void Node<T>::insertLeft(Node<T>* new_node)
	{
		if (left)
		{
			Node<T>* tmp = left;
			left = new_node;
			left->left = tmp;
		}
		else
			left = new_node;
	}

	template<typename T>
	void Node<T>::insertLeft(T new_val)
	{
		insertLeft(new Node<T>(new_val));
	}

	template<typename T>
	void Node<T>::insertRight(Node<T>* new_node)
	{
		if (right)
		{
			Node<T>* tmp = right;
			right = new_node;
			right->right = tmp;
		}
		else
			right = new_node;
	}

	template<typename T>
	void Node<T>::insertRight(T new_val)
	{
		insertRight(new Node<T>(new_val));
	}

	template<typename T>
	Node<T>* Node<T>::lookup(T val)
	{
		std::stack<Node<T>*> node_stack;

		node_stack.push(this);

		while (!node_stack.empty())
		{
			Node<T>* tmp = node_stack.top();
			
			node_stack.pop();
			
			if (tmp->left->val == val)
				return tmp->left;
			else if (tmp->right->val == val)
				return tmp->right;

			if (tmp->left)
				node_stack->push(tmp->left);

			if (tmp->right)
				node_stack->push(tmp->right);
		}

		return nullptr;
	}

	// binary search tree definitions
	template<typename T> requires requires(T x) { x < x <= x == x != x >= x > x; }
	SNode<T>* SNode<T>::fromVector(const std::vector<T>& vec)
	{
		SNode<T>* head = new SNode<T>(vec[0]);

		for (size_t i = 1; i < vec.size(); i++)
			head->insert(vec[i]);

		return head;
	}

	template<typename T> requires requires(T x) { x < x <= x == x != x >= x > x; }
	void SNode<T>::insert(SNode<T>* new_node, SNode<T>* node)
	{
		if (new_node->val > node->val)
		{
			if (node->right)
			{
				insert(new_node, node->right);
			}
			else
			{
				node->right = new_node;
			}
		}
		else
		{
			if (node->left)
			{
				insert(new_node, node->left);
			}
			else
			{
				node->left = new_node;
			}
		}
	}
	
	template<typename T> requires requires(T x) { x < x <= x == x != x >= x > x; }
	void SNode<T>::insert(T new_val)
	{
		insert(new SNode<T>(new_val));
	}
	
	template<typename T> requires requires(T x) { x < x <= x == x != x >= x > x; }
	SNode<T>* SNode<T>::lookup(T val)
	{
		SNode<T>* tmp = this;
			
		while (tmp)
		{
			if (tmp->val == val)
				return tmp;
			else if (tmp->val >= val)
				tmp = tmp->left;
			else if (tmp->val < val)
				tmp = tmp->right;
		}
	
		return nullptr;
	}
}

template<typename T, template<typename> class TNode>
std::ostream& operator<<(std::ostream& stream, const ADS::Bases::NodeBase<T, TNode>* root)
{
	stream << root->toString() << '\n';
	return stream;
}
