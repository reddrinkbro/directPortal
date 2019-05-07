#pragma once
#include "singletonBase.h"

#define CIRCLEGIZMOMAX 36

class gizmoManager : public singletonBase <gizmoManager>
{
private :
	typedef struct tagGizmoVertex
	{
		D3DXVECTOR3 pos;	//  ������ ��ġ
		DWORD		color;	//  ������ �÷�
		enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };
	}GIZMOVERTEX, *LPGIZMOVERTEX;

private :
	D3DXVECTOR3		_circlePos[CIRCLEGIZMOMAX + 1];		//  ���� ���� ��ġ
	


public:
	HRESULT		init(void);
	void		release(void);
	
	
	
	//  ���� �׸��� �Լ�
	void lineMake(D3DXVECTOR3& startPos, D3DXVECTOR3& endPos, DWORD color = 0xff808080);

	//  �׸��� �׸��� �Լ�
	void gridMake(float cellSize, int cellNum);

	//  ���� �׸��� �Լ�
	void circleMake(D3DXVECTOR3& centerPos, float radius, D3DXVECTOR3& axis = D3DXVECTOR3(0, 0, 1), DWORD color = 0xffff00ff);

	//  ���̾� ���� �׸��� �Լ�
	void sphereMake(D3DXVECTOR3& centerPos, float radius, DWORD color = 0xffffff00);

	//  ���� AABB�� �׸��� �Լ�
	void aabbMake(D3DXVECTOR3& minPos, D3DXVECTOR3& maxPos, DWORD color = 0xffff0000);

	gizmoManager() {};
	~gizmoManager() {};



};

