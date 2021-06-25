#pragma once

#include <string>

template <typename T, int skipLetters = 1>
std::string GetClassName() {
	std::string type = typeid(T).name();
	return type.substr(type.find_first_of(' ') + skipLetters, type.size());
}

#if defined(NATURAFORGE_COMPILER_GCC) || defined(NATURAFORGE_COMPILER_CLANG)
# define NATURAFORGE_CLASS_NAME(Class) GetClassName<Class, 2>();
#else
# define NATURAFORGE_CLASS_NAME(Class) GetClassName<Class, 1>();
#endif