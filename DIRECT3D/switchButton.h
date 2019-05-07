#pragma once
#include "baseObject.h"

class player;

enum SWITCHSTATE
{
	SWITCH_NONE,
	SWITCH_ON
};

class switchButton : public baseObject
{
private:
	skinnedAnimation * _animation;
	SWITCHSTATE		   _state;
	player*			   _player;
	boundBox		   _exclusivePlayer;
public:
	HRESULT init(float x, float y ,float z);
	void release(void);
	void update(void);
	void render(void);
public:
	void setPlayerMemoryLink(player* player) { _player = player; }
public:
	boundBox getPlayerBoundBox(void) { return _exclusivePlayer; }
	SWITCHSTATE getState(void) { return _state; }
	void setState(SWITCHSTATE state) { _state = state; }
	boundBox getButtonBox(void) { return _boundBox; }
	switchButton() {};
	~switchButton() {};
};

