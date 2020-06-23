#include "Camera.h"
void Camera::Yaw(float angleInDegrees) {
	this->yaw += angleInDegrees * 0.01;
	this->forward.z = std::cos(yaw);
	this->forward.x = std::sin(yaw);
	this->forward.y = std::sin(pitch);
	this->forward.Normalize();
}

void Camera::Pitch(float angleInDegrees) {
	this->pitch += angleInDegrees * 0.01;
	float pitchInDegrees = RAD_TO_DEGREE(pitch);
	if (pitchInDegrees > 89.9)this->pitch = DEGREE_TO_RAD(89.9);
	if (pitchInDegrees < -89.9)this->pitch = DEGREE_TO_RAD(-89.9);
	this->forward.z = std::cos(yaw);
	this->forward.x = std::sin(yaw);
	this->forward.y = std::sin(pitch);
	this->forward.Normalize();
}