#include "stdafx.h"
#include "xMeshSkinned.h"

char szTemp[128];
LPD3DXEFFECT xMeshSkinned::_sSkinnedMeshEffect = NULL;

xMeshSkinned::xMeshSkinned()
{
	_pNowPlayAnimationSet = NULL;
	_dwWorkingPaletteSize = 0;
	_pmWorkingPalette = NULL;
	_pSkinnedEffect = NULL;
	//Skinned Effect �ε�
	if (_sSkinnedMeshEffect == NULL)
	{
		_sSkinnedMeshEffect = RM_SHADER->addResource("Resources/Shader/SkinnedMesh.fx");
	}
}

HRESULT xMeshSkinned::init(string filePath, D3DXMATRIXA16 * pmatCorrection)
{
	//��������� ���Դٸ�...
	if (pmatCorrection) _matCorrection = *pmatCorrection;
	else D3DXMatrixIdentity(&_matCorrection);

	_pSkinnedEffect = _sSkinnedMeshEffect;

	//��Ų��޽��� ���� ���� ������ �ε������ �����ǵ� BONE_HIERARCHY Ŭ���� ��ü
	BONE_HIERARCHY boneHierachy;
	boneHierachy.SetLoadPath(filePath);
	boneHierachy.SetSkinnedMesh(this);

	//���������� �Ǿ��ִ� XFile �޽� �ε�....
	if (FAILED(D3DXLoadMeshHierarchyFromX(
		filePath.c_str(),
		D3DXMESH_MANAGED,
		device,
		&boneHierachy,					//���� ������ �ε� ����� �����ǵ� ��ü�� �ѱ��.
		NULL,
		&_pRootbone,					//���� ��Ʈ ��������
		&_pAnimController				//���� �ִϸ��̼� ��Ʈ�ѷ�
	))) return E_FAIL;

	//�� ��Ʈ���� ������ ����
	InitBoneMatrixPointer((BONE*)_pRootbone);

	//Animation ������ ��´�.
	_AnimNum = _pAnimController->GetNumAnimationSets();

	for (UINT i = 0; i < _AnimNum; i++)
	{
		//���ϸ��̼� �� ����
		LPD3DXANIMATIONSET animSet;

		//i ��° Animation �� ������ ��´�.
		_pAnimController->GetAnimationSet(i, &animSet);

		//AnimationSet Vector �� ����
		this->_vecAnimSet.push_back(animSet);

		//AnimationSet Map �� ����
		this->_mapAnimSet.insert(std::make_pair(animSet->GetName(), animSet));
	}

	//�ϴ� ù��° Animation ���� ����
	this->Play(0);

	return S_OK;
}

void xMeshSkinned::release()
{
	//�ҷ��� AnimationSet ����
	for (int i = 0; i < this->_vecAnimSet.size(); i++)
	{
		SAFE_RELEASE(_vecAnimSet[i]);
	}

	//���ϸ��̼� ��Ʈ�ѷ� ����
	SAFE_RELEASE(_pAnimController);

	//��Ų��޽��� ���� ���� ������ �ε������ �����ǵ� BONE_HIERARCHY Ŭ���� ��ü
	BONE_HIERARCHY boneHierachy;	//<- ���� ���� ����� �˰� �ִ�.
	D3DXFrameDestroy(_pRootbone, &boneHierachy);
}

void xMeshSkinned::update()
{
	//0 �� Track ������ ��´�
	_pAnimController->GetTrackDesc(0, &_Track_Desc_0);

	//_pNowPlayAnimationSet->GetPeriod() Animation �� �ѽð��� ��´�.
	//���� Animation ���� Factor �� ����
	_AnimationPlayFactor = _Track_Desc_0.Position / _pNowPlayAnimationSet->GetPeriod();

	//�������� �����ߴٸ�...
	if (_AnimationPlayFactor >= 1.0)
	{
		if (this->_bLoop == false)
		{
			//���ư� Animation �� �ִٸ�..
			if (this->_pPrevPlayAnimationSet != NULL)
			{
				_fCrossFadeTime = _fOutCrossFadeTime;
				_fLeftCrossFadeTime = _fOutCrossFadeTime;
				_bLoop = true;
				this->SetAnimation(_pPrevPlayAnimationSet);
				this->_pPrevPlayAnimationSet = NULL;
			}
			//���ư� Animation �� ���ٸ�.
			else
			{
				this->Stop();
			}
		}
	}
	_AnimationPlayFactor = _AnimationPlayFactor - (int)_AnimationPlayFactor;	//�����κ� �Ȼ����....

	if (_bPlay)
	{
		//�ִϸ��̼��� �����Ŵ
		_pAnimController->AdvanceTime(_deltaTime, NULL);
	}

	//ũ�ν� ���̵尡 �������̶��..
	if (_fLeftCrossFadeTime > 0.0f)
	{
		//���� ũ�ν����̵� �ð� ����
		this->_fLeftCrossFadeTime -= _deltaTime;

		//ũ�ν����̵� ��������.
		if (_fLeftCrossFadeTime <= 0.0f)
		{
			_pAnimController->SetTrackWeight(0, 1);
			_pAnimController->SetTrackEnable(1, false);
		}
		else
		{
			float w1 = (_fLeftCrossFadeTime / _fCrossFadeTime);		//1�� Track ����ġ
			float w0 = 1.0f - w1;									//0�� Track ����ġ

																	//����ġ ����
			_pAnimController->SetTrackWeight(0, w0);
			_pAnimController->SetTrackWeight(1, w1);
		}
	}
}

void xMeshSkinned::render(transform * pTrans)
{
	D3DXMATRIXA16 matFinal;
	//Transform �� ���Դٸ�...
	if (pTrans != NULL)
	{
		D3DXMATRIXA16 matWorld = pTrans->getFinalMatrix();
		matFinal = _matCorrection * matWorld;
	}
	else
	{
		matFinal = _matCorrection;
	}

	_pSkinnedEffect->SetTechnique("SkinnedMesh");

	//SkinnedEffect WorldMatrix ����
	_pSkinnedEffect->SetMatrix("matWorld", &matFinal);

	//��� ������Ʈ 
	UpdateMatrices((BONE*)_pRootbone, &matFinal);

	//��Ʈ ������ Ÿ�� ����.
	RenderBone((BONE*)_pRootbone);
}

//���� �����ش�.
void xMeshSkinned::renderBoneName(camera * pCam, transform * transform)
{
	D3DXMATRIXA16 matFinal;

	//Transform �� ���Դٸ�...
	if (transform != NULL)
	{
		D3DXMATRIXA16 matWorld = transform->getFinalMatrix();
		matFinal = _matCorrection * matWorld;
	}
	else
	{
		matFinal = _matCorrection;
	}

	//��� ������Ʈ 
	UpdateMatrices((BONE*)_pRootbone, &matFinal);

	//��Ʈ ������ Ÿ�� ����.
	RenderChildBoneName(pCam, (BONE*)_pRootbone, NULL);
}

void xMeshSkinned::Play(string animName, float crossFadeTime)
{
	_bPlay = true;
	_bLoop = true;

	MAP_ANIMSET::iterator find = this->_mapAnimSet.find(animName);
	if (find != this->_mapAnimSet.end())
	{
		//ũ�ν� ���̵� Ÿ�� ���
		_fCrossFadeTime = crossFadeTime;
		_fLeftCrossFadeTime = crossFadeTime;
		this->SetAnimation(find->second);
	}
}

void xMeshSkinned::Play(int animIndex, float crossFadeTime)
{
	_bPlay = true;
	_bLoop = true;

	if (animIndex < this->_AnimNum)
	{
		//ũ�ν� ���̵� Ÿ�� ���
		_fCrossFadeTime = crossFadeTime;
		_fLeftCrossFadeTime = crossFadeTime;
		this->SetAnimation(_vecAnimSet[animIndex]);
	}
}

void xMeshSkinned::PlayOneShot(string animName, float inCrossFadeTime, float outCrossFadeTime)
{
	_bPlay = true;
	_bLoop = false;

	MAP_ANIMSET::iterator find = this->_mapAnimSet.find(animName);
	if (find != this->_mapAnimSet.end())
	{
		//���� Animaiton �� ����Ѵ�.
		this->_pPrevPlayAnimationSet = this->_pNowPlayAnimationSet;

		//ũ�ν� ���̵� Ÿ�� ���
		_fCrossFadeTime = inCrossFadeTime;
		_fLeftCrossFadeTime = inCrossFadeTime;

		//������ ũ�ν����̵� Ÿ�� ���
		_fOutCrossFadeTime = outCrossFadeTime;

		this->SetAnimation(find->second);
	}
}

void xMeshSkinned::PlayOneShotAfterHold(std::string animName, float crossFadeTime)
{
	_bPlay = true;
	_bLoop = false;

	MAP_ANIMSET::iterator find = this->_mapAnimSet.find(animName);
	if (find != this->_mapAnimSet.end())
	{
		//���ư� Animation �� ����
		this->_pPrevPlayAnimationSet = NULL;

		//ũ�ν� ���̵� Ÿ�� ���
		_fCrossFadeTime = crossFadeTime;
		_fLeftCrossFadeTime = crossFadeTime;

		this->SetAnimation(find->second);
	}
}

void xMeshSkinned::SetPlaySpeed(float speed)
{
	_pAnimController->SetTrackSpeed(0, speed);
}

//Animation ������ ����Ѵ�.
void xMeshSkinned::ShowAnimationName(int x, int y)
{
	int i = 0;
	for (i = 0; i < _AnimNum; i++)
	{
		FONTMANAGER->fontOut(_vecAnimSet[i]->GetName(), x, y + i * 20, 0xffffffff);
	}
}

//���̸����� ��ã�´�.
BONE * xMeshSkinned::GetFineBONE(std::string boneName)
{
	return (BONE*)D3DXFrameFind(_pRootbone, boneName.c_str());
}

//�� ����� �����Ѵ�.
void xMeshSkinned::InitBoneMatrixPointer(BONE * pBone)
{
	if (pBone == NULL) return;

	//�ش� �������ӿ� �޽� �����̳� �� �����Ѵٸ�...
	if (pBone->pMeshContainer)
	{
		//�޽� �����̳ʸ� BONE_MESH ������ ����ȯ
		BONE_MESH* pBoneMesh = (BONE_MESH*)pBone->pMeshContainer;

		//�޽��� ��Ű�� ������ �ִٸ�
		if (pBoneMesh->pSkinInfo)
		{
			//�ش� ��Ų�� ����Ǵ� ���� ������ŭ
			int numBones = pBoneMesh->pSkinInfo->GetNumBones();

			//Matrix ������ �Ҵ�
			pBoneMesh->ppBoneMatrixPtrs = new D3DXMATRIXA16*[numBones];

			//����Ǵ� �� �������� ��Ʈ������ ã�� �ش� ���������� ��� �����͸� ������.
			for (int i = 0; i < numBones; i++)
			{
				//�ش� ��Ų�� ����Ǵ� i ��°�� ���̸����� �ش� �� �������� ���� ������ ã�� ����
				BONE* pB = (BONE*)D3DXFrameFind(_pRootbone, pBoneMesh->pSkinInfo->GetBoneName(i));

				//ã�Ҵٸ�
				if (pB != NULL)
				{
					pBoneMesh->ppBoneMatrixPtrs[i] = &pB->CombinedTransformationMatrix;
				}
				else
				{
					pBoneMesh->ppBoneMatrixPtrs[i] = NULL;
				}
			}
		}
	}

	//�̿� ���� �����Ѵٸ�...
	if (pBone->pFrameSibling)
		InitBoneMatrixPointer((BONE*)pBone->pFrameSibling);

	//�ڽ� ���� �����Ѵٸ�...
	if (pBone->pFrameFirstChild)
		InitBoneMatrixPointer((BONE*)pBone->pFrameFirstChild);
}

void xMeshSkinned::UpdateMatrices(BONE * pBone, D3DXMATRIXA16 * pParentMatrix)
{
	if (pBone == NULL) return;

	//�θ� ����� �ִٸ�..
	if (pParentMatrix != NULL)
	{
		D3DXMATRIXA16 matTransformation = pBone->TransformationMatrix;

		//���� Transform �� ���� �ִٸ�...
		if (pBone->pApplyTransform != NULL)
		{
			D3DXMATRIXA16 pApplyMatrix = pBone->pApplyTransform->getWorldRotateMatrix();
			matTransformation = matTransformation * pApplyMatrix;
			pBone->pApplyTransform = NULL;
		}

		//pBone->CombinedTransformationMatrix = pBone->_transformationMatrix * *(pParentMatrix);
		//pBone �� ��� �θ������ ���Ͽ� pBone �� ���� ����� ���Ѵ�.
		D3DXMatrixMultiply(
			&pBone->CombinedTransformationMatrix,
			&matTransformation,
			pParentMatrix);
	}

	//�θ� ����� ���ٸ�..
	else
	{
		//�ڽ��� ����� ���� ����� �ȴ�.
		pBone->CombinedTransformationMatrix = pBone->TransformationMatrix;
	}

	//�� Transform �� ���� �ִٸ�....
	if (pBone->pBoneTransform != NULL)
	{
		pBone->pBoneTransform->setWorldMatrix(pBone->CombinedTransformationMatrix);

		pBone->pBoneTransform->setScale(1, 1, 1);
		pBone->pBoneTransform = NULL;
	}

	//�̿� ���� �����Ѵٸ�...
	if (pBone->pFrameSibling)
	{
		//�̿��� ���������̱� ������ �θ� ���� ���� �Ű����� ���� �θ��� ����� �ѱ�� �ȴ�.
		UpdateMatrices((BONE*)pBone->pFrameSibling, pParentMatrix);
	}
	//�ڽ� ���� �����Ѵٸ�...
	if (pBone->pFrameFirstChild)
	{
		UpdateMatrices((BONE*)pBone->pFrameFirstChild, &pBone->CombinedTransformationMatrix);
	}
}

//���� �׸���.
void xMeshSkinned::RenderBone(BONE * pBone)
{
	if (pBone == NULL) return;

	//���� �޽� �����̳ʰ� �����Ѵٸ� �׷����Ѵ�.
	if (pBone->pMeshContainer)
	{
		//���� �ִ� �޽� �����̳ʴ� BONE_MESH �̴�
		BONE_MESH* pBoneMesh = (BONE_MESH*)pBone->pMeshContainer;

		//�� �ĺ���̼� ������  
		if (pBoneMesh->BufBoneCombos != NULL)
		{
			//�ش� ���� �ĺ���̼� ������ ��´�.
			LPD3DXBONECOMBINATION pBoneComb =
				(LPD3DXBONECOMBINATION)(pBoneMesh->BufBoneCombos->GetBufferPointer());

			//���� �����ִ� �޽��� ����°����� �Ӽ��׷���� ����
			for (DWORD i = 0; i < pBoneMesh->NumAttributesGroup; i++)
			{
				//�ش� �Ӽ��� �ȷ�Ʈ ��Ʈ�� ����ŭ ���鼭 �۾��� �ȷ�Ʈ ��� ����
				for (DWORD palEntry = 0; palEntry < pBoneMesh->NumPaletteEntries; palEntry++)
				{
					//����Ǵ� ��� ID �� ��´�
					DWORD dwMatrixIndex = pBoneComb[i].BoneId[palEntry];

					//��� �ε����� ��ȿ�ϴٸ�...
					if (dwMatrixIndex != UINT_MAX)
					{
						//�۾� �޷��� �����.
						D3DXMatrixMultiply(&_pmWorkingPalette[palEntry],
							&(pBoneMesh->pBoneOffsetMatices[dwMatrixIndex]),
							pBoneMesh->ppBoneMatrixPtrs[dwMatrixIndex]);
					}
				}

				//������ ���É� �۾������ Effect �ȷ����� �����Ѵ�.
				_pSkinnedEffect->SetMatrixArray("amPalette", _pmWorkingPalette, pBoneMesh->NumPaletteEntries);

				//����Ǵ� ������ ���ִ� ����� �� ���� �ִ� �����  -1 
				_pSkinnedEffect->SetInt("CurNumBones", pBoneMesh->MaxNumFaceInfls - 1);

				//���͸��� �ε���
				DWORD materialIndex = pBoneComb[i].AttribId;

				//�ؽ��� ����
				_pSkinnedEffect->SetTexture("Diffuse_Tex", pBoneMesh->DiffuseTexs[materialIndex]);
				_pSkinnedEffect->SetTexture("Specular_Tex", pBoneMesh->SpecularTexs[materialIndex]);
				_pSkinnedEffect->SetTexture("Normal_Tex", pBoneMesh->NormalTexs[materialIndex]);
				_pSkinnedEffect->SetTexture("Emission_Tex", pBoneMesh->EmissionTexs[materialIndex]);
				_pSkinnedEffect->SetFloat("fSpecPower", pBoneMesh->Materials[materialIndex].Power);

				UINT uiPasses, uiPass;
				_pSkinnedEffect->Begin(&uiPasses, 0);
				for (uiPass = 0; uiPass < uiPasses; ++uiPass)
				{
					_pSkinnedEffect->BeginPass(uiPass);
					pBoneMesh->WorkingMesh->DrawSubset(i);
					_pSkinnedEffect->EndPass();
				}
				_pSkinnedEffect->End();
			}
		}
	}

	//�̿� ���� �����Ѵٸ�...
	if (pBone->pFrameSibling) RenderBone((BONE*)pBone->pFrameSibling);

	//�ڽ� ���� �����Ѵٸ�...
	if (pBone->pFrameFirstChild) RenderBone((BONE*)pBone->pFrameFirstChild);
}

void xMeshSkinned::RenderChildBoneName(camera * pCam, BONE * pBone, D3DXVECTOR3 * parentPos)
{
	if (pBone == NULL) return;

	//���� Transform  �׷�
	if (pBone->pBoneTransform != NULL)
	{
		pBone->pBoneTransform->renderGizmo();
	}
	//����ġ
	D3DXVECTOR3 bonePos = D3DXVECTOR3(
		pBone->CombinedTransformationMatrix._41,
		pBone->CombinedTransformationMatrix._42,
		pBone->CombinedTransformationMatrix._43);

	sprintf(szTemp, "%s", pBone->Name);

	DWORD color = 0xff808000;
	DWORD textColor = 0xffffff00;


	//��Ʈ���̶��.
	if (pBone == _pRootbone)
	{
		color = 0xff808080;
		textColor = 0xffffffff;
		//����ġ�� Gizmo
		GIZMOMANAGER->sphereMake(bonePos, 0.2f, color);
	}

	//����ġ�� Gizmo
	GIZMOMANAGER->sphereMake(bonePos, 0.1f, color);

	//�θ���ġ���� ����ġ�� ��
	if (parentPos != NULL) GIZMOMANAGER->lineMake(bonePos, *parentPos, color);

	//����ũ�� ��ġ
	D3DXVECTOR2 boneScreenPos;
	if (pCam->getWorldPosToScreenPos(&boneScreenPos, &bonePos))
	{
		FONTMANAGER->fontOut(szTemp, (int)boneScreenPos.x, (int)boneScreenPos.y, textColor);
	}

	//�̿� ���� �����Ѵٸ�...
	if (pBone->pFrameSibling) RenderChildBoneName(pCam, (BONE*)pBone->pFrameSibling, parentPos);

	//�ڽ� ���� �����Ѵٸ�...
	if (pBone->pFrameFirstChild) RenderChildBoneName(pCam, (BONE*)pBone->pFrameFirstChild, &bonePos);
}

//Animation �� �ٲ۴�.
void xMeshSkinned::SetAnimation(LPD3DXANIMATIONSET animSet)
{
	//�̹� �÷��̵ǰ� �ִ� Animation �̶�� ������ ���� �ʴ´�.
	if (this->_pNowPlayAnimationSet != NULL &&
		animSet == this->_pNowPlayAnimationSet)
		return;

	//ũ�ν� ���̵尡 �����Ѵٸ�..
	if (this->_fCrossFadeTime > 0.0f)
	{
		//���� Animation �� 1 ��Track ���� ����
		_pAnimController->SetTrackAnimationSet(1, _pNowPlayAnimationSet);
		_pAnimController->SetTrackPosition(1, _Track_Desc_0.Position);	//������ �÷��� �Ǵ� ��ġ�� ����
		_pAnimController->SetTrackEnable(1, true);						//1 �� Track Ȱ��ȭ
		_pAnimController->SetTrackWeight(1, 1.0f);						//1 �� Track ����ġ
		_pAnimController->SetTrackSpeed(1, _Track_Desc_0.Speed);		//�ӵ� 

																		//���ݼ��õǴ� Animation �� 0 ��Track ���� ����
		_pNowPlayAnimationSet = animSet;
		_pAnimController->SetTrackAnimationSet(0, animSet);
		_pAnimController->SetTrackPosition(0, 0.0f);
		_pAnimController->SetTrackWeight(0, 0.0f);						//����ġ�� 0 ���� 

																		//���� �÷��� �ǰ� �ִ� AnimationSet ����
		this->_pNowPlayAnimationSet = animSet;
	}
	//ũ�ν� ���̵尡 ���ٸ�...
	else
	{
		//Track �� �������� �Ǿ����� ������.
		this->_pAnimController->SetTrackPosition(
			0,			//Track ��ȣ 
			0.0			//PlayPosition
		);

		//�Ű������� ���� AnimationSet ���� Animation 0 �� Track�� �÷���
		this->_pAnimController->SetTrackAnimationSet(0, animSet);

		//���� �÷��� �ǰ� �ִ� AnimationSet ����
		this->_pNowPlayAnimationSet = animSet;
	}
}

void xMeshSkinned::setCamera(camera * camera)
{
	//Skinned Effect �ε�
	if (_sSkinnedMeshEffect == NULL)
	{
		_sSkinnedMeshEffect = RM_SHADER->addResource("Resources/Shader/SkinnedMesh.fx");
	}

	D3DXMATRIXA16 matViewProj = camera->getViewProjectionMatrix();
	_sSkinnedMeshEffect->SetMatrix("matViewProjection", &matViewProj);
	D3DXVECTOR3 vEyePos = camera->getWorldPosition();
	_sSkinnedMeshEffect->SetVector("vEyePos", &D3DXVECTOR4(vEyePos, 1));
}

void xMeshSkinned::setBaseLight(lightDirection * pDirLight)
{
	_sSkinnedMeshEffect->SetMatrix("baseDirectionLight", &pDirLight->getLightMatrix());
}

void xMeshSkinned::setLighting(light ** pLights, int lightNum)
{
	//����Ʈ ��� ����
	D3DXMATRIXA16 matLight[10];
	for (int i = 0; i < lightNum; i++)
	{
		matLight[i] = pLights[i]->getLightMatrix();
	}
	//Effect �� ������ ����
	_sSkinnedMeshEffect->SetMatrixArray("matLights", matLight, 10);
	_sSkinnedMeshEffect->SetInt("LightNum", lightNum);
}

void xMeshSkinned::setLighting(vector<light*>* pLights)
{
	//����Ʈ ��� ����
	D3DXMATRIXA16 matLight[10];
	for (int i = 0; i < pLights->size(); i++)
	{
		matLight[i] = (*(pLights))[i]->getLightMatrix();
	}
	//Effect �� ������ ����
	_sSkinnedMeshEffect->SetMatrixArray("matLights", matLight, 10);
	_sSkinnedMeshEffect->SetInt("LightNum", pLights->size());
}

void xMeshSkinned::setTechniqueName(string name)
{
	_sSkinnedMeshEffect->SetTechnique(name.c_str());
}
