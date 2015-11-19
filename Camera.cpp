#include "Camera.h"

// To-do List
// ********************
/// Fill in Camera Manipulation Logic

Camera::Camera(XMFLOAT3 position, XMFLOAT3 at, XMFLOAT3 up, FLOAT wWidth, FLOAT wHeight, FLOAT nearDepth, FLOAT farDepth) 
	: _pPosition(position), _pLookAt(at), _pUp(up), _pWindowWidth(wWidth), _pWindowHeight(wHeight), _pNearDepth(nearDepth), _pFarDepth(farDepth)
{
	// Calculate the lookAt vector by the difference between points "at" and position
	XMFLOAT3 Look = XMFLOAT3(at.x - position.x, at.y - position.y, at.z - position.z);
	XMVECTOR L = XMLoadFloat3(&Look);
	L = XMVector3Normalize(L);
	XMStoreFloat3(&_pLookAt, L);

	// Calculate the Right vector 
	XMVECTOR Up = XMLoadFloat3(&_pUp);
	XMVECTOR Right = XMVector3Cross(Up, L);
	Right = XMVector3Normalize(Right);
	XMStoreFloat3(&_pRight, Right);

	// reCalculate the Up vector
	Up = XMVector3Cross(L, Right);
	Up = XMVector3Normalize(Up);
	XMStoreFloat3(&_pUp, Up);

	XMStoreFloat4x4(&_pProjectionMatrix, XMMatrixPerspectiveFovLH(XM_PIDIV2, _pWindowWidth / _pWindowHeight, _pNearDepth, _pFarDepth));
}
Camera::~Camera()
{
}

void Camera::CalculateViewProjection()
{
	XMVECTOR Look = XMLoadFloat3(&_pLookAt);
	XMVECTOR Up = XMLoadFloat3(&_pUp);
	XMVECTOR Right = XMLoadFloat3(&_pRight);
	XMVECTOR Position = XMLoadFloat3(&_pPosition); // need for dot calculations later

	Look = XMVector3Normalize(Look);
	XMStoreFloat3(&_pLookAt, Look);

	// Calculate the Up vector

	Up = XMVector3Cross(Look, Right);
	Up = XMVector3Normalize(Up);
	XMStoreFloat3(&_pUp, Up);

	// Calculate the Right vector 
	Right = XMVector3Cross(Up, Look);
	XMStoreFloat3(&_pRight, Right);

	// Insert Matrix Components into Matrix
	_pViewMatrix(0, 0) = _pRight.x;
	_pViewMatrix(1, 0) = _pRight.y;
	_pViewMatrix(2, 0) = _pRight.z;
	_pViewMatrix(3, 0) = -XMVectorGetX(XMVector3Dot(Position, Right));

	_pViewMatrix(0, 1) = _pUp.x;
	_pViewMatrix(1, 1) = _pUp.y;
	_pViewMatrix(2, 1) = _pUp.z;
	_pViewMatrix(3, 1) = -XMVectorGetX(XMVector3Dot(Position, Up));

	_pViewMatrix(0, 2) = _pLookAt.x;
	_pViewMatrix(1, 2) = _pLookAt.y;
	_pViewMatrix(2, 2) = _pLookAt.z;
	_pViewMatrix(3, 2) = -XMVectorGetX(XMVector3Dot(Position, Look));

	_pViewMatrix(0, 3) = 0.0f;
	_pViewMatrix(1, 3) = 0.0f;
	_pViewMatrix(2, 3) = 0.0f;
	_pViewMatrix(3, 3) = 1.0f;
}

// Getter Accessors
XMFLOAT4X4 Camera::GetViewMatrix() const
{
	return _pViewMatrix;
}
XMFLOAT4X4 Camera::GetProjectionMatrix() const
{
	return _pProjectionMatrix;
}
XMFLOAT4X4 Camera::GetViewProjection() const
{
	XMFLOAT4X4 output;
	XMMATRIX view = XMLoadFloat4x4(&_pViewMatrix);
	XMMATRIX projection = XMLoadFloat4x4(&_pProjectionMatrix);
	XMStoreFloat4x4(&output, view * projection);
	return output;
}
XMVECTOR Camera::GetPositionVector() const
{
	XMVECTOR output = XMVectorSet(_pPosition.x, _pPosition.y, _pPosition.z, 0.0f);
	return output;
}
XMFLOAT3 Camera::GetPosition() const
{
	return _pPosition;
}
XMFLOAT3 Camera::GetLookAt() const
{
	return _pLookAt;
}
XMFLOAT3 Camera::GetUp() const
{
	return _pUp;
}
XMFLOAT3 Camera::GetRight() const
{
	return _pRight;
}


// Setter Accessors
void Camera::SetPosition(XMFLOAT3 position)
{
	_pPosition = position;
}
void Camera::SetLookAt(XMFLOAT3 at)
{
	_pLookAt = at;
}
void Camera::SetUp(XMFLOAT3 up)
{
	_pUp = up;
}
void Camera::SetRight(XMFLOAT3 right)
{
	_pRight = right;
}

// Camera Operations
void Camera::Reshape(FLOAT windowWidth, FLOAT windowHeight, FLOAT nearDepth, FLOAT farDepth)
{
	_pWindowWidth = windowWidth;
	_pWindowHeight = windowHeight;
	_pNearDepth = nearDepth;
	_pFarDepth = farDepth;

	XMStoreFloat4x4(&_pProjectionMatrix, 
		XMMatrixPerspectiveFovLH(XM_PIDIV2, 
		_pWindowWidth / _pWindowHeight, 
		_pNearDepth, 
		_pFarDepth));
}
void Camera::Pitch(float pitch)
{
	XMMATRIX rotate = XMMatrixRotationAxis(XMLoadFloat3(&_pUp), pitch);

	XMStoreFloat3(&_pUp, XMVector3TransformNormal(XMLoadFloat3(&_pUp), rotate));
	XMStoreFloat3(&_pUp, XMVector3TransformNormal(XMLoadFloat3(&_pLookAt), rotate));
}
void Camera::RotateY(float rotation)
{
	XMMATRIX rotate = XMMatrixRotationY(rotation);

	XMStoreFloat3(&_pUp, XMVector3TransformNormal(XMLoadFloat3(&_pUp), rotate));
	XMStoreFloat3(&_pRight, XMVector3TransformNormal(XMLoadFloat3(&_pRight), rotate));
	XMStoreFloat3(&_pLookAt, XMVector3TransformNormal(XMLoadFloat3(&_pLookAt), rotate));
}
void Camera::Move(float direction)
{
	// Direction vector moves along the XMVECTOR Look (Forward and Back)
	XMVECTOR moveVec = XMVectorReplicate(direction);
	XMVECTOR look = XMLoadFloat3(&_pLookAt);
	XMVECTOR position = XMLoadFloat3(&_pPosition);

	XMStoreFloat3(&_pPosition, XMVectorMultiplyAdd(moveVec, look, position));
}
void Camera::Strafe(float direction)
{
	// Direction Vector moves along the XMVECTOR Right (Left and Right);
	XMVECTOR moveVec = XMVectorReplicate(direction);
	XMVECTOR right = XMLoadFloat3(&_pRight);
	XMVECTOR position = XMLoadFloat3(&_pPosition);

	XMStoreFloat3(&_pPosition, XMVectorMultiplyAdd(moveVec, right, position));
}
void Camera::Reset()
{
	XMFLOAT3 look = _pLookAt;
	XMVECTOR Look = XMLoadFloat3(&look);
	Look = XMVector3Normalize(Look);

	// Calculate the Right Vector
	XMVECTOR Up = XMLoadFloat3(&_pUp);
	XMVECTOR Right = XMVector3Cross(Up, Look);
	Right = XMVector3Normalize(Right);
	XMStoreFloat3(&_pRight, Right);

	// Recalculate the Up Vector
	Up = XMVector3Cross(Look, Right);
	Up = XMVector3Normalize(Up);
	XMStoreFloat3(&_pUp, Up);

	XMStoreFloat4x4(&_pProjectionMatrix, 
		XMMatrixPerspectiveFovLH(XM_PIDIV2,
		_pWindowWidth / _pWindowHeight, 
		_pNearDepth,
		_pFarDepth));
}