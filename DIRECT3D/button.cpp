#include "stdafx.h"
#include "button.h"
#include "cube.h"

HRESULT button::init(float x, float y, float z)
{
	_state = BUTTON_NONE;
	D3DXMATRIXA16 matScale;
	D3DXMatrixScaling(&matScale, 0.02f, 0.02f, 0.02f);
	xMeshSkinned* mesh = RM_SKINNED->addResource("Resources/Model/button/button.X", matScale);
	_animation = new skinnedAnimation;
	_animation->init(mesh);
	setMesh(mesh);
	setActive(true);
	_transform->setWorldPosition(x, y, z);
	_boundBox.setBound(&D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0.8f, 0.2f, 0.8f));
	return S_OK;
}

void button::release(void)
{
	SAFE_DELETE(_animation);
}

void button::update(void)
{
	if (_state != BUTTON_NONE)
	{
		_animation->update();
		if (_state == BUTTON_UP)
		{
			_animation->PlayOneShotAfterHold("up");
		}
		if (_state == BUTTON_DOWN)
		{
			_animation->PlayOneShotAfterHold("down");
		}
	}
}

void button::render(void)
{
	_animation->render(_transform);
}

void button::cubeCollision(cube * cube)
{
	_transform->releaseParent();
	_state = BUTTON_DOWN;
	cubeMemoryLink(cube);
	_cube->_transform->setWorldPosition(_transform->getWorldPosition().x, _transform->getWorldPosition().y + 0.7f, _transform->getWorldPosition().z);
	_cube->setCollisionType(COLLISIONTYPE_BUTTON);
	_cube->setGravity(0);
}
