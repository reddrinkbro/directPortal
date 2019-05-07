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
	boundBox		   _elevatorEvent;			//�浹 ������ ���������Ͱ� ������
	boundBox		   _afterClose;				//�� ������ �浹
	int				   _soundCount;				//���� �Ҹ� ī��Ʈ
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

