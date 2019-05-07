#include "stdafx.h"
#include "quadParticleEmitter.h"

HRESULT quadParticleEmitter::init(DWORD partcleNum, float emission, float liveTimeMin, float liveTimeMax, const D3DXVECTOR3 & velocityMin, const D3DXVECTOR3 & velocityMax, const D3DXVECTOR3 & accelMin, const D3DXVECTOR3 & accelMax, const D3DXVECTOR3 & startRotateMin, const D3DXVECTOR3 & startRotateMax, const D3DXVECTOR3 & rotateMin, const D3DXVECTOR3 & rotateMax, const D3DXVECTOR3 & rotateAccelMin, const D3DXVECTOR3 & rotateAccelMax, const VEC_COLOR & colors, const VEC_SCALE & scales, float scaleMin, float scaleMax, LPDIRECT3DTEXTURE9 pTex, bool bLocal)
{
	//�ش� ��ƼŬ ������ �� ��ƼŬ ��
	_paticleNum = partcleNum;

	//����ƼŬ �� * 4 ��ŭ ���ؽ��� �����
	_particleVerticles = new quadParticle_VERTEX[_paticleNum * 4];

	//����ƼŬ �� * 6 ��ŭ �ε����� �����.
	_particleIndex = new DWORD[_paticleNum * 6];

	//��ƼŬ ��ü ����
	_paticles = new quadParticle[_paticleNum];

	//��ƼŬ ��Ʈ�� Array
	_sortArray = new quadParticle*[_paticleNum];

	//�ʴ� ������
	_emissionPerSec = emission;

	//�ʴ� ������ ���� �߻� ����
	_emisionIntervalTime = 1.0f / _emissionPerSec;

	//���� �ð��� 0
	_emisionDeltaTime = 0.0f;

	//�߻� ���� false
	_isEmission = false;

	//�÷� ����
	_colors.clear();
	for (int i = 0; i < colors.size(); i++)
	{
		_colors.push_back(colors[i]);
	}
	//������ ����
	_scales.clear();
	for (int i = 0; i < scales.size(); i++)
	{
		_scales.push_back(scales[i]);
	}

	//���� ���̺� Ÿ�� ����
	_startLiveTimeMin = liveTimeMin;
	_startLiveTimeMax = liveTimeMax;

	//���� �ӵ� ����
	_startVelocityMin = velocityMin;
	_startVelocityMax = velocityMax;

	//���� ���� ����
	_startAccelateMin = accelMin;
	_startAccelateMax = accelMax;

	//���� ������ ����
	_startScaleMin = scaleMin;
	_startScaleMax = scaleMax;

	//���۽� ȸ������ ���� �Լ�
	_startSetRotateMax = startRotateMax;
	_startSetRotateMin = startRotateMin;

	//����ȸ�� �� ����
	_startRotateMin = rotateMin;
	_startRotateMax = rotateMax;

	//ȸ�� ���� �� ����
	_startRotateAccelateMin = rotateAccelMin;
	_startRotateAccelateMax = rotateAccelMax;

	//���ۼ��� �ʱ�ȭ
	_particleCount = 0;

	//��ƼŬ �ؽ���
	_particleTex = pTex;

	_isLocal = bLocal;

	return S_OK;
}

void quadParticleEmitter::release()
{
	SAFE_DELETE_ARRAY(_sortArray);
	SAFE_DELETE_ARRAY(_paticles);
	SAFE_DELETE_ARRAY(_particleVerticles);
	SAFE_DELETE_ARRAY(_particleIndex);
	SAFE_RELEASE(_particleTex);
}

void quadParticleEmitter::baseObjectUpdate()
{
	//�ʰ� ���� �߻� ���´�?
	if (_isEmission)
	{
		//�ϳ� �߻��ϰ� �����ð�
		_emisionDeltaTime += _deltaTime;

		while (_emisionDeltaTime > _emisionIntervalTime)
		{
			_emisionDeltaTime -= _emisionIntervalTime;

			//��ƼŬ �ϳ� �߻�
			startOneParticle();
		}
	}

	//��� ��ƼŬ ������Ʈ
	for (int i = 0; i < _paticleNum; i++)
	{
		_paticles[i].update();
	}
}

void quadParticleEmitter::baseObjectRender()
{
	//�׸� ��ƼŬ ��
	DWORD drawParticleNum = 0;

	//ī�޶� ��Ʈ�� �ϳ�?
	if (this->_isCameraSort && this->_cameraTransform != NULL)
	{
		//�׸� ��ƼŬ ��
		for (int i = 0; i < _paticleNum; i++)
		{
			//�ش���ƼŬ�� Ȱ��ȭ ���̴�?
			if (_paticles[i].isLive())
			{
				//��Ʈ�迭�� �ش� ��ƼŬ�� ���� ��Ų��.
				_sortArray[drawParticleNum] = &_paticles[i];
				drawParticleNum++;
			}
		}

		//ī�޶� ���� ��ġ
		D3DXVECTOR3 cameraWorld = _cameraTransform->getWorldPosition();

		//�׷����� ���� ��� ī�޶� �Ÿ��� ��Ʈ�� �����Ѵ�.
		for (int i = 0; i < (drawParticleNum - 1); i++)
		{
			for (int j = i + 1; j < drawParticleNum; j++)
			{
				//2���� ī�޶� �Ÿ��� ���
				//i �� ī�޶���� �Ÿ� ����
				D3DXVECTOR3 dirI = cameraWorld - _sortArray[i]->_transform.getWorldPosition();
				float distI = D3DXVec3LengthSq(&dirI);

				//J �� ī�޶���� �Ÿ� ����
				D3DXVECTOR3 dirJ = cameraWorld - _sortArray[j]->_transform.getWorldPosition();
				float distJ = D3DXVec3LengthSq(&dirJ);

				//�հŸ� ������
				if (distJ > distI)
				{
					quadParticle* pTemp = _sortArray[j];
					_sortArray[j] = _sortArray[i];
					_sortArray[i] = pTemp;
				}
			}
		}

		//��Ʈ�迭 ������ŭ���� �޽� ����
		for (int i = 0; i < drawParticleNum; i++)
		{
			//��Ʈ�迭�� �ش� ��ƼŬ�� ���� ��Ų��.
			_sortArray[i]->getParticleVertex(
				&(_particleVerticles[i * 4]),
				&(_particleIndex[i * 6]),
				_colors, _scales, i);
		}
	}
	//��Ʈ ���ϳ�?
	else
	{
		for (int i = 0; i < _paticleNum; i++)
		{
			//�ش���ƼŬ�� Ȱ��ȭ ���̴�?
			if (_paticles[i].isLive())
			{
				//�ش� ��ƼŬ�� ������ ��´�.
				_paticles[i].getParticleVertex(
					&(_particleVerticles[drawParticleNum * 4]),		//���� ���� �ּ� 4 �� ����.
					&(_particleIndex[drawParticleNum * 6]),			//���� �ε��� �ּ� 6 �� ����.
					_colors,
					_scales,
					drawParticleNum	 //�ε��� ������ ������� �׷��� ��ƼŬ���� ���� �Ѱܾ� �Ѵ�....
				);
				drawParticleNum++;
			}
		}
	}

	device->SetRenderState(D3DRS_LIGHTING, false);		//�������� ����.
	device->SetRenderState(D3DRS_ZWRITEENABLE, false);	//z ������ ���⸦ ���´�.

	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//���� ���� ����
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	//device->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	//device->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE);

	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//Texture �� ���� Diffuse ���⼭�� �����÷��� ���İ� �� ���� ���� ����� �Ѵ�.
	device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);

	D3DXMATRIXA16 matWorld;

	if (_isLocal == false)
	{
		D3DXMatrixIdentity(&matWorld);
	}
	else
	{
		matWorld = this->_transform->getFinalMatrix();
	}
	device->SetTransform(D3DTS_WORLD, &matWorld);

	//��ƼŬ Texture ����
	device->SetTexture(0, _particleTex);

	//��ƼŬ ���� ���
	device->SetFVF(quadParticle_VERTEX::FVF);
	device->DrawIndexedPrimitiveUP(
		D3DPT_TRIANGLELIST,
		0,
		drawParticleNum * 4,			//������
		drawParticleNum * 2,			//�ﰢ�� ��
		_particleIndex,
		D3DFMT_INDEX32,
		_particleVerticles,
		sizeof(quadParticle_VERTEX));

	//��ƼŬ �׸��� ���� ��ó��
	device->SetRenderState(D3DRS_LIGHTING, true);
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	device->SetRenderState(D3DRS_ZWRITEENABLE, true);
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	device->SetTexture(0, NULL);
}

void quadParticleEmitter::startEmission()
{
	this->_isEmission = true;
}

void quadParticleEmitter::stopEmission()
{
	this->_isEmission = false;
}

void quadParticleEmitter::setCameraSort(transform * pCameraTrans, bool bSort)
{
	this->_cameraTransform = pCameraTrans;		//ī�޶� ��Ʈ ���
	this->_isCameraSort = bSort;
}

void quadParticleEmitter::startOneParticle()
{
	//���̺� Ÿ�� ����
	float liveTime = randomFloatRange(_startLiveTimeMin, _startLiveTimeMax);

	D3DXVECTOR3 position = _transform->getWorldPosition();

	//������ �ƴѰ�� �ڽ��� ���� ��ġ���� �����ϰ� 
	if (this->_isLocal == false)
	{
		position = this->_transform->getWorldPosition();
	}
	//������ ��� 0 ���� �����Ѵ�.
	else
	{
		position = D3DXVECTOR3(0, 0, 0);
	}

	//���� ����
	D3DXVECTOR3 velocity;
	velocity.x = randomFloatRange(_startVelocityMin.x, _startVelocityMax.x);
	velocity.y = randomFloatRange(_startVelocityMin.y, _startVelocityMax.y);
	velocity.z = randomFloatRange(_startVelocityMin.z, _startVelocityMax.z);

	D3DXVECTOR3 accelation;
	accelation.x = randomFloatRange(_startAccelateMin.x, _startAccelateMax.x);
	accelation.y = randomFloatRange(_startAccelateMin.y, _startAccelateMax.y);
	accelation.z = randomFloatRange(_startAccelateMin.z, _startAccelateMax.z);

	D3DXVECTOR3 setRotate;
	setRotate.x = randomFloatRange(_startSetRotateMin.x, _startSetRotateMax.x);
	setRotate.y = randomFloatRange(_startSetRotateMin.y, _startSetRotateMax.y);
	setRotate.z = randomFloatRange(_startSetRotateMin.z, _startSetRotateMax.z);

	//�������� ȸ���� ����
	D3DXVECTOR3 rotate;
	rotate.x = randomFloatRange(_startRotateMin.x, _startRotateMax.x);
	rotate.y = randomFloatRange(_startRotateMin.y, _startRotateMax.y);
	rotate.z = randomFloatRange(_startRotateMin.z, _startRotateMax.z);

	//�������� ȸ�� ���ӷ� ����
	D3DXVECTOR3 rotateAccel;
	rotateAccel.x = randomFloatRange(_startRotateAccelateMin.x, _startRotateAccelateMax.x);
	rotateAccel.y = randomFloatRange(_startRotateAccelateMin.y, _startRotateAccelateMax.y);
	rotateAccel.z = randomFloatRange(_startRotateAccelateMin.z, _startRotateAccelateMax.z);

	//�ڽ��� ���� ��Ʈ������ ������ �´�.
	if (_isLocal == false)
	{
		D3DXMATRIXA16 matWorld = this->_transform->getFinalMatrix();
		D3DXVec3TransformNormal(&velocity, &velocity, &matWorld);
		D3DXVec3TransformNormal(&accelation, &accelation, &matWorld);
	}

	//�����ϵ� ����
	float scale = randomFloatRange(_startScaleMin, _startScaleMax);

	_paticles[_particleCount]._transform.setRotateLocal(setRotate.x, setRotate.y, setRotate.z);

	//������� �߻���Ų��
	_paticles[_particleCount].Start(
		liveTime,			//���̺� Ÿ��
		&position,			//��ġ
		&velocity,			//���� ��Ƽ
		&accelation,		//����
		&rotate,			//ȸ����
		&rotateAccel,		//ȸ�� ����
		scale				//������
	);

	//���� ��ƼŬ�� ���� ���� ����
	_particleCount++;
	if (_particleCount == this->_paticleNum)
	{
		_particleCount = 0;
	}
}