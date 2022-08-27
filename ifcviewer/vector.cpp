#include "stdafx.h"
#include "float.h"
#include "vector.h"

template <typename T>
inline  bool IsNear(T a, T b, T eps = FLT_EPSILON)
{
	return a < b + eps && b < a + eps;
}

CVector&	CVector::operator =(const CVector &vector)
{
	x = vector.x;
	y = vector.y;
	z = vector.z;
	return  *this;
}

CVector		CVector::operator +(const CVector &vector) const
{
	return	CVector(x + vector.x, y + vector.y, z + vector.z);
}

CVector		CVector::operator -(const CVector &vector) const
{
	return	CVector(x - vector.x, y - vector.y, z - vector.z);
}

CVector		CVector::operator -() const
{
	return	CVector(-x, -y, -z);
}

void	CVector::operator +=(const CVector &vector)
{
	x += vector.x;
	y += vector.y;
	z += vector.z;
}

void	CVector::operator -=(const CVector &vector)
{
	x -= vector.x;
	y -= vector.y;
	z -= vector.z;
}

bool	CVector::operator ==(const CVector &vector) const
{
	return (IsNear(x, vector.x)  &&
			IsNear(y, vector.y)  &&
			IsNear(z, vector.z));
}

bool	CVector::operator !=(const CVector &vector) const
{
	return  !(*this == vector);
}

double	CVector::operator *(const CVector &vector) const
{
	return  (x * vector.x + y * vector.y + z * vector.z);
}

void	CVector::set(double _x, double _y, double _z)
{
	x = (float) _x;
	y = (float) _y;
	z = (float) _z;
}

void	CVector::set(const CVector &vector)
{
	x = vector.x;
	y = vector.y;
	z = vector.z;
}

void	CVector::negate()
{
	x = -x;
	y = -y;
	z = -z;
}

void	CVector::normalize()
{
	double distance = length();
    if  (distance) {
	    distance = 1.0f / distance;
    }
	x *= (float) distance;
	y *= (float) distance;
	z *= (float) distance;
}

void    CVector::scale(double scale)
{
	x *= (float) scale;
	y *= (float) scale;
	z *= (float) scale;
}

double	CVector::length() const
{
	return  sqrt((x * x) + (y * y) + (z * z));
}

void	CVector::crossProduct(const CVector &vector, CVector &result) const
{
	result.x = (y * vector.z) - (vector.y * z);
	result.y = (z * vector.x) - (vector.z * x);
	result.z = (x * vector.y) - (vector.x * y);
}

CVector		CVector::crossProduct(const CVector &vector) const
{
	return  CVector((y * vector.z) - (vector.y * z), 
					(z * vector.x) - (vector.z * x),
					(x * vector.y) - (vector.x * y));
}
