#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

using namespace DirectX;

class Camera
{
private:
	XMFLOAT3 _pPosition;
	XMFLOAT3 _pLookAt;
	XMFLOAT3 _pRight;
	XMFLOAT3 _pUp;

	FLOAT _pWindowHeight;
	FLOAT _pWindowWidth;
	FLOAT _pNearDepth;
	FLOAT _pFarDepth;

	XMFLOAT4X4 _pViewMatrix;
	XMFLOAT4X4 _pProjectionMatrix;

public:
	Camera(XMFLOAT3 position, XMFLOAT3 at, XMFLOAT3 up, FLOAT wWidth, FLOAT wHeight, FLOAT nDepth, FLOAT fDepth);
	~Camera();

	void CalculateViewProjection();
	
	//Getter Accessors
	XMVECTOR GetPositionVector() const;
	XMFLOAT4X4 GetViewMatrix() const;
	XMFLOAT4X4 GetProjectionMatrix() const;
	XMFLOAT4X4 GetViewProjection() const;

	XMFLOAT3 GetPosition() const;
	XMFLOAT3 GetLookAt() const;
	XMFLOAT3 GetUp() const;
	XMFLOAT3 GetRight() const;

	// Setter Accessors
	void SetPosition(XMFLOAT3 position);
	void SetLookAt(XMFLOAT3 at);
	void SetUp(XMFLOAT3 up);
	void SetRight(XMFLOAT3 right);

	// Camera Operations
	void Reshape(FLOAT windowWidth, FLOAT windowHeight, FLOAT nearDepth, FLOAT farDepth);
	void Pitch(float pitch);
	void RotateY(float rotation);
	void Move(float direction);
	void Strafe(float direction);
	void Reset();	
};

