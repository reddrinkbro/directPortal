#include "stdafx.h"
#include "boundBox.h"

void boundBox::getWorldBox(transform * trans, D3DXVECTOR3 * outBoxPos)
{
	//    5-------6
	//   /|      /|
	//  1-------2 |
	//  | 4-----|-7
	//  |/      |/
	//  0-------3 

	// ���� 8 ���� ������ ���Ѵ�
	D3DXVECTOR3 vertices[8];
	vertices[0] = D3DXVECTOR3(_localMinPos.x, _localMinPos.y, _localMinPos.z);
	vertices[1] = D3DXVECTOR3(_localMinPos.x, _localMaxPos.y, _localMinPos.z);
	vertices[2] = D3DXVECTOR3(_localMaxPos.x, _localMaxPos.y, _localMinPos.z);
	vertices[3] = D3DXVECTOR3(_localMaxPos.x, _localMinPos.y, _localMinPos.z);
	vertices[4] = D3DXVECTOR3(_localMinPos.x, _localMinPos.y, _localMaxPos.z);
	vertices[5] = D3DXVECTOR3(_localMinPos.x, _localMaxPos.y, _localMaxPos.z);
	vertices[6] = D3DXVECTOR3(_localMaxPos.x, _localMaxPos.y, _localMaxPos.z);
	vertices[7] = D3DXVECTOR3(_localMaxPos.x, _localMinPos.y, _localMaxPos.z);

	D3DXMATRIXA16 matFinal = trans->getFinalMatrix();

	for (int i = 0; i < 8; i++)
	{
		D3DXVec3TransformCoord(outBoxPos + i, vertices + i, &matFinal);
	}
}

void boundBox::getWorldAABBMinMax(transform * trans, D3DXVECTOR3 * min, D3DXVECTOR3 * max)
{
	// ��ȯ�� �ڽ��� ������ġ 8���� ��´�
	D3DXVECTOR3 worldPos[8];
	this->getWorldBox(trans, worldPos);

	// ���� 8���� �ּ�, �ִ븦 ���ϱ�
	*min = worldPos[0];
	*max = worldPos[0];

	for (int i = 1; i < 8; i++)
	{
		if (min->x > worldPos[i].x) min->x = worldPos[i].x;
		else if (max->x < worldPos[i].x) max->x = worldPos[i].x;

		if (min->y > worldPos[i].y) min->y = worldPos[i].y;
		else if (max->y < worldPos[i].y) max->y = worldPos[i].y;

		if (min->z > worldPos[i].z) min->z = worldPos[i].z;
		else if (max->z < worldPos[i].z) max->z = worldPos[i].z;
	}
}

void boundBox::renderGizmo(transform * trans)
{
	// 		5--------6
	//      /|       /|
	//     1--------2 |
	//     | |      | | 
	//     | 4------| 7
	//     |/       |/
	//     0--------3

	// ��ȯ�� �ڽ��� ������ġ 8���� ��´�
	D3DXVECTOR3 worldPos[8];
	this->getWorldBox(trans, worldPos);

	// �ڽ��� �׸���
	GIZMOMANAGER->lineMake(worldPos[0], worldPos[1], 0xff00ff00);
	GIZMOMANAGER->lineMake(worldPos[1], worldPos[2], 0xff00ff00);
	GIZMOMANAGER->lineMake(worldPos[2], worldPos[3], 0xff00ff00);
	GIZMOMANAGER->lineMake(worldPos[3], worldPos[0], 0xff00ff00);

	GIZMOMANAGER->lineMake(worldPos[4], worldPos[5], 0xff00ff00);
	GIZMOMANAGER->lineMake(worldPos[5], worldPos[6], 0xff00ff00);
	GIZMOMANAGER->lineMake(worldPos[6], worldPos[7], 0xff00ff00);
	GIZMOMANAGER->lineMake(worldPos[7], worldPos[4], 0xff00ff00);

	GIZMOMANAGER->lineMake(worldPos[0], worldPos[4], 0xff00ff00);
	GIZMOMANAGER->lineMake(worldPos[1], worldPos[5], 0xff00ff00);
	GIZMOMANAGER->lineMake(worldPos[2], worldPos[6], 0xff00ff00);
	GIZMOMANAGER->lineMake(worldPos[3], worldPos[7], 0xff00ff00);

	D3DXVECTOR3 min;
	D3DXVECTOR3 max;
	// ���� 8���� �ּ�, �ִ� ���ϱ�
	min = worldPos[0];
	max = worldPos[0];

	for (int i = 1; i < 8; i++)
	{
		if (min.x > worldPos[i].x) min.x = worldPos[i].x;
		else if (max.x < worldPos[i].x) max.x = worldPos[i].x;

		if (min.y > worldPos[i].y) min.y = worldPos[i].y;
		else if (max.y < worldPos[i].y) max.y = worldPos[i].y;

		if (min.z > worldPos[i].z) min.z = worldPos[i].z;
		else if (max.z < worldPos[i].z) max.z = worldPos[i].z;
	}

	// AABB �׸���.
	GIZMOMANAGER->aabbMake(min, max, 0xff008800);
}

void boundBox::setBound(D3DXVECTOR3 * center, D3DXVECTOR3 * halfSize)
{
	boundSphere::setBound(center, halfSize);

	_localMinPos = _localCenter - _halfSize;
	_localMaxPos = _localCenter + _halfSize;
}

