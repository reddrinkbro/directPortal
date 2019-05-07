#pragma once

class boundSphere
{
public:
	D3DXVECTOR3		_localCenter;	// ���� ������ǥ
	D3DXVECTOR3		_halfSize;		// ���� ������
	float			_radius;		//  ������


public:
	// ��������� ���Ϳ� ������ ���
	void getWorldCenterRadius(transform* trans, D3DXVECTOR3* center, float* radius);
	// ���� ������ �׸���
	virtual void renderGizmo(transform* trans);

	//  �ٿ�� �ڽ� ����
	virtual void setBound(D3DXVECTOR3* center, D3DXVECTOR3* halfSize);

	void setLocalCenter(D3DXVECTOR3 localCenter) { _localCenter = localCenter; }
	void setHalfSize(D3DXVECTOR3 halfSize) { _halfSize = halfSize; }


	boundSphere() {}
	~boundSphere() {}
};

