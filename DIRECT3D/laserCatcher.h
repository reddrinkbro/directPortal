#pragma once
#include "baseObject.h"

enum CATCHERSTATE
{
	CATCHERSTATE_NONE,
	CATCHERSTATE_ON
};
class laserCatcher : public baseObject
{
private:
	skinnedAnimation * _animation;
	CATCHERSTATE	   _state;
	boundBox		   _hitBox;		//�������� �´� �κ�
public:
	HRESULT init(float x, float y, float z);
	void release(void);
	void update(void);
	void render(void);
public:
	CATCHERSTATE getState(void) { return _state; }
	boundBox getHitBox(void) { return _hitBox; }
	void setState(CATCHERSTATE state) { _state = state; }
	laserCatcher() {};
	~laserCatcher() {};
};

