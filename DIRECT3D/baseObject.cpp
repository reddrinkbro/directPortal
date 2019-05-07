#include "stdafx.h"
#include "baseObject.h"

void baseObject::update()
{
	if (_isActive)
	{
		baseObjectUpdate();
		if (_skinnedAnim != NULL)
		{
			_skinnedAnim->update();
		}
	}
	else
	{
		baseObjectNoActiveUpdate();
	}
}

void baseObject::render()
{
	if (_isActive)
	{
		baseObjectRender();
	}
}

void baseObject::setMesh(xMesh * pMesh)
{
	_pMesh = pMesh;
	xMeshSkinned* pSkin = dynamic_cast<xMeshSkinned*>(pMesh);
	this->computeBoundBox();

	// staticMesh ? skinnedMesh?
	if (pSkin == NULL)
	{
		if (_skinnedAnim != NULL)
		{
			_skinnedAnim->release();
			SAFE_DELETE(_skinnedAnim);
		}

	}
	else
	{
		_skinnedAnim = new skinnedAnimation;
		_skinnedAnim->init(pSkin);
	}
}

void baseObject::setActive(bool isActive)
{
	//Ȱ��ȭ �Ǿ��ִ� ���¿��� ��Ȱ��ȭ ���ý� 
	if (isActive == false && _isActive)
	{
		_isActive = false;
		baseObjectDisable();
	}
	//��Ȱ��ȭ �Ǿ��ִ� ���¿��� Ȱ��ȭ ���ý�
	else if (isActive && _isActive == false)
	{
		_isActive = true;
		baseObjectEnable();
	}
}

void baseObject::computeBoundBox()
{
	if (_pMesh != NULL)
	{
		/*
		dynamic_cast : ��Ӱ��� �ȿ��� �����ͳ� �������� Ÿ����
		�⺻Ŭ�������� �Ļ�Ŭ������ �ٿ� ĳ���ð�
		���߻�ӿ��� �⺻ Ŭ�������� ������ Ÿ��ĳ���ÿ� ����Ѵ�.
		*/
		xMeshStatic* pStaticMesh = dynamic_cast<xMeshStatic*>(_pMesh);
		//���õ� �޽��� xMeshStatic ���
		if (pStaticMesh != NULL)
		{
			_boundBox._localCenter = pStaticMesh->_boundCenter;
			_boundBox._halfSize = pStaticMesh->_boundHalfSize;
			_boundBox._localMinPos = pStaticMesh->_boundMin;
			_boundBox._localMaxPos = pStaticMesh->_boundMax;
			_boundBox._radius = pStaticMesh->_boundRadius;
		}
		// ���� �� �޽��� xMeshSinned ��� �ӽ÷� �ٿ�� �ڽ� �����
		else
		{	 
			_boundBox.setBound(&D3DXVECTOR3(0, 0, 0),
				&D3DXVECTOR3(10.0f, 10.0f, 10.0f));
		}
	}
}

void baseObject::baseObjectRender()
{
	if (_pMesh != NULL)
	{
		if (_skinnedAnim)
		{
			_skinnedAnim->render(_transform);
		}
		else
		{
			_pMesh->render(_transform);
		}
	}	
}

baseObject::baseObject() :  
	_isActive(FALSE), _pMesh(NULL), _skinnedAnim(NULL) 
{ 
	_transform = new transform;
	_ignoreCreateShadow = false;
}

baseObject::~baseObject()
{
	SAFE_DELETE(_transform);

	if (_skinnedAnim != NULL) {
		_skinnedAnim->release();
		SAFE_DELETE(_skinnedAnim);
	}
}

