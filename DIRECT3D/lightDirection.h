#pragma once
#include "light.h"
class lightDirection : public light
{
public:
	// ����Ʈ ��� ���
	virtual D3DXMATRIXA16 getLightMatrix();
	// ����Ʈ ����� �׸���.
	virtual void renderGizmo();

	lightDirection() {}
	~lightDirection() {}
};