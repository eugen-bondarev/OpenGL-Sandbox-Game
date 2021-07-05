#pragma once

template <typename T = int>
struct Period 
{
	T start;
	T end;

	inline bool operator<(Period<T> const& other) const 
	{
		return start < other.start || (start == other.start && end < other.end);
	}

	inline bool operator==(Period<T> const& other) const 
	{
		return start == other.start && end == other.end;
	}
};