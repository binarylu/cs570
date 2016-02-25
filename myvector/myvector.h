// We have done all the Extra Credit

#ifndef __MYVECTOR_H__
#define __MYVECTOR_H__

#include <new>
#include <limits.h>

template <class T>
class myvector
{
public:
	typedef int size_t;

	myvector();
	myvector(size_t size);
	myvector(const myvector<T> &obj);
	myvector(myvector<T> &&obj);
	myvector(T *arr, size_t len);

	~myvector();


private:
	T *_ptr;
	size_t _size;
	size_t _capacity;

public:
	int size() const; // return the current length of the vector
	int capacity() const; // return the current capacity of the vector
	bool empty() const; // return whether or not the vector is empty
	void resize(int); // resize the vector (change length) to the specified size, changing capacity as needed
	void reserve(int); // change the vector capacity to the specified size, or give an error if capacity would get smaller than length
	void assign(int, T const &); // assign the second argument as the value of the vector at the position of the first argument
	void push_back(T const &); // increase length by 1 and add the argument as the value at the end
	T pop_back(); // decrease length by 1, returning the value removed from the vector
	void insert(int, T const &); // increase length by 1, put the value specified in the second argument at index specified by first argument and shift all values to the right in the vector down by one index
	void erase(int); // decrease length by 1, remove the item at the specified index, and shift all other items left to eliminate the "hole"
	void erase(int, int); // same as erase(int), but removes all elements between the indexes specified by the first and second argument
	void clear(); // remove all elements from the list
	T at(int index) const;
	T &at(int index);
	T operator[](int index) const;
	T &operator[](int index);
	myvector<T> &operator=(const myvector<T> &obj);
	myvector<T> &operator=(myvector<T> &&obj);
};

template <class T>
myvector<T>::myvector() : _ptr(nullptr), _size(0), _capacity(0)
{
}

template <class T>
myvector<T>::myvector(const size_t capacity)
{
	if (capacity > INT_MAX || capacity < 0)
		throw out_of_range("Out of range!");

	_size = 0;
	_capacity = capacity;

	if (_size != 0)
		_ptr = new T[_capacity]();
	else
		_ptr = nullptr;
}

template <class T>
myvector<T>::myvector(const myvector<T> &obj)
{
	_size = obj._size;
	_capacity = _size;
	
	if (0 == _capacity) {
		_ptr = nullptr;
	}
	else {
		_ptr = new T[_capacity]();
		for (int i = 0; i < _size; ++i) {
			_ptr[i] = obj._ptr[i];
		}
	}
}

template <class T>
myvector<T>::myvector(myvector<T> &&obj)
{
	_size = obj._size;
	_capacity = _size;
	_ptr = obj._ptr;

	obj._size = 0;
	obj._capacity = 0;
	obj._ptr = nullptr;
}

template <class T>
myvector<T>::myvector(T *arr, size_t len)
{
	if (len > INT_MAX || len < 0)
		throw out_of_range("Out of range!");

	_size = len;
	_capacity = _size;
	_ptr = new T[_capacity]();
	for (int i = 0; i < _size; ++i) {
		_ptr[i] = arr[i];
	}
}

template <class T>
myvector<T>::~myvector()
{
	delete[] _ptr;
	_ptr = nullptr;
	_size = 0;
	_capacity = 0;
}

template <class T>
myvector<T>& myvector<T>::operator=(const myvector<T> &obj)
{
	_size = obj._size;
	_capacity = _size;
	delete[] _ptr;
	if (0 == _capacity) {
		_ptr = nullptr;
	}
	else {
		_ptr = new T[_capacity]();
		for (int i = 0; i < _size; ++i) {
			_ptr[i] = obj._ptr[i];
		}
	}
}

template <class T>
myvector<T>& myvector<T>::operator=(myvector<T> &&obj)
{
	_size = obj._size;
	_capacity = _size;
	_ptr = obj._ptr;

	obj._size = 0;
	obj._capacity = 0;
	obj._ptr = nullptr;
}

template <class T>
int myvector<T>::size() const
{
	return _size;
}

template <class T>
int myvector<T>::capacity() const
{
	return _capacity;
}

template <class T>
bool myvector<T>::empty() const
{
	return 0 == _size;
}

template <class T>
void myvector<T>::resize(int size)
{
	if (size > INT_MAX || size < 0)
		throw out_of_range("Out of range!");

	if (size > _capacity)
		reserve(size);

	// manually construct and desturct
	if (size > _size) {
		for (int i = _size; i < size; ++i) {
			new(_ptr + i) T();
		}
	}
	else {
		for (int i = size; i < _size; ++i) {
			_ptr[i].~T();
		}
	}

	_size = size;
}

template <class T>
void myvector<T>::reserve(int capacity)
{
	if (capacity < _capacity)
		throw out_of_range("Out of range!");

	_capacity = capacity;
	T *ptr = new T[_capacity]();
	for (int i = 0; i < _size; ++i) {
		ptr[i] = _ptr[i];
	}
	delete[] _ptr;
	_ptr = ptr;
	
	// manually construct and desturct
	for (int i = _size; i < _capacity; ++i) {
		_ptr[i].~T();
	}
}

template <class T>
void myvector<T>::assign(int index, T const &val)
{
	if (index < 0 || index >= _size)
		throw out_of_range("Out of range!");

	_ptr[index] = val;
}

template <class T>
void myvector<T>::push_back(T const &val)
{
	if (_size + 1 > _capacity)
		reserve(_capacity * 2);

	// manually construct
	new(_ptr + _size) T();

	_ptr[_size++] = val;
}

template <class T>
T myvector<T>::pop_back()
{
	T tmp = _ptr[_size - 1];

	// manually desturct
	_ptr[_size - 1].~T();

	--_size;
	return tmp;
}

template <class T>
void myvector<T>::insert(int index, T const &val)
{
	if (index < 0 || index >= _size)
		throw out_of_range("Out of range!");

	if (_size + 1 > _capacity) {
		_capacity *= 2;
		T *ptr = new T[_capacity]();

		int tmp = 0;
		for (int i = 0; i < _size; ++i) {
			if (i == index) {
				tmp = 1;
				ptr[i] = val;
			}
			ptr[i + tmp] = _ptr[i];
		}
		delete[] _ptr;
		_ptr = ptr;
		++_size;

		// manually desturct
		for (int i = _size; i < _capacity; ++i) {
			_ptr[i].~T();
		}
	}
	else {
		new(_ptr + _size) T();
		for (int i = _size - 1; i >= index; --i) {
			_ptr[i + 1] = _ptr[i];
		}
		_ptr[index] = val;
		++_size;
	}
}

template <class T>
void myvector<T>::erase(int index)
{
	if (index < 0 || index >= _size)
		throw out_of_range("Out of range!");

	for (int i = index; i < _size - 1; ++i) {
		_ptr[i] = _ptr[i + 1];
	}

	// manually desturct
	_ptr[_size - 1].~T();

	--_size;
}

template <class T>
void myvector<T>::erase(int start, int end)
{
	if (start < 0 || end >= _size || start > end)
		throw out_of_range("Out of range!");

	for (int i = start; i < (_size - (end - start + 1)); ++i) {
		_ptr[i] = _ptr[i + end - start + 1];
	}

	// manually desturct
	for (int i = (_size - (end - start + 1)); i < _size; ++i) {
		_ptr[i].~T();
	}

	_size -= (end - start + 1);
}

template <class T>
void myvector<T>::clear()
{
	// manually desturct
	for (int i = 0; i < _size; ++i) {
		_ptr[i].~T();
	}

	_size = 0;
}

template <class T>
T myvector<T>::at(int index) const {
	if (index >= _size || index < 0)
		throw out_of_range("Out of range!");
	return _ptr[index];
}

template <class T>
T& myvector<T>::at(int index) {
	if (index >= _size || index < 0)
		throw out_of_range("Out of range!");
	return _ptr[index];
}

template <class T>
T myvector<T>::operator[](int index) const
{
	if (index >= _size || index < 0)
		throw out_of_range("Out of range!");
	return _ptr[index];
}

template <class T>
T& myvector<T>::operator[](int index)
{
	if (index >= _size || index < 0)
		throw out_of_range("Out of range!");
	return _ptr[index];
}

#endif //__MYVECTOR_H__