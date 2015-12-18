#pragma once
#include "Structs.h"

class GameObject
{
protected: 
	OBJMesh _pMesh;
	ID3D11ShaderResourceView* _pTextureRV;
	GameObject* _pParent;
	char* _pName;

	XMFLOAT3 _position;
	XMFLOAT3 _up;
	XMFLOAT3 _right;
	XMFLOAT3 _look;

	XMFLOAT4X4 _world;
	XMFLOAT4X4 _scale;
	XMFLOAT4X4 _rotate;
	XMFLOAT4X4 _translate;

public:
	GameObject(void);
	~GameObject(void);

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
	void SetParent(GameObject* gameObject);
	void SetPosition(float x, float y, float z);
	void SetPosition(XMFLOAT3 position);
	void SetWorld(XMMATRIX& transform);

	GameObject* GetParent() {	return _pParent; }

	XMFLOAT4X4 GetTranslation() { return _translate; }
	XMFLOAT4X4 GetWorld();

	XMFLOAT3 GetPosition() {	return _position; 	}
	XMFLOAT3 GetAt() { return _look; }
	XMFLOAT3 GetUp() { return _up; }
	XMFLOAT3 GetRight() { return _right; }

	void Move(float movement);
	void RotateX(float rotation);
	void RotateY(float rotation);
};

