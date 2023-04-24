#pragma once

#include <ostream>
#include <istream>

struct Serializable
{
	virtual void WriteTo(std::ostream& stream) const = 0;
	virtual void ReadFrom(std::istream& stream) = 0;

	friend std::ostream& operator << (std::ostream& stream, const Serializable& serializable);
	friend std::istream& operator >> (std::istream& stream, Serializable& serializable);

	template<typename T>
	T Load(std::ostream& stream)
	{
		T instance{};
		instance << stream;
		return instance;
	}
};