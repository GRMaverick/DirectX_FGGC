#include "Aircraft.h"


Aircraft::Aircraft(void)
{
	_pParent = nullptr;
	_thrust = 0.0f;
	_position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	_look = XMFLOAT3(0.0f, 0.0f, 1.0f);
	_up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	_rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);

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
Aircraft::~Aircraft(void)
{
	delete _pParent;
	delete _pParent;

	if (_pTextureRV) _pTextureRV->Release();
}

void Aircraft::Initialise(OBJMesh objMesh, ID3D11ShaderResourceView* shaderResourceView)
{
	_pMesh = objMesh;
	_pTextureRV = shaderResourceView;
}
void Aircraft::Update(float elapsedTime)
{
	Move();
	Lift();
	Bank();
	UpdateWorld();
}
void Aircraft::Draw(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pImmediateContext)
{
	pImmediateContext->PSSetShaderResources(0, 1, &_pTextureRV);
	pImmediateContext->IASetVertexBuffers(0, 1, &_pMesh.VertexBuffer, &_pMesh.VBStride, &_pMesh.VBOffset);
	pImmediateContext->IASetIndexBuffer(_pMesh.IndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	pImmediateContext->DrawIndexed(_pMesh.IndexCount, 0, 0);
}

void Aircraft::SetScale(float x, float y, float z){
	XMStoreFloat4x4(&_scale, XMMatrixScaling(x, y, z));
}
void Aircraft::SetRotation(float x, float y, float z){
	_rotation.x = x;
	_rotation.y = y;
	_rotation.z = z;

	XMStoreFloat4x4(&_rotate, XMMatrixRotationX(_rotation.x) * XMMatrixRotationY(_rotation.y) * XMMatrixRotationZ(_rotation.z));
}
void Aircraft::SetTranslation(float x, float y, float z){
	XMStoreFloat4x4(&_translate, XMMatrixTranslation(x, y, z));
}
void Aircraft::SetPosition(float x, float y, float z)
{
	_position.x = x;
	_position.y = y;
	_position.z = z;
}
void Aircraft::SetWorld(XMMATRIX& transform){
	XMStoreFloat4x4(&_world, transform);
}
void Aircraft::SetThrust(float thrust)
{
	if (thrust >= 1.0f)
		_thrust = 1.0f;
	else
		_thrust = thrust;
}

XMFLOAT4X4 Aircraft::GetWorld(){
	XMFLOAT4X4 world;
	XMMATRIX wMatrix;
	if (NULL == _pParent)
		wMatrix = XMLoadFloat4x4(&_scale) * XMLoadFloat4x4(&_rotate) * XMLoadFloat4x4(&_translate) * XMLoadFloat4x4(&_world);
	else
		wMatrix = XMLoadFloat4x4(&_scale) * XMLoadFloat4x4(&_rotate) * XMLoadFloat4x4(&_translate) * XMLoadFloat4x4(&_world) * XMLoadFloat4x4(&_pParent->GetWorld());

	XMStoreFloat4x4(&world, wMatrix);

	return world;
}

void Aircraft::UpdateWorld(){
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
void Aircraft::Move(){
	_movementVector = XMVectorReplicate(_thrust);
	_lookVector = XMLoadFloat3(&_look);
	_positionVector = XMLoadFloat3(&_position);

	XMStoreFloat3(&_position, XMVectorMultiplyAdd(_movementVector, _lookVector, _positionVector));
}
void Aircraft::Strafe(float movement)
{
	_movementVector = XMVectorReplicate(_thrust);
	_rightVector = XMLoadFloat3(&_right);
	_positionVector = XMLoadFloat3(&_position);

	XMStoreFloat3(&_position, XMVectorMultiplyAdd(_movementVector, _rightVector, _positionVector));
}
void Aircraft::UpMovement(float movement)
{
	_movementVector = XMVectorReplicate(movement);
	_upVector = XMLoadFloat3(&_up);
	_positionVector = XMLoadFloat3(&_position);

	XMStoreFloat3(&_position, XMVectorMultiplyAdd(_movementVector, _upVector, _positionVector));
}
void Aircraft::Rotate(float yaw, float pitch, float roll){
	XMStoreFloat4x4(&_rotate, XMMatrixRotationRollPitchYaw(yaw, pitch, roll));
	if (_rotate._33 > 18.0)
		_rotate._33 = 18.0;
	if (_rotate._33 < -18.0)
		_rotate._33 = -18.0f;
}

void Aircraft::Lift()
{
	float liftSpeed = (-_rotation.x) * _thrust;

	XMVECTOR movement = XMVectorReplicate(liftSpeed);
	XMVECTOR up = XMLoadFloat3(&_up);
	XMVECTOR position = XMLoadFloat3(&_position);

	XMStoreFloat3(&_position, XMVectorMultiplyAdd(movement, up, position));
}
void Aircraft::Bank()
{
	float bankSpeed = _rotation.z * _thrust;

	XMVECTOR movement = XMVectorReplicate(bankSpeed);
	XMVECTOR right = XMLoadFloat3(&_right);
	XMVECTOR position = XMLoadFloat3(&_position);

	XMStoreFloat3(&_position, XMVectorMultiplyAdd(movement, right, position));
}