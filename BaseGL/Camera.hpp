#ifndef _CAMERA_H
#define _CAMERA_H

#define _USE_MATH_DEFINES

#include "Transform.hpp"

class Camera : public Transform {
public:
	float getFov () const;
	void setFoV (float f);
	float getAspectRatio () const;
	void setAspectRatio (float a);
	float getNear () const;
	void setNear (float n);
	float getFar () const;
	void setFar (float n);

	// Returns the view matrix: to express the world w.r.t. the camera,
	// we use the inverse of the camera own transform, to all scene's entities.
	// For now, the camera is fixed at [0, 0, 5] so that we can draw geometry at the origin
	// and actually see it.
	glm::mat4 computeViewMatrix();

	// Returns the projection matrix stemming from the camera parameter. CAREFUL: right now, a translation is added (basic view transform)
	glm::mat4 computeProjectionMatrix() const;

private:
	float m_fov = 45.f; // Field of view, in degrees
	float m_aspectRatio = 1.f; // Ratio between the width and the height of the image
	float m_near = 0.1f; // Distance before which geometry is excluded fromt he rasterization process
	float m_far = 10.f; // Distance after which the geometry is excluded fromt he rasterization process
};

#endif //_CAMERA_H