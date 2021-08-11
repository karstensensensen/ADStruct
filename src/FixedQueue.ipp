#include "FixedQueue.h"
#include <cassert>
#include <numeric>
#include "..\include\FixedQueue.h"


namespace ADS
{
	namespace Bases
	{
		template<typename T>
		void FixedQueueBase<T>::push_back(T elem)
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
		template<i_iterator_ct<T> TIter>
		void FixedQueueBase<T>::push_back(TIter begin, TIter end)
		{
			for (auto current = begin; current != end; current++)
				push_back((T)*current);
		}

		template<typename T>
		template<typename TOther> requires std::is_convertible_v<TOther, T>
		void FixedQueueBase<T>::push_back(std::initializer_list<TOther> list)
		{
			for (const TOther& elem : list)
				push_back((T)elem);
		}

		template<typename T>
		template<typename TOther> requires std::is_convertible_v<TOther, T>
		void FixedQueueBase<T>::push_back(FixedQueueBase<TOther>& other)
		{
			for (const TOther& elem : other)
			{
				push_back((T)elem);
			}

			other.clear();
		}

		template<typename T>
		template<typename TOther> requires std::is_convertible_v<TOther, T>
			void FixedQueueBase<T>::push_back(const FixedQueueBase<TOther>& other)
			{
				for (const TOther& elem : other)
				{
					push_back((T)elem);
				}
			}

		template<typename T>
		void FixedQueueBase<T>::pop_front(size_t elem_count)
		{
			assert(m_size > 0 && elem_count <= length());

			m_size -= elem_count;

			m_front_index += elem_count;
			m_front_index %= m_fixed_size;
		}

		template<typename T>
		T& FixedQueueBase<T>::operator[](size_t index)
		{
			assert(index < m_size);
			return m_data[projectIndex(index)];
		}

		template<typename T>
		T FixedQueueBase<T>::operator[](size_t index) const
		{
			assert(index < m_size);
			return m_data[projectIndex(index)];
		}

		template<typename T>
		template<typename TCast> requires std::is_convertible_v<T, TCast>
		std::vector<TCast> FixedQueueBase<T>::toVector() const
		{
			std::vector<TCast> result;
			result.reserve(length());

			for (const T& val : *this)
				result.pushback((TCast)val);

			return result;
		}

		template<typename T>
		template<typename TCast> requires std::is_convertible_v<T, TCast>
		std::unique_ptr<TCast> FixedQueueBase<T>::toCarr() const
		{
			TCast* carr = new TCast[length()];

			for (size_t i = 0; i < length(); i++)
				carr[i] = (TCast)this->operator[](i);

			return std::unique_ptr<TCast>(carr);
		}

		
		template<typename T>
		template<typename TAvg> requires requires(T x) { x + x / x; }
		T FixedQueueBase<T>::avg()
		{
			// standard avrage calculation
			TAvg sum = std::accumulate(begin(), end(), TAvg(0));
			return sum / (TAvg)length();
		}

		template<typename T>
		template<typename TAvg> requires requires(T x) { x + x / x; }
		T FixedQueueBase<T>::avgHuge()
		{
			TAvg avg = TAvg(0);

			for (const T& elem : *this)
			{
				// divide each element by the length before accumulating it
				avg += (TAvg)elem / (TAvg)length();
			}

			return (T) avg;
		}

		template<typename T>
		T FixedQueueBase<T>::max(size_t offset)
		{
			return operator[](iOfMax(offset));
		}

		template<typename T>
		size_t FixedQueueBase<T>::iOfMax(size_t offset)
		{
			size_t i = 0;
			size_t max_i = 0;
			T* max_val = &*(begin() + offset);

			for (FixedQueueIterator<T> it = begin() + 1 + offset; it != end(); it++)
			{
				if (*it > *max_val)
				{
					// prevent large objects from being copied, so hold a pointer instead.
					max_val = &*it;
					max_i = i;
				}

				i++;
			}

			return max_i;
		}


		template<typename T>
		inline T FixedQueueBase<T>::min(size_t offset)
		{
			return operator[](iOfMin(offset));
		}
		
		template<typename T>
		size_t FixedQueueBase<T>::iOfMin(size_t offset)
		{
			size_t i = 0;
			size_t min_i = 0;
			T* min_val = &*(begin() + offset);

			for (FixedQueueIterator<T> it = begin() + 1 + offset; it != end(); it++)
			{
				if (*it < *min_val)
				{
					// prevent large objects from being copied, so hold a pointer instead.
					min_val = &*it;
					min_i = i;
				}

				i++;
			}

			return min_i;
		}

		template<typename T>
		void FixedQueueBase<T>::clear()
		{
			m_size = 0;
			m_front_index = 0;
		}

		template<typename T>
		FixedQueueIterator<T> FixedQueueBase<T>::begin()
		{
			return FixedQueueIterator<T>(m_data + m_front_index, m_data, m_data + m_fixed_size);
		}

		template<typename T>
		ConstFixedQueueIterator<T> FixedQueueBase<T>::begin() const
		{
			return ConstFixedQueueIterator<T>(m_data + m_front_index, m_data, m_data + m_fixed_size);
		}

		template<typename T>
		FixedQueueIterator<T> FixedQueueBase<T>::end()
		{
			return FixedQueueIterator<T>(m_data + (m_front_index + m_size) % m_fixed_size, m_data, m_data + m_fixed_size);
		}

		template<typename T>
		ConstFixedQueueIterator<T> FixedQueueBase<T>::end() const
		{
			return ConstFixedQueueIterator<T>(m_data + (m_front_index + m_size) % m_fixed_size, m_data, m_data + m_fixed_size);
		}

		template<typename T>
		size_t FixedQueueBase<T>::projectIndex(size_t index) const
		{
			return (index + m_front_index) % m_fixed_size;
		}
	}

	template<typename T>
	FixedQueue<T>::FixedQueue(size_t size)
		: Bases::FixedQueueBase<T>(new T[size], size) {}

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

		++*this;

		return tmp;
	}

	template<typename T>
	FixedQueueIterator<T> FixedQueueIterator<T>::operator+(size_t offset)
	{
		FixedQueueIterator<T> result = *this;

		result += offset;

		return result;
	}

	template<typename T>
	FixedQueueIterator<T>& FixedQueueIterator<T>::operator+=(size_t offset)
	{
		if (offset > 0)
		{
			past_self = true;

			size_t diff = m_q_back - m_ptr;

			// if the distance between the current pointer and the end of the queue memory is less than offset, it must cycle back to the start of the queue memory.
			if (diff < offset)
				m_ptr = m_q_front + offset - diff;
			else
				m_ptr += offset;
		}

		return *this;
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

		--*this;

		return tmp;
	}

	template<typename T>
	FixedQueueIterator<T> FixedQueueIterator<T>::operator-(size_t offset)
	{
		FixedQueueIterator<T> result = *this;

		result += offset;

		return result;
	}

	template<typename T>
	FixedQueueIterator<T>& FixedQueueIterator<T>::operator-=(size_t offset)
	{
		if (offset > 0)
		{
			past_self = true;

			size_t diff = m_ptr - m_q_front;

			// if the distance between the current pointer and the start of the queue memory is less than offset, it must cycle back to the end of the queue memory.
			if (diff < offset)
				m_ptr = m_q_back - offset + diff;
			else
				m_ptr -= offset;
		}

		return *this;
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
void operator<<(T& target, ADS::Bases::FixedQueueBase<T>& queue)
{
	target = queue.front();
	queue.pop();
}

template<typename T>
void operator<<(std::vector<T>& target, ADS::Bases::FixedQueueBase<T>& que)
{
	std::copy(que.begin(), que.begin() + std::min(target.size(), que.length()), target.begin());
	que.pop_front(target.size());
}

template<typename T, size_t n>
void operator<<(std::array<T, n>& target, ADS::Bases::FixedQueueBase<T>& que)
{
	std::copy(que.begin(), que.begin() + std::min(n, que.length()), target.begin());
	que.pop_front(n);
}

template<typename T>
std::ostream& operator<<(std::ostream& stream, const ADS::Bases::FixedQueueBase<T>& queue)
{
	for (const T& val : queue)
		stream << val << ' ';

	return stream;
}