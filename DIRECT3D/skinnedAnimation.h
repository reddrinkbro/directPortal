#pragma once
#include "xMeshSkinned.h"

class skinnedAnimation
{
private:
	typedef map<BONE*, transform*>				MAP_BONETRANSFORM;	//���εǴ� ��Transform ��
	typedef map<string, LPD3DXANIMATIONSET >	MAP_ANIMSET;
	typedef vector< LPD3DXANIMATIONSET >		VEC_ANIMSET;

private:
	xMeshSkinned*		_pSkinnedMesh;
	MAP_BONETRANSFORM	_mapApplyBoneTransform;
	MAP_BONETRANSFORM	_mapBoneTransform;

	/////////////////////////////////////////////////////////////////////////////////

	LPD3DXANIMATIONCONTROLLER	_pAnimController;			//Animation ��Ʈ�ѷ�
	UINT						_AnimNum;					//Animation ����
	MAP_ANIMSET					_mapAnimSet;				//Map ���� �Ǿ��ִ� AnimationSet
	VEC_ANIMSET					_vecAnimSet;				//Vector �� �Ǿ��ִ� AnimationSet
	LPD3DXANIMATIONSET			_pNowPlayAnimationSet;		//���� �÷��̵ǰ��ִ� AnimationSet
	D3DXTRACK_DESC				_Track_Desc_0;				//0�� Track_Desc ( 0 �� Track �� ���� )
	bool						_bPlay;						//���� �÷��� ���̴�?
	bool						_bLoop;						//���� �÷��̴�?
	LPD3DXANIMATIONSET			_pPrevPlayAnimationSet;		//OneShot �÷��̽� �ѹ� Animation �÷��̵ǰ� �ٽ� �ǵ��ư� Animaiton
	float						_fCrossFadeTime;			//ũ�ν����̵� Ÿ��
	float						_fLeftCrossFadeTime;		//����ũ�ν����̵� Ÿ��
	float						_fOutCrossFadeTime;			//OneShot Animation ������ �ǵ��ư��� CrossFade Ÿ��
	double						_AnimationPlayFactor;		//Animation ���� Factor ( 0 �̸� ó�� 1 �ϼ��� ������ )
	float						_fAnimDelta;

public:
	HRESULT	init(xMeshSkinned* pSkinnedMesh);
	void release();
	void update();
	void render(transform* pTransform);
	void renderBoneName(camera* pCam, transform* pTransform);

	void Play(string animName, float crossFadeTime = 0.0);
	void Play(int animIndex, float crossFadeTime = 0.0);
	void Play(LPD3DXANIMATIONSET animSet, float crossFadeTime = 0.0);
	void PlayOneShot(string animName, float inCrossFadeTime = 0.0, float outCrossFadeTime = 0.0f);
	void PlayOneShotAfterHold(string animName, float crossFadeTime = 0.0);
	void Stop() { _bPlay = false; }
	void SetPlaySpeed(float speed);

	void AddBoneTransform(std::string boneName, transform* pTransform);
	void AddApplyTransform(std::string boneName, transform* pTransform);
	void RemoveBoneTransform(std::string boneName);
	void RemoveApplyTransform(std::string boneName);
public:
	skinnedAnimation();
	~skinnedAnimation();

private:
	//Animation �� �ٲ۴�.
	void SetAnimation(LPD3DXANIMATIONSET animSet);
};