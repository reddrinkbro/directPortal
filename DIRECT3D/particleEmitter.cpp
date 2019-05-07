#include "stdafx.h"
#include "particleEmitter.h"

void particleEmitter::init(DWORD particleNum, float emission, float liveTimeMin, float liveTimeMax, const D3DXVECTOR3 & velocityMin, const D3DXVECTOR3 & velocityMax, const D3DXVECTOR3 & accelMin, const D3DXVECTOR3 & accelMax, const VEC_COLOR & colors, const VEC_SCALE & scales, float scaleMin, float scaleMax, LPDIRECT3DTEXTURE9 pParticleTexture, bool bLocal)
{
	//�ش� ��ƼŬ ������ �� ��ƼŬ ��
	_particleNum = particleNum;

	//�� ��ƼŬ ���� ŭ ���ؽ� �迭�� �����
	_particleVertices = new PARTICLE_VERTEX[_particleNum];

	//��ƼŬ ��ü ����
	_particles = new particle[_particleNum];

	//�ʴ� ������
	_emissionPerSec = emission;

	//�ʴ� ������ ���� �߻� ����
	_emissionIntervalTime = 1.0f / _emissionPerSec;

	//���� �ð��� 0
	_emissionDeltaTime = 0.0f;

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

	//���ۼ��� �ʱ�ȭ
	_particleCount = 0;

	//Texture ����
	_particleTex = pParticleTexture;

	_isLocal = bLocal;

	emissionType = PZERO;
}

void particleEmitter::release()
{
	SAFE_DELETE_ARRAY(_particles);
	SAFE_DELETE_ARRAY(_particleVertices);
}

void particleEmitter::baseObjectUpdate()
{
	//��� ��ƼŬ ������Ʈ
	for (int i = 0; i < _particleNum; i++)
	{
		_particles[i].update();
	}

	//�ʴ� ���� �߻� ���´�?
	if (_isEmission)
	{
		// �ϳ� �߻��ϰ� ���� �ð�
		_emissionDeltaTime += _deltaTime;

		while (_emissionDeltaTime >= _emissionIntervalTime)
		{
			_emissionDeltaTime -= _emissionIntervalTime;
			//��ƼŬ �ϳ� �߻�
			startOneParticle();
		}
	}
}

void particleEmitter::baseObjectRender()
{
	//�׸� ��ƼŬ ��
	DWORD drawParticleNum = 0;

	for (int i = 0; i < _particleNum; i++)
	{
		//�ش���ƼŬ�� Ȱ��ȭ ���̴�?
		if (_particles[i].getIsLive())
		{
			//�ش� ��ƼŬ�� ������ ��´�.
			_particles[i].getParticleVertex(
				_particleVertices + drawParticleNum,
				_colors, _scales);

			drawParticleNum++;
		}
	}

	device->SetRenderState(D3DRS_LIGHTING, false);							//�������� ����.
	device->SetRenderState(D3DRS_POINTSPRITEENABLE, true);					//����Ʈ ���ö���Ʈ Ȱ��ȭ
	device->SetRenderState(D3DRS_POINTSCALEENABLE, true);					//����Ʈ�� �����ϰ� ���̰ڴ�.
	device->SetRenderState(D3DRS_POINTSIZE_MIN, FloatToDWORD(0.0f));		//����Ʈ�� �ּ� ũ�� ( ȭ����� )
	device->SetRenderState(D3DRS_POINTSIZE_MAX, FloatToDWORD(500.0f));		//����Ʈ�� �ִ� ũ�� ( ȭ����� )
																			//device->SetRenderState( D3DRS_POINTSIZE, FloatToDWORD( 10.0f ) );	//����Ʈ ���� ������ ( ������ ����Ʈ ����� ������ ���õǴµ� );
	device->SetRenderState(D3DRS_ZWRITEENABLE, false);						//z ������ ���⸦ ���´�.

	//��µǴ� POINT size 
	//finalSize = viewportHeight * pointSize *  sqrt( 1 / ( A + B(D) + C(D^2) )  );
	//�Ʒ��� ���� �ϸ� �ڿ������� �Ÿ��� ���� �������� ��Ÿ��
	device->SetRenderState(D3DRS_POINTSCALE_A, FloatToDWORD(0.0f));
	device->SetRenderState(D3DRS_POINTSCALE_B, FloatToDWORD(0.0f));
	device->SetRenderState(D3DRS_POINTSCALE_C, FloatToDWORD(1.0f));

	//���� ���� ����
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//Texture �� ���� Diffuse ���⼭�� �����÷��� ���İ� �� ���� ���� ����� �Ѵ�.
	device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);

	D3DXMATRIXA16 matWorld;

	if (_isLocal == false) D3DXMatrixIdentity(&matWorld);
	else matWorld = this->_transform->getFinalMatrix();

	device->SetTransform(D3DTS_WORLD, &matWorld);

	//��ƼŬ Texture ����
	device->SetTexture(0, _particleTex);
	
	//��ƼŬ ���� ���
	device->SetFVF(PARTICLE_VERTEX::FVF);
	device->DrawPrimitiveUP(D3DPT_POINTLIST, drawParticleNum, _particleVertices, sizeof(PARTICLE_VERTEX));

	//��ƼŬ �׸��� ���� ��ó��
	device->SetRenderState(D3DRS_LIGHTING, true);
	device->SetRenderState(D3DRS_POINTSPRITEENABLE, false);
	device->SetRenderState(D3DRS_POINTSCALEENABLE, false);
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	device->SetRenderState(D3DRS_ZWRITEENABLE, true);
	device->SetTexture(0, NULL);
}

void particleEmitter::startEmission()
{
	_isEmission = true;
}

void particleEmitter::stopEmission()
{
	_isEmission = false;
}

void particleEmitter::burst(int num, float minSpeed, float maxSpeed, float maxLife, float minLife)
{
	for (int i = 0; i < num; i++)
	{
		D3DXVECTOR3 randVec(
			randomFloatRange(-1.0f, 1.0f),
			randomFloatRange(-1.0f, 1.0f),
			randomFloatRange(-1.0f, 1.0f)
		);

		D3DXVec3Normalize(&randVec, &randVec);
		randVec *= randomFloatRange(minSpeed, maxSpeed);

		startOneParticle(randVec, randomFloatRange(maxLife, minLife));
	}
}

void particleEmitter::startOneParticle()
{
	//���̺� Ÿ�� ����
	float liveTime = randomFloatRange(_startLiveTimeMin, _startLiveTimeMax);

	//��ƼŬ�� ������ ��ġ
	D3DXVECTOR3 position;

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
	//���� ������ ���� �߰� ��ġ....
	if (emissionType == PATICLE_EMISSION_TYPE::SPHERE)
	{
		//��������
		D3DXVECTOR3 randDir(
			randomFloatRange(-1.0f, 1.0f),
			randomFloatRange(-1.0f, 1.0f),
			randomFloatRange(-1.0f, 1.0f));

		D3DXVec3Normalize(&randDir, &randDir);

		//�����Ÿ�
		float randDistance = randomFloatRange(0, sphereEmissionRange);

		//�߰���ġ
		position += randDir * randDistance;
	}
	else if (emissionType == PATICLE_EMISSION_TYPE::SPHERE_OUTLINE)
	{
		//��������
		D3DXVECTOR3 randDir(
			randomFloatRange(-1.0f, 1.0f),
			randomFloatRange(-1.0f, 1.0f),
			randomFloatRange(-1.0f, 1.0f));

		D3DXVec3Normalize(&randDir, &randDir);

		//�߰���ġ
		position += randDir * sphereEmissionRange;
	}
	else if (emissionType == PATICLE_EMISSION_TYPE::BOX)
	{
		//��������
		D3DXVECTOR3 randDir(
			randomFloatRange(minEmissionRangeX, maxEmissionRangeX),
			randomFloatRange(minEmissionRangeY, maxEmissionRangeY),
			randomFloatRange(minEmissionRangeZ, maxEmissionRangeZ));

		//�߰���ġ
		position += randDir;
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

	//�ڽ��� ���� ��Ʈ������ ������ �´�.
	if (_isLocal == false)
	{
		D3DXMATRIXA16 matWorld = this->_transform->getFinalMatrix();
		D3DXVec3TransformNormal(&velocity, &velocity, &matWorld);
		D3DXVec3TransformNormal(&accelation, &accelation, &matWorld);
	}

	//�����ϵ� ����
	float scale = randomFloatRange(_startScaleMin, _startScaleMax);

	//������� �߻���Ų��
	_particles[_particleCount].start(liveTime, &position, &velocity, &accelation, scale);

	//���� ��ƼŬ�� ���� ���� ����
	_particleCount++;
	if (_particleCount == this->_particleNum)
	{
		_particleCount = 0;
	}
}

void particleEmitter::startOneParticle(D3DXVECTOR3 dir, float life)
{
	float liveTime = life;

	//��ƼŬ�� ������ ��ġ
	D3DXVECTOR3 position;
	if (this->_isLocal == false)
	{
		position = _transform->getWorldPosition();
	}
	else
	{
		position = D3DXVECTOR3(0, 0, 0);
	}

	//���� ����
	D3DXVECTOR3 velocity;
	velocity.x = dir.x;
	velocity.y = dir.y;
	velocity.z = dir.z;

	D3DXVECTOR3 accelation;
	accelation.x = randomFloatRange(_startAccelateMin.x, _startAccelateMax.x);
	accelation.y = randomFloatRange(_startAccelateMin.y, _startAccelateMax.y);
	accelation.z = randomFloatRange(_startAccelateMin.z, _startAccelateMax.z);

	//�ڽ��� ���� ��Ʈ������ ������ �´�.
	if (_isLocal == false)
	{
		D3DXMATRIXA16 matWorld = this->_transform->getFinalMatrix();
		D3DXVec3TransformNormal(&velocity, &velocity, &matWorld);
		D3DXVec3TransformNormal(&accelation, &accelation, &matWorld);
	}

	//�����ϵ� ����
	float scale = randomFloatRange(_startScaleMin, _startScaleMax);

	//�߻���Ų��
	_particles[_particleCount].start(liveTime, &position, &velocity, &accelation, scale);

	_particleCount++;
	if (_particleCount == this->_particleNum)
	{
		_particleCount = 0;
	}
}