#include "Camera.h"
void Camera::Yaw(float angleInDegrees) {
	angleInDegrees *= MOUSE_SENSTIVITY;
	this->yaw += DEGREE_TO_RAD(angleInDegrees);
	this->forward.z = std::cos(yaw) * std::cos(pitch);
	this->forward.x = std::sin(yaw) * std::cos(pitch);
	this->forward.y = std::sin(pitch);
	this->forward.Normalize();
	BMath::Vec4 tempUp = { 0,1,0,0 };
	this->right = tempUp.Cross(this->forward);
	this->right.Normalize();
	this->up = this->forward.Cross(this->right);
	this->up.Normalize();
}

void Camera::Pitch(float angleInDegrees) {
	angleInDegrees *= MOUSE_SENSTIVITY * 0.5;
	this->pitch += DEGREE_TO_RAD(angleInDegrees);
	float pitchInDegrees = RAD_TO_DEGREE(pitch);
	if (pitchInDegrees > 89.9)this->pitch = DEGREE_TO_RAD(89.9);
	if (pitchInDegrees < -89.9)this->pitch = DEGREE_TO_RAD(-89.9);
	this->forward.z = std::cos(yaw) * std::cos(pitch);
	this->forward.x = std::sin(yaw) * std::cos(pitch);
	this->forward.y = std::sin(pitch);
	this->forward.Normalize();
	BMath::Vec4 tempUp = { 0,1,0,0 };
	this->right = tempUp.Cross(this->forward);
	this->right.Normalize();
	this->up = this->forward.Cross(this->right);
	this->up.Normalize();
}