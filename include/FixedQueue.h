#pragma once

#include <iostream>

namespace ADS
{
	/*
	class for containing a fixed size queue
	if the size of the queue reaches the fixed size

	it is implemented as a cyclic buffer to prevent the whole queue being copied on every push / pop.
	meaning the "front" of the queue will not always be the front of the vector.
	(SIZE = 4)
	 FRONT    BACK
	 |        |
	[1, 2, 3, 4] push 5
			   |
			   |
	 B	F      |
	 |	|      v
	[5, 2, 3, 4]
	*/
	template<typename T>
	class FixedQueue
	{
	public:
		FixedQueue() = default;
		FixedQueue(size_t size);

		T& front() { return m_data[m_front_index]; }
		T front() const { return m_data[m_front_index]; }

		void push_back(T elem);
		inline void push(T elem) { push_back(elem); };

		T& back() { return m_data[(m_front_index + m_size) % m_fixed_size]; }
		T back() const { return m_data[(m_front_index + m_size) % m_fixed_size]; }

		void pop_front();
		inline void pop() { pop_front(); };

		size_t size() const { return m_fixed_size; };
		size_t length() const { return m_size; };
		void resize(size_t new_size);

		T& operator[](size_t index);
		T operator[](size_t index) const;

		// pushes the element to the que
		void operator<<(T elem) { push_back(elem); }

	protected:

		size_t projectIndex(size_t index) const;

		size_t m_fixed_size;
		size_t m_size = 0;

		size_t m_front_index = 0;

		T* m_data = nullptr;
	};

	// Iterator for the FixedQueue class
	template<typename T>
	class FixedQueueIterator
	{
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::random_access_iterator_tag;

		FixedQueueIterator(pointer ptr, size_t& front_index , size_t& size, size_t& fixed_size)
			: m_ptr(ptr), m_front_index(front_index) {}

		FixedQueueIterator& operator++();
		FixedQueueIterator operator++(int);

		FixedQueueIterator& operator--();
		FixedQueueIterator operator--(int);

		bool operator==(const FixedQueue<T>& other);
		bool operator!=(const FixedQueue<T>& other);

		reference operator*();

	protected:
		pointer m_ptr;
		size_t& m_front_index;
	};

	// Const Iterator for the Fixed Queue class
	template<typename T>
	class ConstFixedQueueIterator
	{
		using difference_type = std::ptrdiff_t;
		using value_type = const T;
		using pointer = const T*;
		using reference = const T&;
		using iterator_category = std::random_access_iterator_tag;

		FixedQueueIterator(pointer ptr, size_t& front_index)
			: m_ptr(ptr), m_front_index(front_index) {}

		FixedQueueIterator& operator++();
		FixedQueueIterator operator++(int);

		FixedQueueIterator& operator--();
		FixedQueueIterator operator--(int);

		bool operator==(const FixedQueue<T>& other);
		bool operator!=(const FixedQueue<T>& other);

		reference operator*() const;

	protected:
		pointer m_ptr;
		size_t& m_front_index;
	};
	
	// Special Iterator for the FixedQueue class
	// pops the elements as it iterates over them
	template<typename T>
	class FixedQueuePopIterator
	{

	};
}

// stores the front into target and pops the que
template<typename T>
void operator<<(T& target, ADS::FixedQueue<T>& que);

template<typename T>
std::ostream& operator<<(std::ostream& stream, const ADS::FixedQueue<T>& queue);

#include "FixedQueue.ipp"


