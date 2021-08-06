#pragma once

#include <iostream>
#include <type_traits>
#include <concepts>
#include <string>
#include <vector>

namespace ADS
{
	namespace Bases
	{
		template<typename TB, typename TI>
		concept base = std::is_base_of_v<TB, TI>;

		// base node for all binary tree node types
		template<typename T, template<typename> class TNode>
		struct NodeBase
		{
		public:
			NodeBase(T val = T(), TNode<T>* left = nullptr, TNode<T>* right = nullptr)
				: val(val), left(left), right(right)
			{
				static_assert(std::is_base_of_v<NodeBase<T, TNode>, TNode<T>>);
			}

			~NodeBase() { delete left; delete right; }

			T val;

			TNode<T>* left = nullptr;
			TNode<T>* right = nullptr;

			void remove() { left = nullptr; right = nullptr; ~NodeBase(); };

			void deleteLeft() { delete left; left = nullptr; }
			void deleteRight() { delete right; right = nullptr; }

			void removeLeft() { left->remove(); left = nullptr; }
			void removeRight() { right->remove(); right = nullptr; }

			std::string toString() const;


		protected:
			void toStringHelper(std::string& str, std::string padding, std::string pointer, const NodeBase<T, TNode>* node) const;

		};
	}

	// standard binary tree node type
	template<typename T>
	struct Node: Bases::NodeBase<T, Node>
	{
		Node(T val = T(), Node<T>* left = nullptr, Node<T>* right = nullptr) : NodeBase<T, Node>(val, left, right){}

		

		Node<T>* left = nullptr;
		Node<T>* right = nullptr;

		void insertLeft(Node<T>* new_node);
		void insertLeft(T new_val);

		void insertRight(Node<T>* new_val);
		void insertRight(T new_val);

		Node<T>* lookup(T val);
	};
	
	// binary search tree node type
	template<typename T> requires requires(T x) { x < x <= x == x != x >= x > x; }
	struct SNode: public NodeBase<T, SNode>
	{
		SNode(T val) : NodeBase<T, SNode>(val) {};

		static SNode<T>* fromVector(const std::vector<T>& vec);

		void insert(SNode<T>* val) { insert(val, this); }
		void insert(SNode<T>* val, SNode<T>* node);
		void insert(T val);

		SNode<T>* lookup(T val);
	};
};


template<typename T, template<typename> class TNode>
std::ostream& operator<<(std::ostream& stream, const ADS::Bases::NodeBase<T, TNode>* root);

#include "BinaryTree.ipp"


