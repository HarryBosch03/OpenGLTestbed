#pragma once

struct Serializable
{
	const void* Data() const;
	const size_t Size() const;

	virtual const void* Start() const = 0;
	virtual const void* End() const = 0;
};