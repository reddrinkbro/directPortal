#pragma once
#include "gameNode.h"
#include "VMR9Graph.h"

class endingScene : public gameNode
{
private:
	CVMR9Graph _graph;
	int		   _Layer;
	float	   _Left;
	float	   _Top;
	float	   _Right;
	float	   _Bottom;
	int		   _alpha;
	int		   _playTime;
public:
	HRESULT init(void);
	void release(void);
	void render(void);
	endingScene() {};
	~endingScene() {};
};

