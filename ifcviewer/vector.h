#ifndef VEC_H
#define VEC_H

#include <math.h>

class CVector
{
	public:
		float  x;
		float  y;
		float  z;

		CVector() : x(0.0f), y(0.0f), z(0.0f) {}
		CVector(double _x, double _y, double _z) : x((float) _x), y((float) _y), z((float) _z) {}
		
		CVector(const CVector &vector) : x(vector.x), y(vector.y), z(vector.z) {}

		CVector& operator =(const CVector &vector);
		CVector operator +(const CVector &vector) const;
		CVector operator -(const CVector &vector) const;
		CVector operator -() const;
		void operator +=(const CVector &vector);
		void operator -=(const CVector &vector);
		bool operator ==(const CVector &vector) const;
		bool operator !=(const CVector &vector) const;

		double operator *(const CVector &vector) const;

		void set(double _x, double _y, double _z);
		void set(const CVector &vector);

		void negate();
		void normalize();
		void scale(double scale);
		
		double length() const;

		void crossProduct(const CVector &vector, CVector &result) const;
		CVector crossProduct(const CVector &vector) const;
		
		friend inline CVector operator *(const CVector &vector, double scalar);
};

CVector operator*(const CVector &vector, double scalar)
{
	return  CVector(vector.x * scalar, vector.y * scalar, vector.z * scalar);
}

typedef CVector CPosition;

#endif