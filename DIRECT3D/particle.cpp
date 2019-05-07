#include "stdafx.h"
#include "particle.h"

void particle::start(float liveTime, const D3DXVECTOR3 * pos, const D3DXVECTOR3 * velo, const D3DXVECTOR3 * accel, float scale)
{
	//Ȱ��ȭ ����
	_isLive = true;

	//liveTime �ʱ�ȭ
	_totalLiveTime = liveTime;
	_deltaLiveTime = 0.0f;
	_normalizeLiveTime = 0.0f;

	//��ġ�� setting
	_transform.setWorldPosition(pos->x, pos->y, pos->z);
	_boundBox.setBound(&D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0.15f, 0.15f, 0.15f));
	//���� �ӵ�
	_velocity = *velo;

	//����
	_acceleration = *accel;

	//�⺻������
	_scale = scale;
}

void particle::update()
{
	if (_isLive == false) return;

	//���̺� Ÿ���� �� �Ǿ��ٸ�!
	if (_deltaLiveTime >= _totalLiveTime)
	{
		_isLive = false;
	}
	//Ȱ��ȭ �ð��� ���δ�.
	_deltaLiveTime += _deltaTime;

	//����ȭ �� �ð� �� ���
	_normalizeLiveTime = _deltaLiveTime / _totalLiveTime;
	if (_normalizeLiveTime > 1.0f) _normalizeLiveTime = 1.0f;

	//��ƼŬ�� ���� �̵�
	_transform.movePositionWorld(
		_velocity.x * _deltaTime,
		_velocity.y * _deltaTime,
		_velocity.z * _deltaTime);

	//��ƼŬ ����
	_velocity += _acceleration * _deltaTime;
}

void particle::getParticleVertex(LPPARTICLE_VERTEX pOut, const VEC_COLOR & colors, const VEC_SCALE & scales)
{
	//��ƼŬ�� ��ġ��
	pOut->pos = _transform.getWorldPosition();

	if (_normalizeLiveTime == 1.0f)
	{
		pOut->color = colors[colors.size() - 1];
		pOut->size = _scale*scales[scales.size() - 1];
		return;
	}
	else if (_normalizeLiveTime == 0.0f)
	{
		pOut->color = colors[0];
		pOut->size = _scale*scales[0];
		return;
	}

	//
	//������ ����
	//

	//������ ���� ���
	float s = 0.0f;

	//������ �迭�� ������ �ε���
	int lastIndex = scales.size() - 1;

	//������ ���͹�
	float interval = 1.0f / lastIndex;

	//���� ���̺� Ÿ�� ���� ���� ��ġ
	float position = _normalizeLiveTime / interval;
	//postion �� ���� �����κ��� ���Ǵ� �迭���ε����� �Ǵ°��̰�,
	//�Ҽ����κ��� ���� �迭 �������� ���� ���� �ȴ�.

	//���� �ε���
	int startIndex = (int)position;

	// ���� ���ؽ�
	int endIndex = startIndex + 1;

	//���� ��
	float fN = position - startIndex;
	s = lerp(scales[startIndex], scales[endIndex], fN);

	//���� ������ �� ����
	pOut->size = _scale*s;

	//
	//�÷�����
	//

	//�÷� �迭�� ������ �ε���
	lastIndex = colors.size() - 1;

	//�÷� ���͹�
	interval = 1.0f / lastIndex;

	//���� ���̺� Ÿ�ӷ��� ������ġ
	position = _normalizeLiveTime / interval;

	//�����ε���
	startIndex = (int)position;

	//�����ε���
	endIndex = startIndex + 1;

	//���� ��
	fN = position - startIndex;

	//���� �÷� ��
	D3DXCOLOR color;

	//color ��������
	D3DXColorLerp(
		&color,					//��� ������ ����
		&colors[startIndex],	//From
		&colors[endIndex],		//To
		fN);					//Normalize Factor

	//���� �÷��� ����
	pOut->color = (DWORD)(color);
}