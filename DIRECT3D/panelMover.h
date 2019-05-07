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
	boundBox   _exclusivePlayer;	//�÷��̾� ���� �浹 �ڽ�
	float	   _lerpPos;			//lerp�� ����� ����
	int		   _stopCount;			//�����ִ� �ð�
	bool	   _isStart;			//���� ��ŸƮ ������ �ִ°�
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

