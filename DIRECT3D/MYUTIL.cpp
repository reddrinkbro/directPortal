#include "stdafx.h"
#include "MYUTIL.h"

LPDIRECT3DVERTEXBUFFER9 MYUTIL::_vb;				//���� ����
LPDIRECT3DINDEXBUFFER9 MYUTIL::_ib;					//�ε��� ����
LPDIRECT3DVERTEXDECLARATION9	_vDeclation;		//������������
void MYUTIL::createCube()
{
	//		5--------6
	//     /|       /|
	//    4--------7 |
	//    | |  *   | |     ==> ����(0, 0, 0)
	//    | 1------| 2
	//    |/       |/
	//    0--------3
	//���� ���� �����

	device->CreateVertexBuffer(sizeof(MYVERTEX) * 24, 0, MYVERTEX::FVF, D3DPOOL_MANAGED, &_vb, NULL);

	//���� �÷� ����( ���� 4���� ������ ��� ����)
	MYVERTEX quad[4];

	quad[0].pos = D3DXVECTOR3(-1, -1, -1);
	quad[1].pos = D3DXVECTOR3(-1, 1, -1);
	quad[2].pos = D3DXVECTOR3(1, -1, -1);
	quad[3].pos = D3DXVECTOR3(1, 1, -1);


	//4������ ȸ���ϸ鼭 ������ ���� ��Ʈ����

	//�����迭 �غ�
	MYVERTEX vertices[24];

	//�ո� ����
	{
		memcpy(vertices, quad, sizeof(MYVERTEX) * 4);
		vertices[0].color = 0xffff0000;
		vertices[1].color = 0xffff0000;
		vertices[2].color = 0xffff0000;
		vertices[3].color = 0xffff0000;
	}

	D3DXMATRIXA16 matRot;

	//������ ����
	{
		D3DXMatrixIdentity(&matRot);
		D3DXMatrixRotationY(&matRot, D3DXToRadian(90));

		for (int i = 0; i < 4; i++)
		{
			D3DXVec3TransformCoord(&quad[i].pos, &quad[i].pos, &matRot);
		}
		memcpy(&vertices[4], quad, sizeof(MYVERTEX) * 4);
		vertices[4].color = 0xff00ffff;
		vertices[5].color = 0xff00ff00;
		vertices[6].color = 0xff00ff00;
		vertices[7].color = 0xff00ff00;
	}



	//���� ����
	{
		D3DXMatrixIdentity(&matRot);
		D3DXMatrixRotationY(&matRot, D3DXToRadian(180));

		for (int i = 0; i < 4; i++)
		{
			D3DXVec3TransformCoord(&quad[i].pos, &quad[i].pos, &matRot);
		}
		memcpy(&vertices[8], quad, sizeof(MYVERTEX) * 4);
		vertices[8].color = 0xff00ff00;
		vertices[9].color = 0xff00ff00;
		vertices[10].color = 0xff00ff00;
		vertices[11].color = 0xff00ff00;
	}



	//�޸� ����	
	{
		D3DXMatrixIdentity(&matRot);
		D3DXMatrixRotationY(&matRot, D3DXToRadian(270));

		for (int i = 0; i < 4; i++)
		{
			D3DXVec3TransformCoord(&quad[i].pos, &quad[i].pos, &matRot);
		}
		memcpy(&vertices[12], quad, sizeof(MYVERTEX) * 4);
		vertices[12].color = 0xffff0000;
		vertices[13].color = 0xffff0000;
		vertices[14].color = 0xffff0000;
		vertices[15].color = 0xffff0000;
	}


	//���� ����
	{
		D3DXMatrixIdentity(&matRot);
		D3DXMatrixRotationX(&matRot, D3DXToRadian(-90));

		for (int i = 0; i < 4; i++)
		{
			D3DXVec3TransformCoord(&quad[i].pos, &quad[i].pos, &matRot);
		}
		memcpy(&vertices[16], quad, sizeof(MYVERTEX) * 4);
		vertices[16].color = 0xff0000ff;
		vertices[17].color = 0xff0000ff;
		vertices[18].color = 0xff0000ff;
		vertices[19].color = 0xff0000ff;
	}

	//�ظ� ����
	{
		D3DXMatrixIdentity(&matRot);
		D3DXMatrixRotationX(&matRot, D3DXToRadian(180));

		for (int i = 0; i < 4; i++)
		{
			D3DXVec3TransformCoord(&quad[i].pos, &quad[i].pos, &matRot);
		}
		memcpy(&vertices[20], quad, sizeof(MYVERTEX) * 4);
		vertices[20].color = 0xff0000ff;
		vertices[21].color = 0xff0000ff;
		vertices[22].color = 0xff0000ff;
		vertices[23].color = 0xff0000ff;
	}


	//���� ���� ���ؽ����ۿ� �־��ֱ�
	void* pVertices;

	_vb->Lock(0, 0, (void**)&pVertices, 0);
	memcpy(pVertices, vertices, sizeof(MYVERTEX) * 24);
	_vb->Unlock();

	//�ε��� ���� ����
	device->CreateIndexBuffer(sizeof(DWORD), 0, D3DFMT_INDEX32, D3DPOOL_MANAGED, &_ib, NULL);

	//�ε��� ����

	_ib->Lock(0, 0, (void**)&pVertices, 0);
	_ib->Unlock();
}

void MYUTIL::renderCube()
{
	device->SetFVF(MYVERTEX::FVF);
	device->SetStreamSource(0, _vb, 0, sizeof(MYVERTEX));
	for (int i = 0; i < 6; i++)
	{
		device->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, 2);
	}
}

void MYUTIL::releaseVI()
{
	SAFE_RELEASE(_vb);
	SAFE_RELEASE(_ib);
}

void MYUTIL::createNormalCube()
{
	//���� ���� �����
	device->CreateVertexBuffer(
		sizeof(MYVERTEX_PN) * 24,	//������� ���������� ũ��(�ﰢ���� ���� 3�� �ʿ�)
		D3DUSAGE_WRITEONLY,			//��������� ���������� �Ӽ�(�ϴ� writeonly�� ������ ������, �Ŀ� �������ۿ� ������ ���� �Ͼ�ٸ� ���̳������� ��������)
		MYVERTEX_PN::FVF,			//FVF����
		D3DPOOL_DEFAULT,			//�޸�Ǯ �������
		&_vb,						//������ ���ع���
		NULL);						//�׳� ������ �������� ���� NULL

									//���� �÷� ����
	MYVERTEX_PN quad[4];
	quad[0].pos = D3DXVECTOR3(-0.5f, -0.5f, -0.5f);
	quad[1].pos = D3DXVECTOR3(-0.5f, 0.5f, -0.5f);
	quad[2].pos = D3DXVECTOR3(0.5f, 0.5f, -0.5f);
	quad[3].pos = D3DXVECTOR3(0.5f, -0.5f, -0.5f);

	D3DXMATRIXA16 matRot;
	D3DXMatrixRotationY(&matRot, D3DXToRadian(-90));

	//���� �迭 �غ�
	MYVERTEX_PN vertices[24];

	//�ո�
	memcpy(vertices, quad, sizeof(MYVERTEX_PN) * 4);
	vertices[0].normal = D3DXVECTOR3(0, 0, -1);
	vertices[1].normal = D3DXVECTOR3(0, 0, -1);
	vertices[2].normal = D3DXVECTOR3(0, 0, -1);
	vertices[3].normal = D3DXVECTOR3(0, 0, -1);

	//4�� ���� ������
	for (int i = 0; i < 4; i++)
	{
		D3DXVec3TransformCoord(&quad[i].pos, &quad[i].pos, &matRot);
	}

	//������ ����
	memcpy(vertices + 4, quad, sizeof(MYVERTEX_PN) * 4);
	vertices[4].normal = D3DXVECTOR3(1, 0, 0);
	vertices[5].normal = D3DXVECTOR3(1, 0, 0);
	vertices[6].normal = D3DXVECTOR3(1, 0, 0);
	vertices[7].normal = D3DXVECTOR3(1, 0, 0);

	//4�� ���� ������
	for (int i = 0; i < 4; i++)
	{
		D3DXVec3TransformCoord(&quad[i].pos, &quad[i].pos, &matRot);
	}

	//�޸� ����
	memcpy(vertices + 8, quad, sizeof(MYVERTEX_PN) * 4);
	vertices[8].normal = D3DXVECTOR3(0, 0, 1);
	vertices[9].normal = D3DXVECTOR3(0, 0, 1);
	vertices[10].normal = D3DXVECTOR3(0, 0, 1);
	vertices[11].normal = D3DXVECTOR3(0, 0, 1);

	//4�� ���� ������
	for (int i = 0; i < 4; i++)
	{
		D3DXVec3TransformCoord(&quad[i].pos, &quad[i].pos, &matRot);
	}

	//������ ����
	memcpy(vertices + 12, quad, sizeof(MYVERTEX_PN) * 4);
	vertices[12].normal = D3DXVECTOR3(-1, 0, 0);
	vertices[13].normal = D3DXVECTOR3(-1, 0, 0);
	vertices[14].normal = D3DXVECTOR3(-1, 0, 0);
	vertices[15].normal = D3DXVECTOR3(-1, 0, 0);

	D3DXMatrixRotationZ(&matRot, D3DXToRadian(-90));

	//4�� ���� ������
	for (int i = 0; i < 4; i++)
	{
		D3DXVec3TransformCoord(&quad[i].pos, &quad[i].pos, &matRot);
	}

	//���� ����
	memcpy(vertices + 16, quad, sizeof(MYVERTEX_PN) * 4);
	vertices[16].normal = D3DXVECTOR3(0, 1, 0);
	vertices[17].normal = D3DXVECTOR3(0, 1, 0);
	vertices[18].normal = D3DXVECTOR3(0, 1, 0);
	vertices[19].normal = D3DXVECTOR3(0, 1, 0);

	D3DXMatrixRotationZ(&matRot, D3DXToRadian(-180));

	//4�� ���� ������
	for (int i = 0; i < 4; i++)
	{
		D3DXVec3TransformCoord(&quad[i].pos, &quad[i].pos, &matRot);
	}

	//�Ʒ��� ����
	memcpy(vertices + 20, quad, sizeof(MYVERTEX_PN) * 4);
	vertices[20].normal = D3DXVECTOR3(0, -1, 0);
	vertices[21].normal = D3DXVECTOR3(0, -1, 0);
	vertices[22].normal = D3DXVECTOR3(0, -1, 0);
	vertices[23].normal = D3DXVECTOR3(0, -1, 0);

	//���� ��
	void* pVertices = NULL;
	_vb->Lock(0, 0, &pVertices, 0);
	//�޸� ����
	memcpy(pVertices, vertices, sizeof(MYVERTEX_PN) * 24);
	_vb->Unlock();

	//�ε��� ���� ����
	device->CreateIndexBuffer(
		sizeof(WORD) * 36,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_DEFAULT,
		&_ib,
		NULL);

	//		5--------6
	//     /|       /|
	//    4--------7 |
	//    | |      | | 
	//    | 1------| 2
	//    |/       |/
	//    0--------3

	WORD indices[36];
	for (int i = 0; i < 6; i++)
	{
		//���� ��ŸƮ �ε���
		WORD vertexIndex = i * 4;
		//�ε��� �迭 ��ŸƮ �ε���
		WORD indicesIndex = i * 6;

		indices[indicesIndex + 0] = vertexIndex + 0;
		indices[indicesIndex + 1] = vertexIndex + 1;
		indices[indicesIndex + 2] = vertexIndex + 2;
		indices[indicesIndex + 3] = vertexIndex + 0;
		indices[indicesIndex + 4] = vertexIndex + 2;
		indices[indicesIndex + 5] = vertexIndex + 3;
	}

	//�ε��� ��
	void* pIndices = NULL;
	_ib->Lock(0, 0, &pIndices, 0);
	memcpy(pIndices, indices, sizeof(WORD) * 36);
	_ib->Unlock();
}

void MYUTIL::renderNormalCube()
{
	//���ؽ����� ����
	device->SetStreamSource(0, _vb, 0, sizeof(MYVERTEX_PN));
	//�ε������� ����
	device->SetIndices(_ib);
	//���� �׸��� ������ ���� �����÷��� ����
	device->SetFVF(MYVERTEX_PN::FVF);
	//�׸���
	device->DrawIndexedPrimitive(
		D3DPT_TRIANGLELIST,	//�׸� ���� ���
		0,					//���ؽ��迭 ����
		0,					//0�϶� ó������ ������
		24,					//���� ����
		0,					//�ε����迭 ����
		12					//�׸��� ��ü��(�ﰢ�� 12��)
	);
}

void MYUTIL::createQuadCube()
{
	//�������� ����
	device->CreateVertexBuffer(
		sizeof(MYVERTEX_PU) * 4,
		D3DUSAGE_WRITEONLY,
		MYVERTEX_PU::FVF,
		D3DPOOL_DEFAULT,
		&_vb,
		NULL);

	// 1---2
	// |  /|
	// | / |
	// 0---3
	LPMYVERTEX_PU pVertices = NULL;
	_vb->Lock(0, 0, (void**)&pVertices, 0);
	pVertices[0].pos = D3DXVECTOR3(-0.8, -1.2f, 0);
	pVertices[1].pos = D3DXVECTOR3(-0.8, 1.2f, 0);
	pVertices[2].pos = D3DXVECTOR3(0.8, 1.2f, 0);
	pVertices[3].pos = D3DXVECTOR3(0.8, -1.2f, 0);
	//UV��ǥ�� 0.0f ~ 1.0f (��������ǥ��� ���� lt(0,0), rt(1,0), lb(0,1), rb(1,1))
	//�ִ밪�� 1�ε� 2���ϸ� �� 4���� �̹���, 3�����ϸ� �� 9���� �̹����� ���´�
	pVertices[0].uv = D3DXVECTOR2(0, 1);
	pVertices[1].uv = D3DXVECTOR2(0, 0);
	pVertices[2].uv = D3DXVECTOR2(1, 0);
	pVertices[3].uv = D3DXVECTOR2(1, 1);
	_vb->Unlock();

	// 1---2
	// |  /|
	// | / |
	// 0---3
	//�ε������� ����
	device->CreateIndexBuffer(
		sizeof(DWORD) * 6,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX32,
		D3DPOOL_DEFAULT,
		&_ib,
		NULL);

	DWORD idx[] = { 0, 1, 2, 0, 2, 3 };

	void* pIndices = NULL;
	_ib->Lock(0, 0, &pIndices, 0);
	memcpy(pIndices, idx, sizeof(DWORD) * 6);
	_ib->Unlock();
}

void MYUTIL::renderQuadCube()
{
	//���ؽ����� ����
	device->SetStreamSource(0, _vb, 0, sizeof(MYVERTEX_PU));
	//�ε������� ����
	device->SetIndices(_ib);
	//���� �׸��� ������ ���� �����÷��� ����
	device->SetFVF(MYVERTEX_PU::FVF);
	//�׸��� (4�� ���� �ﰢ�� 2��)
	device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
}

void MYUTIL::createQuadBumpNormal()
{
	//������ġ
	D3DXVECTOR3	vertexPos[] =
	{
		D3DXVECTOR3(-1, -1, 0),
		D3DXVECTOR3(-1, 1, 0),
		D3DXVECTOR3(1, 1, 0),
		D3DXVECTOR3(1, -1, 0)
	};

	//����UV
	D3DXVECTOR2 vertexUV[] =
	{
		D3DXVECTOR2(0, 1),
		D3DXVECTOR2(0, 0),
		D3DXVECTOR2(1, 0),
		D3DXVECTOR2(1, 1)
	};

	//���� �ε���
	DWORD vertexIndices[] = { 0, 1, 2, 0, 2, 3 };

	//�����븻
	D3DXVECTOR3 vertexNormal[4];
	D3DXVECTOR3 vertexTangent[4];
	D3DXVECTOR3 vertexBinormal[4];

	//�����븻 ������ִ� �Լ�(MyUtil�ȿ� ����) 
	ComputeNormal(vertexNormal, vertexPos, 4, vertexIndices, 6);

	//ź��Ʈ ���̳븻 ������ִ� �Լ�
	ComputeTangentBinormal(
		vertexTangent,
		vertexBinormal,
		vertexPos,
		vertexNormal,
		vertexUV,
		vertexIndices,
		2,
		4);

	//�������� ����
	device->CreateVertexBuffer(
		sizeof(MYVERTEX_PT) * 4,
		D3DUSAGE_WRITEONLY,
		0,
		D3DPOOL_DEFAULT,
		&_vb,
		NULL);

	LPMYVERTEX_PT pVertices = NULL;
	_vb->Lock(0, 0, (void**)&pVertices, 0);
	for (int i = 0; i < 4; i++)
	{
		pVertices[i].pos = vertexPos[i];
		pVertices[i].uv = vertexUV[i];
		pVertices[i].normal = vertexNormal[i];
		pVertices[i].tangent = vertexTangent[i];
		pVertices[i].binormal = vertexBinormal[i];
	}
	_vb->Unlock();

	//�ε��� ���� ����
	device->CreateIndexBuffer(
		sizeof(DWORD) * 6,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX32,
		D3DPOOL_DEFAULT,
		&_ib,
		NULL);

	void* pIndices = NULL;
	_ib->Lock(0, 0, &pIndices, 0);
	memcpy(pIndices, vertexIndices, sizeof(DWORD) * 6);
	_ib->Unlock();

	//������ ���¸� �˷��ִ� �迭
	D3DVERTEXELEMENT9	vertexElement[6];

	//POS
	vertexElement[0].Stream = 0;						//��Ʈ��
	vertexElement[0].Offset = 0;						//�޸� ���� ����Ʈ ����
	vertexElement[0].Type = D3DDECLTYPE_FLOAT3;			//�ڷ�Ÿ��
	vertexElement[0].Method = D3DDECLMETHOD_DEFAULT;	//�׳� ����Ʈ
	vertexElement[0].Usage = D3DDECLUSAGE_POSITION;		//������ ��������
	vertexElement[0].UsageIndex = 0;					//�ϴ� 0

														//UV
	vertexElement[1].Stream = 0;						//��Ʈ��
	vertexElement[1].Offset = 12;						//�޸� ���� ����Ʈ ����
	vertexElement[1].Type = D3DDECLTYPE_FLOAT2;			//�ڷ�Ÿ��
	vertexElement[1].Method = D3DDECLMETHOD_DEFAULT;	//�׳� ����Ʈ
	vertexElement[1].Usage = D3DDECLUSAGE_TEXCOORD;		//�ؽ��� ��������
	vertexElement[1].UsageIndex = 0;					//�ϴ� 0

														//Normal
	vertexElement[2].Stream = 0;						//��Ʈ��
	vertexElement[2].Offset = 20;						//�޸� ���� ����Ʈ ����
	vertexElement[2].Type = D3DDECLTYPE_FLOAT3;			//�ڷ�Ÿ��
	vertexElement[2].Method = D3DDECLMETHOD_DEFAULT;	//�׳� ����Ʈ
	vertexElement[2].Usage = D3DDECLUSAGE_NORMAL;		//�ؽ��� ��������
	vertexElement[2].UsageIndex = 0;					//�ϴ� 0

														//biNormal
	vertexElement[3].Stream = 0;						//��Ʈ��
	vertexElement[3].Offset = 32;						//�޸� ���� ����Ʈ ����
	vertexElement[3].Type = D3DDECLTYPE_FLOAT3;			//�ڷ�Ÿ��
	vertexElement[3].Method = D3DDECLMETHOD_DEFAULT;	//�׳� ����Ʈ
	vertexElement[3].Usage = D3DDECLUSAGE_BINORMAL;		//�ؽ��� ��������
	vertexElement[3].UsageIndex = 0;					//�ϴ� 0	

														//tangent
	vertexElement[4].Stream = 0;						//��Ʈ��
	vertexElement[4].Offset = 44;						//�޸� ���� ����Ʈ ����
	vertexElement[4].Type = D3DDECLTYPE_FLOAT3;			//�ڷ�Ÿ��
	vertexElement[4].Method = D3DDECLMETHOD_DEFAULT;	//�׳� ����Ʈ
	vertexElement[4].Usage = D3DDECLUSAGE_TANGENT;		//�ؽ��� ��������
	vertexElement[4].UsageIndex = 0;					//�ϴ� 0	

														//���̻� ������ ��������
	D3DVERTEXELEMENT9 end = D3DDECL_END();	//{0xFF,0,D3DDECLTYPE_UNUSED,0,0,0}
	vertexElement[5] = end;

	//������������ �����
	device->CreateVertexDeclaration(vertexElement, &_vDeclation);
}

void MYUTIL::renderQuadBumpNormal()
{
	//����׸���
	//���ؽ����� ����
	device->SetStreamSource(0, _vb, 0, sizeof(MYVERTEX_PT));
	//�ε������� ����
	device->SetIndices(_ib);
	//������������ ����
	device->SetVertexDeclaration(_vDeclation);
	//�׸��� (4�� ���� �ﰢ�� 2��)
	device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
}

POINT MYUTIL::getMousePos()
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(_hWnd, &pt);
	return pt;
}

void MYUTIL::setMousePos(int x, int y)
{
	POINT pt = { x,y };
	ClientToScreen(_hWnd, &pt);
	SetCursorPos(pt.x, pt.y);
}

float MYUTIL::clamp(float value, float min, float max)
{
	if (value < min)
	{
		return min;
	}
	else if (value > max)
	{
		return max;
	}
	else
	{
		return value;
	}
	return 0.0f;
}

float MYUTIL::clamp01(float value)
{
	if (value < 0)
	{
		return 0;
	}
	else if (value > 1)
	{
		return 1;
	}
	else
	{
		return value;
	}
	return 0.0f;
}

float MYUTIL::clampMinusPlusOne(float value)
{
	if (value < -1)
	{
		return -1;
	}
	else if (value > 1)
	{
		return 1;
	}
	else
	{
		return value;
	}
	return 0.0f;
}

float MYUTIL::lerp(float from, float to, float time)
{
	time = clamp01(time);
	float delta = to - from;

	return (delta * time + from);
}

D3DXVECTOR3 MYUTIL::vecLerp(D3DXVECTOR3 from, D3DXVECTOR3 to, float time)
{
	D3DXVECTOR3 result;
	result.x = lerp(from.x, to.x, time);
	result.y = lerp(from.y, to.y, time);
	result.z = lerp(from.z, to.z, time);

	//D3DXVec3Lerp(&result, &from, &to, time);
	return result;
}

float MYUTIL::inverseLerp(float head, float rear, float value)
{
	float delta = rear - head;
	float inValue = value - head;

	if (inValue < 0)
	{
		inValue = 0;
	}

	return inValue / delta;
}

void MYUTIL::initForRandomSeed()
{
	srand(GetTickCount());
}

int MYUTIL::randomIntRange(int min, int max)
{
	return rand() % (max - min + 1) + min;
}

float MYUTIL::randomFloatRange(float min, float max)
{
	//float rnd = (float)rand() / (float)RAND_MAX;
	//return (rnd * (max - min) + min);

	float delta = max - min;
	float factor = (rand() % 1001) * 0.001f;
	return (delta * factor) + min;
}

DWORD MYUTIL::FloatToDWORD(float f)
{
	float* pFloat = &f;

	DWORD* pDword = (DWORD*)pFloat;

	return *pDword;
}

void MYUTIL::ComputeNormal(D3DXVECTOR3 * pNorams, const D3DXVECTOR3 * pVertices, int verticesNum, const DWORD * pIndices, int indicesNum)
{
	//�븻���� �ʱ�ȭ
	ZeroMemory(pNorams, sizeof(D3DXVECTOR3) * verticesNum);

	//�ﰢ�� ����
	DWORD triNum = indicesNum / 3;
	for (DWORD i = 0; i < triNum; i++)
	{
		//�ش� �ﰢ���� ���� �ε���
		DWORD i0 = pIndices[(i * 3) + 0];
		DWORD i1 = pIndices[(i * 3) + 1];
		DWORD i2 = pIndices[(i * 3) + 2];

		//���� 3��
		D3DXVECTOR3 v0 = pVertices[i0];
		D3DXVECTOR3 v1 = pVertices[i1];
		D3DXVECTOR3 v2 = pVertices[i2];

		//Edge
		D3DXVECTOR3 edge1 = v1 - v0;
		D3DXVECTOR3 edge2 = v2 - v0;

		//Cross
		D3DXVECTOR3 cross;
		D3DXVec3Cross(&cross, &edge1, &edge2);

		//Normal
		D3DXVECTOR3 normal;
		D3DXVec3Normalize(&normal, &cross);

		//���� ��Ų��.
		pNorams[i0] += normal;
		pNorams[i1] += normal;
		pNorams[i2] += normal;
	}

	//���� ������ Normalvector ����ȭ�Ѵ�.
	for (DWORD i = 0; i < verticesNum; i++)
	{
		D3DXVec3Normalize(&pNorams[i], &pNorams[i]);
	}
}

void MYUTIL::ComputeTangentBinormal(D3DXVECTOR3 * outTangets, D3DXVECTOR3 * outBinormals, const D3DXVECTOR3 * pPositions, const D3DXVECTOR3 * pNormals, const D3DXVECTOR2 * pUVs, const DWORD * pIndices, DWORD NumTris, DWORD NumVertices)
{
	//ź��Ʈ ���̳븻 �ʱ�ȭ
	ZeroMemory(outTangets, sizeof(D3DXVECTOR3) * NumVertices);
	ZeroMemory(outBinormals, sizeof(D3DXVECTOR3) * NumVertices);

	//�ӽ� Tangent Binormal �迭
	D3DXVECTOR3* pTangents = new D3DXVECTOR3[NumVertices];
	D3DXVECTOR3* pBinormals = new D3DXVECTOR3[NumVertices];

	//�ӽ� Tangent Binormal �迭 �ʱ�ȭ
	ZeroMemory(pTangents, sizeof(D3DXVECTOR3) * NumVertices);
	ZeroMemory(pBinormals, sizeof(D3DXVECTOR3) * NumVertices);

	//�ϴ� �ﰢ�� �����....
	for (DWORD a = 0; a < NumTris; a++)
	{
		//�ش� �ﰢ���� �ε���
		DWORD i0 = pIndices[a * 3 + 0];
		DWORD i1 = pIndices[a * 3 + 1];
		DWORD i2 = pIndices[a * 3 + 2];

		//�ش� �ﰢ���� ������ġ
		D3DXVECTOR3 p0 = pPositions[i0];
		D3DXVECTOR3 p1 = pPositions[i1];
		D3DXVECTOR3 p2 = pPositions[i2];

		//�ش� �ﰢ���� UV
		D3DXVECTOR2 uv0 = pUVs[i0];
		D3DXVECTOR2 uv1 = pUVs[i1];
		D3DXVECTOR2 uv2 = pUVs[i2];

		//������ Edge
		D3DXVECTOR3 edge1 = p1 - p0;
		D3DXVECTOR3 edge2 = p2 - p0;

		//UV Edge
		D3DXVECTOR2 uvEdge1 = uv1 - uv0;
		D3DXVECTOR2 uvEdge2 = uv2 - uv0;

		// ���� ������ ������ ���� ������ ����
		// edge1 = ( uvEdge1.x ) * Tangent + ( uvEdge1.y ) * Binormal;
		// edge2 = ( uvEdge2.x ) * Tangent + ( uvEdge2.y ) * Binormal;

		// ������ ���� ġȯ
		// E1 = edge1;
		// E2 = edge2;
		// U1 = uvEdge1.x;
		// V1 = uvEdge1.y;
		// U2 = uvEdge2.x;
		// V2 = uvEdge2.y;
		// T = Tangent;
		// B = Binormal;

		// E1 = U1 * T + V1 * B;
		// E2 = U2 * T + V2 * B;

		// | E1 |   | U1 , V1 | | T |
		// |    | = |         | |   |
		// | E2 |   | U2 , V2 | | B |

		// | T |          1        |  V2 , -V1 | | E1 |
		// |   | = --------------- |           | |    |
		// | B |    U1*V2 - V1*U2  | -U2 ,  U1 | | E2 |

		// R = 1 / U1*V2 - V1*U2;

		// T = ( ( E1 * V2 ) + ( E2 * -V1 ) ) * R
		// B = ( ( E1 * -U2 ) + ( E2 * U1 ) ) * R

		float r = 1.0f / ((uvEdge1.x * uvEdge2.y) - (uvEdge1.y * uvEdge2.x));

		//Tangent
		D3DXVECTOR3 t = ((edge1 * uvEdge2.y) + (edge2 * -uvEdge1.y)) * r;
		D3DXVec3Normalize(&t, &t);

		//Binormal 
		D3DXVECTOR3 b = ((edge1 * -uvEdge2.x) + (edge2 * uvEdge1.x)) * r;
		D3DXVec3Normalize(&b, &b);

		//ź��Ʈ ���̳븻 �ӽ� �迭�� �߰�
		pTangents[i0] += t;
		pTangents[i1] += t;
		pTangents[i2] += t;
		pBinormals[i0] += b;
		pBinormals[i1] += b;
		pBinormals[i2] += b;
	}

	//Binormal Tangent �븻�� ����ȭ
	for (int i = 0; i < NumVertices; i++)
	{
		D3DXVECTOR3 n = pNormals[i];

		//Tangent �׶�����Ʈ ����
		D3DXVECTOR3 t = pTangents[i] - (D3DXVec3Dot(&pTangents[i], &n) * n);
		D3DXVec3Normalize(&t, &t);

		//�븻�� ����ȭ�� T �� �����Ͽ� B
		D3DXVECTOR3 b;
		D3DXVec3Cross(&b, &n, &t);
		D3DXVec3Normalize(&b, &b);

		outTangets[i] = t;
		outBinormals[i] = b;
	}

	//�ӽ� �迭 ����
	SAFE_DELETE_ARRAY(pTangents);
	SAFE_DELETE_ARRAY(pBinormals);
}
