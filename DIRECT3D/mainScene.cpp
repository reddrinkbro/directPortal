#include "stdafx.h"
#include "mainScene.h"


HRESULT mainScene::init(void)
{
	_background = RM_TEXTURE->addResource("Resources/Texture/background.png");
	_startBefore = RM_TEXTURE->addResource("Resources/Texture/gamestartBefore.png");
	_startAfter = RM_TEXTURE->addResource("Resources/Texture/gamestartAfter.png");
	_continueBefore = RM_TEXTURE->addResource("Resources/Texture/continueBefore.png");
	_continueAfter = RM_TEXTURE->addResource("Resources/Texture/continueAfter.png");
	_exitBefore = RM_TEXTURE->addResource("Resources/Texture/exitBefore.png");
	_exitAfter = RM_TEXTURE->addResource("Resources/Texture/exitAfter.png");
	_changeScene = RM_TEXTURE->addResource("Resources/Texture/loadingscreenBefore.png");
	_pStart = { 50, 450, 305, 481 };
	_pContinue = { 50,530 ,305, 581 };
	_pExit = { 50,610, 305, 641 };

	_select = MAINSELECT_NONE;
	_isChange = false;
	_changeCount = 0;
	SOUNDMANAGER->play("manu_backgroundMusic", 0.8f);

	sprintf(_nextSceneName, "%s", "Æ©Åä¸®¾ó1");
	return S_OK;
}

void mainScene::release(void)
{
}

void mainScene::update(void)
{
	if (_isChange) _changeCount++;
	if (PtInRect(&_pStart, getMousePos()))
	{
		if (_select == MAINSELECT_NONE)
		{
			SOUNDMANAGER->play("manu_overlap", 1.0f);
			_select = MAINSELECT_START;
		}
		if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			_isChange = true;
		}
	}
	else if (PtInRect(&_pContinue, getMousePos()))
	{
		if (_select == MAINSELECT_NONE)
		{
			SOUNDMANAGER->play("manu_overlap", 1.0f);
			_select = MAINSELECT_OPTION;
		}
		if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			_isChange = true;
			FILE* file;
			file = fopen("save/save.txt", "r");
			fscanf(file, "%s", &_nextSceneName);
			fclose(file);
		}
	}
	else if (PtInRect(&_pExit, getMousePos()))
	{
		if (_select == MAINSELECT_NONE)
		{
			SOUNDMANAGER->play("manu_overlap", 1.0f);
			_select = MAINSELECT_EXIT;
		}
		if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			PostQuitMessage(0);
		}
	}
	else
	{
		_select = MAINSELECT_NONE;
		SOUNDMANAGER->stop("manu_overlap");
	}
}

void mainScene::render(void)
{
	RECT rBackground = { 0, 0, WINSIZEX, WINSIZEY };
	RECT rMenu = { 0, 0, 255, 31 };
	if (!_isChange)
	{
		SPRITEMANAGER->spriteRender(
			_background, &rBackground, 0, 0, 1.2f, 1, 0);
		switch (_select)
		{
		case MAINSELECT_NONE:
			SPRITEMANAGER->spriteRender(
				_startBefore, &rMenu, 50, 450, 1, 1, 0);
			SPRITEMANAGER->spriteRender(
				_continueBefore, &rMenu, 50, 530, 1, 1, 0);
			SPRITEMANAGER->spriteRender(
				_exitBefore, &rMenu, 50, 610, 1, 1, 0);
			break;
		case MAINSELECT_START:
			SPRITEMANAGER->spriteRender(
				_startAfter, &rMenu, 50, 450, 1, 1, 0);
			SPRITEMANAGER->spriteRender(
				_continueBefore, &rMenu, 50, 530, 1, 1, 0);
			SPRITEMANAGER->spriteRender(
				_exitBefore, &rMenu, 50, 610, 1, 1, 0);
			break;
		case MAINSELECT_OPTION:
			SPRITEMANAGER->spriteRender(
				_startBefore, &rMenu, 50, 450, 1, 1, 0);
			SPRITEMANAGER->spriteRender(
				_continueAfter, &rMenu, 50, 530, 1, 1, 0);
			SPRITEMANAGER->spriteRender(
				_exitBefore, &rMenu, 50, 610, 1, 1, 0);
			break;
		case MAINSELECT_EXIT:
			SPRITEMANAGER->spriteRender(
				_startBefore, &rMenu, 50, 450, 1, 1, 0);
			SPRITEMANAGER->spriteRender(
				_continueBefore, &rMenu, 50, 530, 1, 1, 0);
			SPRITEMANAGER->spriteRender(
				_exitAfter, &rMenu, 50, 610, 1, 1, 0);
			break;
		}
	}
	else
	{
		RECT rChange = { -100, 0, WINSIZEX, WINSIZEY + 100 };
		SPRITEMANAGER->spriteRender(
			_changeScene, &rChange, 0, -100, 1, 1, 0);
		SOUNDMANAGER->stop("manu_backgroundMusic");
		if (_changeCount % 10 == 8) SCENEMANAGER->changeScene(_nextSceneName);
	}
}
