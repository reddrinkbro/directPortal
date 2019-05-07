#include "stdafx.h"
#include "tutorialScene_2.h"


HRESULT tutorialScene_2::init(void)
{
	this->save();
	ShowCursor(FALSE);
	_beforeScene = RM_TEXTURE->addResource("Resources/Texture/loadingscreenAfter.png");
	_afterScene = RM_TEXTURE->addResource("Resources/Texture/loadingscreenBefore.png");

	_endRotateAngle = 0;
	_endCount = 0;
	_startCount = 0;
	_sceneStart = false;
	_sceneEnd = false;
	//기본 값
	//기본 광원 생성
	_sceneBaseDirectionLight->_color = D3DXCOLOR(1, 1, 1, 1);
	_sceneBaseDirectionLight->_intensity = 2.0f;
	//맵초기화
	_map = new tutorialMap2;
	_map->init();
	_player = new player;
	_player->init(-18, 0.7f, 1.5f);
	_player->_transform->addChild(_mainCamera);
	_player->setIsEquip(true);
	_mainCamera->setLocalPosition(0, 2, 0);
	_mainCamera->setIsMouse(true);
	for (int i = 0; i < 2; i++)
	{
		_elevator[i] = new elevator;
		_cube[i] = new cube;
		_button[i] = new button;
	}

	_cube[0]->init(1.8f, 3.9f, 7.5f);
	_cube[1]->init(1.8f, 3.9f, -4.5f);
	_button[0]->init(-8.2f, -3.05f, 7.5f);
	_button[1]->init(-8.2f, -3.05f, -4.5);

	//입구
	_elevator[0]->init(-18, -2, 1.5f);
	_elevator[0]->_transform->lookDirection(D3DXVECTOR3(-1, 0, 0));
	_elevator[0]->setState(ELEVATOR_WAIT);
	//출구
	_elevator[1]->init(21, -2, 1.5f);
	_elevator[1]->_transform->lookDirection(D3DXVECTOR3(1, 0, 0));

	_door = new door;
	_door->init(12.7f, 0.1f, 1.4f);
	_door->_transform->lookDirection(D3DXVECTOR3(1, 0, 0));
	for (int i = 0; i < 2; i++)
	{
		_gun[i] = new portalGun;
	}
	//플레이어 포탈건
	_gun[0]->init(_player->_transform->getWorldPosition().x, _player->_transform->getWorldPosition().y + 2.0f, _player->_transform->getWorldPosition().z - 0.3f);
	_player->_transform->addChild(_gun[0]->_transform);
	_player->setPortalGunMemoryLink(_gun[0]);
	_player->getCrosshair()->setState(CROSSHAIRSTATE_ONLYBLUE);
	//땅에 있는 포탈건
	_gun[1]->init(-2, 3.8f, -4.5f);

	_bPortal = new bluePortal;
	_bPortal->init();
	_bPortal->_transform->setWorldPosition(0, 0, -8);
	_oPortal = new orangePortal;
	_oPortal->init();
	_oPortal->_transform->setWorldPosition(-2, 4.8f, -6.35f);
	_oPortal->_transform->lookDirection(D3DXVECTOR3(0, 0, -1));
	_oPortal->setIsSet(true);

	_isGetOrangePortalGun = false;

	return S_OK;
}

void tutorialScene_2::release(void)
{
	_map->release();
	SAFE_DELETE(_map);
	_player->release();
	SAFE_DELETE(_player);
	for (int i = 0; i < 2; i++)
	{
		_cube[i]->release();
		SAFE_DELETE(_cube[i]);
		_button[i]->release();
		SAFE_DELETE(_button[i]);
	}
	for (int i = 0; i < 2; i++)
	{
		_elevator[i]->release();
		SAFE_DELETE(_elevator[i]);
		_gun[i]->release();
		SAFE_DELETE(_gun[i]);
	}
	_door->release();
	SAFE_DELETE(_door);
	_bPortal->release();
	SAFE_DELETE(_bPortal);
	_oPortal->release();
	SAFE_DELETE(_oPortal);
}

void tutorialScene_2::update(void)
{
	if (_sceneStart)
	{
		_mainCamera->defaultControl(!_player->getMenuScene()->getIsOn());
		_mainCamera->setIsMouse(!_player->getMenuScene()->getIsOn());
		
		_player->update(D3DXToRadian(_mainCamera->getNowAngleV()), D3DXToRadian(_mainCamera->getNowAngleH()));
		_gun[0]->update();
		_gun[0]->_transform->setRotateWorld(D3DXToRadian(_mainCamera->getNowAngleV()), D3DXToRadian(_mainCamera->getNowAngleH()), 0);
		
		for (int i = 0; i < 2; i++)
		{
			_elevator[i]->update();
			_cube[i]->update();
			_button[i]->update();
		}
		_door->update();
		this->tutorial2Collision();
		this->nextScene();
	}
	else
	{
		_startCount++;
		if (_startCount % 200 == 0)
		{
			_sceneStart = true;
		}
	}
}

void tutorialScene_2::render(void)
{
	if (_sceneStart)
	{
		_bPortal->render();
		_oPortal->render();
		xMeshStatic::setCamera(_mainCamera);
		_map->render();
		for (int i = 0; i < 2; i++)
		{
			_cube[i]->render();
		}
		//skinned render
		xMeshSkinned::setTechniqueName("Toon");
		xMeshSkinned::setBaseLight(_sceneBaseDirectionLight);
		xMeshSkinned::setCamera(_mainCamera);
		for (int i = 0; i < 2; i++)
		{
			_elevator[i]->render();
			_button[i]->render();
		}
		_gun[0]->render();
		if (!_isGetOrangePortalGun) _gun[1]->render();
		_door->render();
		_player->render();
	}
	else if (_sceneEnd)
	{
		RECT rChange = { -100, 0, WINSIZEX, WINSIZEY + 100 };
		SPRITEMANAGER->spriteRender(
			_afterScene, &rChange, 0, -100, 1, 1, 0);
		_endCount++;
		if (_endCount % 60 == 39)
		{
			SOUNDMANAGER->stop("elevator_door_close");
			SCENEMANAGER->changeScene("스테이지1");
		}
	}
	else
	{
		RECT rChange = { -100, 0, WINSIZEX, WINSIZEY + 100 };
		SPRITEMANAGER->spriteRender(
			_beforeScene, &rChange, 0, -100, 1, 1, 0);
	}
}


void tutorialScene_2::nextScene(void)
{
	if (PHYSICSMANAGER->isOverlap(_player, _elevator[1]))
	{
		SOUNDMANAGER->stop("door_close");
		SOUNDMANAGER->stop("gun_take");
		if (_elevator[1]->_transform->getWorldPosition().x < _player->_transform->getWorldPosition().x)
		{
			if (_elevator[1]->getState() != ELEVATOR_LEAVE) SOUNDMANAGER->play("elevator_door_close", 0.5f);
			_elevator[1]->setState(ELEVATOR_LEAVE);
		}
		if (_elevator[1]->getState() == ELEVATOR_LEAVE)
		{
			if (PHYSICSMANAGER->isOverlap(_elevator[1]->_transform, &_elevator[1]->_boundBox, _player->_transform, &_player->_boundBox))
			{
				PHYSICSMANAGER->isBlocking(_elevator[1]->_transform, &_elevator[1]->getCloseBox(), _player->_transform, &_player->_boundBox);
			}
			_endCount++;
			if (_endCount % 60 == 0)
			{
				_sceneEnd = true;
				_sceneStart = false;
			}
		}
	}
}

void tutorialScene_2::save(void)
{
	FILE* file;
	char name[] = "튜토리얼2";
	file = fopen("save/save.txt", "w");
	fprintf(file, "%s", &name);
	fclose(file);
}
