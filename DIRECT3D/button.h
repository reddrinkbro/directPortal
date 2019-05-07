#pragma once

class cube;

enum BUTTONSTATE
{
	BUTTON_NONE,
	BUTTON_UP,
	BUTTON_DOWN
};

class button : public baseObject
{
private:
	skinnedAnimation * _animation;
	BUTTONSTATE		  _state;
	cube*		      _cube;
public:
	HRESULT init(float x, float y, float z);
	void release(void);
	void update(void);
	void render(void);
	void cubeCollision(cube* cube);
private:
	void cubeMemoryLink(cube* cube) { _cube = cube; }
public:
	BUTTONSTATE getState(void) { return _state; }
	void setState(BUTTONSTATE state) { _state = state; }
	button() {};
	~button() {};
};

