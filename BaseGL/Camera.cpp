#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Camera.hpp"

float Camera::getFov() const {
	return m_fov;
}

float Camera::getAspectRatio() const {
	return m_aspectRatio;
}

float Camera::getNear() const {
	return m_near;
}

float Camera::getFar() const {
	return m_far;
}

void Camera::setFoV(float f) {
	m_fov = f;
}

void Camera::setAspectRatio(float a) {
	m_aspectRatio = a;
}

void Camera::setNear(float n) {
	m_near = n;
}

void Camera::setFar(float n) {
	m_far = n;
}

glm::mat4 Camera::computeViewMatrix () {
	return computeTransformationMatrix();
}

glm::mat4 Camera::computeProjectionMatrix () const {
	return glm::perspective (glm::radians (m_fov), m_aspectRatio, m_near, m_far);
}