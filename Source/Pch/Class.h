#pragma once

#include <string>

namespace Class {

template <typename T, int skipLetters = 1>
std::string GetName() {
	std::string type = typeid(T).name();
	return type.substr(type.find_first_of(' ') + skipLetters, type.size());
}

}

#if defined(NATURAFORGE_COMPILER_GCC) || defined(NATURAFORGE_COMPILER_CLANG)
# define NATURAFORGE_CLASS_NAME(T) ::Class::GetName<T, 2>();
#else
# define NATURAFORGE_CLASS_NAME(T) ::Class::GetName<T, 1>();
#endif

#define NATURAFORGE_INHERIT_CONSTRUCTOR(Derived, Parent) \
template<typename... Args> \
Derived(Args&&... args) : Parent(std::forward<Args>(args)...)