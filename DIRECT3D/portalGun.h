#pragma once
#include "portalBullet.h"
#include "particleEmitter.h"
enum GUNSTATE
{
	GUN_NONE,
	GUN_IDLE,
	GUN_FIRE,
	GUN_CARRY
};

enum BULLETCOLOR
{
	BULLET_NONE,
	BULLET_BLUE,
	BULLET_ORANGE
};

class portalGun : public baseObject
{
private:
	skinnedAnimation * _animation;
	portalBullet*	   _bullet;
	GUNSTATE		   _state;
	BULLETCOLOR		   _color;
	float			   _idleChange;			//idle 상태로 바꾸기 위한 시간
	particleEmitter *  _blueBullet;			//파랑포탈 발사 파티클
	particleEmitter *  _orangeBullet;		//주황포탈 발사 파티클
	bool			   _isHit;				//총알이 벽에 부딪혔는지
public:
	HRESULT init(float x ,float y ,float z);
	HRESULT effectInit(void);
	void release(void);
	void update(void);
	void render(void);
//Getter Setter
public:
	GUNSTATE getState(void) { return _state; }
	void setState(GUNSTATE state) { _state = state; }
	BULLETCOLOR getColor(void) { return _color; }
	void setColor(BULLETCOLOR color) { _color = color; }
	portalBullet* getBullet(void) { return _bullet; }
	particleEmitter * getBlueParticle(void) { return _blueBullet; }
	particleEmitter * getOrangeParticle(void) { return _orangeBullet; }
	bool getIsHit(void) { return _isHit; }
	void setIsHit(bool isHit) { _isHit = isHit; }
	portalGun() {};
	~portalGun() {};
};

