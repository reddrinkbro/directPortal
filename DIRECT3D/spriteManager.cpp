#include "stdafx.h"
#include "spriteManager.h"

HRESULT spriteManager::init(void)
{
	//��������Ʈ ����
	if (FAILED(D3DXCreateSprite(device, &_sprite))) return E_FAIL;
	return S_OK;
}

void spriteManager::release(void)
{
	//��������Ʈ ����
	SAFE_RELEASE(_sprite);
}

//��������Ʈ ���� ���۽� ȣ��
void spriteManager::beginSpriteRender()
{
	_sprite->Begin(D3DXSPRITE_ALPHABLEND);
}
//��������Ʈ ���� ����� ȣ��
void spriteManager::endSpriteRender()
{
	_sprite->End();
}

//��������ǥ�� ����
//�Ϲݷ���(�ؽ���, �׸���Ʈ����, x, y, ���̴� ����, �̹����߽���)
void spriteManager::spriteRender(LPDIRECT3DTEXTURE9 texture, LPRECT rcSour, float posX, float posY, DWORD color, D3DXVECTOR3 * center)
{
	//ȭ�� ���� ��ǥ(��������ǥ��)�� �׸��⶧���� Ʈ�������� �ʱ�ȭ �ϱ�
	D3DXMATRIXA16 matFinal;
	D3DXMatrixIdentity(&matFinal);
	_sprite->SetTransform(&matFinal);

	//�׸� ȭ�� ��ġ
	D3DXVECTOR3 pos(posX, posY, 0.0f);

	//��������Ʈ ������
	_sprite->Begin(D3DXSPRITE_ALPHABLEND);

	//�̰����� ��������Ʈ �׸���
	_sprite->Draw(
		texture,	//�׸� �ؽ���
		rcSour,		//�׸� �ؽ��� �ҽ�����
		(center == NULL) ? &D3DXVECTOR3(0, 0, 0) : center,
		&pos,		//�׸� ȭ�� ��ġ��ǥ
		color);		//�÷�(�̹����� ���ϴ� ���� �������� ȭ�鿡 ��µȴ� �׳ɵθ� �����̹���)

					//��������Ʈ �������
	_sprite->End();
}

//�Ϲݷ���(�̹��� ũ��, ȸ������)
void spriteManager::spriteRender(LPDIRECT3DTEXTURE9 texture, LPRECT rcSour, float posX, float posY, float scaleX, float scaleY, float rotate, DWORD color, D3DXVECTOR3 * center)
{
	//�̹��� ������
	D3DXVECTOR3 pivot = (center == NULL) ? D3DXVECTOR3(0, 0, 0) : *center;

	//������ ���
	D3DXMATRIXA16 matScale;
	D3DXMatrixScaling(&matScale, scaleX, scaleY, 1.0f);

	//ȸ�� ���
	D3DXMATRIXA16 matRotate;
	D3DXMatrixRotationZ(&matRotate, rotate);

	//�̵� ���
	D3DXMATRIXA16 matTrans;
	D3DXMatrixTranslation(&matTrans, posX, posY, 0.0f);

	//�������
	D3DXMATRIXA16 matFinal;
	//ũ��, ȸ��, �̵� => ������� ������� �Ѵ�(SRT)
	matFinal = matScale * matRotate * matTrans;
	_sprite->SetTransform(&matFinal);

	//��������Ʈ ������
	_sprite->Begin(D3DXSPRITE_ALPHABLEND);

	//�̰����� ��������Ʈ �׸���
	_sprite->Draw(
		texture,	//�׸� �ؽ���
		rcSour,		//�׸� �ؽ��� �ҽ�����
		&pivot,
		&D3DXVECTOR3(0, 0, 0),		//�׸� ȭ�� ��ġ��ǥ
		color);		//�÷�(�̹����� ���ϴ� ���� �������� ȭ�鿡 ��µȴ� �׳ɵθ� �����̹���)

					//��������Ʈ �������
	_sprite->End();
}

//��Ʈ�� ������ŭ ����
void spriteManager::renderArea(LPDIRECT3DTEXTURE9 texture, LPRECT rcSour, LPRECT rcDest, DWORD color)
{
	float destWidth = rcDest->right - rcDest->left;
	float destHeight = rcDest->bottom - rcDest->top;
	float sourWidth = rcSour->right - rcSour->left;
	float sourHeight = rcSour->bottom - rcSour->top;

	//���� ������ ���� ������ ��
	float scaleX = destWidth / sourWidth;
	float scaleY = destHeight / sourHeight;

	//������ ���
	D3DXMATRIXA16 matScale;
	D3DXMatrixScaling(&matScale, scaleX, scaleY, 1.0f);

	//�̵� ���
	D3DXMATRIXA16 matTrans;
	D3DXMatrixTranslation(&matTrans, rcDest->left, rcDest->top, 0.0f);

	//�������
	D3DXMATRIXA16 matFinal;
	//ũ��, ȸ��, �̵� => ������� ������� �Ѵ�(SRT)
	matFinal = matScale * matTrans;
	_sprite->SetTransform(&matFinal);

	//��������Ʈ ������
	_sprite->Begin(D3DXSPRITE_ALPHABLEND);

	//�̰����� ��������Ʈ �׸���
	_sprite->Draw(
		texture,				//�׸� �ؽ���
		rcSour,					//�׸� �ؽ��� �ҽ�����
		&D3DXVECTOR3(0, 0, 0),	//�̹��� �߽���ġ
		&D3DXVECTOR3(0, 0, 0),	//�׸� ȭ�� ��ġ��ǥ
		color);					//�÷�(�̹����� ���ϴ� ���� �������� ȭ�鿡 ��µȴ� �׳ɵθ� �����̹���)

								//��������Ʈ �������
	_sprite->End();
}

//������ġ ����
void spriteManager::ninePatchRender(LPDIRECT3DTEXTURE9 texture, LPRECT rcSour, LPRECT rcDest, LPRECT rcCenter, DWORD color, bool isDrawCenter)
{
	//lt, top, rt
	//lc, center, rc
	//lb, bottom, rb

	// ___|___|___
	// ___|___|___
	// ___|___|___

	//�ҽ�����
	RECT rcSourLT = { 0, 0, rcCenter->left, rcCenter->top };
	RECT rcSourTop = { rcCenter->left, 0, rcCenter->right, rcCenter->top };
	RECT rcSourRT = { rcCenter->right, 0, rcSour->right, rcCenter->top };

	RECT rcSourLC = { 0, rcCenter->top, rcCenter->left, rcCenter->bottom };
	RECT rcSourCenter = { rcCenter->left, rcCenter->top, rcCenter->right, rcCenter->bottom };
	RECT rcSourRC = { rcCenter->right, rcCenter->top, rcSour->right, rcCenter->bottom };

	RECT rcSourLB = { 0, rcCenter->bottom, rcCenter->left, rcSour->bottom };
	RECT rcSourBottom = { rcCenter->left, rcCenter->bottom, rcCenter->right, rcSour->bottom };
	RECT rcSourRB = { rcCenter->right, rcCenter->bottom, rcSour->right, rcSour->bottom };

	//�׸��� ����
	float left = rcDest->left;
	float top = rcDest->top;
	float right = rcDest->right;
	float bottom = rcDest->bottom;

	float centerLeft = rcDest->left + (rcCenter->left - rcSour->left);
	float centerRight = rcDest->right - (rcSour->right - rcCenter->right);
	float centerTop = rcDest->top + (rcCenter->top - rcSour->top);
	float centerBottom = rcDest->bottom - (rcSour->bottom - rcCenter->bottom);

	RECT rcDestLT = { left, top, centerLeft, centerTop };
	RECT rcDestTop = { centerLeft, top, centerRight, centerTop };
	RECT rcDestRT = { centerRight, top, right, centerTop };

	RECT rcDestLC = { left, centerTop, centerLeft, centerBottom };
	RECT rcDestCenter = { centerLeft, centerTop, centerRight, centerBottom };
	RECT rcDestRC = { centerRight, centerTop, right, centerBottom };

	RECT rcDestLB = { left, centerBottom, centerLeft, bottom };
	RECT rcDestBottom = { centerLeft, centerBottom, centerRight, bottom };
	RECT rcDestRB = { centerRight, centerBottom, right, bottom };

	//9���� ��Ʈ������ ����
	renderArea(texture, &rcSourLT, &rcDestLT, color);
	renderArea(texture, &rcSourTop, &rcDestTop, color);
	renderArea(texture, &rcSourRT, &rcDestRT, color);
	renderArea(texture, &rcSourLC, &rcDestLC, color);
	//����� true �϶��� �׸���
	if (isDrawCenter) renderArea(texture, &rcSourCenter, &rcDestCenter, color);
	renderArea(texture, &rcSourRC, &rcDestRC, color);
	renderArea(texture, &rcSourLB, &rcDestLB, color);
	renderArea(texture, &rcSourBottom, &rcDestBottom, color);
	renderArea(texture, &rcSourRB, &rcDestRB, color);
}
