#pragma once

//��ƼŬ ���� ����ü
typedef struct tagPARTICLE_VERTEX
{
	D3DXVECTOR3 pos;				//���� ��ġ
	float size;						//��ƼŬ ���� ���� ������
	DWORD color;					//���� �÷�
	enum { FVF = D3DFVF_XYZ | D3DFVF_PSIZE | D3DFVF_DIFFUSE };
}PARTICLE_VERTEX, *LPPARTICLE_VERTEX;

typedef vector<D3DXCOLOR>   VEC_COLOR;
typedef vector<float>		VEC_SCALE;

//���� �ϳ��� ���� ����
class particle
{
public:
	transform		_transform;				//��ƼŬ�� ��ġ��
	boundBox		_boundBox;
private:
	bool			_isLive;				//Ȱ��ȭ ����
	float			_totalLiveTime;			//�� Ȱ��ȭ �ð�
	float			_deltaLiveTime;			//Ȱ��ȭ ���� �ð�
	float			_normalizeLiveTime;		//Ȱ��ȭ ���� �ð�(0~1)
	D3DXVECTOR3		_velocity;				//��ƼŬ�� �ӵ�����
	D3DXVECTOR3		_acceleration;			//�ʴ� �����ϴ� ���� ����
	float			_scale;					//�⺻ �����ϰ�

public:
	void start(
		float liveTime,						//���̺�Ÿ��
		const D3DXVECTOR3* pos,				//���� ��ġ
		const D3DXVECTOR3* velocity,		//���ۼӵ�
		const D3DXVECTOR3* acceleration,	//���ӵ�
		float scale							//�⺻������
	);

	void update();

	//�ڽ��� ���� ������ �������� LPPATICLE_VERTEX  �� ���� �־��ش�.
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