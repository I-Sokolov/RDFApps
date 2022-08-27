#ifndef CAM_H
#define CAM_H

#include "vector.h"

const	double		kCameraDistance = 2.5f;
const	CVector		kWorldUp(0,1,0);
	
class	CCamera
{
	public:
		CCamera();
		
		void		rotateY(double angle, const CPosition &target);
		void		pitch(double angle, const CPosition &target);
						
		void		setEye(const CPosition &eyePos) { eye = eyePos; }
		void		setTarget(const CPosition &target);
		
		void		setFront(double x);
		void		setSide(double x);

		CPosition	getTarget() const { return eye + (z * kCameraDistance); } 
		
		CPosition	getEye() const { return eye; }
		CVector		getCameraZ() const { return z; }
		CVector		getCameraY() const { return y; }
		CVector		getCameraX() const { return x; }
		
	private:	
		CVector		x, y, z;
		CPosition	eye;
};

extern	CCamera	* gCamera;

#endif