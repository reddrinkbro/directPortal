#include "stdafx.h"
#include "cube.h"

HRESULT cube::init(float x, float y, float z)
{
	D3DXMATRIXA16  matScale;
	D3DXMatrixScaling(&matScale, 0.03f, 0.03f, 0.03f);

	setMesh(RM_XMESH->addResource("Resources/Model/cube/cube.X", matScale));
	setActive(true);
	_transform->setWorldPosition(x, y, z);
	_exclusivePlayer.setBound(&D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(2.0f, 1.5f, 2.0f));
	_gravity = 0.2f;
	_collisionType = COLLISIONTYPE_NONE;
	_isWallCollide = false;
	_isButtonCollide = false;
	return S_OK;
}

void cube::release(void)
{
}

void cube::update(void)
{
	_transform->movePositionSelf(0, -_gravity, 0);
}

void cube::render(void)
{
	baseObject::render();
}