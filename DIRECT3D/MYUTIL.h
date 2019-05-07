//MYYUTIL.h
//2017 - 12 - 20
#pragma once

#define EPSILON 0.0001f
//�÷�Ʈ ������ ���� �� ���Ƿ� ������ ������ ����
//a = 10.00001f
//b = 10.00008f
//abs(a - b) < c  => a ,b �� ����

#define FLOATEQUAL(a,b)			( abs( (a) - (b) ) < EPSILON )									// a�� b �� ������
#define FLOATZERO(a)			(abs(a)  < EPSILON)												// a�� ���� 0 �� ������
#define VECTORZERO(v)			(FLOATZERO((v).x) && FLOATZERO((v).y) && FLOATZERO((v).z))		//

//���� ����ü
typedef struct tagRay
{
	D3DXVECTOR3 origin;		//������ ������ġ
	D3DXVECTOR3	direction;	//������ ����(����ȭ �Ǿ� �־�� ��)
}Ray, *LPRay;

namespace MYUTIL
{
	//==================================================================
	//		## ť�� ����� ##
	//==================================================================
	extern LPDIRECT3DVERTEXBUFFER9 _vb;										//���� ����
	extern LPDIRECT3DINDEXBUFFER9 _ib;										//�ε��� ����
	void createCube();														//ť�� ����	
	void renderCube();														//ť�� �׸���
	void releaseVI();														//ť�� ����

	void createNormalCube();												//�븻ť�� ����
	void renderNormalCube();												//ť�� �׸���

	void createQuadCube();													//����ť�� ����
	void renderQuadCube();													//����ť�� �׸���

	void createQuadBumpNormal();											//����븻����ť�� ����
	void renderQuadBumpNormal();											//����븻����ť�� �׸���
	//==================================================================
	//		## ���콺 ##
	//==================================================================
	POINT getMousePos();							//���콺 ��ġ ���
	void setMousePos(int x, int y);					//���콺 ��ġ ����


	//==================================================================
	//		## ���� ���� ##
	//==================================================================

	float clamp(float value, float min, float max);		//value ���� min ~ max ������ ������ ����		
	float clamp01(float value);							//value ���� 0~1 ������ ������ ����	
	float clampMinusPlusOne(float value);				//value ���� -1 ~ 1 ������ ������ ����

	//float ��������
	float lerp(float from, float to, float time);

	//���� ���� ����
	D3DXVECTOR3 vecLerp(D3DXVECTOR3 from, D3DXVECTOR3 to, float time);
	float inverseLerp(float head, float rear, float value);


	//==================================================================
	//		## ���� �Լ� ##
	//==================================================================

	void initForRandomSeed();						//���� �õ� ����
	int randomIntRange(int min, int max);			//min ~ max ������ ���� ���� ����
	float randomFloatRange(float min, float max);	//min ~ max ������ ���� �Ǽ� ����
	DWORD FloatToDWORD(float f);

	//==================================================================
	//		## �븻��� ##
	//==================================================================
	//������ �븻�� ������ִ� �Լ�
	void ComputeNormal(
		D3DXVECTOR3* pNorams,			//����� ���� �븻���� �迭 ������ ( �븻������ �迭�� ���� ���� �迭�� ���� ���� )
		const D3DXVECTOR3* pVertices,	//��꿡 ���� ������ġ �迭 ������
		int verticesNum,				//���� ����
		const DWORD* pIndices,			//�ε��� �迭������ ( �ϴ� �ε����� DWORD �� �뵿�ܰ� )
		int indicesNum					//�ε��� �迭������ ����
	);

	//ź��Ʈ�� ���̳븻 ���
	void ComputeTangentBinormal(
		D3DXVECTOR3* outTangets,			//��� Tangent �迭
		D3DXVECTOR3* outBinormals,			//��� Binormal �迭
		const D3DXVECTOR3* pPositions,		//���� ��ġ �迭
		const D3DXVECTOR3* pNormals,		//���� �븻 �迭
		const D3DXVECTOR2* pUVs,			//������ UV �迭
		const DWORD* pIndices,				//�ε��� �迭
		DWORD NumTris,						//�ﰢ�� ����		
		DWORD NumVertices 					//���� ����
	);
}



