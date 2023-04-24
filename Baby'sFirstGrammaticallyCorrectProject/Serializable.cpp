#include "Serializable.h"

typedef unsigned char byte;

const void* Serializable::Data() const
{
	return (void*)((size_t)this + (size_t)Start());
}

const size_t Serializable::Size() const
{
	return (size_t)End() - (size_t)Start();
}
