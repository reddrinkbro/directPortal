#include "stdafx.h"
#include "xMeshStatic.h"

LPD3DXEFFECT		xMeshStatic::_xMeshEffect = NULL;
LPDIRECT3DTEXTURE9	xMeshStatic::_defDiffuseTex = NULL;
LPDIRECT3DTEXTURE9	xMeshStatic::_defNormalTex = NULL;
LPDIRECT3DTEXTURE9	xMeshStatic::_defSpecularTex = NULL;
LPDIRECT3DTEXTURE9	xMeshStatic::_defEmssionTex = NULL;

HRESULT xMeshStatic::init(string filePath, D3DXMATRIXA16 * matCorrection)
{
	//  ���̴��� ����Ʈ �ؽ��� �ʱ�ȭ
	if (_xMeshEffect == NULL)
	{
		_xMeshEffect = RM_SHADER->addResource("Resources/Shader/StaticMesh.fx");
	}
	if (_defDiffuseTex == NULL)
	{
		_defDiffuseTex = RM_TEXTURE->addResource("Resources/Mesh/diffuseDefault.png");
	}
	if (_defNormalTex == NULL)
	{
		_defNormalTex = RM_TEXTURE->addResource("Resources/Mesh/normalDefault.png");
	}
	if (_defSpecularTex == NULL)
	{
		_defSpecularTex = RM_TEXTURE->addResource("Resources/Mesh/specularDefault.png");
	}
	if (_defEmssionTex == NULL)
	{
		_defEmssionTex = RM_TEXTURE->addResource("Resources/Mesh/emissionDefault.png");
	}


	LPD3DXBUFFER		adjacency = NULL;		//  ���� ����
	LPD3DXBUFFER		material = NULL;		//  ���� ����

												//  xfile�� ���� �޽��� �ҷ���.
	if (FAILED(D3DXLoadMeshFromX(
		filePath.c_str(),			//  X ���� ���
		D3DXMESH_MANAGED,			//  �ɼ� 4���� 32BIT : 32��Ʈ �ε��� ���, WRITEONLY : �޽� �����Ϳ� ���⸸ ���. DYNAMIC : �޽����۴� �������� �������.
		device,						//  �޽��� ����� ��ġ
		&adjacency,					//  ID3DBUFFER(D3DX ���Ǵ� ���� ������ ����ü)�� ����. 
		&material,					//  ������ ����. D3DXMATERIAL ����ü �迭�� ����.
		NULL,						//  EffectInstance. ���ڸ� ����.
		&_materialsNum,				//  ������ ��
		&_mesh						//  X ������ �������� ��ü�� ����.
	)))
	{
		SAFE_RELEASE(adjacency);
		SAFE_RELEASE(material);
		return E_FAIL;
	}

	//  �ε� ��ο��� ���ϸ��� �����ϰ� ��θ�
	string path;
	int lastPathIndex = 0;
	//  '\' ������ ã���� ��.
	lastPathIndex = filePath.find_last_of('/');

	//  ���� ��ã�Ҵٸ�
	if (lastPathIndex == -1)
	{
		lastPathIndex = filePath.find_last_of('\\');
	}
	else
	{
		path = filePath.substr(0, lastPathIndex + 1);
	}

	//  �ؽ��� �ε��� �ʿ��� ���ڿ�
	string texFilePath;			//  ���� ���
	string texExp;				//  ���� Ȯ���� ��
	string texFile;				//  ���ϸ�

								//  ���� ���� ��������
	LPD3DXMATERIAL pMaterials = (LPD3DXMATERIAL)material->GetBufferPointer();

	//  ���� ������ŭ for�� ����
	for (DWORD i = 0; i < _materialsNum; i++)
	{
		//  �ε�� ���׸���
		D3DMATERIAL9 mtrl = pMaterials[i].MatD3D;

		//  �޽��� ���׸��󿡴� Abbient ������ ����.
		//  Diffuse ������ �����ϰ� ����
		mtrl.Ambient = mtrl.Diffuse;

		//  ���׸��� ���� 
		_vMaterials.push_back(mtrl);

		//  ���׸����� texture ������ ������
		if (pMaterials[i].pTextureFilename != NULL)
		{
			//  ���ϰ�� = Mesh ���ϰ�� + �ؽ��� �����̸�
			texFilePath = path + pMaterials[i].pTextureFilename;

			//  �ؽ��ĸ� �ε��ϰ� Ǫ��
			_vDiffuseTex.push_back(RM_TEXTURE->addResource(texFilePath));

			//  ���� �̸��� Ȯ���� ��ġ
			int dotIndex = texFilePath.find_last_of(".");

			//  ���ϸ�� Ȯ����
			texFile = texFilePath.substr(0, dotIndex);
			texExp = texFilePath.substr(dotIndex + 1, filePath.length());

			//  _N �� �븻��, _S �� ����ŧ�� ��, _E �� �̹̼� ���� ����.

			// 
			//  �븻�� _N
			// 
			texFilePath = texFile + "_N." + texExp;
			LPDIRECT3DTEXTURE9 pNorTex = RM_TEXTURE->addResource(texFilePath);

			//  ���� ������ ���ٸ�
			if (pNorTex == NULL)
			{
				_vNormalTex.push_back(_defNormalTex);
			}
			else
			{
				_vNormalTex.push_back(pNorTex);
			}

			// 
			//  ����ŧ��� _S
			// 
			texFilePath = texFile + "_S." + texExp;
			LPDIRECT3DTEXTURE9 pSpecTex = RM_TEXTURE->addResource(texFilePath);

			//  ���� ������ ���ٸ�
			if (pSpecTex == NULL)
			{
				_vSpecularTex.push_back(_defSpecularTex);
			}
			else
			{
				_vSpecularTex.push_back(pSpecTex);
			}

			// 
			//  �̹̼Ǹ� _E
			// 
			texFilePath = texFile + "_E." + texExp;
			LPDIRECT3DTEXTURE9 pEmissionTex = RM_TEXTURE->addResource(texFilePath);

			//  ���� ������ ���ٸ�
			if (pEmissionTex == NULL)
			{
				_vEmissionTex.push_back(_defEmssionTex);
			}
			else
			{
				_vEmissionTex.push_back(pEmissionTex);
			}
		}
		else
		{
			_vDiffuseTex.push_back(_defDiffuseTex);
			_vNormalTex.push_back(_defNormalTex);
			_vSpecularTex.push_back(_defSpecularTex);
			_vEmissionTex.push_back(_defEmssionTex);
		}
	}

	//  pMaterial ���� ���۴� �ʿ䰡 ������ �ݵ�� ���� ������� ��.
	SAFE_RELEASE(material);


	//  �޽� ����ȭ
	_mesh->OptimizeInplace(
		D3DXMESHOPT_ATTRSORT |		//  �޽� ����� ������� ����(DrawSubset ȿ�� up)
		D3DXMESHOPT_COMPACT |		//  �޽����� ������ �ʴ� ������ �ε��� ����
		D3DXMESHOPT_VERTEXCACHE,	//  ���� ĳ������ ��Ʈ���� ������.
		(DWORD*)adjacency->GetBufferPointer(),		//  �������� �ѱ�.
		NULL,						//  ����ȭ�� ��ġ�� ����� ��� ��������
		NULL,						//  ����ȭ�� �ε��� ����
		NULL						//  ����ȭ�� ���� ���� ����
	);

	//  ����ȭ�� �Ϸ� �Ǹ� ���� ���� ����
	SAFE_RELEASE(adjacency);

	//  �޽� ���� ó��
	//  ���� ����� ������
	if (matCorrection)
	{
		meshCorrection(matCorrection);
	}
	else
	{
		//  ���� ����� ������ ����ó���� �ؾ� bound ������ ���� �� ����
		D3DXMATRIXA16 matIdentity;
		D3DXMatrixIdentity(&matIdentity);
		meshCorrection(&matIdentity);
	}


	return S_OK;
}

void xMeshStatic::release(void)
{
	SAFE_RELEASE(_mesh);
}

void xMeshStatic::render(transform * trans)
{
	//  ���� ��� ����
	D3DXMATRIXA16 matWorld = trans->getFinalMatrix();
	_xMeshEffect->SetMatrix("matWorld", &matWorld);

	//  �ϴ� ���� ���� ����
	D3DXVECTOR4 lightDir(1, -1, 1, 1);
	_xMeshEffect->SetVector("vLightDir", &lightDir);

	//  ����Ʈ �÷� �ϴ� ���
	D3DXVECTOR4 lightColor(1, 1, 1, 1);
	_xMeshEffect->SetVector("vLightColor", &lightColor);

	//  effect�� �׸��� ����
	_xMeshEffect->SetTechnique("Mesh");

	//  ���̴��� ť�귻��
	UINT iPassNum = 0;
	//  ����Ʈ�� ������ ����
	_xMeshEffect->Begin(&iPassNum, 0);

	//  �н� ���ڸ�ŭ 
	for (UINT i = 0; i < iPassNum; i++)
	{
		_xMeshEffect->BeginPass(i);
		//  ť�� ����
		for (int m = 0; m < _materialsNum; m++)
		{
			//  �ؽ��� ����
			_xMeshEffect->SetTexture("Diffuse_Tex", _vDiffuseTex[m]);
			_xMeshEffect->SetTexture("Normal_Tex", _vNormalTex[m]);
			_xMeshEffect->SetTexture("Specular_Tex", _vSpecularTex[m]);
			_xMeshEffect->SetTexture("Emission_Tex", _vEmissionTex[m]);
			//  ���� �Ŀ�
			_xMeshEffect->SetFloat("fSpecPower", _vMaterials[m].Power);
			//  
			_xMeshEffect->CommitChanges();
			//  �޽� �׸�
			_mesh->DrawSubset(m);
		}
		_xMeshEffect->EndPass();
	}
	_xMeshEffect->End();

}

void xMeshStatic::meshCorrection(D3DXMATRIXA16 * matCorrection)
{
	//  ���� Element�� ���ͼ� ���� ������ ����
	D3DVERTEXELEMENT9 pVerElement[MAX_FVF_DECL_SIZE];

	//  pVerElement �迭���� ������ ��� ������ �־���
	_mesh->GetDeclaration(pVerElement);

	//  ���� ������ offset
	int positionOffset = -1;

	//  ���� �븻 offset
	int normalOffset = -1;

	//  ���� ���� offset
	int tangentOffset = -1;

	//  ���� ���� offset
	int binormalOffset = -1;

	//  ���� ���� �ְ� ������ ��ŭ 
	for (DWORD i = 0; i < MAX_FVF_DECL_SIZE; i++)
	{
		//  �������̶��
		if (pVerElement[i].Type == D3DDECLTYPE_UNUSED)
		{
			break;
		}

		//  ���� ��ġ ������ ������
		if (pVerElement[i].Usage == D3DDECLUSAGE_POSITION)
		{
			positionOffset = pVerElement[i].Offset;
		}
		else if (pVerElement[i].Usage == D3DDECLUSAGE_NORMAL)
		{
			normalOffset = pVerElement[i].Offset;
		}
		else if (pVerElement[i].Usage == D3DDECLUSAGE_TANGENT)
		{
			tangentOffset = pVerElement[i].Offset;
		}
		else if (pVerElement[i].Usage == D3DDECLUSAGE_BINORMAL)
		{
			binormalOffset = pVerElement[i].Offset;
		}
	}

	// ���ؽ� ����
	DWORD verNum = _mesh->GetNumVertices();

	// �޽��� ���� FVF ����Ȥ�� Decl ������ �̿��Ͽ� �����ϳ��� ũ�⸦ �˾Ƴ���.
	// DWORD verSize = D3DXGetFVFVertexSize( pMesh->GetFVF() );
	DWORD verSize = D3DXGetDeclVertexSize(pVerElement, 0);

	// �޽��� ���ؽ� ���۸� Lock �Ѵ�
	void* p = NULL;
	_mesh->LockVertexBuffer(0, &p);

	// �ٿ�� MinMax ����� ���� �ʱ�ȭ
	_boundMin = D3DXVECTOR3(0, 0, 0);
	_boundMax = D3DXVECTOR3(0, 0, 0);

	// ���ؽ� ����Ŭ ���� �糤��
	for (DWORD i = 0; i < verNum; i++) {

		// ���ؽ� ���� �ּ�
		char* pVertex = ((char*)p + (i * verSize));

		// ���� ��ġ�� �ִٸ�
		if (positionOffset != -1)
		{
			D3DXVECTOR3* pos = (D3DXVECTOR3*)(pVertex + positionOffset);

			D3DXVec3TransformCoord(pos, pos, matCorrection);

			// ���� �ּ� ������
			if (_boundMin.x > pos->x)		_boundMin.x = pos->x;
			if (_boundMin.y > pos->y)		_boundMin.y = pos->y;
			if (_boundMin.z > pos->z)		_boundMin.z = pos->z;

			// ���� �ִ� ������
			if (_boundMax.x < pos->x)		_boundMax.x = pos->x;
			if (_boundMax.y < pos->y)		_boundMax.y = pos->y;
			if (_boundMax.z < pos->z)		_boundMax.z = pos->z;

			// ���� ��ġ Ǫ��
			_vertices.push_back(*pos);
		}

		// �븻������ �ִٸ�..
		if (normalOffset != -1)
		{
			D3DXVECTOR3* nor = (D3DXVECTOR3*)(pVertex + normalOffset);
			D3DXVec3TransformNormal(nor, nor, matCorrection);
			D3DXVec3Normalize(nor, nor);
			// ���� �븻 Ǫ��
			_normals.push_back(*nor);
		}

		// tangent ������ �ִٸ�.
		if (tangentOffset != -1)
		{
			D3DXVECTOR3* tangent = (D3DXVECTOR3*)(pVertex + tangentOffset);
			D3DXVec3TransformNormal(tangent, tangent, matCorrection);
			D3DXVec3Normalize(tangent, tangent);
		}

		// binormal ������ �ִٸ�
		if (binormalOffset != -1)
		{
			D3DXVECTOR3* binor = (D3DXVECTOR3*)(pVertex + binormalOffset);
			D3DXVec3TransformNormal(binor, binor, matCorrection);
			D3DXVec3Normalize(binor, binor);
		}
	}

	_mesh->UnlockVertexBuffer();

	// Bound �߰� ���
	_boundCenter = (_boundMin + _boundMax) * 0.5f;
	_boundSize = D3DXVECTOR3(
		_boundMax.x - _boundMin.x,
		_boundMax.y - _boundMin.y,
		_boundMax.z - _boundMin.z);
	_boundHalfSize = _boundSize * 0.5f;
	_boundRadius = D3DXVec3Length(&(_boundCenter - _boundMin));

	// 
	//  �ε��� ���۸� ��´�
	// 
	LPDIRECT3DINDEXBUFFER9 pIndexBuffer;
	_mesh->GetIndexBuffer(&pIndexBuffer);

	// ���� ����
	_triNum = _mesh->GetNumFaces();

	// �ε��� ���ۿ� ���� ������ ��´�.
	D3DINDEXBUFFER_DESC desc;
	pIndexBuffer->GetDesc(&desc);

	if (desc.Format == D3DFMT_INDEX16)
	{
		WORD* p = NULL;
		pIndexBuffer->Lock(0, 0, (void**)&p, 0);
		for (DWORD i = 0; i < _triNum; i++)
		{
			_indices.push_back(p[i * 3 + 0]);
			_indices.push_back(p[i * 3 + 1]);
			_indices.push_back(p[i * 3 + 2]);
		}
		pIndexBuffer->Unlock();
	}
	else if (desc.Format == D3DFMT_INDEX32)
	{
		DWORD* p = NULL;
		pIndexBuffer->Lock(0, 0, (void**)&p, 0);
		for (DWORD i = 0; i < _triNum; i++)
		{
			_indices.push_back(p[i * 3 + 0]);
			_indices.push_back(p[i * 3 + 1]);
			_indices.push_back(p[i * 3 + 2]);
		}
		pIndexBuffer->Unlock();
	}

	// ���� �ε��� ���۴� ����
	SAFE_RELEASE(pIndexBuffer);


}

void xMeshStatic::setCamera(camera * cam)
{
	D3DXMATRIXA16 matViewProj = cam->getViewProjectionMatrix();
	_xMeshEffect->SetMatrix("matViewProjection", &matViewProj);
	D3DXVECTOR3 vEyePos = cam->getWorldPosition();
	_xMeshEffect->SetVector("vEyePos", &D3DXVECTOR4(vEyePos, 1));
	_xMeshEffect->SetFloat("camFar", cam->getCamFar());
	_xMeshEffect->SetFloat("camNear", cam->getCamNear());
}

void xMeshStatic::setBaseLight(lightDirection * pDirLight)
{
	_xMeshEffect->SetMatrix("baseDirectionLight", &pDirLight->getLightMatrix());
}

void xMeshStatic::setLighting(vector<light*>* pLights)
{
	// ����Ʈ ��� ����
	D3DXMATRIXA16 matLights[10];

	for (int i = 0; i < pLights->size(); i++)
	{
		matLights[i] = (*(pLights))[i]->getLightMatrix();
	}

	// effect�� lighting ����
	_xMeshEffect->SetMatrixArray("matLights", matLights, 10);
	_xMeshEffect->SetInt("LightNum", pLights->size());
}

void xMeshStatic::setLighting(light ** pLights, int lightNum)
{
	// ����Ʈ ��� ����
	D3DXMATRIXA16 matLight[10];
	for (int i = 0; i < lightNum; i++)
	{
		matLight[i] = pLights[i]->getLightMatrix();
	}
	// Effect�� ������ ����
	_xMeshEffect->SetMatrixArray("matLights", matLight, 10);
	_xMeshEffect->SetInt("LightNum", lightNum);
}

void xMeshStatic::setTechniqueName(string name)
{
	_xMeshEffect->SetTechnique(name.c_str());
}
