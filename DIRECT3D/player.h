#pragma once
#include "skinnedAnimation.h"
#include "menuScene.h"
#include "crosshair.h"

class portalGun;
class cube;

//현재 어떤 오브젝트와 충돌 중인지
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
	float			   _speed;									//스피드
	float			   _gravity;								//중력값
	bool			   _isEquip;								//포탈건을 획득했는지
	bool			   _isUpStairs;								//계단을 오르는지
	bool			   _isCarry;								//옮기는 중인지
	bool			   _isJump;									//점프 하는 중인지
	float			   _jumpTime;								//점프 시간
	float			   _jumpMaxForce;							//점프 최대거리
	float			   _jumpAccelate;							//점프 가속
	D3DXVECTOR3		   _jumpVector;								//점프 벡터
public:
	HRESULT init(float x, float y, float z);
	void release(void);
	void update(float x, float y);
	void render(void);
public:
	void control(float x, float y);								//플레이어 컨트롤
	void playerPortalGunCollision(void);						//플레이어 포탈건 충돌
	void cubeCollision(cube* cube);								//큐브 충돌
	void jump(void);										//포탈이동
public:
	void setPortalGunMemoryLink(portalGun* gun) { _gun = gun; }	//포탈건 가져오기
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

