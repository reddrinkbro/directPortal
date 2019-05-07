#include "stdafx.h"
#include "switchButton.h"
#include "player.h"

HRESULT switchButton::init(float x, float y, float z)
{
	_state = SWITCH_NONE;
	D3DXMATRIXA16 matScale;
	D3DXMatrixScaling(&matScale, 0.03f, 0.03f, 0.03f);
	xMeshSkinned* mesh = RM_SKINNED->addResource("Resources/Model/switch/switch.X", matScale);
	_animation = new skinnedAnimation;
	_animation->init(mesh);
	setMesh(mesh);
	setActive(true);
	_transform->setWorldPosition(x, y, z);
	_boundBox.setBound(&D3DXVECTOR3(0, 1, 0), &D3DXVECTOR3(0.5f, 1, 0.5f));
	_exclusivePlayer.setBound(&D3DXVECTOR3(0, 1, 0), &D3DXVECTOR3(0.95f, 1, 0.95f));
	return S_OK;
}

void switchButton::release(void)
{
	SAFE_DELETE(_animation);
}

void switchButton::update(void)
{

	if (_state != SWITCH_NONE)
	{
		_animation->update();
		if (_state == SWITCH_ON)
		{
			_animation->PlayOneShotAfterHold("click");
		}
	}
}

void switchButton::render(void)
{
	_animation->render(_transform);
}