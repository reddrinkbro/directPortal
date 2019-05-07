#include "stdafx.h"
#include "tutorialScene_2.h"


void tutorialScene_2::tutorial2Collision(void)
{

	//��Ȳ��Ż�̶� �÷��̾� �浹
	if (PHYSICSMANAGER->isOverlap(_oPortal->_transform, &_oPortal->_boundBox, _player->_transform, &_player->_boundBox))
	{
		_player->_transform->setWorldPosition(_bPortal->getPlayerPos().getWorldPosition());
		SOUNDMANAGER->play("portal_exit", 1.0f);
		if (SOUNDMANAGER->isPlaySound("portal_enter")) SOUNDMANAGER->stop("portal_enter");
	}
	//�Ķ���Ż�̶� �÷��̾� �浹
	else if (PHYSICSMANAGER->isOverlap(_bPortal->_transform, &_bPortal->_boundBox, _player->_transform, &_player->_boundBox))
	{
		_player->_transform->setWorldPosition(_oPortal->getPlayerPos().getWorldPosition());
		SOUNDMANAGER->play("portal_enter", 1.0f);
		if (SOUNDMANAGER->isPlaySound("portal_exit")) SOUNDMANAGER->stop("portal_exit");
	}
	//��Ȳ ��Ż���̶� �浹
	if (PHYSICSMANAGER->isOverlap(_gun[1]->_transform, &_gun[1]->_boundBox, _player->_transform, &_player->_boundBox))
	{
		_isGetOrangePortalGun = true;
		_gun[1]->_transform->setWorldPosition(0, 0, -8);
		_player->getCrosshair()->setState(CROSSHAIRSTATE_ALL);
		SOUNDMANAGER->play("gun_take", 1.0f);
	}
	for (int i = 0; i < 2; i++)
	{
		//��Ȳ��Ż�̶� ť�� �浹
		if (PHYSICSMANAGER->isOverlap(_oPortal->_transform, &_oPortal->_boundBox, _cube[i]->_transform, &_cube[i]->_boundBox) && _player->getIsCollide() != C_CUBE)
		{
			_cube[i]->_transform->setWorldPosition(_bPortal->getPlayerPos().getWorldPosition());
			SOUNDMANAGER->play("portal_exit", 1.0f);
			if (SOUNDMANAGER->isPlaySound("portal_enter")) SOUNDMANAGER->stop("portal_enter");
		}
		//�Ķ���Ż�̶� ť�� �浹
		if (PHYSICSMANAGER->isOverlap(_bPortal->_transform, &_bPortal->_boundBox, _cube[i]->_transform, &_cube[i]->_boundBox) && _player->getIsCollide() != C_CUBE)
		{
			_cube[i]->_transform->setWorldPosition(_oPortal->getPlayerPos().getWorldPosition());
			SOUNDMANAGER->play("portal_enter", 1.0f);
			if (SOUNDMANAGER->isPlaySound("portal_exit")) SOUNDMANAGER->stop("portal_exit");
		}
	}
	for (int i = 0; i < _gun[0]->getBullet()->getBulletObejct().size(); i++)
	{
		for (int j = 0; j < _map->getBoundBox().size(); j++)
		{
			if (PHYSICSMANAGER->isOverlap(_gun[0]->getBullet()->getBulletObejct()[i].object->_transform,
				&_gun[0]->getBullet()->getBulletObejct()[i].object->_boundBox,
				_map->getObject()->_transform,
				&_map->getBoundBox()[j].box))
			{
				if (SOUNDMANAGER->isPlaySound("portalgun_shoot_blue") || SOUNDMANAGER->isPlaySound("portalgun_shoot_orange"))
				{
					SOUNDMANAGER->play("portal_open", 0.5f);
					SOUNDMANAGER->stop("portalgun_shoot_blue");
					SOUNDMANAGER->stop("portalgun_shoot_orange");
				}
				//��Ż�� �������� �ʴ� ���� ����
				if (!_map->getBoundBox()[j].isPortalAble)
				{
					_gun[0]->getBullet()->removeBullet(i);
					break;
				}
				else
				{
					//�Ķ� ��Ż�϶�
					if (_gun[0]->getColor() == BULLET_BLUE)
					{
						if (PHYSICSMANAGER->isOverlap(_oPortal->_transform, &_oPortal->_boundBox, _gun[0]->getBullet()->getBulletObejct()[i].object->_transform,
							&_gun[0]->getBullet()->getBulletObejct()[i].object->_boundBox)) break;
						_bPortal->setIsSet(true);
						switch (_map->getBoundBox()[j].dir)
						{
						case BOUNDBOXDIR_BACK:
						{
							_bPortal->_transform->setWorldPosition(_map->getBoundBox()[j].trans.getWorldPosition().x + 0.15f,
								_gun[0]->getBullet()->getBulletObejct()[i].object->_transform->getWorldPosition().y,
								_gun[0]->getBullet()->getBulletObejct()[i].object->_transform->getWorldPosition().z);
							_bPortal->_transform->lookDirection(_map->getBoundBox()[j].trans.getForward());
							break;
						}
						case BOUNDBOXDIR_FORWARD:
						{
							_bPortal->_transform->setWorldPosition(_map->getBoundBox()[j].trans.getWorldPosition().x - 0.15f,
								_gun[0]->getBullet()->getBulletObejct()[i].object->_transform->getWorldPosition().y,
								_gun[0]->getBullet()->getBulletObejct()[i].object->_transform->getWorldPosition().z);
							_bPortal->_transform->lookDirection(_map->getBoundBox()[j].trans.getForward());
							break;
						}
						case BOUNDBOXDIR_DOWN:
						{
							_bPortal->_transform->setWorldPosition(_gun[0]->getBullet()->getBulletObejct()[i].object->_transform->getWorldPosition().x,
								_map->getBoundBox()[j].trans.getWorldPosition().y - 0.15f,
								_gun[0]->getBullet()->getBulletObejct()[i].object->_transform->getWorldPosition().z);
							_bPortal->_transform->lookDirection(_map->getBoundBox()[j].trans.getForward(), D3DXVECTOR3(0, 0, -1));
							break;
						}
						case BOUNDBOXDIR_UP:
						{
							_bPortal->_transform->setWorldPosition(_gun[0]->getBullet()->getBulletObejct()[i].object->_transform->getWorldPosition().x,
								_map->getBoundBox()[j].trans.getWorldPosition().y + 0.15f,
								_gun[0]->getBullet()->getBulletObejct()[i].object->_transform->getWorldPosition().z);
							_bPortal->_transform->lookDirection(_map->getBoundBox()[j].trans.getForward(), D3DXVECTOR3(0, 0, -1));
							break;
						}
						case BOUNDBOXDIR_LEFT:
						{
							_bPortal->_transform->setWorldPosition(_gun[0]->getBullet()->getBulletObejct()[i].object->_transform->getWorldPosition().x,
								_gun[0]->getBullet()->getBulletObejct()[i].object->_transform->getWorldPosition().y,
								_map->getBoundBox()[j].trans.getWorldPosition().z - 0.15f);
							_bPortal->_transform->lookDirection(_map->getBoundBox()[j].trans.getForward());
							break;
						}
						case BOUNDBOXDIR_RIGHT:
						{
							_bPortal->_transform->setWorldPosition(_gun[0]->getBullet()->getBulletObejct()[i].object->_transform->getWorldPosition().x,
								_gun[0]->getBullet()->getBulletObejct()[i].object->_transform->getWorldPosition().y,
								_map->getBoundBox()[j].trans.getWorldPosition().z + 0.15f);
							_bPortal->_transform->lookDirection(_map->getBoundBox()[j].trans.getForward());
							break;
						}
						}
					}
					//��Ȳ ��Ż�϶�
					else if (_gun[0]->getColor() == BULLET_ORANGE)
					{
						if (PHYSICSMANAGER->isOverlap(_bPortal->_transform, &_bPortal->_boundBox, _gun[0]->getBullet()->getBulletObejct()[i].object->_transform,
							&_gun[0]->getBullet()->getBulletObejct()[i].object->_boundBox)) break;
						if (_isGetOrangePortalGun)
						{
							_oPortal->setIsSet(true);
							switch (_map->getBoundBox()[j].dir)
							{
							case BOUNDBOXDIR_BACK:
							{
								_oPortal->_transform->setWorldPosition(_map->getBoundBox()[j].trans.getWorldPosition().x + 0.15f,
									_gun[0]->getBullet()->getBulletObejct()[i].object->_transform->getWorldPosition().y,
									_gun[0]->getBullet()->getBulletObejct()[i].object->_transform->getWorldPosition().z);
								_oPortal->_transform->lookDirection(_map->getBoundBox()[j].trans.getForward());
								break;
							}
							case BOUNDBOXDIR_FORWARD:
							{
								_oPortal->_transform->setWorldPosition(_map->getBoundBox()[j].trans.getWorldPosition().x - 0.15f,
									_gun[0]->getBullet()->getBulletObejct()[i].object->_transform->getWorldPosition().y,
									_gun[0]->getBullet()->getBulletObejct()[i].object->_transform->getWorldPosition().z);
								_oPortal->_transform->lookDirection(_map->getBoundBox()[j].trans.getForward());
								break;
							}
							case BOUNDBOXDIR_DOWN:
							{
								_oPortal->_transform->setWorldPosition(_gun[0]->getBullet()->getBulletObejct()[i].object->_transform->getWorldPosition().x,
									_map->getBoundBox()[j].trans.getWorldPosition().y - 0.15f,
									_gun[0]->getBullet()->getBulletObejct()[i].object->_transform->getWorldPosition().z);
								_oPortal->_transform->lookDirection(_map->getBoundBox()[j].trans.getForward(), D3DXVECTOR3(0, 0, -1));
								break;
							}
							case BOUNDBOXDIR_UP:
							{
								_oPortal->_transform->setWorldPosition(_gun[0]->getBullet()->getBulletObejct()[i].object->_transform->getWorldPosition().x,
									_map->getBoundBox()[j].trans.getWorldPosition().y + 0.15f,
									_gun[0]->getBullet()->getBulletObejct()[i].object->_transform->getWorldPosition().z);
								_oPortal->_transform->lookDirection(_map->getBoundBox()[j].trans.getForward(), D3DXVECTOR3(0, 0, -1));
								break;
							}
							case BOUNDBOXDIR_LEFT:
							{
								_oPortal->_transform->setWorldPosition(_gun[0]->getBullet()->getBulletObejct()[i].object->_transform->getWorldPosition().x,
									_gun[0]->getBullet()->getBulletObejct()[i].object->_transform->getWorldPosition().y,
									_map->getBoundBox()[j].trans.getWorldPosition().z - 0.15f);
								_oPortal->_transform->lookDirection(_map->getBoundBox()[j].trans.getForward());
								break;
							}
							case BOUNDBOXDIR_RIGHT:
							{
								_oPortal->_transform->setWorldPosition(_gun[0]->getBullet()->getBulletObejct()[i].object->_transform->getWorldPosition().x,
									_gun[0]->getBullet()->getBulletObejct()[i].object->_transform->getWorldPosition().y,
									_map->getBoundBox()[j].trans.getWorldPosition().z + 0.15f);
								_oPortal->_transform->lookDirection(_map->getBoundBox()[j].trans.getForward());
								break;
							}
							}
						}
					}
					_gun[0]->setIsHit(true);
					_gun[0]->getBullet()->removeBullet(i);
					break;
				}
			}
		}
		if (!_gun[0]->getIsHit())
		{
			for (int j = 0; j < _map->getbasicBox().size(); j++)
			{
				if (PHYSICSMANAGER->isOverlap(_gun[0]->getBullet()->getBulletObejct()[i].object->_transform,
					&_gun[0]->getBullet()->getBulletObejct()[i].object->_boundBox,
					_map->getObject()->_transform,
					&_map->getbasicBox()[j].box))
				{
					//��Ż�� �������� �ʴ� ���� ����
					if (!_map->getbasicBox()[j].isPortalAble) continue;
					else
					{
						//�Ķ� ��Ż�϶�
						if (_gun[0]->getColor() == BULLET_BLUE)
						{
							_bPortal->_transform->setWorldPosition(_map->getbasicBox()[j].trans.getWorldPosition());
							if (_map->getbasicBox()[j].dir == BOUNDBOXDIR_UP || _map->getbasicBox()[j].dir == BOUNDBOXDIR_DOWN)
							{
								_bPortal->_transform->lookDirection(_map->getbasicBox()[j].trans.getForward(), D3DXVECTOR3(0, 0, -1));
							}
							else _bPortal->_transform->lookDirection(_map->getbasicBox()[j].trans.getForward());
						}
						else if (_gun[0]->getColor() == BULLET_ORANGE)
						{
							if (_isGetOrangePortalGun)
							{
								_oPortal->_transform->setWorldPosition(_map->getbasicBox()[j].trans.getWorldPosition());
								if (_map->getbasicBox()[j].dir == BOUNDBOXDIR_UP || _map->getbasicBox()[j].dir == BOUNDBOXDIR_DOWN)
								{
									_oPortal->_transform->lookDirection(_map->getbasicBox()[j].trans.getForward(), D3DXVECTOR3(0, 0, -1));
								}
								else _oPortal->_transform->lookDirection(_map->getbasicBox()[j].trans.getForward());
							}
						}
					}
					_gun[0]->getBullet()->removeBullet(i);
					_gun[0]->setIsHit(true);
					break;
				}
			}
		}
	}
	//���� ��Ż�� �浹 �ϸ� �ϳ��� �������� �ʵ��� ��
	if (PHYSICSMANAGER->isOverlap(_oPortal, _bPortal))
	{
		if (_gun[0]->getColor() == BULLET_ORANGE)
		{
			_oPortal->setIsSet(false);
		}
		else if (_gun[0]->getColor() == BULLET_BLUE)
		{
			_bPortal->setIsSet(false);
		}
	}
	//�Ѿ� ������Ʈ �浹
	for (int i = 0; i < _gun[0]->getBullet()->getBulletObejct().size(); i++)
	{
		if (PHYSICSMANAGER->isOverlap(_door->_transform, &_door->_boundBox,
			_gun[0]->getBullet()->getBulletObejct()[i].object->_transform, &_gun[0]->getBullet()->getBulletObejct()[i].object->_boundBox) &&
			_gun[0]->getBullet()->getBulletObejct().size() > 0)
		{
			_gun[0]->getBullet()->removeBullet(i);
			break;
		}
		for (int j = 0; j < 2; j++)
		{
			if (_gun[0]->getBullet()->getBulletObejct().size() <= 0) break;
			else if (PHYSICSMANAGER->isOverlap(_elevator[i]->_transform, &_elevator[i]->_boundBox,
				_gun[0]->getBullet()->getBulletObejct()[i].object->_transform, &_gun[0]->getBullet()->getBulletObejct()[i].object->_boundBox) &&
				_gun[0]->getBullet()->getBulletObejct().size() > 0)
			{
				_gun[0]->getBullet()->removeBullet(i);
				break;
			}
			else if (PHYSICSMANAGER->isOverlap(_button[j]->_transform, &_button[j]->_boundBox,
				_gun[0]->getBullet()->getBulletObejct()[i].object->_transform, &_gun[0]->getBullet()->getBulletObejct()[i].object->_boundBox))
			{
				_gun[0]->getBullet()->removeBullet(i);
				break;
			}
			else if (PHYSICSMANAGER->isOverlap(_cube[j]->_transform, &_cube[j]->_boundBox,
				_gun[0]->getBullet()->getBulletObejct()[i].object->_transform, &_gun[0]->getBullet()->getBulletObejct()[i].object->_boundBox))
			{
				_gun[0]->getBullet()->removeBullet(i);
				break;
			}
		}
	}
	// ���������� ���� ��ȭ
	if (PHYSICSMANAGER->isOverlap(_elevator[0]->_transform, &_elevator[0]->getElevatorEventBox(), _player->_transform, &_player->_boundBox))
	{
		if (_elevator[0]->getState() != ELEVATOR_LEAVE) SOUNDMANAGER->play("elevator_door_close", 0.2f);
		_elevator[0]->setState(ELEVATOR_LEAVE);
	}
	else if (PHYSICSMANAGER->isOverlap(_elevator[1]->_transform, &_elevator[1]->getElevatorEventBox(), _player->_transform, &_player->_boundBox))
	{
		_elevator[1]->setState(ELEVATOR_ARRIVE);
	}
	//���������Ϳ� �غκ� �浹
	if (_elevator[0]->getState() != ELEVATOR_LEAVE)
	{
		if (PHYSICSMANAGER->isOverlap(_elevator[0]->_transform, &_elevator[0]->_boundBox, _player->_transform, &_player->_boundBox))
		{
			_player->_transform->setWorldPosition(_player->_transform->getWorldPosition().x, _elevator[0]->_transform->getWorldPosition().y + 2.5f, _player->_transform->getWorldPosition().z);
			_player->setGravity(0);
		}
		else
		{
			_player->setGravity(0.1f);
		}
	}
	else if (PHYSICSMANAGER->isOverlap(_elevator[1]->_transform, &_elevator[1]->_boundBox, _player->_transform, &_player->_boundBox))
	{
		_player->_transform->setWorldPosition(_player->_transform->getWorldPosition().x, _elevator[1]->_transform->getWorldPosition().y + 2.5f, _player->_transform->getWorldPosition().z);
		_player->setGravity(0);
	}
	Ray ray;
	_mainCamera->computeRay(&ray, &D3DXVECTOR2(getMousePos().x, getMousePos().y));
	//�÷��̾ EŰ�� ������ ť�긦 �鶧

	if (_player->getIsCarry())
	{
		bool isCube1 = false;
		if (PHYSICSMANAGER->isRayHitStaticMeshObject(&ray, _cube[0], NULL, NULL))
		{
			_cube[1]->_transform->releaseParent();
			isCube1 = false;
		}

		else if (PHYSICSMANAGER->isRayHitStaticMeshObject(&ray, _cube[1], NULL, NULL))
		{
			_cube[0]->_transform->releaseParent();
			isCube1 = true;
		}

		//�ٿ��ڽ��� ��ġ��
		if (PHYSICSMANAGER->isOverlap(_cube[0]->_transform, &_cube[0]->getPlayerBoundBox(), _player->_transform, &_player->_boundBox) && !isCube1)
		{
			_player->setIsCollide(C_CUBE);
			_player->cubeCollision(_cube[0]);
			_cube[0]->_transform->setLocalPosition(0, 2.0f + _mainCamera->getForward().y * 2.5f, 1.9f);
		}
		else if (PHYSICSMANAGER->isOverlap(_cube[1]->_transform, &_cube[1]->getPlayerBoundBox(), _player->_transform, &_player->_boundBox) && isCube1)
		{
			_player->setIsCollide(C_CUBE);
			_player->cubeCollision(_cube[1]);
			_cube[1]->_transform->setLocalPosition(0, 2.0f + _mainCamera->getForward().y * 2.5f, 1.9f);
		}
	}
	else
	{
		_cube[0]->_transform->releaseParent();
		_cube[1]->_transform->releaseParent();
		if (_player->getIsCollide() == C_CUBE) SOUNDMANAGER->play("box_lay", 1.0f);
		_player->setIsCollide(C_NONE);
	}
	for (int i = 0; i < 2; i++)
	{
		if (PHYSICSMANAGER->isOverlap(_cube[i]->_transform, &_cube[i]->getPlayerBoundBox(), _player->_transform, &_player->_boundBox))
		{
			PHYSICSMANAGER->isBlocking(_player->_transform, &_player->_boundBox, _cube[i]->_transform, &_cube[i]->_boundBox);
			break;
		}
	}
	for (int i = 0; i < 2; i++)
	{
		//ť��� ���浹
		if (PHYSICSMANAGER->isOverlap(_cube[i], _map->getObject()))
		{
			for (int j = 0; j < _map->getBoundBox().size(); j++)
			{
				PHYSICSMANAGER->isBlocking(_map->getObject()->_transform, &_map->getBoundBox()[j].box, _cube[i]->_transform, &_cube[i]->_boundBox);
				if (_map->getBoundBox()[j].dir != BOUNDBOXDIR_UP)
				{
					_cube[i]->setCollisionType(COLLISIONTYPE_WALL);
				}
				else
				{
					_cube[i]->setCollisionType(COLLISIONTYPE_NONE);
				}
			}
		}
		//ť�� �÷��̾� �浹
		if (PHYSICSMANAGER->isOverlap(_cube[i]->_transform, &_cube[i]->_boundBox, _player->_transform, &_player->_boundBox))
		{
			if (_cube[i]->getCollisionType() == COLLISIONTYPE_WALL)
			{
				PHYSICSMANAGER->isBlocking(_cube[i]->_transform, &_cube[i]->_boundBox, _player->_transform, &_player->_boundBox);
				_player->setIsCarry(false);
				_cube[i]->_transform->releaseParent();
			}
		}
		else if (PHYSICSMANAGER->isOverlap(_cube[i]->_transform, &_cube[i]->_boundBox, _player->_transform, &_player->_boundBox))
		{
			PHYSICSMANAGER->isBlocking(_player->_transform, &_player->_boundBox, _cube[i]->_transform, &_cube[i]->_boundBox);
		}
	}
	if (PHYSICSMANAGER->isOverlap(_cube[0], _cube[1]))
	{
		//�Ÿ��� �缭 �� �÷��̾� ���� �ָ��ִ� ť�긦 �о��
		float distance0 = D3DXVec3Length(&(_cube[0]->_transform->getWorldPosition() - _player->_transform->getWorldPosition()));
		float distance1 = D3DXVec3Length(&(_cube[1]->_transform->getWorldPosition() - _player->_transform->getWorldPosition()));
		if (distance0 > distance1)
		{
			PHYSICSMANAGER->isBlocking(_cube[1], _cube[0]);
		}
		else
		{
			PHYSICSMANAGER->isBlocking(_cube[0], _cube[1]);
		}
	}
	//�ʰ� �÷��̾� �浹
	if (PHYSICSMANAGER->isOverlap(_player, _map->getObject()))
	{
		for (int i = 0; i < _map->getBoundBox().size(); i++)
		{
			PHYSICSMANAGER->isBlocking(_map->getObject()->_transform, &_map->getBoundBox()[i].box, _player->_transform, &_player->_boundBox);
		}
		for (int i = 0; i < _map->getbasicBox().size(); i++)
		{
			PHYSICSMANAGER->isBlocking(_map->getObject()->_transform, &_map->getbasicBox()[i].box, _player->_transform, &_player->_boundBox);
		}
	}
	if (PHYSICSMANAGER->isOverlap(_cube[0]->_transform, &_cube[0]->_boundBox, _cube[1]->_transform, &_cube[1]->_boundBox))
	{
		PHYSICSMANAGER->isBlocking(_cube[0]->_transform, &_cube[0]->_boundBox, _cube[1]->_transform, &_cube[1]->_boundBox);
	}
	//ť�� ��ư1 �浹
	if (PHYSICSMANAGER->isOverlap(_button[0]->_transform, &_button[0]->_boundBox, _cube[0]->_transform, &_cube[0]->_boundBox))
	{
		_button[0]->cubeCollision(_cube[0]);
	}
	else if (PHYSICSMANAGER->isOverlap(_button[0]->_transform, &_button[0]->_boundBox, _cube[1]->_transform, &_cube[1]->_boundBox))
	{
		_button[0]->cubeCollision(_cube[1]);
	}
	else
	{
		_button[0]->setState(BUTTON_UP);
	}
	//ť�� ��ư2 �浹
	if (PHYSICSMANAGER->isOverlap(_button[1]->_transform, &_button[1]->_boundBox, _cube[0]->_transform, &_cube[0]->_boundBox))
	{
		_button[1]->cubeCollision(_cube[0]);
	}
	else if (PHYSICSMANAGER->isOverlap(_button[1]->_transform, &_button[1]->_boundBox, _cube[1]->_transform, &_cube[1]->_boundBox))
	{
		_button[1]->cubeCollision(_cube[1]);
	}
	else
	{
		_button[1]->setState(BUTTON_UP);
	}

	if (PHYSICSMANAGER->isOverlap(_door->_transform, &_door->getCloseTrigger(), _player->_transform, &_player->_boundBox))
	{
		_cube[1]->_transform->movePositionSelf(0, 2, 0);
		if (_door->getState() != DOOR_CLOSE)
		{
			SOUNDMANAGER->stop("door_open");
			SOUNDMANAGER->play("door_close", 0.5f);
		}
		_door->setState(DOOR_CLOSE);
	}
	else if (_button[0]->getState() == BUTTON_DOWN && _button[1]->getState() == BUTTON_DOWN)
	{
		if (_door->getState() != DOOR_OPEN)
		{
			SOUNDMANAGER->play("door_open", 0.2f);
			SOUNDMANAGER->stop("door_close");
		}
		_door->setState(DOOR_OPEN);
	}
	else
	{
		if (_door->getState() != DOOR_CLOSE)
		{
			SOUNDMANAGER->stop("door_open");
			SOUNDMANAGER->play("door_close", 0.2f);
		}
		_door->setState(DOOR_CLOSE);
	}

	
	//���� ���̸� ���� �浹�� �Ѵ�
	if (_door->getState() == DOOR_CLOSE)
	{
		PHYSICSMANAGER->isBlocking(_door->_transform, &_door->_boundBox, _player->_transform, &_player->_boundBox);
	}
	//��� ��ư�� ������ ������ ������� ���� ����
	
	//��ư�� �÷��̾� �浹
	for (int i = 0; i < 2; i++)
	{
		if (PHYSICSMANAGER->isOverlap(_button[i]->_transform, &_button[i]->_boundBox, _player->_transform, &_player->_boundBox))
		{
			PHYSICSMANAGER->isBlocking(_button[i]->_transform, &_button[i]->_boundBox, _player->_transform, &_player->_boundBox);
			//�÷��̾ ��ư ���� �ö󰣴�
			_player->_transform->setWorldPosition(_player->_transform->getWorldPosition().x, _button[i]->_transform->getWorldPosition().y + 0.4f, _player->_transform->getWorldPosition().z);
			_player->setGravity(0);
		}
		else
		{
			if (_player->getGravity() == 0.2f) return;
			_player->setGravity(0.2f);
		}
	}
}

