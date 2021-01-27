#pragma once

namespace MathHelper
{
	template <typename T>
	T Max(T a, T b);

	template <typename T>
	T Min(T a, T b);

	template <typename T>
	T Clamp(T value, T min, T max);

	template <typename T>
	T Random();

	template <typename T>
	T Random(T min, T max);

	extern const float INFINITY_FLOAT;

	template <typename T>
	extern const T MAX_VALUE;

	template <typename T>
	extern const T MIN_VALUE;
}



