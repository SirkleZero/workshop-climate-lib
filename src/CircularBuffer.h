/*
Inspired by: https://playground.arduino.cc/Main/RunningAverage/
*/

#ifndef CircularBuffer_h
#define CircularBuffer_h

#include <Arduino.h>

template <typename T>
class CircularBuffer
{
public:
	CircularBuffer(int size = 10);
	~CircularBuffer();

	void Add(T value);
	float Average();
	void Clear();
private:
	T* data;
	int arraySize = 0;
	int currentIndex = 0;
	T total = 0;
	int count = 0;
};

template <typename T>
CircularBuffer<T>::CircularBuffer(int size)
{
	this->arraySize = size;
	this->data = (T*)malloc(this->arraySize * sizeof(T));
	if (this->data == NULL)
	{
		this->arraySize = 0;
	}
	this->Clear();
}

template <typename T>
CircularBuffer<T>::~CircularBuffer()
{
	if (this->data != NULL)
	{
		free(this->data);
	}
}

template <typename T>
void CircularBuffer<T>::Add(T value)
{
	if (this->data == NULL)
	{
		return;
	}
	/*
	old way
	this->total = this->total - this->data[this->currentIndex];
	this->data[this->currentIndex] = value;
	this->total = this->total + this->data[this->currentIndex];
	this->currentIndex = this->currentIndex + 1;

	if (this->currentIndex >= arraySize)
	{
		this->currentIndex = 0;
	}
	if (this->count < arraySize)
	{
		this->count = this->count + 1;
	}*/

	// simpler / less verbose
	this->total -= this->data[this->currentIndex];
	this->data[this->currentIndex] = value;
	this->total += this->data[this->currentIndex];
	this->currentIndex++;

	if (this->currentIndex == this->arraySize)
	{
		this->currentIndex = 0;
	}
	if (this->count < this->arraySize)
	{
		this->count++;
	}
}

template <typename T>
float CircularBuffer<T>::Average()
{
	if (this->count == 0)
	{
		return NAN;
	}
	return this->total / this->count;
}

template <typename T>
void CircularBuffer<T>::Clear()
{
	this->count = 0;
	this->currentIndex = 0;
	this->total = 0;
	for (int i = 0; i < this->arraySize; i++)
	{
		this->data[i] = 0;
	}
}

#endif
