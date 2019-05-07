#include "stdafx.h"
#include "crosshair.h"

HRESULT crosshair::init(void)
{
	_blueCrosshair = RM_TEXTURE->addResource("Resources/Texture/blueCrosshair.png");
	_orangeCrosshair = RM_TEXTURE->addResource("Resources/Texture/orangeCrosshair.png");
	_centerCrosshair = RM_TEXTURE->addResource("Resources/Texture/centerCrosshair.png");

	_state = CROSSHAIRSTATE_NONE;
	return S_OK;
}

void crosshair::release(void)
{
	SAFE_RELEASE(_blueCrosshair);
	SAFE_RELEASE(_orangeCrosshair);
	SAFE_RELEASE(_centerCrosshair);
}

void crosshair::render(void)
{
	if (_state == CROSSHAIRSTATE_NONE)
	{
		RECT rCenter = { 0, 0, 16, 16 };
		SPRITEMANAGER->spriteRender(
			_centerCrosshair, &rCenter, WINSIZEX / 2 , WINSIZEY / 2, 1, 1, 0);
	}
	else if (_state == CROSSHAIRSTATE_ONLYBLUE)
	{
		RECT rCenter = { 0, 0, 16, 16 };
		RECT rBlue = { 0, 0, 64, 64 };
		SPRITEMANAGER->spriteRender(
			_centerCrosshair, &rCenter, WINSIZEX / 2, WINSIZEY / 2, 1, 1, 0);
		SPRITEMANAGER->spriteRender(
			_blueCrosshair, &rBlue, WINSIZEX / 2 - 35, WINSIZEY / 2 - 35, 1, 1, 0);
	}
	else
	{
		RECT rCenter = { 0, 0, 16, 16 };
		RECT rBlue = { 0, 0, 64, 64 };
		RECT rOrange = { 0, 0, 64, 64 };
		SPRITEMANAGER->spriteRender(
			_centerCrosshair, &rCenter, WINSIZEX / 2, WINSIZEY / 2, 1, 1, 0);
		SPRITEMANAGER->spriteRender(
			_blueCrosshair, &rBlue, WINSIZEX / 2 - 35, WINSIZEY / 2 - 35, 1, 1, 0);
		SPRITEMANAGER->spriteRender(
			_orangeCrosshair, &rOrange, WINSIZEX / 2 - 15, WINSIZEY / 2 - 15, 1, 1, 0);
	}
}
