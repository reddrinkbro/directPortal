#pragma once
#include "baseObject.h"

class laserEmitter : public baseObject
{
public:
	HRESULT init(float x, float y, float z);
	void release(void);
	void render(void);
public:
	laserEmitter() {};
	~laserEmitter() {};
};

