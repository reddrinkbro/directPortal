#include "stdafx.h"
#include "quadTree.h"

quadTree::quadTree()
{
	//���ϵ� ������ NULL ó��
	ZeroMemory(_childs, sizeof(quadTree*) * 4);
	//�����ε��� -1 �� �ʱ�ȭ
	_center = -1;
}
quadTree::~quadTree()
{
	//�ڽ� ���� �����
	SAFE_DELETE(_childs[0]);
	SAFE_DELETE(_childs[1]);
	SAFE_DELETE(_childs[2]);
	SAFE_DELETE(_childs[3]);
}

//���� Ʈ�� �ʱ�ȭ(Terrain ���� �迭,QuadTree �� ���� �Ѹ��� ����) 
HRESULT quadTree::init(terrain::LPTERRAINVERTEX vertices, DWORD verNumEdge)
{
	//Terrain ���� ������ ������
	_terrainVertices = vertices;

	//�� �ڳ� �ε��� ����Ѵ�.
	_corners[CORNER_LT] = 0;
	_corners[CORNER_RT] = verNumEdge - 1;
	_corners[CORNER_LB] = (verNumEdge - 1) * verNumEdge;
	_corners[CORNER_RB] = verNumEdge * verNumEdge - 1;

	//�ڽ� Ʈ�� �����.
	createChildTree();

	return S_OK;
}
//�ڽ� Ʈ�� �����
void quadTree::createChildTree()
{
	//�߽���ġ
	//���ڳ��� ������ ���� ��
	_centerPos = (_terrainVertices[_corners[0]].pos +
		_terrainVertices[_corners[1]].pos +
		_terrainVertices[_corners[2]].pos +
		_terrainVertices[_corners[3]].pos) * 0.25f;

	//�ƹ��� �ϳ��� �Ÿ��� �������̴�.
	D3DXVECTOR3 dir = _terrainVertices[_corners[0]].pos - _centerPos;
	float prevLengthSq = D3DXVec3LengthSq(&dir);
	_radius = sqrt(prevLengthSq);

	//�ڽ��� �ִٸ� ����ؾ� �Ѵ�
	if ((_corners[CORNER_RT] - _corners[CORNER_LT]) > 1)
	{
		//���� �ε��� ���
		_center = (_corners[0] + _corners[1] + _corners[2] + _corners[3]) / 4;

		DWORD topCenter = (_corners[CORNER_RT] + _corners[CORNER_LT]) / 2;		//��� �߾�
		DWORD leftCenter = (_corners[CORNER_LT] + _corners[CORNER_LB]) / 2;		//�� �߾�
		DWORD rightCenter = (_corners[CORNER_RT] + _corners[CORNER_RB]) / 2;	//�� �߾�
		DWORD bottomCenter = (_corners[CORNER_RB] + _corners[CORNER_LB]) / 2;	//�� �߾�

																				//�»�� �ڽ�
		_childs[CORNER_LT] = new quadTree;
		_childs[CORNER_LT]->_corners[CORNER_LT] = _corners[CORNER_LT];
		_childs[CORNER_LT]->_corners[CORNER_RT] = topCenter;
		_childs[CORNER_LT]->_corners[CORNER_LB] = leftCenter;
		_childs[CORNER_LT]->_corners[CORNER_RB] = _center;
		_childs[CORNER_LT]->_terrainVertices = _terrainVertices;
		_childs[CORNER_LT]->createChildTree();

		//���� �ڽ�
		_childs[CORNER_RT] = new quadTree;
		_childs[CORNER_RT]->_corners[CORNER_LT] = topCenter;
		_childs[CORNER_RT]->_corners[CORNER_RT] = _corners[CORNER_RT];
		_childs[CORNER_RT]->_corners[CORNER_LB] = _center;
		_childs[CORNER_RT]->_corners[CORNER_RB] = rightCenter;
		_childs[CORNER_RT]->_terrainVertices = _terrainVertices;
		_childs[CORNER_RT]->createChildTree();

		//���ϴ� �ڽ�
		_childs[CORNER_LB] = new quadTree;
		_childs[CORNER_LB]->_corners[CORNER_LT] = leftCenter;
		_childs[CORNER_LB]->_corners[CORNER_RT] = _center;
		_childs[CORNER_LB]->_corners[CORNER_LB] = _corners[CORNER_LB];
		_childs[CORNER_LB]->_corners[CORNER_RB] = bottomCenter;
		_childs[CORNER_LB]->_terrainVertices = _terrainVertices;
		_childs[CORNER_LB]->createChildTree();

		//���ϴ� �ڽ�
		_childs[CORNER_RB] = new quadTree;
		_childs[CORNER_RB]->_corners[CORNER_LT] = _center;
		_childs[CORNER_RB]->_corners[CORNER_RT] = rightCenter;
		_childs[CORNER_RB]->_corners[CORNER_LB] = bottomCenter;
		_childs[CORNER_RB]->_corners[CORNER_RB] = _corners[CORNER_RB];
		_childs[CORNER_RB]->_terrainVertices = _terrainVertices;
		_childs[CORNER_RB]->createChildTree();
	}
}
//���� �浹������ ��´�
void quadTree::getRayHits(vector<D3DXVECTOR3>* pHits, const LPRay pRay)
{
	//���� �ٿ��� �浹�ߴ�?
	if (PHYSICSMANAGER->isRayHitSphere(pRay, &_centerPos, _radius, NULL, NULL))
	{
		//�ڽ����մ�?
		if ((_corners[CORNER_RT] - _corners[CORNER_LT]) > 1)
		{
			//�ڽ� ���
			for (int i = 0; i < 4; i++)
			{
				_childs[i]->getRayHits(pHits, pRay);
			}
		}
		//�ڽ��� ���� �Ǹ����� ����?
		else
		{
			//Tri 2�� ��´�.

			// lt---rt
			//  |  /|
			//  | / |
			//  |/  |
			// lb---rb

			//���� ������� Ray �� Hit ������ ����
			float dist = 0.0f;

			D3DXVECTOR3 lt = _terrainVertices[_corners[CORNER_LT]].pos;
			D3DXVECTOR3 rt = _terrainVertices[_corners[CORNER_RT]].pos;
			D3DXVECTOR3 lb = _terrainVertices[_corners[CORNER_LB]].pos;
			D3DXVECTOR3 rb = _terrainVertices[_corners[CORNER_RB]].pos;

			//�»�� ������ üũ
			if (D3DXIntersectTri(
				&lt, &rt, &lb,		//������ ���� 3 ��ġ (�η�� ���� ����)
				&pRay->origin,		//���� ��ġ
				&pRay->direction,	//���� ����
				NULL, NULL,
				&dist				//��Ʈ�� �Ǿ��ٸ� Origin ���� ���� hit ��ġ������ �Ÿ��� ���´�.
			))
			{
				//��Ʈ����
				D3DXVECTOR3 hitPos = pRay->origin + pRay->direction * dist;
				//Ǫ��
				pHits->push_back(hitPos);
				return;
			}

			//���ϴ� ������ üũ
			if (D3DXIntersectTri(
				&lb, &rt, &rb,		//������ ���� 3 ��ġ
				&pRay->origin,		//���� ��ġ
				&pRay->direction,	//���� ����
				NULL, NULL,
				&dist				//��Ʈ�� �Ǿ��ٸ� Origin ���� ���� hit ��ġ������ �Ÿ��� ���´�.
			))
			{
				//��Ʈ����
				D3DXVECTOR3 hitPos = pRay->origin + pRay->direction * dist;
				//Ǫ��
				pHits->push_back(hitPos);
				return;
			}
		}
	}
}


