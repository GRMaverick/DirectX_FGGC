#pragma once
#include "Structs.h"
#include <dinput.h>

class Camera
{
private:
	XMFLOAT4 _eye; 
	XMFLOAT4 _at;
	XMFLOAT4 _up;
	XMFLOAT4 _right;

	FLOAT _windowWidth;
	FLOAT _windowHeight;
	FLOAT _nearDepth;
	FLOAT _farDepth;

	FLOAT _angleAt;

	XMFLOAT4X4 _view;
	XMFLOAT4X4 _projection;

	DIMOUSESTATE _mState;

public:
	Camera(XMFLOAT4 eye, XMFLOAT4 at, XMFLOAT4 up, FLOAT windowWidth, FLOAT windowHeight, FLOAT nearDepth, FLOAT farDepth);
	~Camera();

	void CalculateViewProjection();

	XMFLOAT4X4 GetView() const { return _view; }
	XMFLOAT4X4 GetProjection() const { return _projection; }

	XMFLOAT4X4 GetViewProjection() const;

	XMFLOAT4 GetEye() const { return _eye; }
	XMFLOAT4 GetAt() const { return _at; }
	XMFLOAT4 GetUp() const { return _up; }
	XMFLOAT4 GetRight() const { return _right; }

	void SetEye(XMFLOAT4 eye) { _eye = eye; }
	void SetAt(XMFLOAT4 at) { _at = at; }
	void SetUp(XMFLOAT4 up) { _up = up; }
	void SetRight(XMFLOAT4 right) { _right = right; }

	void SetAngleAt(FLOAT angle);
	FLOAT GetAngleAt() const { return _angleAt;  }

	void SetPriorState(DIMOUSESTATE mState) { _mState = mState; }
	DIMOUSESTATE GetPriorState() const { return _mState; }

	void Reshape(FLOAT windowWidth, FLOAT windowHeight, FLOAT nearDepth, FLOAT farDepth);
	void Walk(float direction);
	void Strafe(float direction);
	void RotateY(float angle);
	void Pitch(float angle);
};

