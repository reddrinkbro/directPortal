#include "stdafx.h"
#include "skinnedAnimation.h"
#include "transform.h"

skinnedAnimation::skinnedAnimation()
{
	_mapApplyBoneTransform.clear();
	_mapBoneTransform.clear();
}
skinnedAnimation::~skinnedAnimation()
{
}

HRESULT skinnedAnimation::init(xMeshSkinned * pSkinnedMesh)
{
	if (pSkinnedMesh == NULL) return E_FAIL;

	_pSkinnedMesh = pSkinnedMesh;

	//SKinned Mesh �� Animation �� �����Ѵ�.
	_pSkinnedMesh->_pAnimController->CloneAnimationController(
		_pSkinnedMesh->_pAnimController->GetMaxNumAnimationOutputs(),
		_pSkinnedMesh->_pAnimController->GetMaxNumAnimationSets(),
		_pSkinnedMesh->_pAnimController->GetMaxNumTracks(),
		_pSkinnedMesh->_pAnimController->GetMaxNumEvents(),
		&_pAnimController);

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
		this->_mapAnimSet.insert(make_pair(animSet->GetName(), animSet));
	}

	//�ϴ� ù��° Animation ���� ����
	this->Play(0);
}

void skinnedAnimation::release()
{
	SAFE_RELEASE(_pAnimController);
}

void skinnedAnimation::update()
{
	//0 �� Track ������ ��´�
	_pAnimController->GetTrackDesc(0, &_Track_Desc_0);

	//_pNowPlayAnimationSet->GetPeriod() Animation �� �ѽð��� ��´�.
	//���� Animation ���� Factor �� ����
	_AnimationPlayFactor = _Track_Desc_0.Position / _pNowPlayAnimationSet->GetPeriod();

	//�������� �����ߴٸ�...
	if (_AnimationPlayFactor >= 0.95f)
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
		//_pAnimController->AdvanceTime( _timeDelta, NULL );
		_fAnimDelta = _deltaTime;
	}
	else
	{
		_fAnimDelta = 0.0f;
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

void skinnedAnimation::render(transform * pTransform)
{
	//���� �ڽ��� Animation ������ ����
	_pAnimController->AdvanceTime(_fAnimDelta, NULL);
	_fAnimDelta = 0.0f;		//�ִϸ��̼��� ������ױ� ������ delta ���� 0 ����....

							//�� ���� ��� ����
	MAP_BONETRANSFORM::iterator iterBone;
	for (iterBone = _mapBoneTransform.begin(); iterBone != _mapBoneTransform.end(); ++iterBone)
	{
		iterBone->first->pBoneTransform = iterBone->second;
	}
	for (iterBone = _mapApplyBoneTransform.begin(); iterBone != _mapApplyBoneTransform.end(); ++iterBone)
	{
		iterBone->first->pApplyTransform = iterBone->second;
	}
	_pSkinnedMesh->render(pTransform);
}

void skinnedAnimation::renderBoneName(camera * pCam, transform * pTransform)
{
	//���� �ڽ��� Animation ������ ����
	_pAnimController->AdvanceTime(_fAnimDelta, NULL);
	_fAnimDelta = 0.0f;		//�ִϸ��̼��� ������ױ� ������ delta ���� 0 ����....

							//�� ���� ��� ����
	MAP_BONETRANSFORM::iterator iterBone;
	for (iterBone = _mapBoneTransform.begin(); iterBone != _mapBoneTransform.end(); ++iterBone)
	{
		iterBone->first->pBoneTransform = iterBone->second;
	}
	for (iterBone = _mapApplyBoneTransform.begin(); iterBone != _mapApplyBoneTransform.end(); ++iterBone)
	{
		iterBone->first->pApplyTransform = iterBone->second;
	}
	_pSkinnedMesh->renderBoneName(pCam, pTransform);
}

void skinnedAnimation::Play(string animName, float crossFadeTime)
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

void skinnedAnimation::Play(int animIndex, float crossFadeTime)
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

void skinnedAnimation::Play(LPD3DXANIMATIONSET animSet, float crossFadeTime)
{
	_bPlay = true;
	_bLoop = true;

	//ũ�ν� ���̵� Ÿ�� ���
	_fCrossFadeTime = crossFadeTime;
	_fLeftCrossFadeTime = crossFadeTime;

	this->SetAnimation(animSet);
}

void skinnedAnimation::PlayOneShot(string animName, float inCrossFadeTime, float outCrossFadeTime)
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

void skinnedAnimation::PlayOneShotAfterHold(string animName, float crossFadeTime)
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

void skinnedAnimation::SetPlaySpeed(float speed)
{
	_pAnimController->SetTrackSpeed(0, speed);
}

void skinnedAnimation::AddBoneTransform(string boneName, transform * pTransform)
{
	BONE* pBone = this->_pSkinnedMesh->GetFineBONE(boneName);
	this->_mapBoneTransform.insert(make_pair(pBone, pTransform));
}

void skinnedAnimation::AddApplyTransform(string boneName, transform * pTransform)
{
	BONE* pBone = this->_pSkinnedMesh->GetFineBONE(boneName);
	this->_mapApplyBoneTransform.insert(std::make_pair(pBone, pTransform));
}

void skinnedAnimation::RemoveBoneTransform(string boneName)
{
	BONE* pBone = this->_pSkinnedMesh->GetFineBONE(boneName);
	MAP_BONETRANSFORM::iterator boneIter = _mapBoneTransform.find(pBone);
	if (boneIter != _mapBoneTransform.end())
	{
		_mapBoneTransform.erase(boneIter);
	}
}

void skinnedAnimation::RemoveApplyTransform(string boneName)
{
	BONE* pBone = this->_pSkinnedMesh->GetFineBONE(boneName);
	MAP_BONETRANSFORM::iterator boneIter = _mapApplyBoneTransform.find(pBone);
	if (boneIter != _mapApplyBoneTransform.end())
	{
		_mapApplyBoneTransform.erase(boneIter);
	}
}

//Animation �� �ٲ۴�.
void skinnedAnimation::SetAnimation(LPD3DXANIMATIONSET animSet)
{
	//�̹� �÷��̵ǰ� �ִ� Animation �̶�� ������ ���� �ʴ´�.
	if (this->_pNowPlayAnimationSet != NULL &&
		animSet == this->_pNowPlayAnimationSet) return;

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
	//ũ�ν� ���̵尡 ���ٸ�
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