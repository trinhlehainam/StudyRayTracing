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
}


