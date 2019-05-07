#include "stdafx.h"
#include "panelMover.h"


HRESULT panelMover::init(float x, float y, float z)
{
	D3DXMATRIXA16  matScale;
	D3DXMatrixScaling(&matScale, 0.02f, 0.02f, 0.02f);

	setMesh(RM_XMESH->addResource("Resources/Model/panel_mover/panel_mover.X", matScale));
	setActive(true);
	_transform->setWorldPosition(x, y, z);
	_transform->setRotateWorld(D3DXToRadian(270), D3DXToRadian(90), D3DXToRadian(180));
	_exclusivePlayer.setBound(&D3DXVECTOR3(0, 1.4f, -3.4f), &D3DXVECTOR3(1.2f, 0.8f, 0.4f));
	_lerpPos = 0;
	_isStart = true;
	_stopCount = 0;
	_state = MOVERSTATE_NONE;
	return S_OK;
}

void panelMover::release(void)
{
}

void panelMover::update(transform& start, transform& end, int time)
{
	if (_state == MOVERSTATE_STOP)
	{
		_stopCount++;
		if (_stopCount % time == 0)
		{
			_state = MOVERSTATE_MOVE;
			SOUNDMANAGER->resume("panelmover_moving");
			_stopCount = 0;
		}
	}
	else if (_state == MOVERSTATE_MOVE)
	{
		if (_isStart)
		{
			_lerpPos += 0.005f;
			if (_lerpPos > 1)
			{
				_isStart = false;
				_state = MOVERSTATE_STOP;
				SOUNDMANAGER->pause("panelmover_moving");
			}
		}
		else
		{
			_lerpPos -= 0.005f;
			if (_lerpPos < 0)
			{
				_isStart = true;
				_state = MOVERSTATE_STOP;
				SOUNDMANAGER->pause("panelmover_moving");
			}
		}
		_transform->positionLerp(start, end, _lerpPos);
	}
}

void panelMover::render(void)
{
	baseObject::render();
}