#pragma once

//��ƼŬ ���� ����ü
typedef struct tagquadParticle_VERTEX
{
	D3DXVECTOR3 pos;			//���� ��ġ
	DWORD color;				//���� �÷�
	D3DXVECTOR2 uv;				//���� UV
	enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 };
}quadParticle_VERTEX, *LPquadParticle_VERTEX;

typedef vector<D3DXCOLOR>		VEC_COLOR;
typedef vector<float>			VEC_SCALE;

class quadParticle
{
public:
	transform		_transform;				//��ƼŬ�� ��ġ��

private:
	bool			_isLive;				//Ȱ��ȭ ����
	float			_totalLiveTime;			//�� Ȱ��ȭ �ð�
	float			_deltaLiveTime;			//Ȱ��ȭ ���� �ð�
	float			_nomalizeLiveTime;		//Ȱ��ȭ ���� �ð�( 0 ~ 1 )
	D3DXVECTOR3		_velocity;				//��ƼŬ�� �ӵ�����
	D3DXVECTOR3		_acceleration;			//�ʴ� �����ϴ� ���Ӻ���
	D3DXVECTOR3		_rotate;				//�ʴ� ȸ�� ����
	D3DXVECTOR3		_rotateAccel;			//�ʴ� �����ϴ� ȸ������
	float			_scale;					//�⺻ �����ϰ�

public:
	void Start(
		float liveTime,						//���̺� Ÿ��
		const D3DXVECTOR3* pos,				//���� ��ġ
		const D3DXVECTOR3* velo,			//���� �ӵ�
		const D3DXVECTOR3* accel,			//���� ��
		const D3DXVECTOR3* rotate,			//�ʴ� ȸ�� ��
		const D3DXVECTOR3* rotateAccel,		//�ʴ� ȸ�� ������
		float scale							//�⺻ ������
	);

	void update();

	//�ڽ��� ���� ������ �������� LPquadParticle_VERTEX �� ���� �־��ش�.
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

