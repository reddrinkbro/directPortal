#pragma once

//파티클 정점 구조체
typedef struct tagquadParticle_VERTEX
{
	D3DXVECTOR3 pos;			//정점 위치
	DWORD color;				//정점 컬러
	D3DXVECTOR2 uv;				//정점 UV
	enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 };
}quadParticle_VERTEX, *LPquadParticle_VERTEX;

typedef vector<D3DXCOLOR>		VEC_COLOR;
typedef vector<float>			VEC_SCALE;

class quadParticle
{
public:
	transform		_transform;				//파티클의 위치값

private:
	bool			_isLive;				//활성화 여부
	float			_totalLiveTime;			//총 활성화 시간
	float			_deltaLiveTime;			//활성화 지난 시간
	float			_nomalizeLiveTime;		//활성화 비율 시간( 0 ~ 1 )
	D3DXVECTOR3		_velocity;				//파티클의 속도벡터
	D3DXVECTOR3		_acceleration;			//초당 증가하는 가속벡터
	D3DXVECTOR3		_rotate;				//초당 회전 벡터
	D3DXVECTOR3		_rotateAccel;			//초당 증가하는 회전벡터
	float			_scale;					//기본 스케일값

public:
	void Start(
		float liveTime,						//라이브 타임
		const D3DXVECTOR3* pos,				//시작 위치
		const D3DXVECTOR3* velo,			//시작 속도
		const D3DXVECTOR3* accel,			//가속 값
		const D3DXVECTOR3* rotate,			//초당 회전 값
		const D3DXVECTOR3* rotateAccel,		//초당 회전 증가값
		float scale							//기본 스케일
	);

	void update();

	//자신의 정점 정보를 바탕으로 LPquadParticle_VERTEX 의 값을 넣어준다.
	void getParticleVertex(
		LPquadParticle_VERTEX pOut,
		DWORD* pIndex,
		const VEC_COLOR& colors,
		const VEC_SCALE& scales,
		DWORD particleNum);

	bool isLive() { return _isLive; }

	quadParticle() {}
	~quadParticle() {}
};

