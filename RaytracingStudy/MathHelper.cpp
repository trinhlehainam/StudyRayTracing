#include "MathHelper.h"

namespace MathHelper
{
	template<typename T>
	T Max(T a, T b)
	{
		return a > b ? a : b;
	}

	template<typename T>
	T Min(T a, T b)
	{
		return a < b ? a : b;
	}

	template<typename T>
	T Clamp(T value, T min, T max)
	{
		return Min(max, Max(value, min));
	}
}


namespace MathHelper
{
	template int Max(int, int);
	template float Max(float, float);
	template double Max(double, double);
	
	template int Max(int, int);
	template float Min(float, float);
	template double Min(double, double);
	
	template int Clamp(int, int, int);
	template float Clamp(float, float, float);
	template double Clamp(double, double, double);
}