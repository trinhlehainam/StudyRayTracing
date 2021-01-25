#include "Vector3D.h"

#include <cmath>
#include <iostream>

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

void Vector3D::Print() const
{
	std::cout << "(" << X << ", " << Y << ", " << Z << ")" << "\n";
}

Vector3D Vector3D::Normalize() const
{
	return *this / Length();
}

Vector3D operator+(const Vector3D& a, const Vector3D& b)
{
	return Vector3D(a.X + b.X, a.Y + b.Y, a.Z + b.Z);
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
	return Vector3D(a.X/scalar, a.Y / scalar, a.Z / scalar);
}

Vector3D operator*(float scalar, const Vector3D& a)
{
	return Vector3D(a.X * scalar, a.Y * scalar, a.Z * scalar);
}

Vector3D operator/(float scalar, const Vector3D& a)
{
	return Vector3D(a.X / scalar, a.Y / scalar, a.Z / scalar);
}

float Dot(const Vector3D& a, const Vector3D& b)
{
	return a.X*b.X + a.Y * b.Y + a.Z * b.Z;
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
