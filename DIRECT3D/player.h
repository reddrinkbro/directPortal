#pragma once
#include "skinnedAnimation.h"
#include "menuScene.h"
#include "crosshair.h"

class portalGun;
class cube;

//���� � ������Ʈ�� �浹 ������
enum PLAYERCOLLIDE
{
	C_NONE,
	C_SWITCH,
	C_CUBE
};

class player : public baseObject
{
private:
	PLAYERCOLLIDE	   _isCollide;
	portalGun*		   _gun;
	cube*			   _cube;
	menuScene*		   _menu;
	crosshair*		   _crosshair;
	float			   _speed;									//���ǵ�
	float			   _gravity;								//�߷°�
	bool			   _isEquip;								//��Ż���� ȹ���ߴ���
	bool			   _isUpStairs;								//����� ��������
	bool			   _isCarry;								//�ű�� ������
	bool			   _isJump;									//���� �ϴ� ������
	float			   _jumpTime;								//���� �ð�
	float			   _jumpMaxForce;							//���� �ִ�Ÿ�
	float			   _jumpAccelate;							//���� ����
	D3DXVECTOR3		   _jumpVector;								//���� ����
public:
	HRESULT init(float x, float y, float z);
	void release(void);
	void update(float x, float y);
	void render(void);
public:
	void control(float x, float y);								//�÷��̾� ��Ʈ��
	void playerPortalGunCollision(void);						//�÷��̾� ��Ż�� �浹
	void cubeCollision(cube* cube);								//ť�� �浹
	void jump(void);										//��Ż�̵�
public:
	void setPortalGunMemoryLink(portalGun* gun) { _gun = gun; }	//��Ż�� ��������
	void setCubeMemoryLink(cube* cube) { _cube = cube; }
	//Getter Setter
public:
	PLAYERCOLLIDE getIsCollide(void) { return _isCollide; }
	void setIsCollide(PLAYERCOLLIDE isCollide) { _isCollide = isCollide; }
	bool getIsCarry(void) { return _isCarry; }
	void setIsCarry(bool isCarry) { _isCarry = isCarry; }
	bool getIsEquip(void) { return _isEquip; }
	void setIsEquip(bool isEquip) { _isEquip = isEquip; }
	bool getIsUpStairs() { return _isUpStairs; }
	void setIsUpStairs(bool isUpStairs) { _isUpStairs = isUpStairs; }
	float getSpeed(void) { return _speed; }
	void setSpeed(float speed) { _speed = speed; }
	float getGravity(void) { return _gravity; }
	void setGravity(float gravity) { _gravity = gravity; }
	menuScene* getMenuScene(void) { return _menu; }
	crosshair* getCrosshair(void) { return _crosshair; }
public:
	player();
	~player();
};

