#include "Vector3D.h"

#include <cmath>

#include "MathHelper.h"

Vector3D::Vector3D():X(0.0f),Y(0.0f),Z(0.0f)
{
}

Vector3D::Vector3D(float x, float y, float z):X(x),Y(y),Z(z)
{
}

Vector3D::Vector3D(const Vector3D& other):X(other.X),Y(other.Y),Z(other.Z)
{
}

Vector3D& Vector3D::operator=(const Vector3D& other)
{
	X = other.X;
	Y = other.Y;
	Z = other.Z;

	return *this;
}

Vector3D& Vector3D::operator +=(const Vector3D& other)
{
	X += other.X;
	Y += other.Y;
	Z += other.Z;

	return *this;
}

Vector3D& Vector3D::operator-=(const Vector3D& other)
{
	X -= other.X;
	Y -= other.Y;
	Z -= other.Z;

	return *this;
}

Vector3D& Vector3D::operator*=(float scalar)
{
	X *= scalar;
	Y *= scalar;
	Z *= scalar;

	return *this;
}

Vector3D& Vector3D::operator/=(float scalar)
{
	X /= scalar;
	Y /= scalar;
	Z /= scalar;

	return *this;
}

Vector3D& Vector3D::Normalized()
{
	return *this /= Length();
}

float Vector3D::Length() const
{
	return std::sqrtf(X * X + Y * Y + Z * Z);
}

void Vector3D::Pow(float scalar)
{
	X = powf(X, scalar);
	Y = powf(Y, scalar);
	Z = powf(Z, scalar);
}

bool Vector3D::IsNearZero() const
{
	return fabsf(X) < MathHelper::MIN_VALUE<float> &&
		fabsf(Y) < MathHelper::MIN_VALUE<float> &&
		fabsf(Z) < MathHelper::MIN_VALUE<float>;
}

Vector3D Vector3D::Normalize() const
{
	return *this / Length();
}

Vector3D operator+(const Vector3D& a, const Vector3D& b)
{
	return Vector3D(a.X + b.X, a.Y + b.Y, a.Z + b.Z);
}

Vector3D operator+(const Vector3D& a, float scalar)
{
	return a + Vector3D(scalar, scalar, scalar);
}

Vector3D operator-(const Vector3D& a, const Vector3D& b)
{
	return Vector3D(a.X - b.X, a.Y - b.Y, a.Z - b.Z);
}

Vector3D operator*(const Vector3D& a, float scalar)
{
	return Vector3D(a.X * scalar, a.Y * scalar, a.Z * scalar);
}

Vector3D operator/(const Vector3D& a, float scalar)
{
	return a * (1.0f / scalar);
}

Vector3D operator*(float scalar, const Vector3D& a)
{
	return a * scalar;
}

Vector3D operator/(float scalar, const Vector3D& a)
{
	return a / scalar;
}

Vector3D operator*(const Vector3D& a, const Vector3D& b)
{
	return Vector3D(a.X * b.X, a.Y * b.Y, a.Z * b.Z);
}

Vector3D operator-(const Vector3D& a)
{
	return -1.0f*a;
}

float Length(const Vector3D& a)
{
	return a.Length();
}

float Dot(const Vector3D& a, const Vector3D& b)
{
	return a.X * b.X + a.Y * b.Y + a.Z * b.Z;
}

Vector3D Cross(const Vector3D a, const Vector3D& b)
{
	return Vector3D(a.Y*b.Z - b.Y*a.Z, a.Z * b.X - b.Z * a.X, a.X * b.Y - b.X * a.Y);
}

Vector3D Normalize(const Vector3D a)
{
	return a / a.Length();
}

Vector3D Clamp(Vector3D value, const Vector3D& max, const Vector3D& min)
{
	return Vector3D(MathHelper::Clamp(value.X,max.X,min.X),
		MathHelper::Clamp(value.Y, max.Y, min.Y),
		MathHelper::Clamp(value.Z, max.X, min.Z));
}

Vector3D RandomVector(float min, float max)
{
	return Vector3D(MathHelper::Random(min,max),
		MathHelper::Random(min, max),
		MathHelper::Random(min, max));
}

Vector3D Pow(const Vector3D& a, float scalar)
{
	return Vector3D(powf(a.X,scalar), powf(a.Y, scalar), powf(a.Z, scalar));
}

Vector3D RandomPositionInUnitSphere()
{
	Vector3D v;
	do
		v = RandomVector(-1.0f, 1.0f);
	while (Dot(v, v) >= 1);

	return v;
}

Vector3D RandomUnitVector()
{
	return Normalize(RandomVector(-1,1));
}

Vector3D RandomPositionInUnitCircle()
{
	Vector3D p(MathHelper::Random<float>(-1.0, 1.0f), MathHelper::Random<float>(-1.0, 1.0f), 0.0f);
	while (Dot(p, p) >= 1.0f)
	{
		p.X = MathHelper::Random<float>(-1.0, 1.0f);
		p.Y = MathHelper::Random<float>(-1.0, 1.0f);
	}
	return p;
}

Vector3D RandomPositionInHemisphere(const Vector3D& normal)
{
	Vector3D pos = RandomVector(-1.0f, 1.0f);
	return Dot(pos,normal) > 0 ? pos : -pos;
}

Vector3D ReflectedVector(const Vector3D& i, const Vector3D& n)
{
	return i - 2.0f * Dot(i, n) * n;
}

Vector3D RefractedVector(const Vector3D& i, const Vector3D& n, float refractionRatio)
{
	float cosTheta = Dot(-i, n);
	Vector3D perpendicularVector = refractionRatio * (i + cosTheta * n);
	Vector3D parallelVector = -std::sqrtf(1.0f - Dot(perpendicularVector, perpendicularVector)) * n;
	return perpendicularVector + parallelVector;
}
