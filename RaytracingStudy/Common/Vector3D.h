#pragma once

struct Vector3D
{
	Vector3D();
	Vector3D(float x, float y, float z);
	Vector3D(const Vector3D& other);

	Vector3D& operator += (const Vector3D& other);
	Vector3D& operator -= (const Vector3D& other);
	Vector3D& operator *= (float scalar);
	Vector3D& operator /= (float scalar);

	// make this vector to become unit vector
	Vector3D& Normalized();

	// return unit vector of this vector
	Vector3D Normalize() const;

	float Length() const;

	void Print() const;

	void Pow(float scalar);

	float X, Y, Z;
};

Vector3D operator + (const Vector3D& a, const Vector3D& b);
Vector3D operator + (const Vector3D& a, float scalar);
Vector3D operator - (const Vector3D& a, const Vector3D& b);
Vector3D operator * (const Vector3D& a, float scalar);
Vector3D operator / (const Vector3D& a, float scalar);
Vector3D operator * (float scalar, const Vector3D& a);
Vector3D operator / (float scalar, const Vector3D& a);
Vector3D operator - (const Vector3D& a);

float Dot(const Vector3D& a, const Vector3D& b);
Vector3D Cross(const Vector3D a, const Vector3D& b);
Vector3D Normalize(const Vector3D a);
Vector3D Clamp(Vector3D value, const Vector3D& max, const Vector3D& min);
Vector3D RandomVector(float min, float max);
Vector3D Pow(const Vector3D& a, float scalar);
Vector3D RandomPositionInUnitSphere();
Vector3D RandomUnitVector();

using Position3 = Vector3D;
using Color3 = Vector3D;

