#include "stdafx.h"
#include "tutorialScene_1.h"


void tutorialScene_1::tutorial1Collision(void)
{
	//주황포탈이랑 플레이어 충돌
	if (PHYSICSMANAGER->isOverlap(_oPortal->_transform, &_oPortal->_boundBox, _player->_transform, &_player->_boundBox))
	{
		_player->_transform->setWorldPosition(_bPortal->getPlayerPos().getWorldPosition());
		SOUNDMANAGER->play("portal_exit", 1.0f);
		if (SOUNDMANAGER->isPlaySound("portal_enter")) SOUNDMANAGER->stop("portal_enter");
	}
	//파랑포탈이랑 플레이어 충돌
	else if (PHYSICSMANAGER->isOverlap(_bPortal->_transform, &_bPortal->_boundBox, _player->_transform, &_player->_boundBox))
	{
		_player->_transform->setWorldPosition(_oPortal->getPlayerPos().getWorldPosition());
		SOUNDMANAGER->play("portal_enter", 1.0f);
		if (SOUNDMANAGER->isPlaySound("portal_exit")) SOUNDMANAGER->stop("portal_exit");
	}
	//주황포탈이랑 큐브 충돌
	if (PHYSICSMANAGER->isOverlap(_oPortal->_transform, &_oPortal->_boundBox, _cube->_transform, &_cube->_boundBox) && _player->getIsCollide() != C_CUBE)
	{
		_cube->_transform->setWorldPosition(_bPortal->getPlayerPos().getWorldPosition());
		SOUNDMANAGER->play("portal_exit", 1.0f);
		if (SOUNDMANAGER->isPlaySound("portal_enter")) SOUNDMANAGER->stop("portal_enter");
	}
	//파랑포탈이랑 큐브 충돌
	else if (PHYSICSMANAGER->isOverlap(_bPortal->_transform, &_bPortal->_boundBox, _cube->_transform, &_cube->_boundBox) && _player->getIsCollide() != C_CUBE)
	{
		_cube->_transform->setWorldPosition(_oPortal->getPlayerPos().getWorldPosition());
		SOUNDMANAGER->play("portal_enter", 1.0f);
		if (SOUNDMANAGER->isPlaySound("portal_exit")) SOUNDMANAGER->stop("portal_exit");
	}
	//총알이랑 벽 충돌
	for (int i = 0; i < _gun->getBullet()->getBulletObejct().size(); i++)
	{
		for (int j = 0; j < _map->getBoundBox().size(); j++)
		{
			if (PHYSICSMANAGER->isOverlap(_gun->getBullet()->getBulletObejct()[i].object->_transform,
				&_gun->getBullet()->getBulletObejct()[i].object->_boundBox,
				_map->getObject()->_transform,
				&_map->getBoundBox()[j].box))
			{
				if (SOUNDMANAGER->isPlaySound("portalgun_shoot_blue") || SOUNDMANAGER->isPlaySound("portalgun_shoot_orange"))
				{
					SOUNDMANAGER->play("portal_open", 0.5f);
					SOUNDMANAGER->stop("portalgun_shoot_blue");
					SOUNDMANAGER->stop("portalgun_shoot_orange");
				}
				//포탈이 생성되지 않는 벽은 무시
				//포탈이 생성되지 않는 벽은 무시
				if (!_map->getBoundBox()[j].isPortalAble)
				{
					_gun->getBullet()->removeBullet(i);
					break;
				}
				else
				{
					//파란 포탈일때
					if (_gun->getColor() == BULLET_BLUE)
					{
						if (PHYSICSMANAGER->isOverlap(_oPortal->_transform, &_oPortal->_boundBox, _gun->getBullet()->getBulletObejct()[i].object->_transform,
							&_gun->getBullet()->getBulletObejct()[i].object->_boundBox)) break;
						switch (_map->getBoundBox()[j].dir)
						{
						case BOUNDBOXDIR_BACK:
						{
							_bPortal->_transform->setWorldPosition(_map->getBoundBox()[j].trans.getWorldPosition().x + 0.15f,
								_gun->getBullet()->getBulletObejct()[i].object->_transform->getWorldPosition().y,
								_gun->getBullet()->getBulletObejct()[i].object->_transform->getWorldPosition().z);
							_bPortal->_transform->lookDirection(_map->getBoundBox()[j].trans.getForward());
							break;
						}
						case BOUNDBOXDIR_FORWARD:
						{
							_bPortal->_transform->setWorldPosition(_map->getBoundBox()[j].trans.getWorldPosition().x - 0.15f,
								_gun->getBullet()->getBulletObejct()[i].object->_transform->getWorldPosition().y,
								_gun->getBullet()->getBulletObejct()[i].object->_transform->getWorldPosition().z);
							_bPortal->_transform->lookDirection(_map->getBoundBox()[j].trans.getForward());
							break;
						}
						case BOUNDBOXDIR_DOWN:
						{
							_bPortal->_transform->setWorldPosition(_gun->getBullet()->getBulletObejct()[i].object->_transform->getWorldPosition().x,
								_map->getBoundBox()[j].trans.getWorldPosition().y - 0.15f,
								_gun->getBullet()->getBulletObejct()[i].object->_transform->getWorldPosition().z);
							_bPortal->_transform->lookDirection(_map->getBoundBox()[j].trans.getForward(), D3DXVECTOR3(0, 0, -1));
							break;
						}
						case BOUNDBOXDIR_UP:
						{
							_bPortal->_transform->setWorldPosition(_gun->getBullet()->getBulletObejct()[i].object->_transform->getWorldPosition().x,
								_map->getBoundBox()[j].trans.getWorldPosition().y + 0.15f,
								_gun->getBullet()->getBulletObejct()[i].object->_transform->getWorldPosition().z);
							_bPortal->_transform->lookDirection(_map->getBoundBox()[j].trans.getForward(), D3DXVECTOR3(0, 0, -1));
							break;
						}
						case BOUNDBOXDIR_LEFT:
						{
							_bPortal->_transform->setWorldPosition(_gun->getBullet()->getBulletObejct()[i].object->_transform->getWorldPosition().x,
								_gun->getBullet()->getBulletObejct()[i].object->_transform->getWorldPosition().y,
								_map->getBoundBox()[j].trans.getWorldPosition().z - 0.15f);
							_bPortal->_transform->lookDirection(_map->getBoundBox()[j].trans.getForward());
							break;
						}
						case BOUNDBOXDIR_RIGHT:
						{
							_bPortal->_transform->setWorldPosition(_gun->getBullet()->getBulletObejct()[i].object->_transform->getWorldPosition().x,
								_gun->getBullet()->getBulletObejct()[i].object->_transform->getWorldPosition().y,
								_map->getBoundBox()[j].trans.getWorldPosition().z + 0.15f);
							_bPortal->_transform->lookDirection(_map->getBoundBox()[j].trans.getForward());
							break;
						}
						}
					}
					_gun->getBullet()->removeBullet(i);
					break;
				}
			}
		}
	}
	//총알 오브젝트 충돌
	for (int i = 0; i < _gun->getBullet()->getBulletObejct().size(); i++)
	{
		if (PHYSICSMANAGER->isOverlap(_cube->_transform, &_cube->_boundBox,
			_gun->getBullet()->getBulletObejct()[i].object->_transform, &_gun->getBullet()->getBulletObejct()[i].object->_boundBox) &&
			_gun->getBullet()->getBulletObejct().size() > 0)
		{
			_gun->getBullet()->removeBullet(i);
			break;
		}
		else if (PHYSICSMANAGER->isOverlap(_dropper->_transform, &_dropper->_boundBox,
			_gun->getBullet()->getBulletObejct()[i].object->_transform, &_gun->getBullet()->getBulletObejct()[i].object->_boundBox) &&
			_gun->getBullet()->getBulletObejct().size() > 0)
		{
			_gun->getBullet()->removeBullet(i);
			break;
		}
		else if (PHYSICSMANAGER->isOverlap(_door->_transform, &_door->_boundBox,
			_gun->getBullet()->getBulletObejct()[i].object->_transform, &_gun->getBullet()->getBulletObejct()[i].object->_boundBox) &&
			_gun->getBullet()->getBulletObejct().size() > 0)
		{
			_gun->getBullet()->removeBullet(i);
			break;
		}
		else if (PHYSICSMANAGER->isOverlap(_elevator->_transform, &_elevator->_boundBox,
			_gun->getBullet()->getBulletObejct()[i].object->_transform, &_gun->getBullet()->getBulletObejct()[i].object->_boundBox) &&
			_gun->getBullet()->getBulletObejct().size() > 0)
		{
			_gun->getBullet()->removeBullet(i);
			break;
		}
		for (int j = 0; j < 3; j++)
		{
			if (_gun->getBullet()->getBulletObejct().size() <= 0) break;
			else if (PHYSICSMANAGER->isOverlap(_switch[j]->_transform, &_switch[j]->_boundBox,
				_gun->getBullet()->getBulletObejct()[i].object->_transform, &_gun->getBullet()->getBulletObejct()[i].object->_boundBox))
			{
				_gun->getBullet()->removeBullet(i);
				break;
			}
			else if (PHYSICSMANAGER->isOverlap(_button[j]->_transform, &_button[j]->_boundBox,
				_gun->getBullet()->getBulletObejct()[i].object->_transform, &_gun->getBullet()->getBulletObejct()[i].object->_boundBox))
			{
				_gun->getBullet()->removeBullet(i);
				break;
			}
		}
	}
	//엘리베이터 내려오기 위한 충돌
	if (PHYSICSMANAGER->isOverlap(_elevator->_transform, &_elevator->getElevatorEventBox(), _player->_transform, &_player->_boundBox))
	{
		_elevator->setState(ELEVATOR_ARRIVE);
	}
	//엘리베이터와 충돌
	if (PHYSICSMANAGER->isOverlap(_elevator->_transform, &_elevator->_boundBox, _player->_transform, &_player->_boundBox))
	{
		_player->_transform->setWorldPosition(_player->_transform->getWorldPosition().x, _elevator->_transform->getWorldPosition().y + 2.5f, _player->_transform->getWorldPosition().z);
		_player->setGravity(0);
	}
	//큐브 플레이어 , 큐브 맵 충돌
	//플레이어가 E키를 눌러서 큐브를 들때
	if (_player->getIsCarry())
	{
		//바운드박스가 겹치면
		if (PHYSICSMANAGER->isOverlap(_cube->_transform, &_cube->getPlayerBoundBox(), _player->_transform, &_player->_boundBox))
		{
			_player->cubeCollision(_cube);
			_cube->_transform->setLocalPosition(0, 2.0f + _mainCamera->getForward().y * 2.5f, 1.9f);
		}
	}
	else
	{
		_cube->_transform->releaseParent();
		if (_player->getIsCollide() == C_CUBE) SOUNDMANAGER->play("box_lay", 1.0f);
		_player->setIsCollide(C_NONE);
		_cube->setGravity(0.2f);
	}

	if (PHYSICSMANAGER->isOverlap(_cube->_transform, &_cube->getPlayerBoundBox(), _player->_transform, &_player->_boundBox))
	{
		PHYSICSMANAGER->isBlocking(_player->_transform, &_player->_boundBox, _cube->_transform, &_cube->_boundBox);
	}

	//큐브와 맵충돌
	if (PHYSICSMANAGER->isOverlap(_cube, _map->getObject()))
	{
		for (int i = 0; i < _map->getBoundBox().size(); i++)
		{
			PHYSICSMANAGER->isBlocking(_map->getObject()->_transform, &_map->getBoundBox()[i].box, _cube->_transform, &_cube->_boundBox);
			if (_map->getBoundBox()[i].dir != BOUNDBOXDIR_UP)
			{
				//if (SOUNDMANAGER->isPlaySound("box_lay")) SOUNDMANAGER->stop("box_lay");
				_cube->setCollisionType(COLLISIONTYPE_WALL);
			}
			else
			{
				if (_cube->getGravity() == 0.2f && _cube->getCollisionType() == COLLISIONTYPE_WALL)
					_cube->setCollisionType(COLLISIONTYPE_NONE);
			}
		}
	}
	//큐브 플레이어 충돌
	if (PHYSICSMANAGER->isOverlap(_cube->_transform, &_cube->_boundBox, _player->_transform, &_player->_boundBox))
	{
		if (_cube->getCollisionType() == COLLISIONTYPE_WALL)
		{
			PHYSICSMANAGER->isBlocking(_cube->_transform, &_cube->_boundBox, _player->_transform, &_player->_boundBox);
			_player->setIsCarry(false);
			_cube->_transform->releaseParent();
		}
	}
	else if (PHYSICSMANAGER->isOverlap(_cube->_transform, &_cube->_boundBox, _player->_transform, &_player->_boundBox))
	{
		PHYSICSMANAGER->isBlocking(_player->_transform, &_player->_boundBox, _cube->_transform, &_cube->_boundBox);
	}

	//맵과 플레이어 충돌
	if (PHYSICSMANAGER->isOverlap(_player, _map->getObject()))
	{
		for (int i = 0; i < _map->getBoundBox().size() - 1; i++)
		{
			PHYSICSMANAGER->isBlocking(_map->getObject()->_transform, &_map->getBoundBox()[i].box, _player->_transform, &_player->_boundBox);
			if (i == 3)
			{
				_player->setIsUpStairs(false);
			}
		}
	}

	//계단 충돌
	for (int i = 0; i < _map->getStairsCollision().size(); i++)
	{
		if (PHYSICSMANAGER->isOverlap(_player->_transform, &_player->_boundBox, _map->getObject()->_transform, &_map->getStairsCollision()[i].box))
		{
			_player->setIsUpStairs(true);
			_player->setGravity(0);
			//보간을 이용하여 계단을 오르락내리락함
			//계단위 트랜스폼
			transform lerpPositionUp;
			//계단 아래 트랜스폼
			transform lerpPositionDown;
			//플레이어 위치를 저장할 트랜스폼
			transform tempPos;
			lerpPositionUp.setWorldPosition(5.2f, 20.0f, 34.5f);
			lerpPositionDown.setWorldPosition(5.2f, -15.0f, 26.5f);
			tempPos.setWorldPosition(_player->_transform->getWorldPosition());
			int angle = ((int)_mainCamera->getNowAngleH() - 45) % 360;
			//양수 부분
			//0~ 90
			if (angle >= 0 && angle < 90)
			{
				//A를 누르면 올라간다, 위쪽 트랜스폼이랑 보간하여 계산함
				if (KEYMANAGER->isStayKeyDown('A')) _player->_transform->positionLerp(tempPos, lerpPositionUp, 0.002f);
				if (KEYMANAGER->isOnceKeyUp('A')) _player->_transform->setWorldPosition(tempPos.getWorldPosition());
				//D를 누르면 내려간다, 아래쪽 트랜스폼이랑 보간하여 계산함
				if (KEYMANAGER->isStayKeyDown('D'))_player->_transform->positionLerp(tempPos, lerpPositionDown, 0.002f);
				if (KEYMANAGER->isOnceKeyUp('D')) _player->_transform->setWorldPosition(tempPos.getWorldPosition());
			}
			//90~ 180
			else if (angle >= 90 && angle < 180)
			{
				//S를 누르면 올라간다, 위쪽 트랜스폼이랑 보간하여 계산함
				if (KEYMANAGER->isStayKeyDown('S'))  _player->_transform->positionLerp(tempPos, lerpPositionUp, 0.002f);
				if (KEYMANAGER->isOnceKeyUp('S')) _player->_transform->setWorldPosition(tempPos.getWorldPosition());
				//W를 누르면 내려간다, 아래쪽 트랜스폼이랑 보간하여 계산함
				if (KEYMANAGER->isStayKeyDown('W'))_player->_transform->positionLerp(tempPos, lerpPositionDown, 0.002f);
				if (KEYMANAGER->isOnceKeyUp('W')) _player->_transform->setWorldPosition(tempPos.getWorldPosition());
			}
			//180~ 270
			else if (angle >= 180 && angle < 270)
			{
				//D를 누르면 올라간다, 위쪽 트랜스폼이랑 보간하여 계산함
				if (KEYMANAGER->isStayKeyDown('D')) _player->_transform->positionLerp(tempPos, lerpPositionUp, 0.002f);
				if (KEYMANAGER->isOnceKeyUp('D')) _player->_transform->setWorldPosition(tempPos.getWorldPosition());
				//A를 누르면 내려간다, 아래쪽 트랜스폼이랑 보간하여 계산함
				if (KEYMANAGER->isStayKeyDown('A'))_player->_transform->positionLerp(tempPos, lerpPositionDown, 0.002f);
				if (KEYMANAGER->isOnceKeyUp('A')) _player->_transform->setWorldPosition(tempPos.getWorldPosition());
			}
			//270~ 360
			else if (angle >= 270 && angle < 360)
			{
				//W를 누르면 올라간다, 위쪽 트랜스폼이랑 보간하여 계산함
				if (KEYMANAGER->isStayKeyDown('W')) _player->_transform->positionLerp(tempPos, lerpPositionUp, 0.002f);
				if (KEYMANAGER->isOnceKeyUp('W')) _player->_transform->setWorldPosition(tempPos.getWorldPosition());
				//S를 누르면 내려간다, 아래쪽 트랜스폼이랑 보간하여 계산함
				if (KEYMANAGER->isStayKeyDown('S')) _player->_transform->positionLerp(tempPos, lerpPositionDown, 0.002f);
				if (KEYMANAGER->isOnceKeyUp('S')) _player->_transform->setWorldPosition(tempPos.getWorldPosition());
			}
			//음수 부분
			//-90 ~ 0
			else if (angle <0 && angle >-90)
			{
				//W를 누르면 올라간다, 위쪽 트랜스폼이랑 보간하여 계산함
				if (KEYMANAGER->isStayKeyDown('W')) _player->_transform->positionLerp(tempPos, lerpPositionUp, 0.002f);
				if (KEYMANAGER->isOnceKeyUp('W')) _player->_transform->setWorldPosition(tempPos.getWorldPosition());
				//W를 누르면 내려간다, 아래쪽 트랜스폼이랑 보간하여 계산함
				if (KEYMANAGER->isStayKeyDown('S')) _player->_transform->positionLerp(tempPos, lerpPositionDown, 0.002f);
				if (KEYMANAGER->isOnceKeyUp('S')) _player->_transform->setWorldPosition(tempPos.getWorldPosition());
			}
			//-180 ~ -90
			else if (angle < -90 && angle >= -180)
			{
				//D를 누르면 올라간다, 위쪽 트랜스폼이랑 보간하여 계산함
				if (KEYMANAGER->isStayKeyDown('D')) _player->_transform->positionLerp(tempPos, lerpPositionUp, 0.002f);
				if (KEYMANAGER->isOnceKeyUp('D')) _player->_transform->setWorldPosition(tempPos.getWorldPosition());
				//A를 누르면 내려간다, 아래쪽 트랜스폼이랑 보간하여 계산함
				if (KEYMANAGER->isStayKeyDown('A')) _player->_transform->positionLerp(tempPos, lerpPositionDown, 0.002f);
				if (KEYMANAGER->isOnceKeyUp('A')) _player->_transform->setWorldPosition(tempPos.getWorldPosition());
			}
			//-270 ~ -180
			else if (angle < -180 && angle >= -270)
			{
				//S를 누르면 올라간다, 위쪽 트랜스폼이랑 보간하여 계산함
				if (KEYMANAGER->isStayKeyDown('S')) _player->_transform->positionLerp(tempPos, lerpPositionUp, 0.002f);
				if (KEYMANAGER->isOnceKeyUp('S')) _player->_transform->setWorldPosition(tempPos.getWorldPosition());
				//W를 누르면 내려간다, 아래쪽 트랜스폼이랑 보간하여 계산함
				if (KEYMANAGER->isStayKeyDown('W')) _player->_transform->positionLerp(tempPos, lerpPositionDown, 0.002f);
				if (KEYMANAGER->isOnceKeyUp('W')) _player->_transform->setWorldPosition(tempPos.getWorldPosition());
			}
			//-360 ~ -270
			else if (angle <-270 && angle >-360)
			{
				//A를 누르면 올라간다, 위쪽 트랜스폼이랑 보간하여 계산함
				if (KEYMANAGER->isStayKeyDown('A')) _player->_transform->positionLerp(tempPos, lerpPositionUp, 0.002f);
				if (KEYMANAGER->isOnceKeyUp('A')) _player->_transform->setWorldPosition(tempPos.getWorldPosition());
				//D를 누르면 내려간다, 아래쪽 트랜스폼이랑 보간하여 계산함
				if (KEYMANAGER->isStayKeyDown('D')) _player->_transform->positionLerp(tempPos, lerpPositionDown, 0.002f);
				if (KEYMANAGER->isOnceKeyUp('D')) _player->_transform->setWorldPosition(tempPos.getWorldPosition());
			}
		}
	}

	//플레이어 스위치 충돌
	for (int i = 0; i < 3; i++)
	{
		if (PHYSICSMANAGER->isOverlap(_switch[i]->_transform, &_switch[i]->getPlayerBoundBox(), _player->_transform, &_player->_boundBox))
		{

			PHYSICSMANAGER->isBlocking(_switch[i]->_transform, &_switch[i]->_boundBox, _player->_transform, &_player->_boundBox);
			//E키를 누르면 스위치를 클릭
			if (_player->getIsCollide() == C_CUBE) break;
			_player->setIsCollide(C_SWITCH);
			if (KEYMANAGER->isOnceKeyDown('E'))
			{
				clickSwitch(i);
				if (i == 0)
				{
					_bPortal->_transform->setWorldPosition(1, 1.55f, -6.4f);
					_bPortal->_transform->lookDirection(D3DXVECTOR3(0, 0, -1));
					if (SOUNDMANAGER->isPlaySound("switch_click")) SOUNDMANAGER->stop("switch_click");
					SOUNDMANAGER->play("switch_click", 1.0f);
					if (SOUNDMANAGER->isPlaySound("portal_open")) SOUNDMANAGER->stop("portal_open");
					SOUNDMANAGER->play("portal_open", 1.0f);
				}
				else if (i == 1)
				{
					_bPortal->_transform->setWorldPosition(8.6f, 1.55f, 3.5f);
					_bPortal->_transform->lookDirection(D3DXVECTOR3(1, 0, 0));
					if (SOUNDMANAGER->isPlaySound("switch_click")) SOUNDMANAGER->stop("switch_click");
					SOUNDMANAGER->play("switch_click", 1.0f);
					if (SOUNDMANAGER->isPlaySound("portal_open")) SOUNDMANAGER->stop("portal_open");
					SOUNDMANAGER->play("portal_open", 1.0f);
				}
				else
				{
					_oPortal->_transform->setWorldPosition(-7.1f, 4.4f, 37);
					_oPortal->_transform->lookDirection(D3DXVECTOR3(-1, 0, 0));
					if (SOUNDMANAGER->isPlaySound("switch_click")) SOUNDMANAGER->stop("switch_click");
					SOUNDMANAGER->play("switch_click", 1.0f);
					if (SOUNDMANAGER->isPlaySound("portal_open")) SOUNDMANAGER->stop("portal_open");
					SOUNDMANAGER->play("portal_open", 1.0f);
				}
				break;
			}
		}
	}
	//큐브 버튼 충돌
	for (int i = 0; i < 2; i++)
	{
		if (PHYSICSMANAGER->isOverlap(_button[i]->_transform, &_button[i]->_boundBox, _cube->_transform, &_cube->_boundBox))
		{
			_button[i]->_transform->releaseParent();
			_button[i]->setState(BUTTON_DOWN);
			_cube->_transform->setWorldPosition(_button[i]->_transform->getWorldPosition().x, _button[i]->_transform->getWorldPosition().y + 0.7f, _button[i]->_transform->getWorldPosition().z);
			if (i == 1)
			{
				if (PHYSICSMANAGER->isOverlap(_door->_transform, &_door->getCloseTrigger(), _player->_transform, &_player->_boundBox))
				{
					if (_door->getState() != DOOR_CLOSE)
					{
						SOUNDMANAGER->stop("door_open");
						SOUNDMANAGER->play("door_close", 0.5f);
					}
					_door->setState(DOOR_CLOSE);
				}
				//문이 닫이면 문과 충돌을 한다
				else if (_door->getState() == DOOR_CLOSE)
				{
					PHYSICSMANAGER->isBlocking(_door->_transform, &_door->_boundBox, _player->_transform, &_player->_boundBox);
				}
				else if (!PHYSICSMANAGER->isOverlap(_door->_transform, &_door->getCloseTrigger(), _player->_transform, &_player->_boundBox))
				{
					if (_door->getState() != DOOR_OPEN) SOUNDMANAGER->play("door_open", 0.2f);
					_door->setState(DOOR_OPEN);
				}

			}
			else
			{
				_oPortal->_transform->setWorldPosition(0.5f, 1.55f, 9.7f);
				_oPortal->_transform->lookDirection(D3DXVECTOR3(0, 0, 1));
			}
		}
		else
		{
			_button[i]->setState(BUTTON_UP);
		}
	}
	
	//버튼과 플레이어 충돌
	if (PHYSICSMANAGER->isOverlap(_button[0]->_transform, &_button[0]->_boundBox, _player->_transform, &_player->_boundBox))
	{
		PHYSICSMANAGER->isBlocking(_button[0]->_transform, &_button[0]->_boundBox, _player->_transform, &_player->_boundBox);
		//플레이어가 버튼 위로 올라간다
		_player->_transform->setWorldPosition(_player->_transform->getWorldPosition().x, _button[0]->_transform->getWorldPosition().y + 0.4f, _player->_transform->getWorldPosition().z);
		_player->setGravity(0);
	}
	else if (PHYSICSMANAGER->isOverlap(_button[1]->_transform, &_button[1]->_boundBox, _player->_transform, &_player->_boundBox))
	{
		PHYSICSMANAGER->isBlocking(_button[1]->_transform, &_button[1]->_boundBox, _player->_transform, &_player->_boundBox);
		_player->_transform->setWorldPosition(_player->_transform->getWorldPosition().x, _button[1]->_transform->getWorldPosition().y + 0.4f, _player->_transform->getWorldPosition().z);
		_player->setGravity(0);
	}
	else
	{
		if (_player->getGravity() == 0.2f || _player->getIsUpStairs()) return;
		_player->setGravity(0.2f);
	}

}