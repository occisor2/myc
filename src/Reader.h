#pragma once

#include <iterator>
#include <vector>

template<class Iterator,
		 class T = typename std::iterator_traits<Iterator>::value_type>
class Reader
{
public:
	Reader(Iterator begin, Iterator end)
		: begin(begin), end(end)
	{}

	T consume()
	{
		auto value = *begin;
		begin++;
		return value;
	}
	const T& peek() const
	{
		return *begin;
	}

	bool isEmpty() const
	{
		return begin >= end;
	}

private:
	Iterator begin;
	Iterator end;
};
