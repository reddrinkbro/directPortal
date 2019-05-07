#pragma once
#include "baseObject.h"
#include "quadParticle.h"

class quadParticleEmitter : public baseObject
{
private:
	DWORD					_paticleNum;			//입자의 총량
	quadParticle*			_paticles;				//파티클 배열
	quadParticle**			_sortArray;				//파티클 소트용 Array
	LPquadParticle_VERTEX	_particleVerticles;		//파티클 버텍스
	DWORD*					_particleIndex;			//파티클 인덱스
	float					_emissionPerSec;		//초당 발생량
	DWORD					_particleCount;			//활성화 순번
	VEC_COLOR				_colors;				//파티클 컬러 배열
	VEC_SCALE				_scales;				//파티클 사이즈 배열
	transform*				_cameraTransform;		//카메라 트랜스폼
	bool					_isCameraSort;			//카메라 거리별 소트할꺼냐?
	
	//파티클 라이브 타임 최대 최소
	float					_startLiveTimeMin;		
	float					_startLiveTimeMax;		

	//파티클 시작 속도 최대 최소
	D3DXVECTOR3				_startVelocityMin;
	D3DXVECTOR3				_startVelocityMax;

	//파티클 시작 가속 최대 최소
	D3DXVECTOR3				_startAccelateMin;
	D3DXVECTOR3				_startAccelateMax;

	//파티클 시작할때 회전 시작값 최대 최소
	D3DXVECTOR3				_startSetRotateMin;
	D3DXVECTOR3				_startSetRotateMax;

	//파티클 시작 회전 최대 최소
	D3DXVECTOR3				_startRotateMin;
	D3DXVECTOR3				_startRotateMax;

	//파티클 회전 가속 최대 최소
	D3DXVECTOR3				_startRotateAccelateMin;
	D3DXVECTOR3				_startRotateAccelateMax;

	//파티클 시작 사이즈 최대 최소
	float					_startScaleMin;
	float					_startScaleMax;

	bool					_isEmission;			//현재 생성중이니?
	float					_emisionIntervalTime;	//생성 간격 시간
	float					_emisionDeltaTime;		//하나 생성하구 지난시간

	LPDIRECT3DTEXTURE9		_particleTex;			//파티클 텍스쳐
	bool					_isLocal;

public:
	quadParticleEmitter() : _cameraTransform(NULL), _isCameraSort(false) {}
	~quadParticleEmitter() {}

	HRESULT init(
		DWORD partcleNum,							//총 파티클 량
		float emission,								//초당 발생량
		float liveTimeMin,
		float liveTimeMax,
		const D3DXVECTOR3& velocityMin,
		const D3DXVECTOR3& velocityMax,
		const D3DXVECTOR3& accelMin,
		const D3DXVECTOR3& accelMax,
		const D3DXVECTOR3& startRotateMin,			//시작할때 회전 최소값 
		const D3DXVECTOR3& startRotateMax,			//시작할때 회전 최대값
		const D3DXVECTOR3& rotateMin,				//움직임에대한 회전 최소량
		const D3DXVECTOR3& rotateMax,				//움직임에대한 회전 최대량
		const D3DXVECTOR3& rotateAccelMin,			//움직임 가속에대한 회전 최소량
		const D3DXVECTOR3& rotateAccelMax,			//움직임 가속에대한 회전 최대량
		const VEC_COLOR& colors,
		const VEC_SCALE& scales,
		float scaleMin,
		float scaleMax,
		LPDIRECT3DTEXTURE9 particleTex,
		bool isLocal = false
	);

	void release();

	virtual void baseObjectUpdate() override;	//BaseObject 가 Update 때 실행
	virtual void baseObjectRender() override;

	void startEmission();			//파티클 생성 시작
	void stopEmission();			//파티클 생성 중지

									//카메라 소팅을 할꺼니?
	void setCameraSort(transform* cameraTrans, bool bSort);

	void setRenderLocal(bool bLocal) { _isLocal = bLocal; }

private:
	void startOneParticle();		//파티클 하나 생성
};