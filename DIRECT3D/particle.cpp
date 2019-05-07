#include "stdafx.h"
#include "particle.h"

void particle::start(float liveTime, const D3DXVECTOR3 * pos, const D3DXVECTOR3 * velo, const D3DXVECTOR3 * accel, float scale)
{
	//활성화 여부
	_isLive = true;

	//liveTime 초기화
	_totalLiveTime = liveTime;
	_deltaLiveTime = 0.0f;
	_normalizeLiveTime = 0.0f;

	//위치값 setting
	_transform.setWorldPosition(pos->x, pos->y, pos->z);
	_boundBox.setBound(&D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0.15f, 0.15f, 0.15f));
	//시작 속도
	_velocity = *velo;

	//가속
	_acceleration = *accel;

	//기본스케일
	_scale = scale;
}

void particle::update()
{
	if (_isLive == false) return;

	//라이브 타임이 다 되었다면!
	if (_deltaLiveTime >= _totalLiveTime)
	{
		_isLive = false;
	}
	//활성화 시간이 쌓인다.
	_deltaLiveTime += _deltaTime;

	//정규화 된 시간 값 계산
	_normalizeLiveTime = _deltaLiveTime / _totalLiveTime;
	if (_normalizeLiveTime > 1.0f) _normalizeLiveTime = 1.0f;

	//파티클의 월드 이동
	_transform.movePositionWorld(
		_velocity.x * _deltaTime,
		_velocity.y * _deltaTime,
		_velocity.z * _deltaTime);

	//파티클 가속
	_velocity += _acceleration * _deltaTime;
}

void particle::getParticleVertex(LPPARTICLE_VERTEX pOut, const VEC_COLOR & colors, const VEC_SCALE & scales)
{
	//파티클의 위치값
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
	//스케일 보간
	//

	//스케일 보간 결과
	float s = 0.0f;

	//스케일 배열의 마지막 인덱스
	int lastIndex = scales.size() - 1;

	//스케일 인터벌
	float interval = 1.0f / lastIndex;

	//현재 라이브 타임 량에 따른 위치
	float position = _normalizeLiveTime / interval;
	//postion 의 값에 정수부분이 사용되는 배열의인덱스가 되는것이고,
	//소숫점부분이 다음 배열 값까지의 보간 량이 된다.

	//시작 인덱스
	int startIndex = (int)position;

	// 종료 인텍스
	int endIndex = startIndex + 1;

	//보간 값
	float fN = position - startIndex;
	s = lerp(scales[startIndex], scales[endIndex], fN);

	//최종 스케일 값 세팅
	pOut->size = _scale*s;

	//
	//컬러보간
	//

	//컬러 배열의 마지막 인덱스
	lastIndex = colors.size() - 1;

	//컬러 인터벌
	interval = 1.0f / lastIndex;

	//현재 라이브 타임량에 따른위치
	position = _normalizeLiveTime / interval;

	//시작인덱스
	startIndex = (int)position;

	//종료인덱스
	endIndex = startIndex + 1;

	//보간 값
	fN = position - startIndex;

	//최종 컬러 값
	D3DXCOLOR color;

	//color 선형보간
	D3DXColorLerp(
		&color,					//결과 저장할 변수
		&colors[startIndex],	//From
		&colors[endIndex],		//To
		fN);					//Normalize Factor

	//최종 컬러값 세팅
	pOut->color = (DWORD)(color);
}