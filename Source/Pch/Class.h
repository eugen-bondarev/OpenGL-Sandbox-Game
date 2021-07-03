#pragma once

#include <string>

namespace Class {

template <typename T, int skipLetters = 1>
std::string GetName() {
	std::string type = typeid(T).name();
	return type.substr(type.find_first_of(' ') + skipLetters, type.size());
}

}

#if defined(NF_COMPILER_GCC) || defined(NF_COMPILER_CLANG)
# define NF_CLASS_NAME(T) ::Class::GetName<T, 2>();
#else
# define NF_CLASS_NAME(T) ::Class::GetName<T, 1>();
#endif

#define NF_INHERIT_CONSTRUCTOR(Derived, Parent) \
template<typename... Args> \
Derived(Args&&... args) : Parent(std::forward<Args>(args)...)