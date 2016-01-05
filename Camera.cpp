#include "Camera.h"

Camera::Camera(XMFLOAT4 eye, XMFLOAT4 at, XMFLOAT4 up, FLOAT windowWidth, FLOAT windowHeight, FLOAT nearDepth, FLOAT farDepth)
	: _eye(eye), _at(at), _up(up), _windowWidth(windowWidth), _windowHeight(windowHeight), _nearDepth(nearDepth), _farDepth(farDepth)
{
	_angleAt = 0;
	CalculateViewProjection();
}
Camera::~Camera()
{
}

void Camera::CalculateViewProjection()
{
    // Initialize the view matrix
	XMVECTOR Eye = XMLoadFloat4(&_eye);
	XMVECTOR At = XMLoadFloat4(&_at); 
	XMVECTOR Up = XMLoadFloat4(&_up); 

	XMStoreFloat4x4(&_view, XMMatrixLookAtLH(Eye, At, Up));

    // Initialize the projection matrix
	XMStoreFloat4x4(&_projection, XMMatrixPerspectiveFovLH(XM_PIDIV2, _windowWidth / _windowHeight, _nearDepth, _farDepth));
}
XMFLOAT4X4 Camera::GetViewProjection() const
{
	XMMATRIX view = XMLoadFloat4x4(&_view);
	XMMATRIX projection = XMLoadFloat4x4(&_projection);

	XMFLOAT4X4 viewProj;

	XMStoreFloat4x4(&viewProj, view * projection);

	return viewProj;
}

void Camera::SetAngleAt(float angle)
{
	_angleAt = angle;

	if (_angleAt > 6.28f) 	
		_angleAt = _angleAt - 6.28f;
	else if (_angleAt < 0.0f)
		_angleAt = _angleAt + 6.28f;
}
void Camera::Reshape(FLOAT windowWidth, FLOAT windowHeight, FLOAT nearDepth, FLOAT farDepth)
{
	_windowWidth = windowWidth;
	_windowHeight = windowHeight;
	_nearDepth = nearDepth;
	_farDepth = farDepth;
}
void Camera::Walk(float direction)
{
	this->_eye.z+= direction;
	this->_at.z += direction;
}
void Camera::Strafe(float direction)
{
	this->_eye.x += direction;
	this->_at.x += direction;
}
void Camera::RotateY(float angle)
{
	XMMATRIX rotation = XMMatrixRotationY(angle);

	XMStoreFloat4(&_at, XMVector4Transform(XMLoadFloat4(&_at), rotation));
	XMStoreFloat4(&_up, XMVector4Transform(XMLoadFloat4(&_up), rotation));
	XMStoreFloat4(&_right, XMVector4Transform(XMLoadFloat4(&_right), rotation));
}
void Camera::Pitch(float angle)
{
	XMMATRIX pitch = XMMatrixRotationAxis(XMLoadFloat4(&_right), angle);
	XMStoreFloat4(&_up, XMVector4Transform(XMLoadFloat4(&_up), pitch));
	XMStoreFloat4(&_at, XMVector4Transform(XMLoadFloat4(&_at), pitch));
}