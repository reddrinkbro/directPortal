#include "stdafx.h"
#include "quadParticle.h"

void quadParticle::Start(float liveTime, const D3DXVECTOR3 * pos, const D3DXVECTOR3 * velo, const D3DXVECTOR3 * accel, const D3DXVECTOR3 * rotate, const D3DXVECTOR3 * rotateAccel, float scale)
{
	//Ȱ��ȭ ����
	_isLive = true;

	//���̺� Ÿ�� �ʱ�ȭ
	_totalLiveTime = liveTime;
	_deltaLiveTime = 0.0f;

	//��ġ ������
	_transform.setWorldPosition(pos->x, pos->y, pos->z);

	//���� �ӵ�
	_velocity = *velo;

	//����
	_acceleration = *accel;

	//ȸ��
	_rotate = *rotate;

	//ȸ�� ����
	_rotateAccel = *rotateAccel;

	//�⺻ ������
	_scale = scale;
}

void quadParticle::update()
{
	if (_isLive == false) return;

	//���̺� Ÿ���� �ٵǾ��ٸ�...
	if (_deltaLiveTime > _totalLiveTime) { _isLive = false; }

	//Ȱ��ȭ �ð� �״´�.
	_deltaLiveTime += _deltaTime;

	//����ȭ�� �ð����� ���
	_nomalizeLiveTime = _deltaLiveTime / _totalLiveTime;
	if (_nomalizeLiveTime > 1.0f) _nomalizeLiveTime = 1.0f;

	//��ƼŬ ���� �̵�
	_transform.movePositionWorld(
		_velocity.x * _deltaTime,
		_velocity.y * _deltaTime,
		_velocity.z * _deltaTime);

	_transform.rotateSelf(
		_rotate.x * _deltaTime,
		_rotate.y * _deltaTime,
		_rotate.z * _deltaTime);

	//��ƼŬ ����
	_velocity += _acceleration * _deltaTime;

	//��ƼŬ ȸ�� ����
	_rotate += _rotateAccel * _deltaTime;
}

void quadParticle::getParticleVertex(LPquadParticle_VERTEX pOut, DWORD * pIndex, const VEC_COLOR & colors, const VEC_SCALE & scales, DWORD dwParticleNum)
{
	//��ƼŬ ��ġ ��
	D3DXVECTOR3 center = _transform.getWorldPosition();

	DWORD dwcolor = 0;
	float scale;

	//
	// ������ ����
	//

	//������ ���� ���
	float s = 0.0f;

	//������ �迭�� ������ �ε���
	int lastIndex = scales.size() - 1;

	if (_nomalizeLiveTime == 1.0f)
	{
		dwcolor = colors[colors.size() - 1];
		scale = _scale * scales[scales.size() - 1];
	}

	else if (_nomalizeLiveTime == 0.0f)
	{
		dwcolor = colors[0];
		scale = _scale * scales[0];
	}
	else
	{

		//������ ���͹�
		float interval = 1.0f / lastIndex;

		//���� ���̺� Ÿ�� ���� ���� ��ġ
		float position = _nomalizeLiveTime / interval;

		//�����ε���
		int startIndex = (int)position;

		//���� �ε���
		int endIndex = startIndex + 1;

		//������
		float fN = position - startIndex;
		s = (scales[endIndex] - scales[startIndex]) * fN + scales[startIndex];

		//���� ������ �� ����
		scale = _scale * s;

		//
		// �÷� ����
		//

		//�÷� �迭�� ������ �ε���
		lastIndex = colors.size() - 1;

		//�÷� ���͹�
		interval = 1.0f / lastIndex;

		//���� ���̺� Ÿ�� ���� ���� ��ġ
		position = _nomalizeLiveTime / interval;

		//�����ε���
		startIndex = (int)position;

		//���� �ε���
		endIndex = startIndex + 1;

		//������
		fN = position - startIndex;

		//���� �÷���
		D3DXCOLOR color;

		//�÷� ���� ����
		D3DXColorLerp(&color,			//���
			&colors[startIndex],		//from
			&colors[endIndex],			//to
			fN);						//normalize Factor

		dwcolor = (DWORD)(color);
	}

	//�����Ͽ� ���� halfSize;
	float halfScale = scale * 0.5f;

	//����
	D3DXVECTOR3 x = _transform.getRight();
	D3DXVECTOR3 y = _transform.getUp();

	//���� ���� ����
	(pOut + 0)->pos = center + (-x * halfScale) + (y * halfScale);
	(pOut + 1)->pos = center + (x * halfScale) + (y * halfScale);
	(pOut + 2)->pos = center + (-x * halfScale) + (-y * halfScale);
	(pOut + 3)->pos = center + (x * halfScale) + (-y * halfScale);

	(pOut + 0)->uv = D3DXVECTOR2(0, 0);
	(pOut + 1)->uv = D3DXVECTOR2(1, 0);
	(pOut + 2)->uv = D3DXVECTOR2(0, 1);
	(pOut + 3)->uv = D3DXVECTOR2(1, 1);

	(pOut + 0)->color = dwcolor;
	(pOut + 1)->color = dwcolor;
	(pOut + 2)->color = dwcolor;
	(pOut + 3)->color = dwcolor;

	//0----1
	//|   /|
	//|  / |
	//| /  |
	//|/   |
	//2----3

	//�ε��� ���� ���� ( �ε��� ������ ���ݱ��� �׷����� Quad ����ŭ �������� ���� �־�� �Ѵ� )
	*(pIndex + 0) = (dwParticleNum * 4) + 0;
	*(pIndex + 1) = (dwParticleNum * 4) + 1;
	*(pIndex + 2) = (dwParticleNum * 4) + 2;
	*(pIndex + 3) = (dwParticleNum * 4) + 2;
	*(pIndex + 4) = (dwParticleNum * 4) + 1;
	*(pIndex + 5) = (dwParticleNum * 4) + 3;
}
