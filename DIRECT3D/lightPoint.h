#pragma once
#include "light.h"
class lightPoint : public light
{
public:
	float _minRange;		// ���� �ּҰŸ�
	float _maxRange;		// ���� �ִ�Ÿ�
	float _distancePow;	// �Ÿ� ������

public:
	// ����Ʈ ��� ���
	virtual D3DXMATRIXA16 getLightMatrix();
	// ����Ʈ ����� �׸���.
	virtual void renderGizmo();

	lightPoint() {}
	~lightPoint() {}
};