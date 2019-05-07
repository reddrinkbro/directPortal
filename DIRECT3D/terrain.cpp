#include "stdafx.h"
#include "terrain.h"
#include "quadTree.h"

HRESULT terrain::init(int vertexNum, float cellSize)
{
	// ������
	_verNumX = vertexNum;
	_verNumZ = vertexNum;
	_totalVerNum = _verNumX * _verNumZ;

	// ����
	_cellNumX = _verNumX - 1;
	_cellNumZ = _verNumZ - 1;
	_totalCellNum = _cellNumX * _cellNumZ;

	// ��������
	_cellScale = cellSize;

	// ������ ����
	_totalTriangle = _totalCellNum * 2;

	// ���� ������
	_terrainSizeX = _cellNumX * cellSize;
	_terrainSizeZ = _cellNumZ * cellSize;

	// ���� ���� ����(���� ����� ���� ��ġ ������ �����)
	_vertexPosArray = new D3DXVECTOR3[_totalVerNum];

	// ���� ������ 0,0,0
	D3DXVECTOR3 startPos(0, 0, 0);

	for (int z = 0; z < _verNumZ; z++)
	{
		for (int x = 0; x < _verNumX; x++)
		{
			// ���� �ε���
			int idx = z * _verNumX + x;
			// ���� ��ġ
			_vertexPosArray[idx].x = startPos.x + (x * _cellScale);
			_vertexPosArray[idx].z = startPos.z + (z * _cellScale);
			// _vertexPosArray[idx].y = 0;
			_vertexPosArray[idx].y = sinf(x * D3DXToRadian(20)) * 3.0f;
		}
	}

	// ���� �ε���
	_indices = new TERRAININDEX[_totalTriangle];

	for (int z = 0; z < _cellNumZ; z++)
	{
		// �������
		int nowZ = z;
		int nextZ = z + 1;

		for (int x = 0; x < _cellNumX; x++)
		{
			//��������
			int nowX = x;
			int nextX = x + 1;

			// �簢�� �𼭸�(lt, rt, lb, rb)
			//	lt---rt
			//	|	/ |
			//	|  /  |
			//	| /	  |
			//	lb---rb

			// �𼭸� ���� �ε���
			DWORD lt = nextZ * _verNumX + nowX;
			DWORD rt = nextZ * _verNumX + nextX;
			DWORD lb = nowZ * _verNumX + nowX;
			DWORD rb = nowZ * _verNumX + nextX;

			// �ﰢ�� �迭�ε���
			int idx = (z * _cellNumX + x) * 2;
			// �������� 1
			// lb, lt, rt
			_indices[idx]._0 = lb;
			_indices[idx]._1 = lt;
			_indices[idx]._2 = rt;
			// �������� 2
			// lb, rt, rb
			_indices[idx + 1]._0 = lb;
			_indices[idx + 1]._1 = rt;
			_indices[idx + 1]._2 = rb;
		}
	}

	// ������ ���¸� �˷��ִ� �迭
	D3DVERTEXELEMENT9	vertElement[] = {
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		D3DDECL_END() };

	device->CreateVertexDeclaration(
		vertElement,
		&_terrainDecl);

	// ���� ���۸� ����
	if (FAILED(
		device->CreateVertexBuffer(
			sizeof(TERRAINVERTEX) * _totalVerNum,
			D3DUSAGE_WRITEONLY,
			0,
			D3DPOOL_DEFAULT,
			&_terrainVb,
			NULL))) return E_FAIL;

	LPTERRAINVERTEX	pVertices = NULL;
	if (SUCCEEDED(_terrainVb->Lock(0, 0, (void**)&pVertices, 0)))
	{
		for (int i = 0; i < _totalVerNum; i++)
		{
			//���� ��ġ ����
			pVertices[i].pos = _vertexPosArray[i];
		}
		_terrainVb->Unlock();
	}

	// �ε��� ���� ����
	if (FAILED(device->CreateIndexBuffer(
		sizeof(TERRAININDEX) * _totalTriangle,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX32,
		D3DPOOL_DEFAULT,
		&_terrainIb,
		NULL))) return E_FAIL;

	void* pIndices = NULL;

	if (SUCCEEDED(_terrainIb->Lock(0, 0, &pIndices, 0)))
	{
		memcpy(pIndices, _indices, sizeof(TERRAININDEX) * _totalTriangle);
		_terrainIb->Unlock();
	}

	return S_OK;
}

HRESULT terrain::init(char * heightMapName, char * tile_0, char * tile_1, char * tile_2, char * tile_3, char * tileSplat, float cellSize, float heightScale, int smoothLevel, int tileNum)
{
	// �����ϰ� ����
	_heightScale = heightScale;
	_cellScale = cellSize;

	// ���̸� �ҷ��´�
	_heightMap = RM_TEXTURE->addResource(heightMapName);
	// �ҷ����µ� �����ϸ� �ٷ� ����
	if (_heightMap == NULL) return E_FAIL;

	// ���� �ʿ� ���� �̹��� ������ �ҷ��´�. (�ε��� Texture ������ ��´�)
	D3DSURFACE_DESC sd;
	// GetLevelDesc(�Ӹ�ü�� ����, D3DSURFACE_DESC ����ü ������)
	_heightMap->GetLevelDesc(0, &sd);

	// ���μ��� ���� ���� ���Ѵ�.
	_verNumX = sd.Width + 1;	// ���� ���������� ���� �ʿ� ���� �ػ� + 1 �� ����. (������ ����Ʈ�������� ���������� 2��N�� + 1 �̿��� �ϱ� ������)
	_verNumZ = sd.Height + 1;	// ���� ��������� ���� �ʿ� ���� �ػ� + 1 �� ����. (������ ����Ʈ�������� ���������� 2��N�� + 1 �̿��� �ϱ� ������)
	_totalVerNum = _verNumX * _verNumZ;	// �� ���� ����

	// ���μ��� ����
	_cellNumX = _verNumX - 1;
	_cellNumZ = _verNumZ - 1;
	_totalCellNum = _cellNumX * _cellNumZ;

	// �� �ﰢ������
	_totalTriangle = _totalCellNum * 2;

	// �ͷ����� �����.
	// ������ ���� (�̰��� Ŭ���� ������ �ε巯������)
	// Ÿ�� Texture �� ��� ������������ ����
	if (FAILED(createTerrain(smoothLevel, tileNum)))
	{
		release();
		return E_FAIL;
	}

	// �ͷ��� ũ��
	_terrainSizeX = _cellNumX * _cellScale;
	_terrainSizeZ = _cellNumZ * _cellScale;

	// �ͷ��� ����
	_terrainStartX = _terrainVertices[0].pos.x;
	_terrainStartZ = _terrainVertices[0].pos.z;
	_terrainEndX = _terrainVertices[_totalVerNum - 1].pos.x;
	_terrainEndZ = _terrainVertices[_totalVerNum - 1].pos.z;

	// ����Ʈ���� �����.
	_quadTree = new quadTree;
	_quadTree->init(_terrainVertices, _verNumX);

	// ���� �ؽ��� �ε�
	_texTile_0 = RM_TEXTURE->addResource(tile_0);
	_texTile_1 = RM_TEXTURE->addResource(tile_1);
	_texTile_2 = RM_TEXTURE->addResource(tile_2);
	_texTile_3 = RM_TEXTURE->addResource(tile_3);
	_texSlat = RM_TEXTURE->addResource(tileSplat);

	// ���� ���̴�����Ʈ �ε�
	_terrainEffect = RM_SHADER->addResource("Resources/Shader/TerrainBase.fx");

	return S_OK;
}

void terrain::release(void)
{
	SAFE_RELEASE(_terrainVb);
	SAFE_RELEASE(_terrainIb);
	SAFE_DELETE_ARRAY(_vertexPosArray);
	SAFE_DELETE_ARRAY(_indices);
}

void terrain::update(void)
{
}

void terrain::render(void)
{
	//���̾������� ���
	device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	//������� �ʱ�ȭ
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);
	device->SetTransform(D3DTS_WORLD, &matWorld);

	//���� �׸���
	device->SetStreamSource(0, _terrainVb, 0, sizeof(TERRAINVERTEX));
	device->SetIndices(_terrainIb);
	device->SetVertexDeclaration(_terrainDecl);
	device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, _totalVerNum, 0, _totalTriangle);

	//�ָ������ ��ȯ(������ �׸��� �� ���� ����)
	device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

void terrain::render(camera * cam, lightDirection * directionLight)
{
	//���� ��ļ���
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);
	//���̴�����Ʈ ������� ����
	_terrainEffect->SetMatrix("matWorld", &matWorld);
	//���̴�����Ʈ ����� ����
	_terrainEffect->SetMatrix("matViewProjection", &cam->getViewProjectionMatrix());
	//���̴�����Ʈ �ؽ��� ����
	_terrainEffect->SetTexture("Terrain0_Tex", _texTile_0);
	_terrainEffect->SetTexture("Terrain1_Tex", _texTile_1);
	_terrainEffect->SetTexture("Terrain2_Tex", _texTile_2);
	_terrainEffect->SetTexture("Terrain3_Tex", _texTile_3);
	_terrainEffect->SetTexture("TerrainControl_Tex", _texSlat);
	//���̴�����Ʈ ���� ����
	D3DXVECTOR3 dirLight = directionLight->_transform.getForward();
	_terrainEffect->SetVector("worldLightDir", &D3DXVECTOR4(dirLight, 1));

	//���̴� ����
	UINT passNum = 0;
	_terrainEffect->Begin(&passNum, 0);
	for (UINT i = 0; i < passNum; i++)
	{
		_terrainEffect->BeginPass(i);
		device->SetStreamSource(0, _terrainVb, 0, sizeof(TERRAINVERTEX));
		device->SetVertexDeclaration(_terrainDecl);
		device->SetIndices(_terrainIb);
		device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, _totalVerNum, 0, _totalTriangle);
		_terrainEffect->EndPass();
	}
	_terrainEffect->End();

	/*
	//��������
	device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	device->SetStreamSource(0, _terrainVb, 0, sizeof(TERRAINVERTEX));
	device->SetIndices(_terrainIb);
	device->SetVertexDeclaration(_terrainDecl);
	device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, _totalVerNum, 0, _totalTriangle);
	device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	*/
}

void terrain::render(camera * cam, lightDirection * directionLight, camera * directionLightCamera)
{
	//ShadowMap ����
	_terrainEffect->SetTexture("Shadow_Tex", directionLightCamera->getRenderTexture());

	//ShadowViewProjection ���� 
	_terrainEffect->SetMatrix("matLightViewProjection", &directionLightCamera->getViewProjectionMatrix());

	//���� ��ļ���
	D3DXMATRIXA16 matInd;
	D3DXMatrixIdentity(&matInd);
	_terrainEffect->SetMatrix("matWorld", &matInd);

	//�� ��ļ���
	_terrainEffect->SetMatrix("matViewProjection", &cam->getViewProjectionMatrix());

	//Texture ����
	_terrainEffect->SetTexture("Terrain0_Tex", _texTile_0);
	_terrainEffect->SetTexture("Terrain1_Tex", _texTile_1);
	_terrainEffect->SetTexture("Terrain2_Tex", _texTile_2);
	_terrainEffect->SetTexture("Terrain3_Tex", _texTile_3);
	_terrainEffect->SetTexture("TerrainControl_Tex", _texSlat);

	//���� ����
	D3DXVECTOR3 dirLight = directionLight->_transform.getForward();
	_terrainEffect->SetVector("worldLightDir", &D3DXVECTOR4(dirLight, 1));

	_terrainEffect->SetTechnique("ReciveShadow");

	UINT passNum = 0;
	_terrainEffect->Begin(&passNum, 0);
	for (UINT i = 0; i < passNum; i++)
	{
		_terrainEffect->BeginPass(i);
		device->SetStreamSource(0, _terrainVb, 0, sizeof(TERRAINVERTEX));
		device->SetVertexDeclaration(this->_terrainDecl);
		device->SetIndices(_terrainIb);
		device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, _totalVerNum, 0, _totalTriangle);
		_terrainEffect->EndPass();
	}
	_terrainEffect->End();
}

void terrain::renderShadow(camera * directionLightCam)
{
	//���� ��ļ���
	D3DXMATRIXA16 matInd;
	D3DXMatrixIdentity(&matInd);
	_terrainEffect->SetMatrix("matWorld", &matInd);

	//�� ��ļ���
	_terrainEffect->SetMatrix("matViewProjection", &directionLightCam->getViewProjectionMatrix());

	_terrainEffect->SetTechnique("CreateShadow");

	UINT passNum = 0;
	_terrainEffect->Begin(&passNum, 0);
	for (UINT i = 0; i < passNum; i++)
	{
		_terrainEffect->BeginPass(i);
		device->SetStreamSource(0, _terrainVb, 0, sizeof(TERRAINVERTEX));
		device->SetVertexDeclaration(this->_terrainDecl);
		device->SetIndices(_terrainIb);
		device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, _totalVerNum, 0, _totalTriangle);
		_terrainEffect->EndPass();
	}
	_terrainEffect->End();
}

bool terrain::isIntersectRay(D3DXVECTOR3 * pOut, LPRay ray)
{
	vector<D3DXVECTOR3> hits;

	// �ֻ�� ���� Ʈ������ Ray Check ����.
	_quadTree->getRayHits(&hits, ray);

	// ��Ʈ�Ȱ� ����
	if (hits.size() == 0) return false;

	// ���� ó�� ��ġ�� ó�� ��ġ������ �Ÿ� ����
	D3DXVECTOR3 hitPos = hits[0];
	D3DXVECTOR3 dir = hitPos - ray->origin;
	float distSq = D3DXVec3LengthSq(&dir);

	for (int i = 1; i < hits.size(); ++i)
	{
		dir = hits[i] - ray->origin;
		float newDist = D3DXVec3LengthSq(&dir);

		if (newDist < distSq)
		{
			distSq = newDist;
			hitPos = hits[i];
		}
	}

	// ��������´ٸ� ���� �Ÿ��� ����� Hit ��ġ�� ���´�
	*pOut = hitPos;

	return true;
}

float terrain::getHeight(float x, float z)
{
	// �ͷ��� ������ �Ѿ�� 0.0 ���� �����Ѵ�
	if (x < _terrainStartX || x > _terrainEndX ||
		z > _terrainStartZ || z < _terrainEndZ)
	{
		return 0.0f;
	}

	// Terrain �� �»�� 0 �� �������� ���� Terrain �� ������ ��ġ�� ã��
	float pX = x - _terrainStartX;
	float pZ = -(z + _terrainEndZ);

	// �ش� ��ġ�� ��� ���� ���ԵǴ��� �ľ�
	float invCell = 1.0f / _cellScale;
	pX *= invCell;
	pZ *= invCell;

	// �ش� ��ġ�� �� �ε���
	int idxX = static_cast<int>(pX);
	int idxZ = static_cast<int>(pZ);

	// ���� �ױ��� ������ ��´�.
	// lt-----rt
	//  |    /|
	//  |   / |
	//  |  /  |
	//  | /   |
	//  |/    |
	// lb-----rb

	D3DXVECTOR3 lt = _terrainVertices[idxZ * _verNumX + idxX].pos;
	D3DXVECTOR3 rt = _terrainVertices[idxZ * _verNumX + idxX + 1].pos;
	D3DXVECTOR3 lb = _terrainVertices[(idxZ + 1) * _verNumX + idxX].pos;
	D3DXVECTOR3 rb = _terrainVertices[(idxZ + 1) * _verNumX + idxX + 1].pos;

	// �ش� �������� delta ���� ���Ѵ�
	float dX = pX - static_cast<float>(idxX);
	float dZ = pZ - static_cast<float>(idxZ);

	float height = 0.0f;

	// �ش� ���� �»�ܿ� �ִ�?
	if (dX < 1.0f - dZ)
	{
		float deltaU = rt.y - lt.y;		// lt �������� rt ���������� y ��ġ�� ������
		float deltaV = lb.y - lt.y;		// lt �������� lb ���������� y ��ġ�� ������

		height = lt.y + (deltaU * dX) + (deltaV * dZ);
	}
	// �ش� ���� ���ϴ��� �ִ�?
	else
	{
		float deltaU = lb.y - rb.y;
		float deltaV = rt.y - rb.y;

		// �쿡�� �·� �Ͽ��� ������ ���������� �ٲ���� ������
		// delta ���� ������ ���Ѵ�.
		dX = 1.0f - dX;
		dZ = 1.0f - dZ;

		height = rb.y + (deltaU * dX) + (deltaV * dZ);
	}

	return height;
}

bool terrain::getSlant(D3DXVECTOR3 * pOut, float gravityPower, float x, float z)
{
	// �ͷ��� ������ �Ѿ�� 0.0 ���� �����Ѵ�
	if (x < _terrainStartX || x > _terrainEndX ||
		z > _terrainStartZ || z < _terrainEndZ)
	{
		return false;
	}

	// Terrain �� �»�� 0 �� �������� ���� Terrain �� ������ ��ġ�� ã��
	float pX = x - _terrainStartX;
	float pZ = -(z + _terrainEndZ);

	// �ش� ��ġ�� ��� ���� ���ԵǴ��� �ľ�
	float invCell = 1.0f / _cellScale;
	pX *= invCell;
	pZ *= invCell;

	// �ش� ��ġ�� �� �ε���
	int idxX = static_cast<int>(pX);
	int idxZ = static_cast<int>(pZ);

	// ���� �ױ��� ������ ��´�.
	// lt-----rt
	//  |    /|
	//  |   / |
	//  |  /  |
	//  | /   |
	//  |/    |
	// lb-----rb

	D3DXVECTOR3 lt = _terrainVertices[idxZ * _verNumX + idxX].pos;
	D3DXVECTOR3 rt = _terrainVertices[idxZ * _verNumX + idxX + 1].pos;
	D3DXVECTOR3 lb = _terrainVertices[(idxZ + 1) * _verNumX + idxX].pos;
	D3DXVECTOR3 rb = _terrainVertices[(idxZ + 1) * _verNumX + idxX + 1].pos;

	// �ش� �п����� delta ���� ���Ѵ�.
	float dX = pX - static_cast<float>(idxX);
	float dZ = pZ - static_cast<float>(idxZ);

	float fHeight = 0.0f;

	// �������� �븻 ����
	D3DXVECTOR3 normal;

	// �ش� ���� �»�ܿ� �ִ�?
	if (dX < 1.0f - dZ)
	{
		//�ش��������� ���� ���͸� ���Ѵ�.
		D3DXVECTOR3 edge1 = rt - lt;
		D3DXVECTOR3 edge2 = lb - lt;
		D3DXVec3Cross(&normal, &edge1, &edge2);
	}

	// �ش� ���� ���ϴ��� �ִ�?
	else
	{
		// �ش��������� ���� ���͸� ���Ѵ�.
		D3DXVECTOR3 edge1 = rt - lb;
		D3DXVECTOR3 edge2 = rb - lb;
		D3DXVec3Cross(&normal, &edge1, &edge2);
	}

	// �븻�� ����ȭ
	D3DXVec3Normalize(&normal, &normal);

	// �߷� ����
	D3DXVECTOR3 gravity(0, -gravityPower, 0);

	// ������ ����
	D3DXVECTOR3 right;
	D3DXVec3Cross(&right, &normal, &gravity);

	// �������Ϳ��� �������͸� ������ ����� ������ ��� ������ �ȴ�.
	D3DXVec3Cross(pOut, &right, &normal);

	return true;
}

HRESULT terrain::createTerrain(int smooth, int tileNum)
{
	// ���� ��ġ ���Ѵ�.

	// Ÿ�ϸ� ������ ���� ���� (������ uv ����)
	float tileIntervalX = static_cast<float>(tileNum) / _cellNumX;
	float tileIntervalY = static_cast<float>(tileNum) / _cellNumZ;

	// ���� ���� ������ �־�� �Ѵ�.
	_terrainVertices = new TERRAINVERTEX[_totalVerNum];

	// �ؽ����� pixel ������ ��� ���� Texture �� lock �Ѵ�.
	D3DLOCKED_RECT lockRect;
	_heightMap->LockRect(
		0,					// lock �� �� �Ӹ� ���� 0
		&lockRect,			// lock ���� ���� D3DLOCKED_RECT ���� ����ü
		0,					// lock �� �Ϻ� ������ �ϰ� ���� ��� �簢��RECT ����ü�� �����͸� �ִµ� ��ü �Ҳ��� NULL
		0					// lock �÷��� �ϴ� 0
	);

	// lockRect->Pitch	lock �� �� ���� �̹����� ���� byte ũ�� (���� ����Ʈũ��� ������ �����Ѵ� pitch % 4 == 0 ) < 3byte �÷��� pitch byte ���ϴ� ���� ( ���� �ȼ��� * 3 + 3 ) & ~3 = pitch  >
	// lockRect->pBits	�̹��������Ͱ� ���۵Ǵ� ������ �ּ�

	// ������ġ�� ���� UV �� ����ϱ�
	for (int z = 0; z < _verNumZ; z++)
	{
		for (int x = 0; x < _verNumX; x++)
		{
			D3DXVECTOR3 pos;

			// ������ x, z ��ġ ���
			pos.x = (x - (_cellNumX * 0.5)) * _cellScale;
			pos.z = (-z + (_cellNumZ * 0.5)) * _cellScale;

			// ���θ����� �����̶�� (���� ������ ���� Y ��ġ�� �����)
			if (x == _verNumX - 1)
			{
				int idx = z * _verNumX + x - 1;
				pos.y = _terrainVertices[idx].pos.y;
			}

			// ���� ������ �����̶�� (���� ������ ���� Y ��ġ�� �����)
			else if (z == _verNumZ - 1)
			{
				int idx = (z - 1) * _verNumX + x;
				pos.y = _terrainVertices[idx].pos.y;
			}
			else
			{
				// �ش� �ȼ��� �÷� ���� ��´�.
				DWORD* pStart = (DWORD*)lockRect.pBits;	//(DWORD ������ ����ȯ�� lock �� �̹������� ���� �ּ�
				DWORD dwColor = *(pStart + (z * (lockRect.Pitch / 4) + x));

				// �� �÷� ���� 0 ~ 1 ������ �Ǽ��� ������.
				float inv = 1.0f / 255.0f;
				float r = ((dwColor & 0x00ff0000) >> 16) * inv;
				float g = ((dwColor & 0x0000ff00) >> 8) * inv;
				float b = ((dwColor & 0x000000ff)) * inv;

				// ���� �� ��
				float factor = (r + g + b) / 3.0f;

				// ���� ��
				pos.y = factor * _heightScale;
			}

			// ���� UV ���

			//Terrain Tile Splating UV
			D3DXVECTOR2 baseUV;
			baseUV.x = x / static_cast<float>(_verNumX - 1);
			baseUV.y = z / static_cast<float>(_verNumZ - 1);

			//Terrain Tile UV
			D3DXVECTOR2 tileUV;
			tileUV.x = x * tileIntervalX;
			tileUV.y = z * tileIntervalY;

			//���ؽ� �迭�ε��� ���
			int idx = z * _verNumX + x;

			_terrainVertices[idx].pos = pos;
			_terrainVertices[idx].normal = D3DXVECTOR3(0, 0, 0);	//�Ʒ����� ���� �븻 ���Ҷ� �������� �ϴ� 0 ���ͷ� �ʱ�ȭ
			_terrainVertices[idx].baseUV = baseUV;
			_terrainVertices[idx].tileUV = tileUV;
		}
	}

	// �ؽ����� pixel ���� Unlock
	_heightMap->UnlockRect(0);

	// ���� ������ 
	this->smoothTerrain(smooth);

	// ���� �ε����� ���Ѵ�
	LPTERRAININDEX pIndices = new tagTerrainIndex[_totalTriangle];

	// �ε��� �迭 �ε���
	int idx = 0;

	for (DWORD z = 0; z < _cellNumZ; z++)
	{
		for (DWORD x = 0; x < _cellNumX; x++)
		{
			// lt-----rt
			//  |    /|
			//  |   / |
			//  |  /  |
			//  | /   |
			//  |/    |
			// lb-----rb

			// �ش� ���� ���� ���� �ε����� ����
			DWORD lt = z * _verNumX + x;
			DWORD rt = z * _verNumX + x + 1;
			DWORD lb = ((z + 1)* _verNumX) + x;
			DWORD rb = ((z + 1)* _verNumX) + x + 1;

			// ���� �ﰢ�� �ϳ�
			pIndices[idx]._0 = lt;
			pIndices[idx]._1 = rt;
			pIndices[idx]._2 = lb;
			idx++;

			// ���� �ﰢ�� �ϳ�
			pIndices[idx]._0 = lb;
			pIndices[idx]._1 = rt;
			pIndices[idx]._2 = rb;
			idx++;
		}
	}

	// �븻�̶� Binormal �̶� Tangent ����ϱ�
	D3DXVECTOR3* poses = new D3DXVECTOR3[_totalVerNum];		//������ġ
	D3DXVECTOR3* normals = new D3DXVECTOR3[_totalVerNum];
	D3DXVECTOR3* tangents = new D3DXVECTOR3[_totalVerNum];
	D3DXVECTOR3* binormals = new D3DXVECTOR3[_totalVerNum];
	D3DXVECTOR2* uvs = new D3DXVECTOR2[_totalVerNum];
	DWORD* indices = (DWORD*)pIndices;

	// ������ġ �� UV ����
	for (int i = 0; i < _totalVerNum; i++)
	{
		poses[i] = _terrainVertices[i].pos;
		uvs[i] = _terrainVertices[i].baseUV;
	}

	// �븻���
	ComputeNormal(normals, poses, _totalVerNum, indices, _totalTriangle * 3);

	// ź��Ʈ ���̳븻 ���
	ComputeTangentBinormal(
		tangents,
		binormals,
		poses,
		normals,
		uvs,
		indices,
		_totalTriangle,
		_totalVerNum);

	// ���Ȱ� ����
	for (int i = 0; i < _totalVerNum; i++)
	{
		_terrainVertices[i].normal = normals[i];
		_terrainVertices[i].binormal = binormals[i];
		_terrainVertices[i].tangent = tangents[i];
	}

	// ���� ����
	// �������� �����
	device->CreateVertexBuffer(
		sizeof(TERRAINVERTEX) * _totalVerNum,
		D3DUSAGE_WRITEONLY,
		0,
		D3DPOOL_MANAGED,
		&_terrainVb,
		0);

	// ������� ���� ���۸� Lock �Ͽ� ���� ���� ���� ����
	void* p = NULL;
	_terrainVb->Lock(0, 0, &p, 0);
	memcpy(p, _terrainVertices, sizeof(TERRAINVERTEX) * _totalVerNum);
	_terrainVb->Unlock();

	// �ε��� ���� �����
	device->CreateIndexBuffer(
		sizeof(TERRAININDEX) * _totalTriangle,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX32,
		D3DPOOL_DEFAULT,
		&_terrainIb,
		0);

	// �ε��� ���ۿ� ���� ����.
	_terrainIb->Lock(0, 0, &p, 0);
	memcpy(p, pIndices, sizeof(TERRAININDEX) * _totalTriangle);
	_terrainIb->Unlock();

	// ���� ����θ� �����.
	// ������ ���¸� �˷��ִ� �迭
	D3DVERTEXELEMENT9 vertElement[7];	// �迭�� �������� ���� + 1

	// Position 
	vertElement[0].Stream = 0;							// Stream �� 0
	vertElement[0].Offset = 0;							// �޸��� ���� Byte ���� 0
	vertElement[0].Type = D3DDECLTYPE_FLOAT3;			// �ڷ��� Ÿ��
	vertElement[0].Method = D3DDECLMETHOD_DEFAULT;		// �ϴ� ������  D3DDECLMETHOD_DEFAULT
	vertElement[0].Usage = D3DDECLUSAGE_POSITION;		// ���� Ÿ��
	vertElement[0].UsageIndex = 0;						// UsageIndex �ϴ� 0
														   
														// Normal
	vertElement[1].Stream = 0;							// Stream �� 0
	vertElement[1].Offset = 12;							// �޸��� ���� Byte ���� 0
	vertElement[1].Type = D3DDECLTYPE_FLOAT3;			// �ڷ��� Ÿ��
	vertElement[1].Method = D3DDECLMETHOD_DEFAULT;		// �ϴ� ������  D3DDECLMETHOD_DEFAULT
	vertElement[1].Usage = D3DDECLUSAGE_NORMAL;			// ���� Ÿ��
	vertElement[1].UsageIndex = 0;						// UsageIndex �ϴ� 0
														   
														// BiNormal
	vertElement[2].Stream = 0;							// Stream �� 0
	vertElement[2].Offset = 24;							// �޸��� ���� Byte ���� 0
	vertElement[2].Type = D3DDECLTYPE_FLOAT3;			// �ڷ��� Ÿ��
	vertElement[2].Method = D3DDECLMETHOD_DEFAULT;		// �ϴ� ������  D3DDECLMETHOD_DEFAULT
	vertElement[2].Usage = D3DDECLUSAGE_BINORMAL;		// ���� Ÿ��
	vertElement[2].UsageIndex = 0;						// UsageIndex �ϴ� 0

														// Tangent
	vertElement[3].Stream = 0;							// Stream �� 0
	vertElement[3].Offset = 36;							// �޸��� ���� Byte ���� 0
	vertElement[3].Type = D3DDECLTYPE_FLOAT3;			// �ڷ��� Ÿ��
	vertElement[3].Method = D3DDECLMETHOD_DEFAULT;		// �ϴ� ������  D3DDECLMETHOD_DEFAULT
	vertElement[3].Usage = D3DDECLUSAGE_TANGENT;		// ���� Ÿ��
	vertElement[3].UsageIndex = 0;						// UsageIndex �ϴ� 0

														// BaseUV
	vertElement[4].Stream = 0;							// Stream �� 0
	vertElement[4].Offset = 48;							// �޸��� ���� Byte ���� 0
	vertElement[4].Type = D3DDECLTYPE_FLOAT2;			// �ڷ��� Ÿ��
	vertElement[4].Method = D3DDECLMETHOD_DEFAULT;		// �ϴ� ������  D3DDECLMETHOD_DEFAULT
	vertElement[4].Usage = D3DDECLUSAGE_TEXCOORD;		// ���� Ÿ��
	vertElement[4].UsageIndex = 0;						// UsageIndex �ϴ� 0

														// tileUV
	vertElement[5].Stream = 0;							// Stream �� 0
	vertElement[5].Offset = 56;							// �޸��� ���� Byte ���� 0
	vertElement[5].Type = D3DDECLTYPE_FLOAT2;			// �ڷ��� Ÿ��
	vertElement[5].Method = D3DDECLMETHOD_DEFAULT;		// �ϴ� ������  D3DDECLMETHOD_DEFAULT
	vertElement[5].Usage = D3DDECLUSAGE_TEXCOORD;		// ���� Ÿ��
	vertElement[5].UsageIndex = 1;						// UsageIndex �ι�° UV 1 

														// ���̻� ������ ����
	D3DVERTEXELEMENT9 end = D3DDECL_END(); // {0xFF,0,D3DDECLTYPE_UNUSED,0,0,0}
	vertElement[6] = end;

	// LPDIRECT3DVERTEXDECLARATION9 ����
	device->CreateVertexDeclaration(
		vertElement,			// �տ��� ���� D3DVERTEXELEMENT9 �迭 ������
		&_terrainDecl			// ���� LPDIRECT3DVERTEXDECLARATION9 ������
	);

	SAFE_DELETE_ARRAY(poses);
	SAFE_DELETE_ARRAY(normals);
	SAFE_DELETE_ARRAY(tangents);
	SAFE_DELETE_ARRAY(binormals);
	SAFE_DELETE_ARRAY(uvs);
	SAFE_DELETE_ARRAY(indices);

	return S_OK;
}

void terrain::smoothTerrain(int passed)
{
	if (passed <= 0) return;

	float* smooth = new float[_totalVerNum];

	while (passed > 0)
	{
		passed--;
		for (int z = 0; z < _verNumZ; z++)
		{
			for (int x = 0; x < _verNumX; x++)
			{
				int adjacentSections = 0;		// ��� ������ ��հ��� ����?
				float totalSections = 0.0f;		// �ֺ��� ���� ���� ������ �󸶴�?

				// ���� üũ
				if ((x - 1) > 0)
				{
					totalSections += _terrainVertices[(z * _verNumX) + (x - 1)].pos.y;
					adjacentSections++;
					// ���� ���
					if ((z - 1) > 0)
					{
						totalSections += _terrainVertices[((z - 1) * _verNumX) + (x - 1)].pos.y;
						adjacentSections++;
					}
					// ���� �ϴ�
					if ((z + 1) < _verNumZ)
					{
						totalSections += _terrainVertices[((z + 1) * _verNumX) + (x - 1)].pos.y;
						adjacentSections++;
					}
				}
				// ������ üũ
				if ((x + 1) < _verNumX)
				{
					totalSections += _terrainVertices[(z * _verNumX) + (x + 1)].pos.y;
					adjacentSections++;

					// ������ ���
					if ((z - 1) > 0)
					{
						totalSections += _terrainVertices[((z - 1) * _verNumX) + (x + 1)].pos.y;
						adjacentSections++;
					}
					// ������ �ϴ� 
					if ((z + 1) < _verNumZ)
					{
						totalSections += _terrainVertices[((z + 1) * _verNumX) + (x + 1)].pos.y;
						adjacentSections++;
					}
				}
				// ���
				if ((z - 1) > 0)
				{
					totalSections += _terrainVertices[((z - 1) * _verNumX) + x].pos.y;
					adjacentSections++;
				}
				// �ϴ�
				if ((z + 1) < _verNumZ)
				{
					totalSections += _terrainVertices[((z + 1) * _verNumX) + x].pos.y;
					adjacentSections++;
				}
				smooth[(z * _verNumX) + x] = (
					_terrainVertices[(z * _verNumX) + x].pos.y +
					(totalSections / adjacentSections)) * 0.5f;
			}
		}

		// ������ ���� y ������ ����
		for (int i = 0; i < _totalVerNum; i++)
		{
			_terrainVertices[i].pos.y = smooth[i];
		}
	}

	SAFE_DELETE_ARRAY(smooth);
}
