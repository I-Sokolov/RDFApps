#include "stdafx.h"
#include "camera.h"

CCamera::CCamera()
{
	x.set(1., 0., 0.);
	y.set(0., 1., 0.);
	z.set(0., 0., 1.);

	eye.set(0., 0., -kCameraDistance);
}

void    CCamera::rotateY(double angle, const CPosition &target)
{
	double  _x, _z,
	        sinAngle, cosAngle;
	
	sinAngle = sin(angle);
	cosAngle = cos(angle);
	
	eye -= target;
	
	_x = eye.x;
	_z = eye.z;
	
	eye.x = (float) (_x * cosAngle + _z * sinAngle);
	eye.z = (float) (_x * -sinAngle + _z * cosAngle);
	
	eye += target;
	
	setTarget(target);
}

void    CCamera::pitch(double angle, const CPosition &target)
{
	double  sinAngle = sin(angle),
	        cosAngle = cos(angle);

	eye -= target;

	double  factor = sqrt(eye.x*eye.x + eye.y*eye.y + eye.z*eye.z);

	double  _x = (x.x * x.x * (1.0f - cosAngle) + cosAngle) * eye.x +
			 	 (x.x * x.y * (1.0f - cosAngle) + x.z * sinAngle) * eye.y +
				 (x.x * x.z * (1.0f - cosAngle) - x.y * sinAngle) * eye.z;

	double  _y = (x.x * x.y * (1.0f - cosAngle) - x.z * sinAngle) * eye.x +
				 (x.y * x.y * (1.0f - cosAngle) + cosAngle) * eye.y +
				 (x.y * x.z * (1.0f - cosAngle) + x.x * sinAngle) * eye.z;

	double  _z = (x.x * x.z * (1.0f - cosAngle) + x.y * sinAngle) * eye.x +
				 (x.y * x.z * (1.0f - cosAngle) - x.x * sinAngle) * eye.y +
				 (x.z * x.z * (1.0f - cosAngle) + cosAngle) * eye.z;

	eye.set(_x, _y, _z);
	eye += target;
	
	CVector	vector = eye - target;
	vector.normalize();
	vector = vector * (factor/kCameraDistance);

	eye = target + (vector * kCameraDistance);
	
	setTarget(target);
}

void    CCamera::setFront(double xpar)
{
	eye.x = (float) (kCameraDistance * xpar);
	eye.y = 0;
	eye.z = 0;
}

void    CCamera::setSide(double xpar)
{
	eye.x = 0;
	eye.y = 0;
	eye.z = (float) (kCameraDistance * xpar);
}

void    CCamera::setTarget(const CPosition &target)
{
	z = target - eye;
	z.normalize();
	
	x = kWorldUp.crossProduct(z);
	x.normalize();
	
	y = z.crossProduct(x);
}

CCamera		theCamera;
CCamera		* gCamera = &theCamera;
