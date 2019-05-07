#include "stdafx.h"
#include "elevator.h"



HRESULT elevator::init(float x, float y, float z)
{
	_state = ELEVATOR_NONE;
	D3DXMATRIXA16 matScale, matRotate, matCombine;
	D3DXMatrixScaling(&matScale, 0.028f, 0.028f, 0.028f);
	D3DXMatrixRotationY(&matRotate, D3DXToRadian(-90));
	matCombine = matScale * matRotate;
	xMeshSkinned* mesh = RM_SKINNED->addResource("Resources/Model/elevator/elevator.X", matCombine);
	_animation = new skinnedAnimation;
	_animation->init(mesh);
	setMesh(mesh);
	setActive(true);
	_boundBox.setBound(&D3DXVECTOR3(0, 2.5f, 0), &D3DXVECTOR3(1.0f, 0.5f, 3.0f));
	_transform->setWorldPosition(x, y, z);
	_elevatorEvent.setBound(&D3DXVECTOR3(0, 3.5f, -10.0f), &D3DXVECTOR3(0.8f, 0.5f, 0.5f));
	_afterClose.setBound(&D3DXVECTOR3(0, 3.5f, -1.0f), &D3DXVECTOR3(0.8f, 0.5f, 0.5f));
	_soundCount = 0;
	return S_OK;
}

void elevator::release(void)
{
	SAFE_DELETE(_animation);
}

void elevator::update(void)
{
	
	if (_state == ELEVATOR_ARRIVE)
	{
		_animation->update();
		_animation->PlayOneShotAfterHold("arrive");
		_soundCount++;
		if (_soundCount == 130) SOUNDMANAGER->play("elevator_door_open", 0.5f);
	}
	if (_state == ELEVATOR_LEAVE)
	{
		_animation->update();
		_animation->PlayOneShotAfterHold("leave");
	}
}

void elevator::render(void)
{
	if (_state == ELEVATOR_NONE) return;
	_animation->render(_transform);
}
