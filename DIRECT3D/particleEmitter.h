#pragma once
#include "baseObject.h"
#include "particle.h"

enum PATICLE_EMISSION_TYPE
{
	PZERO,					//�������� ���
	SPHERE,					//������ ���
	SPHERE_OUTLINE,			//������ ����ϴµ� �ܺ�ǥ�鿡����
	BOX						//�ڽ����� ���
};

//��ƼŬ�� ������ִ� ��
class particleEmitter : public baseObject
{
private:
	DWORD					_particleNum;			//������ �ѷ�
	particle*				_particles;				//��ƼŬ �迭
	LPPARTICLE_VERTEX		_particleVertices;		//��ƼŬ ���ؽ�
	float					_emissionPerSec;		//��ƼŬ �ʴ� �߻���
	DWORD					_particleCount;			//Ȱ��ȭ ����
	VEC_COLOR				_colors;				//��ƼŬ �÷� �迭
	VEC_SCALE				_scales;				//��ƼŬ ������ �迭
	float					_startLiveTimeMin;		//��ƼŬ ���̺�Ÿ�� �ּ�
	float					_startLiveTimeMax;		//��ƼŬ ���̺�Ÿ�� �ִ�
	D3DXVECTOR3				_startVelocityMin;		//��ƼŬ ���ۼӵ� �ּ� �ӵ�
	D3DXVECTOR3				_startVelocityMax;		//��ƼŬ ���ۼӵ� �ִ� �ӵ�
	D3DXVECTOR3				_startAccelateMin;		//��ƼŬ ���� ���� �ּ�
	D3DXVECTOR3				_startAccelateMax;		//��ƼŬ ���� ���� �ִ�
	float					_startScaleMin;			//��ƼŬ ���� ������ �ּ�
	float					_startScaleMax;			//��ƼŬ ���� ������ �ּ�
	bool					_isEmission;			//���� �������̴�?
	float					_emissionIntervalTime;	//���� ���� �ð�
	float					_emissionDeltaTime;		//�ϳ� �����ϰ� ���� ���� �ð�
	LPDIRECT3DBASETEXTURE9	_particleTex;			//��ƼŬ �ؽ���
	bool					_isLocal;				//�����̴�?

public:
	PATICLE_EMISSION_TYPE	emissionType;			//��ƼŬ ���� Ÿ��
	//particle ���� ����
	float					minEmissionRangeX;		//x ��������
	float					maxEmissionRangeX;
	float					minEmissionRangeY;		//y ��������
	float					maxEmissionRangeY;
	float					minEmissionRangeZ;		//z ��������
	float					maxEmissionRangeZ;
	float					sphereEmissionRange;	//�������� ���� ������

public:
	particleEmitter() {}
	~particleEmitter() {}

	void init(
		DWORD particleNum,							//�� ��ƼŬ ��
		float emission,								//�ʴ� �߻���
		float liveTimeMin,							//��ƼŬ �ϳ��� ����ð�
		float liveTimeMax,							//��ƼŬ �ϳ��� ����ð�
		const D3DXVECTOR3&	velocityMin,			//��ƼŬ ���� �ӵ� �ּ�
		const D3DXVECTOR3&	velocityMax,			//��ƼŬ ���� �ӵ� �ִ�
		const D3DXVECTOR3&	accelMin,				//��ƼŬ ���ӵ� �ּ�
		const D3DXVECTOR3&	accelMax,				//��ƼŬ ���ӵ� �ִ�
		const VEC_COLOR&	colors,					//��ƼŬ �÷� ���͹迭
		const VEC_SCALE&	scales,					//��ƼŬ ������ ���� �迭
		float scaleMin,								//��ƼŬ�� ������ �ּ�
		float scaleMax,								//��ƼŬ�� ������ �ִ�
		LPDIRECT3DTEXTURE9	pParticleTexture,		//��ƼŬ �ؽ���
		bool isLocal = false						//true �̸� ��ƼŬ�� �������� ���� Transform Local �������� �����δ�.(����)
	);

	void release();

	//BaseObject ��  Update �� ����
	virtual void baseObjectUpdate() override;
	virtual void baseObjectRender() override;
	
	//��ƼŬ ���� ����
	void startEmission();
	//��ƼŬ ���� ����
	void stopEmission();		

	//��� �ȹ����� ���ڸ� ��Ʈ����.
	void burst(int num, float minSpeed, float maxSpeed, float maxLife, float minLife);

	void setRenderLocal(bool isLocal) { _isLocal = isLocal; }
	particle* getParticle(void) { return _particles; }
private:
	//��ƼŬ �ϳ� ����
	void startOneParticle();
	//��ƼŬ �ϳ� ����(���� �־)
	void startOneParticle(D3DXVECTOR3 dir, float life);
};