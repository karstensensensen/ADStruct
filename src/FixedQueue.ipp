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
	size_t FixedQueue<T>::projectIndex(size_t index) const
	{
		return (index + m_front_index) % m_fixed_size;
	}

}

template<typename T>
void operator<<(T& target, ADS::FixedQueue<T>& queue)
{
	target = queue.front();
	queue.pop();
}

template<typename T>
inline std::ostream& operator<<(std::ostream& stream, const ADS::FixedQueue<T>& queue)
{
	for (size_t i = 0; i < queue.length(); i++)
		stream << queue[i] << ' ';
	return stream;
}
