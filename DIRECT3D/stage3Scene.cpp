#include "stdafx.h"
#include "stage3Scene.h"

HRESULT stage3Scene::init(void)
{
	this->save();
	ShowCursor(FALSE);
	_beforeScene = RM_TEXTURE->addResource("Resources/Texture/loadingscreenAfter.png");
	_endRotateAngle = 0.0f;
	_endCount = 0;
	_startCount = 0;
	_sceneStart = false;
	_sceneEnd = false;
	//기본 값
	//기본 광원 생성
	_sceneBaseDirectionLight->_color = D3DXCOLOR(1, 1, 1, 1);
	_sceneBaseDirectionLight->_intensity = 2.0f;
	_mainCamera->setIsMouse(true);
	//맵초기화
	_map = new stage3;
	_map->init();
	//플레이어 초기화
	_player = new player;
	_player->init(-0.8f, 0.4f, 19.0f);
	_player->_transform->addChild(_mainCamera);
	_player->setIsEquip(true);
	_mainCamera->setLocalPosition(0, 2, 0);
	//큐브 초기화
	_cube = new cube;
	_cube->init(5.6f, 3.5f, 10.5f);
	//버튼 초기화
	_button = new button;
	_button->init(1, 3.2f, -9);
	//엘리베이터 ,문 초기화
	for (int i = 0; i < 2; i++)
	{
		_elevator[i] = new elevator;
		_door[i] = new door;
	}
	_elevator[0]->init(-0.8f, -2.1f, 19.0f);
	_elevator[0]->setState(ELEVATOR_WAIT);
	_elevator[1]->init(-9.8f, 0.9f, -5.8f);
	_elevator[1]->_transform->setRotateWorld(0, D3DXToRadian(270), 0);

	_door[0]->init(4.3f, 0, -2.2f);
	_door[0]->_transform->setScale(1.1f, 1.1f, 1.1f);
	_door[0]->_transform->setRotateWorld(0, D3DXToRadian(90), 0);

	_door[1]->init(-2.3f, 3.1f, -5.8f);
	_door[1]->_transform->setScale(1.1f, 1.1f, 1.1f);
	_door[1]->_transform->setRotateWorld(0, D3DXToRadian(270), 0);

	//포탈건 초기화
	_gun = new portalGun;
	_gun->init(_player->_transform->getWorldPosition().x, _player->_transform->getWorldPosition().y + 2.0f, _player->_transform->getWorldPosition().z - 0.3f);
	_player->_transform->addChild(_gun->_transform);
	_player->setPortalGunMemoryLink(_gun);
	_player->getCrosshair()->setState(CROSSHAIRSTATE_ALL);

	D3DXVECTOR3 length = _map->getBoundBox()[15].trans.getWorldPosition() - _map->getBoundBox()[13].trans.getWorldPosition();
	float laserLength = D3DXVec3Length(&length);
	D3DXVec3Normalize(&length, &length);
	for (int i = 0; i < 2; i++)
	{
		_laser[i] = new laserPoint;
	}
	_laser[0]->init(_map->getBoundBox()[13].trans.getWorldPosition().x + (length.x * laserLength) / 2, 0.8f, 3.8f);
	_laser[0]->_transform->lookDirection(D3DXVECTOR3(0, 1, 0), D3DXVECTOR3(0, 0, 1));
	_laser[0]->_transform->rotateWorld(0, D3DXToRadian(90), 0);
	_laser[0]->_transform->setScale(0.04f, (length.x * laserLength) / 2 - 1, 0.04f);
	_laser[0]->setIsSet(true);
	_laser[1]->init(120, 1, 1);
	_laser[1]->_transform->lookDirection(D3DXVECTOR3(0, 1, 0), D3DXVECTOR3(0, 0, 1));
	_laser[1]->_transform->setScale(0.04f, 0.1f, 0.04f);
	_laser[1]->setIsSet(false);
	//포탈 초기화
	_bPortal = new bluePortal;
	_bPortal->init();
	_bPortal->_transform->setWorldPosition(-299.6, 1.2f, 3.8f);
	_oPortal = new orangePortal;
	_oPortal->init();
	_oPortal->_transform->setWorldPosition(-100, 0, 0);

	for (int i = 0; i < 2; i++)
	{
		_panelMover[i] = new panelMover;
	}
	_panelMover[0]->init(19.8f, 5.8f, 6.6f);
	_panelMover[0]->_transform->rotateSelf(0, 0, D3DXToRadian(180));
	_panelMover[1]->init(15.9f, 5.8f, -6.9f);
	_panelMover[1]->_transform->rotateSelf(0, 0, D3DXToRadian(270));

	_emitter = new laserEmitter;
	_emitter->init(4, 1.2f, 3.8f);
	_emitter->_transform->setRotateWorld(0, D3DXToRadian(270), 0);

	_catcher = new laserCatcher;
	_catcher->init(11, 1.7f, 8.85f);
	_laserCollideCount = 0;
	_laserScale = 1;
	return S_OK;
}

void stage3Scene::release(void)
{
	_map->release();
	SAFE_DELETE(_map);
	_player->release();
	SAFE_DELETE(_player);
	_gun->release();
	SAFE_DELETE(_gun);
	for (int i = 0; i < 2; i++)
	{
		_elevator[i];
		SAFE_DELETE(_elevator[i]);
		_door[i];
		SAFE_DELETE(_door[i]);
	}
	_cube->release();
	SAFE_DELETE(_cube);
	_button->release();
	SAFE_DELETE(_button);
	_bPortal->release();
	SAFE_DELETE(_bPortal);
	_oPortal->release();
	SAFE_DELETE(_oPortal);
	for (int i = 0; i < 2; i++)
	{
		_panelMover[i]->release();
		SAFE_DELETE(_panelMover[i]);
		_laser[i]->release();
		SAFE_DELETE(_laser[i]);
	}
	_emitter->release();
	SAFE_DELETE(_emitter);
	_catcher->release();
	SAFE_DELETE(_catcher);

}

void stage3Scene::update(void)
{
	if (_sceneStart)
	{
		_mainCamera->defaultControl(!_player->getMenuScene()->getIsOn());
		_mainCamera->setIsMouse(!_player->getMenuScene()->getIsOn());
		_map->update();
		_gun->update();
		_gun->_transform->setRotateWorld(D3DXToRadian(_mainCamera->getNowAngleV()), D3DXToRadian(_mainCamera->getNowAngleH()), 0);
		_cube->update();
		_button->update();
		for (int i = 0; i < 2; i++)
		{
			_elevator[i]->update();
			_door[i]->update();
		}
		_bPortal->update();
		_oPortal->update();
		_catcher->update();
		if (_catcher->getState() == CATCHERSTATE_ON) _catcher->update();
		if (_player->getMenuScene()->getIsOn())
		{
			for (int i = 0; i < 2; i++)
			{
				_panelMover[i]->setState(MOVERSTATE_NONE);
			}
			SOUNDMANAGER->pause("panelmover_moving");
		}
		this->stage3Collision();
		_player->update(D3DXToRadian(_mainCamera->getNowAngleV()), D3DXToRadian(_mainCamera->getNowAngleH()));
		this->panelStateChange();
		this->laserSetting();
		if (_laser[1]->getIsSet())
		{
			this->laserSetting(_laser[1]->getLaserBoxDir());
		}
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
	if (KEYMANAGER->isOnceKeyDown(VK_F1))
	{
		_mainCamera->releaseParent();
	}
}

void stage3Scene::render(void)
{
	if (_sceneStart)
	{
		xMeshStatic::setCamera(_mainCamera);
		_map->render();
		_cube->render();
		for (int i = 0; i < 2; i++)
		{
			_panelMover[i]->render();
		}
		_emitter->render();
		xMeshSkinned::setTechniqueName("Toon");
		xMeshSkinned::setBaseLight(_sceneBaseDirectionLight);
		xMeshSkinned::setCamera(_mainCamera);
		_gun->render();
		_button->render();
		_catcher->render();
		for (int i = 0; i < 2; i++)
		{
			_elevator[i]->render();
			_door[i]->render();
		}
		_bPortal->render();
		_oPortal->render();
		for (int i = 0; i < 2; i++)
		{
			_laser[i]->render();
		}
		_player->render();
	}

	else
	{
		RECT rChange = { -100, 0, WINSIZEX, WINSIZEY + 100 };
		SPRITEMANAGER->spriteRender(
			_beforeScene, &rChange, 0, -100, 1, 1, 0);
	}
}

void stage3Scene::panelStateChange(void)
{
	//1번 발판 상태변화
	if (_panelMover[0]->getState() != MOVERSTATE_NONE)
	{
		transform start;
		start.setWorldPosition(19.8f, 5.8f, 6.6f);
		transform end;
		boundBox endbox;
		end.setWorldPosition(19.8f, 5.8f, -5); //36.843
		endbox.setBound(&D3DXVECTOR3(19.8f, 5.8f, -5), &D3DXVECTOR3(0.1f, 0.1f, 0.1f));
		if (!PHYSICSMANAGER->isOverlap(_panelMover[0]->_transform, &_panelMover[0]->_boundBox, &end, &endbox))
		{
			_panelMover[0]->update(start, end, 100);
		}

	}
	//2번 발판 상태변화
	if (_panelMover[1]->getState() != MOVERSTATE_NONE)
	{
		transform start;
		start.setWorldPosition(15.9f, 5.8f, -6.9f);
		transform end;
		end.setWorldPosition(5.9f, 5.8f, -6.9f);
		_panelMover[1]->update(start, end, 100);
	}
}

void stage3Scene::laserSetting(BOUNDBOXDIR dir)
{
	//레이저 생성기에서 나오는 레이저와 파랑포탈 충돌
	if (PHYSICSMANAGER->isOverlap(_laser[0]->_transform, &_laser[0]->_boundBox, _bPortal->_transform, &_bPortal->_boundBox))
	{
		for (int i = 0; i < _map->getBoundBox().size(); i++)
		{
			//다른 레이저가 맵이랑 충돌할때
			if (PHYSICSMANAGER->isOverlap(_laser[1]->_transform, &_laser[1]->_boundBox, &_map->getBoundBox()[i].trans, &_map->getBoundBox()[i].box))
			{
				_laserCollideCount++;
			}
		}
		//두번 부딪치면
		if (_laserCollideCount < 2)
		{
			_laserScale += 0.5f;
			_laser[1]->setLaser(_oPortal->_transform->getWorldPosition(), dir, _laserScale);
			_laser[1]->setIsSet(true);
		}
	}
	//레이저 생성기에서 나오는 레이저와 주황포탈 충돌
	else if (PHYSICSMANAGER->isOverlap(_laser[0]->_transform, &_laser[0]->_boundBox, _oPortal->_transform, &_oPortal->_boundBox))
	{
		for (int i = 0; i < _map->getBoundBox().size(); i++)
		{
			//다른 레이저가 맵이랑 충돌할때
			if (PHYSICSMANAGER->isOverlap(_laser[1]->_transform, &_laser[1]->_boundBox, &_map->getBoundBox()[i].trans, &_map->getBoundBox()[i].box))
			{
				_laserCollideCount++;
			}
		}
		//두번 부딪치면
		if (_laserCollideCount < 2)
		{
			_laserScale += 0.5f;
			_laser[1]->setLaser(_bPortal->_transform->getWorldPosition(), dir, _laserScale);
			_laser[1]->setIsSet(true);
		}
		cout << _laserCollideCount << endl;
	}
	//둘다 충돌 아닐때
	else
	{
		_laser[1]->setIsSet(false);
	}
}

void stage3Scene::laserSetting()
{
	//둘다 설치 중일때
	if (_bPortal->getIsSet() && _oPortal->getIsSet())
	{
		//파랑 포탈이랑 레이저가 충돌 중일때
		if (PHYSICSMANAGER->isOverlap(_laser[0]->_transform, &_laser[0]->_boundBox, _bPortal->_transform, &_bPortal->_boundBox))
		{
			_laser[1]->setIsSet(true);
		}
		//주황 포탈이랑 레이저가 충돌 중일떄
		else if (PHYSICSMANAGER->isOverlap(_laser[0]->_transform, &_laser[0]->_boundBox, _oPortal->_transform, &_oPortal->_boundBox))
		{
			_laser[1]->setIsSet(true);
		}
	}
}

void stage3Scene::nextScene(void)
{
	if (PHYSICSMANAGER->isOverlap(_player, _elevator[1]))
	{
		SOUNDMANAGER->stop("door_close");
		if (_elevator[1]->_transform->getWorldPosition().x > _player->_transform->getWorldPosition().x)
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
				SCENEMANAGER->changeScene("엔딩");
			}
		}
	}
}

void stage3Scene::save(void)
{
	FILE* file;
	char name[] = "스테이지3";
	file = fopen("save/save.txt", "w");
	fprintf(file, "%s", &name);
	fclose(file);
}


void stage3Scene::playerLaserCollision()
{
	_laserCollideTime++;
	if (_laserCollideTime % 15 == 0)
	{
		_player->_transform->setWorldPosition(-0.8f, 0.4f, 15.0f);
		_player->_transform->lookDirection(D3DXVECTOR3(0, 0, -1));
	}
}
