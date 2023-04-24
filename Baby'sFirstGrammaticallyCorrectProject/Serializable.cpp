#include "Serializable.h"

typedef unsigned char byte;

std::ostream& operator<<(std::ostream& stream, const Serializable& serializable)
{
	serializable.WriteTo(stream);
	return stream;
}

std::istream& operator>>(std::istream& stream, Serializable& serializable)
{
	serializable.ReadFrom(stream);
	return stream;
}
