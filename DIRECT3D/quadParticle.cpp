#include "stdafx.h"
#include "quadParticle.h"

void quadParticle::Start(float liveTime, const D3DXVECTOR3 * pos, const D3DXVECTOR3 * velo, const D3DXVECTOR3 * accel, const D3DXVECTOR3 * rotate, const D3DXVECTOR3 * rotateAccel, float scale)
{
	//활성화 여부
	_isLive = true;

	//라이브 타임 초기화
	_totalLiveTime = liveTime;
	_deltaLiveTime = 0.0f;

	//위치 값세팅
	_transform.setWorldPosition(pos->x, pos->y, pos->z);

	//시작 속도
	_velocity = *velo;

	//가속
	_acceleration = *accel;

	//회전
	_rotate = *rotate;

	//회전 가속
	_rotateAccel = *rotateAccel;

	//기본 스케일
	_scale = scale;
}

void quadParticle::update()
{
	if (_isLive == false) return;

	//라이브 타임이 다되었다면...
	if (_deltaLiveTime > _totalLiveTime) { _isLive = false; }

	//활성화 시간 쌓는다.
	_deltaLiveTime += _deltaTime;

	//정규화된 시간값을 계산
	_nomalizeLiveTime = _deltaLiveTime / _totalLiveTime;
	if (_nomalizeLiveTime > 1.0f) _nomalizeLiveTime = 1.0f;

	//파티클 월드 이동
	_transform.movePositionWorld(
		_velocity.x * _deltaTime,
		_velocity.y * _deltaTime,
		_velocity.z * _deltaTime);

	_transform.rotateSelf(
		_rotate.x * _deltaTime,
		_rotate.y * _deltaTime,
		_rotate.z * _deltaTime);

	//파티클 가속
	_velocity += _acceleration * _deltaTime;

	//파티클 회전 가속
	_rotate += _rotateAccel * _deltaTime;
}

void quadParticle::getParticleVertex(LPquadParticle_VERTEX pOut, DWORD * pIndex, const VEC_COLOR & colors, const VEC_SCALE & scales, DWORD dwParticleNum)
{
	//파티클 위치 값
	D3DXVECTOR3 center = _transform.getWorldPosition();

	DWORD dwcolor = 0;
	float scale;

	//
	// 스케일 보간
	//

	//스케일 보간 결과
	float s = 0.0f;

	//스케일 배열의 마지막 인덱스
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

		//스케일 인터벌
		float interval = 1.0f / lastIndex;

		//현재 라이브 타임 량에 따른 위치
		float position = _nomalizeLiveTime / interval;

		//시작인덱스
		int startIndex = (int)position;

		//종료 인덱스
		int endIndex = startIndex + 1;

		//보간값
		float fN = position - startIndex;
		s = (scales[endIndex] - scales[startIndex]) * fN + scales[startIndex];

		//최종 스케일 값 세팅
		scale = _scale * s;

		//
		// 컬러 보간
		//

		//컬러 배열의 마지막 인덱스
		lastIndex = colors.size() - 1;

		//컬러 인터벌
		interval = 1.0f / lastIndex;

		//현재 라이브 타임 량에 따른 위치
		position = _nomalizeLiveTime / interval;

		//시작인덱스
		startIndex = (int)position;

		//종료 인덱스
		endIndex = startIndex + 1;

		//보간값
		fN = position - startIndex;

		//최종 컬러값
		D3DXCOLOR color;

		//컬러 선형 보간
		D3DXColorLerp(&color,			//결과
			&colors[startIndex],		//from
			&colors[endIndex],			//to
			fN);						//normalize Factor

		dwcolor = (DWORD)(color);
	}

	//스케일에 따른 halfSize;
	float halfScale = scale * 0.5f;

	//방향
	D3DXVECTOR3 x = _transform.getRight();
	D3DXVECTOR3 y = _transform.getUp();

	//정점 정보 대입
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

	//인덱스 정보 대입 ( 인덱스 넣을때 지금까지 그려지는 Quad 수만큼 점프해한 값을 넣어야 한다 )
	*(pIndex + 0) = (dwParticleNum * 4) + 0;
	*(pIndex + 1) = (dwParticleNum * 4) + 1;
	*(pIndex + 2) = (dwParticleNum * 4) + 2;
	*(pIndex + 3) = (dwParticleNum * 4) + 2;
	*(pIndex + 4) = (dwParticleNum * 4) + 1;
	*(pIndex + 5) = (dwParticleNum * 4) + 3;
}
