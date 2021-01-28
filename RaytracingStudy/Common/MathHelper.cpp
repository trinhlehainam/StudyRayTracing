#include "MathHelper.h"

#include <cmath>
#include <limits>

namespace MathHelper
{
	const float INFINITY_FLOAT = std::numeric_limits<float>::infinity();
	template<> const float PI<float> = 3.14159265358979323846f;
	template<> const double PI<double> = 3.14159265358979323846;
	template <> const float MAX_VALUE<float> = std::numeric_limits<float>::max();
	template <> const float MIN_VALUE<float> = std::numeric_limits<float>::min();
}

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

	template<typename T>
	T Random()
	{
		return static_cast<T>(rand())/static_cast<T>(RAND_MAX);
	}
	template<typename T>

	T Random(T min, T max)
	{
		return min + (max - min) * Random<T>();
	}

	template<typename T>
	T DegreeToRadian(T degrees)
	{
		return static_cast<T>(degrees * PI<T>/180);
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

	template int Random();
	template float Random();
	template double Random();

	template int Random(int, int);
	template float Random(float, float);
	template double Random(double, double);

	template float DegreeToRadian(float);
	template double DegreeToRadian(double);
}