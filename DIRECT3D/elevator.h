#pragma once

enum ELEVATORSTATE
{
	ELEVATOR_NONE,
	ELEVATOR_ARRIVE,
	ELEVATOR_WAIT,
	ELEVATOR_LEAVE
};


class elevator : public baseObject
{
private:
	skinnedAnimation * _animation;
	ELEVATORSTATE	   _state;
	boundBox		   _elevatorEvent;			//충돌 했을시 엘리베이터가 내려옴
	boundBox		   _afterClose;				//문 닫은후 충돌
	int				   _soundCount;				//사운드 소리 카운트
public:
	HRESULT init(float x ,float y, float z);
	void release(void);
	void update(void);
	void render(void);
public:
	ELEVATORSTATE getState(void) { return _state; }
	void setState(ELEVATORSTATE state) { _state = state; }
	boundBox getElevatorEventBox(void) { return _elevatorEvent; }
	boundBox getCloseBox(void) { return _afterClose; }
	elevator() {};
	~elevator() {};
};

