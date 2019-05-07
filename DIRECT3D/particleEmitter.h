#pragma once
#include "baseObject.h"
#include "particle.h"

enum PATICLE_EMISSION_TYPE
{
	PZERO,					//한점에서 방사
	SPHERE,					//구에서 방사
	SPHERE_OUTLINE,			//구에서 방사하는데 외부표면에서만
	BOX						//박스에서 방사
};

//파티클을 재생해주는 놈
class particleEmitter : public baseObject
{
private:
	DWORD					_particleNum;			//입자의 총량
	particle*				_particles;				//파티클 배열
	LPPARTICLE_VERTEX		_particleVertices;		//파티클 버텍스
	float					_emissionPerSec;		//파티클 초당 발생량
	DWORD					_particleCount;			//활성화 순번
	VEC_COLOR				_colors;				//파티클 컬러 배열
	VEC_SCALE				_scales;				//파티클 사이즈 배열
	float					_startLiveTimeMin;		//파티클 라이브타임 최소
	float					_startLiveTimeMax;		//파티클 라이브타임 최대
	D3DXVECTOR3				_startVelocityMin;		//파티클 시작속도 최소 속도
	D3DXVECTOR3				_startVelocityMax;		//파티클 시작속도 최대 속도
	D3DXVECTOR3				_startAccelateMin;		//파티클 시작 가속 최소
	D3DXVECTOR3				_startAccelateMax;		//파티클 시작 가속 최대
	float					_startScaleMin;			//파티클 시작 사이즈 최소
	float					_startScaleMax;			//파티클 시작 사이즈 최소
	bool					_isEmission;			//현재 생성중이니?
	float					_emissionIntervalTime;	//생성 간격 시간
	float					_emissionDeltaTime;		//하나 생성하고 나서 지난 시간
	LPDIRECT3DBASETEXTURE9	_particleTex;			//파티클 텍스쳐
	bool					_isLocal;				//로컬이니?

public:
	PATICLE_EMISSION_TYPE	emissionType;			//파티클 생성 타입
	//particle 생성 범위
	float					minEmissionRangeX;		//x 생성범위
	float					maxEmissionRangeX;
	float					minEmissionRangeY;		//y 생성범위
	float					maxEmissionRangeY;
	float					minEmissionRangeZ;		//z 생성범위
	float					maxEmissionRangeZ;
	float					sphereEmissionRange;	//구생성시 생성 반지름

public:
	particleEmitter() {}
	~particleEmitter() {}

	void init(
		DWORD particleNum,							//총 파티클 량
		float emission,								//초당 발생량
		float liveTimeMin,							//파티클 하나의 생명시간
		float liveTimeMax,							//파티클 하나의 생명시간
		const D3DXVECTOR3&	velocityMin,			//파티클 시작 속도 최소
		const D3DXVECTOR3&	velocityMax,			//파티클 시작 속도 최대
		const D3DXVECTOR3&	accelMin,				//파티클 가속도 최소
		const D3DXVECTOR3&	accelMax,				//파티클 가속도 최대
		const VEC_COLOR&	colors,					//파티클 컬러 벡터배열
		const VEC_SCALE&	scales,					//파티클 스케일 벡터 배열
		float scaleMin,								//파티클의 스케일 최소
		float scaleMax,								//파티클의 스케일 최대
		LPDIRECT3DTEXTURE9	pParticleTexture,		//파티클 텍스쳐
		bool isLocal = false						//true 이면 파티클의 움직임이 나의 Transform Local 기준으로 움직인다.(아콘)
	);

	void release();

	//BaseObject 가  Update 때 실행
	virtual void baseObjectUpdate() override;
	virtual void baseObjectRender() override;
	
	//파티클 생성 시작
	void startEmission();
	//파티클 생성 중지
	void stopEmission();		

	//사방 팔방으로 입자를 퍼트린다.
	void burst(int num, float minSpeed, float maxSpeed, float maxLife, float minLife);

	void setRenderLocal(bool isLocal) { _isLocal = isLocal; }
	particle* getParticle(void) { return _particles; }
private:
	//파티클 하나 생성
	void startOneParticle();
	//파티클 하나 생성(방향 넣어서)
	void startOneParticle(D3DXVECTOR3 dir, float life);
};