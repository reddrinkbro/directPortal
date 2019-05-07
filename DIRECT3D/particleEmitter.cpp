#include "stdafx.h"
#include "particleEmitter.h"

void particleEmitter::init(DWORD particleNum, float emission, float liveTimeMin, float liveTimeMax, const D3DXVECTOR3 & velocityMin, const D3DXVECTOR3 & velocityMax, const D3DXVECTOR3 & accelMin, const D3DXVECTOR3 & accelMax, const VEC_COLOR & colors, const VEC_SCALE & scales, float scaleMin, float scaleMax, LPDIRECT3DTEXTURE9 pParticleTexture, bool bLocal)
{
	//해당 파티클 렌더의 총 파티클 수
	_particleNum = particleNum;

	//총 파티클 수만 큼 버텍스 배열을 만든다
	_particleVertices = new PARTICLE_VERTEX[_particleNum];

	//파티클 객체 생성
	_particles = new particle[_particleNum];

	//초당 생성량
	_emissionPerSec = emission;

	//초당 생성량 따른 발생 간격
	_emissionIntervalTime = 1.0f / _emissionPerSec;

	//지난 시간도 0
	_emissionDeltaTime = 0.0f;

	//발생 여부 false
	_isEmission = false;

	//컬러 대입
	_colors.clear();
	for (int i = 0; i < colors.size(); i++)
	{
		_colors.push_back(colors[i]);
	}

	//사이즈 대입
	_scales.clear();
	for (int i = 0; i < scales.size(); i++)
	{
		_scales.push_back(scales[i]);
	}

	//시작 라이브 타임 대입
	_startLiveTimeMin = liveTimeMin;
	_startLiveTimeMax = liveTimeMax;

	//시작 속도 대입
	_startVelocityMin = velocityMin;
	_startVelocityMax = velocityMax;

	//시작 가속 대입
	_startAccelateMin = accelMin;
	_startAccelateMax = accelMax;

	//시작 스케일 대입
	_startScaleMin = scaleMin;
	_startScaleMax = scaleMax;

	//시작순번 초기화
	_particleCount = 0;

	//Texture 참조
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
	//모든 파티클 업데이트
	for (int i = 0; i < _particleNum; i++)
	{
		_particles[i].update();
	}

	//너는 지금 발생 상태니?
	if (_isEmission)
	{
		// 하나 발생하고 지난 시간
		_emissionDeltaTime += _deltaTime;

		while (_emissionDeltaTime >= _emissionIntervalTime)
		{
			_emissionDeltaTime -= _emissionIntervalTime;
			//파티클 하나 발사
			startOneParticle();
		}
	}
}

void particleEmitter::baseObjectRender()
{
	//그릴 파티클 수
	DWORD drawParticleNum = 0;

	for (int i = 0; i < _particleNum; i++)
	{
		//해당파티클이 활성화 중이니?
		if (_particles[i].getIsLive())
		{
			//해당 파티클의 정보를 얻는다.
			_particles[i].getParticleVertex(
				_particleVertices + drawParticleNum,
				_colors, _scales);

			drawParticleNum++;
		}
	}

	device->SetRenderState(D3DRS_LIGHTING, false);							//라이팅을 끈다.
	device->SetRenderState(D3DRS_POINTSPRITEENABLE, true);					//포인트 스플라이트 활성화
	device->SetRenderState(D3DRS_POINTSCALEENABLE, true);					//포인트의 스케일값 먹이겠다.
	device->SetRenderState(D3DRS_POINTSIZE_MIN, FloatToDWORD(0.0f));		//포인트의 최소 크기 ( 화면기준 )
	device->SetRenderState(D3DRS_POINTSIZE_MAX, FloatToDWORD(500.0f));		//포인트의 최대 크기 ( 화면기준 )
																			//device->SetRenderState( D3DRS_POINTSIZE, FloatToDWORD( 10.0f ) );	//포인트 기준 사이즈 ( 정점의 포인트 사이즈가 있으면 무시되는듯 );
	device->SetRenderState(D3DRS_ZWRITEENABLE, false);						//z 버퍼의 쓰기를 막는다.

	//출력되는 POINT size 
	//finalSize = viewportHeight * pointSize *  sqrt( 1 / ( A + B(D) + C(D^2) )  );
	//아래와 같이 하면 자연스러운 거리에 따른 스케일이 나타남
	device->SetRenderState(D3DRS_POINTSCALE_A, FloatToDWORD(0.0f));
	device->SetRenderState(D3DRS_POINTSCALE_B, FloatToDWORD(0.0f));
	device->SetRenderState(D3DRS_POINTSCALE_C, FloatToDWORD(1.0f));

	//알파 블렌딩 세팅
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//Texture 의 값과 Diffuse 여기서는 정점컬러의 알파값 을 섞어 최종 출력을 한다.
	device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);

	D3DXMATRIXA16 matWorld;

	if (_isLocal == false) D3DXMatrixIdentity(&matWorld);
	else matWorld = this->_transform->getFinalMatrix();

	device->SetTransform(D3DTS_WORLD, &matWorld);

	//파티클 Texture 세팅
	device->SetTexture(0, _particleTex);
	
	//파티클 정점 출력
	device->SetFVF(PARTICLE_VERTEX::FVF);
	device->DrawPrimitiveUP(D3DPT_POINTLIST, drawParticleNum, _particleVertices, sizeof(PARTICLE_VERTEX));

	//파티클 그리고 난후 후처리
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
	//라이브 타임 랜덤
	float liveTime = randomFloatRange(_startLiveTimeMin, _startLiveTimeMax);

	//파티클이 생성될 위치
	D3DXVECTOR3 position;

	//로컬이 아닌경우 자신의 월드 위치에서 시작하고 
	if (this->_isLocal == false)
	{
		position = this->_transform->getWorldPosition();
	}
	//로컬인 경우 0 에서 시작한다.
	else
	{
		position = D3DXVECTOR3(0, 0, 0);
	}
	//생성 범위에 따른 추가 위치....
	if (emissionType == PATICLE_EMISSION_TYPE::SPHERE)
	{
		//랜덤벡터
		D3DXVECTOR3 randDir(
			randomFloatRange(-1.0f, 1.0f),
			randomFloatRange(-1.0f, 1.0f),
			randomFloatRange(-1.0f, 1.0f));

		D3DXVec3Normalize(&randDir, &randDir);

		//랜덤거리
		float randDistance = randomFloatRange(0, sphereEmissionRange);

		//추가위치
		position += randDir * randDistance;
	}
	else if (emissionType == PATICLE_EMISSION_TYPE::SPHERE_OUTLINE)
	{
		//랜덤벡터
		D3DXVECTOR3 randDir(
			randomFloatRange(-1.0f, 1.0f),
			randomFloatRange(-1.0f, 1.0f),
			randomFloatRange(-1.0f, 1.0f));

		D3DXVec3Normalize(&randDir, &randDir);

		//추가위치
		position += randDir * sphereEmissionRange;
	}
	else if (emissionType == PATICLE_EMISSION_TYPE::BOX)
	{
		//랜덤벡터
		D3DXVECTOR3 randDir(
			randomFloatRange(minEmissionRangeX, maxEmissionRangeX),
			randomFloatRange(minEmissionRangeY, maxEmissionRangeY),
			randomFloatRange(minEmissionRangeZ, maxEmissionRangeZ));

		//추가위치
		position += randDir;
	}

	//벡터 랜덤
	D3DXVECTOR3 velocity;
	velocity.x = randomFloatRange(_startVelocityMin.x, _startVelocityMax.x);
	velocity.y = randomFloatRange(_startVelocityMin.y, _startVelocityMax.y);
	velocity.z = randomFloatRange(_startVelocityMin.z, _startVelocityMax.z);

	D3DXVECTOR3 accelation;
	accelation.x = randomFloatRange(_startAccelateMin.x, _startAccelateMax.x);
	accelation.y = randomFloatRange(_startAccelateMin.y, _startAccelateMax.y);
	accelation.z = randomFloatRange(_startAccelateMin.z, _startAccelateMax.z);

	//자신의 월드 매트릭스를 가지고 온다.
	if (_isLocal == false)
	{
		D3DXMATRIXA16 matWorld = this->_transform->getFinalMatrix();
		D3DXVec3TransformNormal(&velocity, &velocity, &matWorld);
		D3DXVec3TransformNormal(&accelation, &accelation, &matWorld);
	}

	//스케일도 랜덤
	float scale = randomFloatRange(_startScaleMin, _startScaleMax);

	//순번대로 발생시킨다
	_particles[_particleCount].start(liveTime, &position, &velocity, &accelation, scale);

	//다음 파티클을 위한 순번 증가
	_particleCount++;
	if (_particleCount == this->_particleNum)
	{
		_particleCount = 0;
	}
}

void particleEmitter::startOneParticle(D3DXVECTOR3 dir, float life)
{
	float liveTime = life;

	//파티클이 생성될 위치
	D3DXVECTOR3 position;
	if (this->_isLocal == false)
	{
		position = _transform->getWorldPosition();
	}
	else
	{
		position = D3DXVECTOR3(0, 0, 0);
	}

	//벡터 랜덤
	D3DXVECTOR3 velocity;
	velocity.x = dir.x;
	velocity.y = dir.y;
	velocity.z = dir.z;

	D3DXVECTOR3 accelation;
	accelation.x = randomFloatRange(_startAccelateMin.x, _startAccelateMax.x);
	accelation.y = randomFloatRange(_startAccelateMin.y, _startAccelateMax.y);
	accelation.z = randomFloatRange(_startAccelateMin.z, _startAccelateMax.z);

	//자신의 월드 매트릭스를 가지고 온다.
	if (_isLocal == false)
	{
		D3DXMATRIXA16 matWorld = this->_transform->getFinalMatrix();
		D3DXVec3TransformNormal(&velocity, &velocity, &matWorld);
		D3DXVec3TransformNormal(&accelation, &accelation, &matWorld);
	}

	//스케일도 랜덤
	float scale = randomFloatRange(_startScaleMin, _startScaleMax);

	//발생시킨다
	_particles[_particleCount].start(liveTime, &position, &velocity, &accelation, scale);

	_particleCount++;
	if (_particleCount == this->_particleNum)
	{
		_particleCount = 0;
	}
}