#pragma once

#include <vector>

template<typename T>
class Tree
{
	const T* data = nullptr;
	std::vector<Tree<T>> children;

public:
	Tree<T>& operator[](int index)
	{
		return children[index];
	}

	Tree() = default;
	Tree(std::vector<Tree<T>> c) : children(c) {}
	
	operator std::vector<Tree<T>>& ()
	{
		return children;
	}

	operator std::vector<Tree<T>> const ()
	{
		return children;
	}

	Tree& operator=(const T& other)
	{
		data = &other;
	}
};