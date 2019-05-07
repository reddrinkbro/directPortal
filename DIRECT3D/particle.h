#pragma once

//파티클 정점 구조체
typedef struct tagPARTICLE_VERTEX
{
	D3DXVECTOR3 pos;				//정점 위치
	float size;						//파티클 사용시 정점 사이즈
	DWORD color;					//정점 컬러
	enum { FVF = D3DFVF_XYZ | D3DFVF_PSIZE | D3DFVF_DIFFUSE };
}PARTICLE_VERTEX, *LPPARTICLE_VERTEX;

typedef vector<D3DXCOLOR>   VEC_COLOR;
typedef vector<float>		VEC_SCALE;

//입자 하나에 대한 정보
class particle
{
public:
	transform		_transform;				//파티클의 위치값
	boundBox		_boundBox;
private:
	bool			_isLive;				//활성화 여부
	float			_totalLiveTime;			//총 활성화 시간
	float			_deltaLiveTime;			//활성화 지난 시간
	float			_normalizeLiveTime;		//활성화 비율 시간(0~1)
	D3DXVECTOR3		_velocity;				//파티클의 속도벡터
	D3DXVECTOR3		_acceleration;			//초당 증가하는 가속 벡터
	float			_scale;					//기본 스케일값

public:
	void start(
		float liveTime,						//라이브타임
		const D3DXVECTOR3* pos,				//시작 위치
		const D3DXVECTOR3* velocity,		//시작속도
		const D3DXVECTOR3* acceleration,	//가속도
		float scale							//기본스케일
	);

	void update();

	//자신의 정점 정보를 바탕으로 LPPATICLE_VERTEX  의 값을 넣어준다.
	void getParticleVertex(
		LPPARTICLE_VERTEX pOut,
		const VEC_COLOR& colors,
		const VEC_SCALE& scales
	);

	bool getIsLive() { return _isLive; }
	void setnormalizeLiveTime(float normalizeLiveTime) { _normalizeLiveTime = normalizeLiveTime; }
	particle() : _isLive(false) {}
	~particle() {}
};