#include "stdafx.h"
#include "menuScene.h"

HRESULT menuScene::init(void)
{
	_backBefore = RM_TEXTURE->addResource("Resources/Texture/backBefore.png");
	_backAfter = RM_TEXTURE->addResource("Resources/Texture/backAfter.png");
	_menuBefore = RM_TEXTURE->addResource("Resources/Texture/mainMenuBefore.png");
	_menuAfter = RM_TEXTURE->addResource("Resources/Texture/mainMenuAfter.png");

	_pMenu = { 50, 370, 305, 430 };
	_pBack = { 50, 530 ,305, 570 };
	
	_select = MENUSELECT_NONE;
	_isOn = false;
	return S_OK;
}

void menuScene::release(void)
{
	SAFE_RELEASE(_backBefore);
	SAFE_RELEASE(_backAfter);
	SAFE_RELEASE(_menuBefore);
	SAFE_RELEASE(_menuAfter);
}

void menuScene::update(void)
{
	if (!_isOn) return;
	if (PtInRect(&_pBack, getMousePos()))
	{
		if (_select == MENUSELECT_NONE)
		{
			SOUNDMANAGER->play("manu_overlap", 1.0f);
			_select = MENUSELECT_BACK;
		}
		if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			_isOn = false;
			ShowCursor(FALSE);
		}
	}
	else if (PtInRect(&_pMenu, getMousePos()))
	{
		if (_select == MENUSELECT_NONE)
		{
			SOUNDMANAGER->play("manu_overlap", 1.0f);
			_select = MENUSELECT_MAIN;
		}
		if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			SCENEMANAGER->changeScene("메인화면");
		}
	}
	else
	{
		_select = MENUSELECT_NONE;
		SOUNDMANAGER->stop("manu_overlap");
	}
}

void menuScene::render(void)
{
	if (!_isOn) return;
	RECT rMenu = { 0, 0, 255, 31 };

	switch (_select)
	{
	case MENUSELECT_NONE:
		SPRITEMANAGER->spriteRender(
			_menuBefore, &rMenu, 50, 400, 1, 1, 0);
		SPRITEMANAGER->spriteRender(
			_backBefore, &rMenu, 50, 530, 1, 1, 0);
		break;
	case MENUSELECT_BACK:
		SPRITEMANAGER->spriteRender(
			_menuBefore, &rMenu, 50, 400, 1, 1, 0);
		SPRITEMANAGER->spriteRender(
			_backAfter, &rMenu, 50, 530, 1, 1, 0);
		break;
	case MENUSELECT_MAIN:
		SPRITEMANAGER->spriteRender(
			_menuAfter, &rMenu, 50, 400, 1, 1, 0);
		SPRITEMANAGER->spriteRender(
			_backBefore, &rMenu, 50, 530, 1, 1, 0);
		break;
	}
}
