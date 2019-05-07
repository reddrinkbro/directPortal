#pragma once

enum DOORSTATE
{
	DOOR_NONE,
	DOOR_OPEN,
	DOOR_CLOSE
};

class door : public baseObject
{
private:
	skinnedAnimation * _animation;
	DOORSTATE		   _state;
	boundBox		   _closeTrigger;
	boundBox		   _openTrigger;
public:
	HRESULT init(float x, float y, float z);
	void release(void);
	void update(void);
	void render(void);
public:
	DOORSTATE getState(void) { return _state; }
	void setState(DOORSTATE state) { _state = state; }
	boundBox getCloseTrigger(void) { return _closeTrigger; }
	boundBox getOpenTrigger(void) { return _openTrigger; }
	door() {};
	~door() {};
};

