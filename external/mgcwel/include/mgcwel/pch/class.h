#pragma once

#include <string>

namespace mw {
namespace Class {

template <typename T, int skipLetters = 1>
std::string GetName()
{
	std::string type = typeid(T).name();
	return type.substr(type.find_first_of(' ') + skipLetters, type.size());
}

}
}

#if defined(MW_COMPILER_GCC) || defined(MW_COMPILER_CLANG)
#	define MW_CLASS_NAME(T) ::mw::Class::GetName<T, 2>();
#else
#	define MW_CLASS_NAME(T) ::mw::Class::GetName<T, 1>();
#endif

#define MW_INHERIT_CONSTRUCTOR(Derived, Parent) \
	template <typename... Args>                 \
	Derived(Args &&...args) : Parent(std::forward<Args>(args)...)