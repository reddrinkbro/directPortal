#include "stdafx.h"
#include "laserCatcher.h"

HRESULT laserCatcher::init(float x, float y, float z)
{
	_state = CATCHERSTATE_NONE;
	D3DXMATRIXA16 matScale;
	D3DXMatrixScaling(&matScale, 0.03f, 0.03f, 0.03f);
	xMeshSkinned* mesh = RM_SKINNED->addResource("Resources/Model/laser_catcher/laser_catcher.X", matScale);
	_animation = new skinnedAnimation;
	_animation->init(mesh);
	setMesh(mesh);
	setActive(true);
	_transform->setWorldPosition(x, y, z);
	_boundBox.setBound(&D3DXVECTOR3(0, 0, -0.4f), &D3DXVECTOR3(1.0f, 1.0f, 0.05f));
	_hitBox.setBound(&D3DXVECTOR3(0, -0.4f, -0.5f), &D3DXVECTOR3(0.1f, 0.1f, 0.1f));
	return S_OK;
}

void laserCatcher::release(void)
{
	SAFE_DELETE(_animation);
}

void laserCatcher::update(void)
{
	if(_state == CATCHERSTATE_ON) _animation->update();
}

void laserCatcher::render(void)
{
	_animation->render(_transform);
}
