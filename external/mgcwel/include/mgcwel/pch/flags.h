#pragma once

#define MW_ENUM_USE_FOR_FLAGS(T)\
inline T operator~ (T a) 			{ return static_cast<T>(~static_cast<int>(a)); }\
inline T operator| (T a, T b) 		{ return static_cast<T>(static_cast<int>(a) | static_cast<int>(b)); }\
inline T operator& (T a, T b) 		{ return static_cast<T>(static_cast<int>(a) & static_cast<int>(b)); }\
inline T operator^ (T a, T b) 		{ return static_cast<T>(static_cast<int>(a) ^ static_cast<int>(b)); }\
inline T& operator|= (T& a, T b) 	{ return reinterpret_cast<T&>(reinterpret_cast<int&>(a) |= static_cast<int>(b)); }\
inline T& operator&= (T& a, T b) 	{ return reinterpret_cast<T&>(reinterpret_cast<int&>(a) &= static_cast<int>(b)); }\
inline T& operator^= (T& a, T b) 	{ return reinterpret_cast<T&>(reinterpret_cast<int&>(a) ^= static_cast<int>(b)); }