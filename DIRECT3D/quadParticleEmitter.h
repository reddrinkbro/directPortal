#pragma once
#include "baseObject.h"
#include "quadParticle.h"

class quadParticleEmitter : public baseObject
{
private:
	DWORD					_paticleNum;			//������ �ѷ�
	quadParticle*			_paticles;				//��ƼŬ �迭
	quadParticle**			_sortArray;				//��ƼŬ ��Ʈ�� Array
	LPquadParticle_VERTEX	_particleVerticles;		//��ƼŬ ���ؽ�
	DWORD*					_particleIndex;			//��ƼŬ �ε���
	float					_emissionPerSec;		//�ʴ� �߻���
	DWORD					_particleCount;			//Ȱ��ȭ ����
	VEC_COLOR				_colors;				//��ƼŬ �÷� �迭
	VEC_SCALE				_scales;				//��ƼŬ ������ �迭
	transform*				_cameraTransform;		//ī�޶� Ʈ������
	bool					_isCameraSort;			//ī�޶� �Ÿ��� ��Ʈ�Ҳ���?
	
	//��ƼŬ ���̺� Ÿ�� �ִ� �ּ�
	float					_startLiveTimeMin;		
	float					_startLiveTimeMax;		

	//��ƼŬ ���� �ӵ� �ִ� �ּ�
	D3DXVECTOR3				_startVelocityMin;
	D3DXVECTOR3				_startVelocityMax;

	//��ƼŬ ���� ���� �ִ� �ּ�
	D3DXVECTOR3				_startAccelateMin;
	D3DXVECTOR3				_startAccelateMax;

	//��ƼŬ �����Ҷ� ȸ�� ���۰� �ִ� �ּ�
	D3DXVECTOR3				_startSetRotateMin;
	D3DXVECTOR3				_startSetRotateMax;

	//��ƼŬ ���� ȸ�� �ִ� �ּ�
	D3DXVECTOR3				_startRotateMin;
	D3DXVECTOR3				_startRotateMax;

	//��ƼŬ ȸ�� ���� �ִ� �ּ�
	D3DXVECTOR3				_startRotateAccelateMin;
	D3DXVECTOR3				_startRotateAccelateMax;

	//��ƼŬ ���� ������ �ִ� �ּ�
	float					_startScaleMin;
	float					_startScaleMax;

	bool					_isEmission;			//���� �������̴�?
	float					_emisionIntervalTime;	//���� ���� �ð�
	float					_emisionDeltaTime;		//�ϳ� �����ϱ� �����ð�

	LPDIRECT3DTEXTURE9		_particleTex;			//��ƼŬ �ؽ���
	bool					_isLocal;

public:
	quadParticleEmitter() : _cameraTransform(NULL), _isCameraSort(false) {}
	~quadParticleEmitter() {}

	HRESULT init(
		DWORD partcleNum,							//�� ��ƼŬ ��
		float emission,								//�ʴ� �߻���
		float liveTimeMin,
		float liveTimeMax,
		const D3DXVECTOR3& velocityMin,
		const D3DXVECTOR3& velocityMax,
		const D3DXVECTOR3& accelMin,
		const D3DXVECTOR3& accelMax,
		const D3DXVECTOR3& startRotateMin,			//�����Ҷ� ȸ�� �ּҰ� 
		const D3DXVECTOR3& startRotateMax,			//�����Ҷ� ȸ�� �ִ밪
		const D3DXVECTOR3& rotateMin,				//�����ӿ����� ȸ�� �ּҷ�
		const D3DXVECTOR3& rotateMax,				//�����ӿ����� ȸ�� �ִ뷮
		const D3DXVECTOR3& rotateAccelMin,			//������ ���ӿ����� ȸ�� �ּҷ�
		const D3DXVECTOR3& rotateAccelMax,			//������ ���ӿ����� ȸ�� �ִ뷮
		const VEC_COLOR& colors,
		const VEC_SCALE& scales,
		float scaleMin,
		float scaleMax,
		LPDIRECT3DTEXTURE9 particleTex,
		bool isLocal = false
	);

	void release();

	virtual void baseObjectUpdate() override;	//BaseObject �� Update �� ����
	virtual void baseObjectRender() override;

	void startEmission();			//��ƼŬ ���� ����
	void stopEmission();			//��ƼŬ ���� ����

									//ī�޶� ������ �Ҳ���?
	void setCameraSort(transform* cameraTrans, bool bSort);

	void setRenderLocal(bool bLocal) { _isLocal = bLocal; }

private:
	void startOneParticle();		//��ƼŬ �ϳ� ����
};