#pragma once
#include "Structs.h"

class Aircraft
{
protected: 
	float _thrust;
	char* _pName;

	Aircraft* _pParent;
	OBJMesh _pMesh;
	ID3D11ShaderResourceView* _pTextureRV;

	XMFLOAT3 _up;
	XMFLOAT3 _right;
	XMFLOAT3 _look;
	XMFLOAT3 _rotation;
	XMFLOAT3 _position;

	XMFLOAT4X4 _world;
	XMFLOAT4X4 _scale;
	XMFLOAT4X4 _rotate;
	XMFLOAT4X4 _translate;

	XMVECTOR _upVector;
	XMVECTOR _rightVector;
	XMVECTOR _lookVector;
	XMVECTOR _positionVector;
	XMVECTOR _movementVector;

	XMVECTOR _rotationVector;

public:
	Aircraft(void);
	~Aircraft(void);

	void Initialise(OBJMesh OBJMesh, ID3D11ShaderResourceView* shaderResourceView);
	virtual void Update(float elapsedTime);
	void UpdateWorld();
	void Draw(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pImmediateContext);

	void SetAt(XMFLOAT3 at);
	void SetScale(float x, float y, float z);
	void SetScale(XMFLOAT4X4 s);
	void SetRotation(float x, float y, float z);
	void SetTranslation(float x, float y, float z);
	void SetTranslation(XMFLOAT4X4 t);
	void SetPosition(float x, float y, float z);
	void SetPosition(XMFLOAT3 position);
	void SetWorld(XMMATRIX& transform);
	void SetThrust(float thrust);
	
	XMFLOAT4X4 GetTranslation() { return _translate; }
	XMFLOAT4X4 GetWorld();
	XMFLOAT4X4 GetRotation() const {	return _rotate;	}
	XMFLOAT3 GetRotationVar() const { return _rotation; }

	XMFLOAT3 GetPosition() {	return _position; 	}
	XMFLOAT3 GetAt() { return _look; }
	XMFLOAT3 GetUp() { return _up; }
	XMFLOAT3 GetRight() { return _right; }
	float GetThrust() const { return _thrust; }
	int GetIndexCount() const { return _pMesh.IndexCount; }

	void Move();
	void Strafe(float movement);
	void UpMovement(float movement);
	void Rotate(float yaw, float pitch, float roll);
	void Lift();
	void Bank();
};

