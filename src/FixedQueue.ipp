#include "FixedQueue.h"
#include <cassert>

namespace ADS
{
	template<typename T>
	FixedQueue<T>::FixedQueue(size_t size)
		: m_fixed_size(size)
	{
		m_data = new T[m_fixed_size];
	}

	template<typename T>
	void FixedQueue<T>::push_back(T elem)
	{

		if (m_size < m_fixed_size)
		{
			m_data[projectIndex(m_size)] = elem;
			m_size++;
		}
		else
		{
			m_data[m_front_index] = elem;
			m_front_index++;
			m_front_index %= m_fixed_size;
		}
	}

	template<typename T>
	void FixedQueue<T>::pop_front()
	{
		assert(m_size > 0);

		m_size--;

		m_front_index++;
		m_front_index %= m_fixed_size;
	}

	// changes the queues maximum size, if there is not enough space to store part of the data, it is deleted.
	// data is deleted from back to front
	// que will be reorganized so the queue front is at the array front instead of potentially in the middle of it, when resized
	template<typename T>
	void FixedQueue<T>::resize(size_t new_size)
	{
		T* new_arr = new T[new_size];

		for (size_t i = 0; i < std::min(m_size, new_size); i++)
		{
			new_arr[i] = this->operator[](i);
		}

		std::swap(m_data, new_arr);
		delete new_arr;

		m_fixed_size = new_size;
		// make sure the size of the queue is updated if it is shrunken
		m_size = std::min(m_fixed_size, m_size);
		// put the front index to the start of the array
		m_front_index = 0;
	}

	template<typename T>
	T& FixedQueue<T>::operator[](size_t index)
	{
		assert(index < m_size);
		return m_data[projectIndex(index)];
	}

	template<typename T>
	T FixedQueue<T>::operator[](size_t index) const
	{
		assert(index < m_size);
		return m_data[projectIndex(index)];
	}

	template<typename T>
	void FixedQueue<T>::clear()
	{
		m_size = 0;
		m_front_index = 0;
	}

	template<typename T>
	FixedQueueIterator<T> FixedQueue<T>::begin()
	{
		return FixedQueueIterator<T>(m_data + m_front_index, m_data, m_data + m_fixed_size);
	}

	template<typename T>
	ConstFixedQueueIterator<T> FixedQueue<T>::begin() const
	{
		return ConstFixedQueueIterator<T>(m_data + m_front_index, m_data, m_data + m_fixed_size);
	}

	template<typename T>
	FixedQueueIterator<T> FixedQueue<T>::end()
	{
		return FixedQueueIterator<T>(m_data + (m_front_index + m_size) % m_fixed_size, m_data, m_data + m_fixed_size);
	}

	template<typename T>
	ConstFixedQueueIterator<T> FixedQueue<T>::end() const
	{
		return ConstFixedQueueIterator<T>(m_data + (m_front_index + m_size) % m_fixed_size, m_data, m_data + m_fixed_size);
	}

	template<typename T>
	size_t FixedQueue<T>::projectIndex(size_t index) const
	{
		return (index + m_front_index) % m_fixed_size;
	}

	// SFixedQueue definitions

	template<typename T, size_t n>
	void SFixedQueue<T, n>::push_back(T elem)
	{

		if (m_size < n)
		{
			m_data[projectIndex(m_size)] = elem;
			m_size++;
		}
		else
		{
			m_data[m_front_index] = elem;
			m_front_index++;
			m_front_index %= n;
		}
	}

	template<typename T, size_t n>
	void SFixedQueue<T, n>::pop_front()
	{
		assert(m_size > 0);

		m_size--;

		m_front_index++;
		m_front_index %= n;
	}

	template<typename T, size_t n>
	T& SFixedQueue<T, n>::operator[](size_t index)
	{
		assert(index < m_size);
		return m_data[projectIndex(index)];
	}

	template<typename T, size_t n>
	T SFixedQueue<T, n>::operator[](size_t index) const
	{
		assert(index < m_size);
		return m_data[projectIndex(index)];
	}

	template<typename T, size_t n>
	void SFixedQueue<T, n>::clear()
	{
		m_size = 0;
		m_front_index = 0;
	}


	template<typename T, size_t n>
	FixedQueueIterator<T> SFixedQueue<T, n>::begin()
	{
		return FixedQueueIterator<T>(m_data + m_front_index, m_data, m_data + n);
	}

	template<typename T, size_t n>
	ConstFixedQueueIterator<T> SFixedQueue<T, n>::begin() const
	{
		return ConstFixedQueueIterator<T>(m_data + m_front_index, m_data, m_data + n);
	}

	template<typename T, size_t n>
	FixedQueueIterator<T> SFixedQueue<T, n>::end()
	{
		return FixedQueueIterator<T>(m_data + (m_front_index + m_size) % n, m_data, m_data + n);
	}

	template<typename T, size_t n>
	ConstFixedQueueIterator<T> SFixedQueue<T, n>::end() const
	{
		return ConstFixedQueueIterator<T>(m_data + (m_front_index + m_size) % n, m_data, m_data + n);
	}

	template<typename T, size_t n>
	size_t SFixedQueue<T, n>::projectIndex(size_t index) const
	{
		return (index + m_front_index) % n;
	}

	// FixedQueueIterator

	template<typename T>
	FixedQueueIterator<T>& FixedQueueIterator<T>::operator++()
	{
		m_ptr++;
		past_self = true;

		if (m_ptr >= m_q_back)
			m_ptr = m_q_front;

		return *this;
	}

	template<typename T>
	FixedQueueIterator<T> FixedQueueIterator<T>::operator++(int)
	{
		FixedQueueIterator<T> tmp = *this;

		*this++;

		return tmp;
	}

	template<typename T>
	FixedQueueIterator<T>& FixedQueueIterator<T>::operator--()
	{
		m_ptr--;
		past_self = true;

		if (m_ptr < m_q_front)
			m_ptr = m_q_back;

		return *this;
	}

	template<typename T>
	FixedQueueIterator<T> FixedQueueIterator<T>::operator--(int)
	{
		FixedQueueIterator<T> tmp = *this;

		*this--;

		return tmp;
	}

	template<typename T>
	bool FixedQueueIterator<T>::operator==(const FixedQueueIterator<T>& other)
	{
		return m_ptr == other.m_ptr && past_self;
	}

	template<typename T>
	bool FixedQueueIterator<T>::operator!=(const FixedQueueIterator<T>& other)
	{
		return !(*this == other);
	}

	template<typename T>
	typename FixedQueueIterator<T>::reference FixedQueueIterator<T>::operator*()
	{
		return *m_ptr;
	}

	// ConstFixedQueueIterator
	template<typename T>
	ConstFixedQueueIterator<T>& ConstFixedQueueIterator<T>::operator++()
	{
		m_ptr++;
		past_self = true;

		if (m_ptr >= m_q_back)
			m_ptr = m_q_front;

		return *this;
	}

	template<typename T>
	ConstFixedQueueIterator<T> ConstFixedQueueIterator<T>::operator++(int)
	{
		ConstFixedQueueIterator<T> tmp = *this;

		*this++;

		return tmp;
	}

	template<typename T>
	ConstFixedQueueIterator<T>& ConstFixedQueueIterator<T>::operator--()
	{
		m_ptr--;
		past_self = true;

		if (m_ptr < m_q_front)
			m_ptr = m_q_back;

		return *this;
	}

	template<typename T>
	ConstFixedQueueIterator<T> ConstFixedQueueIterator<T>::operator--(int)
	{
		ConstFixedQueueIterator<T> tmp = *this;

		*this--;

		return tmp;
	}

	template<typename T>
	bool ConstFixedQueueIterator<T>::operator==(const ConstFixedQueueIterator<T>& other)
	{
		return m_ptr == other.m_ptr && past_self;
	}

	template<typename T>
	bool ConstFixedQueueIterator<T>::operator!=(const ConstFixedQueueIterator<T>& other)
	{
		return !(*this == other);
	}

	template<typename T>
	typename ConstFixedQueueIterator<T>::reference ConstFixedQueueIterator<T>::operator*()
	{
		return *m_ptr;
	}
}

template<typename T>
void operator<<(T& target, ADS::FixedQueue<T>& queue)
{
	target = queue.front();
	queue.pop();
}

template<typename T, size_t n>
void operator<<(T& target, ADS::SFixedQueue<T, n>& queue)
{
	target = queue.front();
	queue.pop();
}

template<typename T>
inline std::ostream& operator<<(std::ostream& stream, const ADS::FixedQueue<T>& queue)
{
	for (const T& val : queue)
		stream << val << ' ';

	return stream;
}

template<typename T, size_t n>
inline std::ostream& operator<<(std::ostream& stream, const ADS::SFixedQueue<T, n>& queue)
{
	for (const T& val : queue)
		stream << val << ' ';

	return stream;
}
