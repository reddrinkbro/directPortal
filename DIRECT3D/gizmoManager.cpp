#include "stdafx.h"
#include "gizmoManager.h"

HRESULT gizmoManager::init(void)
{
	//  ���� ���� �⺻ �غ�
	float intervalAngle = (D3DX_PI * 2.0f) / CIRCLEGIZMOMAX;

	//  ���� �׸��� ���� ���� ��ġ ����
	for (int i = 0; i <= CIRCLEGIZMOMAX; i++)
	{
		float angle = i * intervalAngle;
		_circlePos[i] = D3DXVECTOR3(cosf(angle), sinf(angle), 0.0f);
	}

	return S_OK;
}

void gizmoManager::release(void)
{
}

void gizmoManager::lineMake(D3DXVECTOR3 & startPos, D3DXVECTOR3 & endPos, DWORD color)
{
	// ������ ������� ����ϱ�
	D3DXMATRIXA16 matPrevWorld;
	device->GetTransform(D3DTS_WORLD, &matPrevWorld);

	// ������� �ʱ�ȭ
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);
	device->SetTransform(D3DTS_WORLD, &matWorld);

	// ������ ����Ʈ ��� ����ϱ�
	DWORD prevLight = 0;
	device->GetRenderState(D3DRS_LIGHTING, &prevLight);

	// ����Ʈ ��� ����
	device->SetRenderState(D3DRS_LIGHTING, FALSE);

	// �ӽ� ���� �迭 2��
	GIZMOVERTEX vertices[2];
	vertices[0].pos = startPos;
	vertices[1].pos = endPos;
	vertices[0].color = color;
	vertices[1].color = color;

	// ���� �׸���
	device->SetFVF(GIZMOVERTEX::FVF);
	device->DrawPrimitiveUP(D3DPT_LINELIST, 1, vertices, sizeof(GIZMOVERTEX));

	// �̺κ��� �����ִ� �����غ���
	// ���� �ٽ� ���󺹱�
	device->SetTransform(D3DTS_WORLD, &matPrevWorld);
	// ����Ʈ ��� ���󺹱�
	device->SetRenderState(D3DRS_LIGHTING, prevLight);
}

void gizmoManager::gridMake(float cellSize, int cellNum)
{
	// ���� ���� ����
	float axisLineLength = cellSize * cellNum;

	// x�� ����
	D3DXVECTOR3 xStart(-axisLineLength, 0, 0);
	D3DXVECTOR3 xEnd(axisLineLength, 0, 0);

	// y�� ����
	D3DXVECTOR3 yStart(0, -axisLineLength, 0);
	D3DXVECTOR3 yEnd(0, axisLineLength, 0);

	// z�� ����
	D3DXVECTOR3 zStart(0, 0, -axisLineLength);
	D3DXVECTOR3 zEnd(0, 0, axisLineLength);

	// ��׸���
	this->lineMake(xStart, xEnd, 0xffff0000); // ��
	this->lineMake(yStart, yEnd, 0xff00ff00); // ��
	this->lineMake(zStart, zEnd, 0xff0000ff); // ��

											  // ��ȭ��ǥ
	D3DXVECTOR3 arrowPos[4];
	arrowPos[0] = D3DXVECTOR3(-1.0f, 0.0f, -1.0f);
	arrowPos[1] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	arrowPos[2] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	arrowPos[3] = D3DXVECTOR3(1.0f, 0.0f, -1.0f);

	D3DXVECTOR3 arrowVertices[4];

	D3DXMATRIXA16 matTrans;
	D3DXMatrixTranslation(&matTrans, zEnd.x, zEnd.y, zEnd.z);
	D3DXMATRIXA16 matRotation;
	D3DXMATRIXA16 matFinal = matTrans;

	// z��ȯ
	for (int i = 0; i < 4; i++)
	{
		D3DXVec3TransformCoord(&arrowVertices[i], &arrowPos[i], &matTrans);
	}
	// �׸���
	this->lineMake(arrowVertices[0], arrowVertices[1], 0xff0000ff);
	this->lineMake(arrowVertices[2], arrowVertices[3], 0xff0000ff);

	// x��
	D3DXMatrixTranslation(&matTrans, xEnd.x, xEnd.y, xEnd.z);
	D3DXMatrixRotationY(&matRotation, D3DXToRadian(90));
	matFinal = matRotation * matTrans;

	// x��ȯ
	for (int i = 0; i < 4; i++)
	{
		D3DXVec3TransformCoord(&arrowVertices[i], &arrowPos[i], &matFinal);
	}
	// �׸���
	this->lineMake(arrowVertices[0], arrowVertices[1], 0xffff0000);
	this->lineMake(arrowVertices[2], arrowVertices[3], 0xffff0000);

	// y��
	D3DXMatrixTranslation(&matTrans, yEnd.x, yEnd.y, yEnd.z);
	D3DXMatrixRotationX(&matRotation, D3DXToRadian(-90));
	matFinal = matRotation * matTrans;
	// y��ȯ
	for (int i = 0; i < 4; i++)
	{
		D3DXVec3TransformCoord(&arrowVertices[i], &arrowPos[i], &matFinal);
	}
	// �׸���
	this->lineMake(arrowVertices[0], arrowVertices[1], 0xff00ff00);
	this->lineMake(arrowVertices[2], arrowVertices[3], 0xff00ff00);

	// ���� �׸��� �׸���
	for (int i = -cellNum; i <= cellNum; i++)
	{
		// 0�� �ѱ��
		if (i == 0) continue;

		D3DXVECTOR3 posStartX(i * cellSize, 0, -axisLineLength);
		D3DXVECTOR3 posEndX(i * cellSize, 0, axisLineLength);

		this->lineMake(posStartX, posEndX);

		D3DXVECTOR3 posStartZ(-axisLineLength, 0, i * cellSize);
		D3DXVECTOR3 posEndZ(axisLineLength, 0, i * cellSize);

		this->lineMake(posStartZ, posEndZ);
	}
}

void gizmoManager::circleMake(D3DXVECTOR3 & centerPos, float radius, D3DXVECTOR3 & axis, DWORD color)
{

	// ������ ������� ����ϱ�
	D3DXMATRIXA16 matPrevWorld;
	device->GetTransform(D3DTS_WORLD, &matPrevWorld);

	// ������ ����Ʈ ��� ����ϱ�
	DWORD prevLight = 0;
	device->GetRenderState(D3DRS_LIGHTING, &prevLight);

	// ����Ʈ ��� ����
	device->SetRenderState(D3DRS_LIGHTING, FALSE);

	// �ӽ� ���� �迭 2��
	GIZMOVERTEX vertices[CIRCLEGIZMOMAX + 1];
	for (int i = 0; i < CIRCLEGIZMOMAX + 1; i++)
	{
		vertices[i].pos = _circlePos[i];
		vertices[i].color = color;
	}

	// ���� ���� ��� �غ�
	// ȸ���غ�
	D3DXVECTOR3 forward = axis;
	D3DXVECTOR3 right;
	D3DXVECTOR3 up(0, 1, 0);
	if (forward == up) up = D3DXVECTOR3(0, 0, -1);
	else if (forward == D3DXVECTOR3(0, -1, 0)) up = D3DXVECTOR3(0, 0, 1);

	// ������ ������ ���� �κ����� ������ �������͸� �˼� �ִ�
	D3DXVec3Cross(&right, &up, &forward);
	D3DXVec3Cross(&up, &forward, &right);

	// �� ������
	forward = forward * radius;
	right = right * radius;
	up = up * radius;

	// ������� �ʱ�ȭ
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);

	memcpy(&matWorld._11, right, sizeof(D3DXVECTOR3));
	memcpy(&matWorld._21, up, sizeof(D3DXVECTOR3));
	memcpy(&matWorld._31, forward, sizeof(D3DXVECTOR3));
	memcpy(&matWorld._41, centerPos, sizeof(D3DXVECTOR3));

	device->SetTransform(D3DTS_WORLD, &matWorld);

	// ���� �׸���
	device->SetFVF(GIZMOVERTEX::FVF);
	device->DrawPrimitiveUP(D3DPT_LINESTRIP, CIRCLEGIZMOMAX, vertices, sizeof(GIZMOVERTEX));

	// ���� �ٽ� ���󺹱�
	device->SetTransform(D3DTS_WORLD, &matPrevWorld);
	// ����Ʈ ��� ���󺹱�
	device->SetRenderState(D3DRS_LIGHTING, prevLight);
}

void gizmoManager::sphereMake(D3DXVECTOR3 & centerPos, float radius, DWORD color)
{
	circleMake(centerPos, radius, D3DXVECTOR3(1, 0, 0), color);
	circleMake(centerPos, radius, D3DXVECTOR3(0, 1, 0), color);
	circleMake(centerPos, radius, D3DXVECTOR3(0, 0, 1), color);
}

void gizmoManager::aabbMake(D3DXVECTOR3 & minPos, D3DXVECTOR3 & maxPos, DWORD color)
{

	// 		5--------6
	//      /|       /|
	//     4--------7 |
	//     | |  *   | |     ==> ����(0, 0, 0)
	//     | 1------| 2
	//     |/       |/
	//     0--------3
	lineMake(D3DXVECTOR3(minPos.x, minPos.y, minPos.z), D3DXVECTOR3(maxPos.x, minPos.y, minPos.z), color);
	lineMake(D3DXVECTOR3(maxPos.x, minPos.y, minPos.z), D3DXVECTOR3(maxPos.x, maxPos.y, minPos.z), color);
	lineMake(D3DXVECTOR3(maxPos.x, maxPos.y, minPos.z), D3DXVECTOR3(minPos.x, maxPos.y, minPos.z), color);
	lineMake(D3DXVECTOR3(minPos.x, maxPos.y, minPos.z), D3DXVECTOR3(minPos.x, minPos.y, minPos.z), color);

	lineMake(D3DXVECTOR3(minPos.x, minPos.y, maxPos.z), D3DXVECTOR3(maxPos.x, minPos.y, maxPos.z), color);
	lineMake(D3DXVECTOR3(maxPos.x, minPos.y, maxPos.z), D3DXVECTOR3(maxPos.x, maxPos.y, maxPos.z), color);
	lineMake(D3DXVECTOR3(maxPos.x, maxPos.y, maxPos.z), D3DXVECTOR3(minPos.x, maxPos.y, maxPos.z), color);
	lineMake(D3DXVECTOR3(minPos.x, maxPos.y, maxPos.z), D3DXVECTOR3(minPos.x, minPos.y, maxPos.z), color);

	lineMake(D3DXVECTOR3(minPos.x, minPos.y, minPos.z), D3DXVECTOR3(minPos.x, minPos.y, maxPos.z), color);
	lineMake(D3DXVECTOR3(maxPos.x, minPos.y, minPos.z), D3DXVECTOR3(maxPos.x, minPos.y, maxPos.z), color);
	lineMake(D3DXVECTOR3(maxPos.x, maxPos.y, minPos.z), D3DXVECTOR3(maxPos.x, maxPos.y, maxPos.z), color);
	lineMake(D3DXVECTOR3(minPos.x, maxPos.y, minPos.z), D3DXVECTOR3(minPos.x, maxPos.y, maxPos.z), color);
}
