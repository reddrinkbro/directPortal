#pragma once
#include "terrain.h"

class quadTree
{
private:
	//�� �ڳ� �� �ε��� ���
	enum CORNER { CORNER_LT = 0, CORNER_RT, CORNER_LB, CORNER_RB };
	terrain::LPTERRAINVERTEX _terrainVertices;	// �ڽ��� �����ִ� �ͷ����� ���� ����

	DWORD		_corners[4];	// �ڽ��� ���� Ʈ���� ���ڳ� ���� �ε���
	DWORD		_center;		// �ڽ��� ���� Ʈ���� �߾� ���� �ε���	(�̰� -1 �̶�� �ڽľ���)
	quadTree*	_childs[4];		// �ڽ��� �ڽ� ���� Ʈ��	(�ڽ��� ������ ����� �� NULL)

	D3DXVECTOR3 _centerPos;		// �ڽ��� ����Ʈ�� �߽� ��ġ
	float		_radius;		// �ڽ��� ����Ʈ�� ���� ������

public:
	quadTree();
	~quadTree();

	// ���� Ʈ�� �ʱ�ȭ(Terrain ���� �迭,QuadTree �� ���� �Ѹ��� ����) 
	HRESULT init(terrain::LPTERRAINVERTEX vertices, DWORD verNumEdge);
	// �ڽ� Ʈ�� �����
	void createChildTree();
	// ���� �浹������ ��´�
	void getRayHits(vector<D3DXVECTOR3>* pHits, const LPRay pRay);
};

