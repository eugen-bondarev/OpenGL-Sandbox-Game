#pragma once

#include <vector>

namespace Vector {

template <typename T>
inline static void Join(const std::vector<T> &a, const std::vector<T> &b, std::vector<T> &result)
{
	for (int i = 0; i < a.size(); i++)
	{
		result.push_back(a[i]);
	}

	for (int i = 0; i < b.size(); i++)
	{
		result.push_back(b[i]);
	}
}

template <typename T>
inline static bool Contains(const std::vector<T> &a, const T &value)
{
	bool found{false};

	for (const T &el : a)
	{
		if (el == value)
		{
			found = true;
			break;
		}
	}

	return found;
}

}
