#pragma once
#include "baseObject.h"

enum MOVERSTATE
{
	MOVERSTATE_NONE,
	MOVERSTATE_STOP,
	MOVERSTATE_MOVE
};

class panelMover : public baseObject
{
private:
	MOVERSTATE _state;
	boundBox   _exclusivePlayer;	//플레이어 전용 충돌 박스
	float	   _lerpPos;			//lerp에 사용할 변수
	int		   _stopCount;			//멈춰있는 시간
	bool	   _isStart;			//현재 스타트 지점에 있는가
public:
	HRESULT init(float x, float y, float z);
	void release(void);
	void update(transform& start, transform& end, int time);
	void render(void);
	//Getter Setter
public:
	MOVERSTATE getState(void) { return _state; }
	void setState(MOVERSTATE state) { _state = state; }
	boundBox getPlayerBoundBox(void) { return _exclusivePlayer; }
	panelMover() {};
	~panelMover() {};
};

