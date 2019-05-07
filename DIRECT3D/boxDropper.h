#pragma once

enum DROPPERSTATE
{
	DROPPER_NONE,
	DROPPER_OPEN,
	DROPPER_CLOSE
};

class boxDropper : public baseObject
{
private:
	skinnedAnimation * _animation;
	DROPPERSTATE	   _state;
	float			   _closeTime;
public:
	HRESULT init(float x, float y ,float z);
	void release(void);
	void update(void);
	void render(void);
//Getter Setter
public:
	DROPPERSTATE getState(void) { return _state; }
	void setState(DROPPERSTATE state) { _state = state; }
	boxDropper() {};
	~boxDropper() {};
};

