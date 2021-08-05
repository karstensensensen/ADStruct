#pragma once

#include <iostream>

namespace ADS
{

	template<typename T>
	class FixedQueueIterator;
	template<typename T>
	class ConstFixedQueueIterator;

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

		// sets size to 0 and puts front_index at the start of the m_data array
		void clear();

		FixedQueueIterator<T> begin();
		ConstFixedQueueIterator<T> begin() const;

		FixedQueueIterator<T> end();
		ConstFixedQueueIterator<T> end() const;

		// pushes the element to the que
		void operator<<(T elem) { push_back(elem); }

	protected:

		size_t projectIndex(size_t index) const;

		size_t m_fixed_size;
		size_t m_size = 0;

		size_t m_front_index = 0;

		T* m_data = nullptr;
	};

	// a static version of FixedQueue
	template<typename T, size_t n>
	class SFixedQueue
	{
	public:
		SFixedQueue() = default;

		T& front() { return m_data[m_front_index]; }
		T front() const { return m_data[m_front_index]; }

		void push_back(T elem);
		inline void push(T elem) { push_back(elem); };

		T& back() { return m_data[(m_front_index + m_size) % n]; }
		T back() const { return m_data[(m_front_index + m_size) % n]; }

		void pop_front();
		inline void pop() { pop_front(); };

		// sets size to 0 and puts front_index at the start of the m_data array
		void clear();

		size_t size() const { return n; };
		size_t length() const { return m_size; };

		T& operator[](size_t index);
		T operator[](size_t index) const;

		FixedQueueIterator<T> begin();
		ConstFixedQueueIterator<T> begin() const;

		FixedQueueIterator<T> end();
		ConstFixedQueueIterator<T> end() const;

		// pushes the element to the que
		void operator<<(T elem) { push_back(elem); }

	protected:

		size_t projectIndex(size_t index) const;

		size_t m_size = 0;

		size_t m_front_index = 0;

		T m_data[n];
	};

	// Iterator for the FixedQueue class
	template<typename T>
	class FixedQueueIterator
	{
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::random_access_iterator_tag;

		FixedQueueIterator(pointer ptr, const pointer q_front, const pointer q_back)
			: m_ptr(ptr), m_q_front(q_front), m_q_back(q_back) {}

		FixedQueueIterator& operator++();
		FixedQueueIterator operator++(int);

		FixedQueueIterator& operator--();
		FixedQueueIterator operator--(int);

		bool operator==(const FixedQueueIterator<T>& other);
		bool operator!=(const FixedQueueIterator<T>& other);

		reference operator*();

	protected:
		pointer m_ptr;
		// stores the front and back of the m_data array, not the queue itself
		const pointer m_q_front;
		const pointer m_q_back;
		bool past_self = false;
	};

	// Const Iterator for the Fixed Queue class
	template<typename T>
	class ConstFixedQueueIterator
	{
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = const T;
		using pointer = const T*;
		using reference = const T&;
		using iterator_category = std::random_access_iterator_tag;

		ConstFixedQueueIterator(pointer ptr, const pointer q_front, const pointer q_back)
			: m_ptr(ptr), m_q_front(q_front), m_q_back(q_back) {}

		ConstFixedQueueIterator& operator++();
		ConstFixedQueueIterator operator++(int);

		ConstFixedQueueIterator& operator--();
		ConstFixedQueueIterator operator--(int);

		bool operator==(const ConstFixedQueueIterator<T>& other);
		bool operator!=(const ConstFixedQueueIterator<T>& other);

		reference operator*();

	protected:
		pointer m_ptr;
		// stores the front and back of the m_data array, not the queue itself
		const pointer m_q_front;
		const pointer m_q_back;
		bool past_self = false;
	};
}

// stores the front into target and pops the que
template<typename T>
void operator<<(T& target, ADS::FixedQueue<T>& que);

template<typename T, size_t n>
void operator<<(T& target, ADS::SFixedQueue<T, n>& que);

template<typename T>
std::ostream& operator<<(std::ostream& stream, const ADS::FixedQueue<T>& queue);

template<typename T, size_t n>
std::ostream& operator<<(std::ostream& stream, const ADS::SFixedQueue<T, n>& queue);

#include "FixedQueue.ipp"


