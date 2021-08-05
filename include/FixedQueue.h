#include <vector>

// class for containing a fixed size queue
// if the size of the queue reaches the fixed size, the last element will be popped
template<typename T>
class FixedQueue
{
	FixedQueue() = default;
	FixedQueue(size_t size);
	
	T& front();
	T front() const;

	void push_front(T elem);
	void push_back(T elem);
	using push = push_back;

	T& back();
	T back();

	void pop_front();
	void pop_back();
	using pop = pop_front;

	T& operator[](size_t index);
	T operator[](size_T index) const;

protected:
	size_t m_size;
	std::vector<T> m_data;
};
