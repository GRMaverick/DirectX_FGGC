#include "GameObject.h"


GameObject::GameObject(void)
{
	_pParent = nullptr;
	_position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	_look = XMFLOAT3(0.0f, 0.0f, 1.0f);
	_up = XMFLOAT3(0.0f, 1.0f, 0.0f);

	XMVECTOR look = XMLoadFloat3(&_look);
	XMVECTOR up = XMLoadFloat3(&_up);
	XMVECTOR right = XMVector3Cross(look, up);

	right = XMVector3Normalize(right);
	XMStoreFloat3(&_right, right);

	XMStoreFloat4x4(&_world, XMMatrixIdentity());
	XMStoreFloat4x4(&_scale, XMMatrixIdentity());
	XMStoreFloat4x4(&_rotate, XMMatrixIdentity());
	XMStoreFloat4x4(&_translate, XMMatrixIdentity());
}
GameObject::~GameObject(void)
{
	delete _pParent;
	delete _pParent;

	if (_pVertexShader) _pVertexShader->Release();
}

void GameObject::Initialise(MeshData meshData, ID3D11VertexShader* vertexShader)
{
	_pMesh = meshData;
	_pVertexShader = vertexShader;
}
void GameObject::Update(float elapsedTime)
{

}
void GameObject::Draw(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pImmediateContext)
{
	pImmediateContext->VSSetShader(_pVertexShader, nullptr, 0);
	pImmediateContext->IASetVertexBuffers(0, 1, &_pMesh.VertexBuffer, &_pMesh.uIStride, &_pMesh.uIOffset);
	pImmediateContext->IASetIndexBuffer(_pMesh.IndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	pImmediateContext->DrawIndexed(_pMesh.IndexCount, 0, 0);
}

void GameObject::SetScale(float x, float y, float z){
	XMStoreFloat4x4(&_scale, XMMatrixScaling(x, y, z));
}
void GameObject::SetRotation(float x, float y, float z){
	XMStoreFloat4x4(&_rotate, XMMatrixRotationX(x) * XMMatrixRotationY(y) * XMMatrixRotationZ(z));
}
void GameObject::SetTranslation(float x, float y, float z){
	XMStoreFloat4x4(&_translate, XMMatrixTranslation(x, y, z));
}
void GameObject::SetWorld(XMMATRIX& transform){
	XMStoreFloat4x4(&_world, transform);
}

XMFLOAT4X4 GameObject::GetWorld(){
	XMFLOAT4X4 world;
	XMMATRIX wMatrix;
	if (NULL == _pParent)
		wMatrix = XMLoadFloat4x4(&_scale) * XMLoadFloat4x4(&_rotate) * XMLoadFloat4x4(&_translate) * XMLoadFloat4x4(&_world);
	else
		wMatrix = XMLoadFloat4x4(&_scale) * XMLoadFloat4x4(&_rotate) * XMLoadFloat4x4(&_translate) * XMLoadFloat4x4(&_world) * XMLoadFloat4x4(&_pParent->GetWorld());

	XMStoreFloat4x4(&world, wMatrix);

	return world;
}

void GameObject::UpdateWorld(){
	// Calculate the LookAt vector by the difference between points "at" and postion
	XMFLOAT3 look;
	XMVECTOR L = XMLoadFloat3(&_look);
	L = XMVector3Normalize(L);

	// Calculate the Right Vector
	XMFLOAT3 Up = { 0.0f, 1.0f, 0.0f };
	XMVECTOR U = XMLoadFloat3(&Up);

	XMFLOAT3 Right = { 0.0f, 0.0f, 0.0f };
	XMVECTOR R = XMVector3Cross(L, U);
	R = XMVector3Normalize(R);

	// Needed for DOT-PRODUCT Calculations Later
	XMVECTOR Position = XMLoadFloat3(&_position);

	XMStoreFloat3(&_look, L);
	XMStoreFloat3(&_right, R);
	XMStoreFloat3(&_up, U);

	_world(0, 0) = _right.x;
	_world(1, 0) = _right.y;
	_world(2, 0) = _right.z;
	_world(3, 0) = _position.x;

	_world(0, 1) = _up.x;
	_world(1, 1) = _up.y;
	_world(2, 1) = _up.z;
	_world(3, 1) = _position.y;

	_world(0, 2) = _look.x;
	_world(1, 2) = _look.y;
	_world(2, 2) = _look.z;
	_world(3, 2) = _position.z;

	_world(0, 3) = 0.0f;
	_world(1, 3) = 0.0f;
	_world(2, 3) = 0.0f;
	_world(3, 3) = 1.0f;
}
void GameObject::Move(float movement){
	XMVECTOR moveVec = XMVectorReplicate(movement);
	XMVECTOR look = XMLoadFloat3(&_look);
	XMVECTOR position = XMLoadFloat3(&_position);

	XMStoreFloat3(&_position, XMVectorMultiplyAdd(moveVec, look, position));
}
void GameObject::RotateX(float rotation){
	XMMATRIX rotate = XMMatrixRotationAxis(XMLoadFloat3(&_right), rotation);

	XMStoreFloat3(&_up, XMVector3TransformNormal(XMLoadFloat3(&_up), rotate));
	XMStoreFloat3(&_look, XMVector3TransformNormal(XMLoadFloat3(&_look), rotate));
	XMStoreFloat3(&_right, XMVector3TransformNormal(XMLoadFloat3(&_right), rotate));
}
void GameObject::RotateY(float rotation){
	XMMATRIX rotate = XMMatrixRotationY(rotation);

	XMStoreFloat3(&_up, XMVector3TransformNormal(XMLoadFloat3(&_up), rotate));
	XMStoreFloat3(&_right, XMVector3TransformNormal(XMLoadFloat3(&_right), rotate));
	XMStoreFloat3(&_look, XMVector3TransformNormal(XMLoadFloat3(&_look), rotate));
}
