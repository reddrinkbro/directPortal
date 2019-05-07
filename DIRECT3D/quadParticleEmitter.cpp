#include "stdafx.h"
#include "quadParticleEmitter.h"

HRESULT quadParticleEmitter::init(DWORD partcleNum, float emission, float liveTimeMin, float liveTimeMax, const D3DXVECTOR3 & velocityMin, const D3DXVECTOR3 & velocityMax, const D3DXVECTOR3 & accelMin, const D3DXVECTOR3 & accelMax, const D3DXVECTOR3 & startRotateMin, const D3DXVECTOR3 & startRotateMax, const D3DXVECTOR3 & rotateMin, const D3DXVECTOR3 & rotateMax, const D3DXVECTOR3 & rotateAccelMin, const D3DXVECTOR3 & rotateAccelMax, const VEC_COLOR & colors, const VEC_SCALE & scales, float scaleMin, float scaleMax, LPDIRECT3DTEXTURE9 pTex, bool bLocal)
{
	//해당 파티클 랜더의 총 파티클 수
	_paticleNum = partcleNum;

	//총파티클 수 * 4 만큼 버텍스를 만든다
	_particleVerticles = new quadParticle_VERTEX[_paticleNum * 4];

	//총파티클 수 * 6 만큼 인덱스를 만든다.
	_particleIndex = new DWORD[_paticleNum * 6];

	//파티클 객체 생성
	_paticles = new quadParticle[_paticleNum];

	//파티클 소트용 Array
	_sortArray = new quadParticle*[_paticleNum];

	//초당 생성량
	_emissionPerSec = emission;

	//초당 생성량 따른 발생 간격
	_emisionIntervalTime = 1.0f / _emissionPerSec;

	//지난 시간도 0
	_emisionDeltaTime = 0.0f;

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

	//시작시 회전방향 세팅 함수
	_startSetRotateMax = startRotateMax;
	_startSetRotateMin = startRotateMin;

	//시작회전 값 대입
	_startRotateMin = rotateMin;
	_startRotateMax = rotateMax;

	//회전 가속 값 대입
	_startRotateAccelateMin = rotateAccelMin;
	_startRotateAccelateMax = rotateAccelMax;

	//시작순번 초기화
	_particleCount = 0;

	//파티클 텍스쳐
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
	//너가 지금 발생 상태니?
	if (_isEmission)
	{
		//하나 발생하고 지난시간
		_emisionDeltaTime += _deltaTime;

		while (_emisionDeltaTime > _emisionIntervalTime)
		{
			_emisionDeltaTime -= _emisionIntervalTime;

			//파티클 하나 발사
			startOneParticle();
		}
	}

	//모든 파티클 업데이트
	for (int i = 0; i < _paticleNum; i++)
	{
		_paticles[i].update();
	}
}

void quadParticleEmitter::baseObjectRender()
{
	//그릴 파티클 수
	DWORD drawParticleNum = 0;

	//카메라 소트를 하냐?
	if (this->_isCameraSort && this->_cameraTransform != NULL)
	{
		//그릴 파티클 수
		for (int i = 0; i < _paticleNum; i++)
		{
			//해당파티클이 활성화 중이니?
			if (_paticles[i].isLive())
			{
				//소트배열에 해당 파티클을 참조 시킨다.
				_sortArray[drawParticleNum] = &_paticles[i];
				drawParticleNum++;
			}
		}

		//카메라 월드 위치
		D3DXVECTOR3 cameraWorld = _cameraTransform->getWorldPosition();

		//그려지는 갯수 대로 카메라 거리별 소트를 진행한다.
		for (int i = 0; i < (drawParticleNum - 1); i++)
		{
			for (int j = i + 1; j < drawParticleNum; j++)
			{
				//2개의 카메라 거리를 잰다
				//i 의 카메라와의 거리 제곱
				D3DXVECTOR3 dirI = cameraWorld - _sortArray[i]->_transform.getWorldPosition();
				float distI = D3DXVec3LengthSq(&dirI);

				//J 의 카메라와의 거리 제곱
				D3DXVECTOR3 dirJ = cameraWorld - _sortArray[j]->_transform.getWorldPosition();
				float distJ = D3DXVec3LengthSq(&dirJ);

				//먼거를 앞으로
				if (distJ > distI)
				{
					quadParticle* pTemp = _sortArray[j];
					_sortArray[j] = _sortArray[i];
					_sortArray[i] = pTemp;
				}
			}
		}

		//소트배열 갯수만큼돌며 메쉬 조합
		for (int i = 0; i < drawParticleNum; i++)
		{
			//소트배열에 해당 파티클을 참조 시킨다.
			_sortArray[i]->getParticleVertex(
				&(_particleVerticles[i * 4]),
				&(_particleIndex[i * 6]),
				_colors, _scales, i);
		}
	}
	//소트 안하냐?
	else
	{
		for (int i = 0; i < _paticleNum; i++)
		{
			//해당파티클이 활성화 중이니?
			if (_paticles[i].isLive())
			{
				//해당 파티클의 정보를 얻는다.
				_paticles[i].getParticleVertex(
					&(_particleVerticles[drawParticleNum * 4]),		//써질 정점 주소 4 씩 쓴다.
					&(_particleIndex[drawParticleNum * 6]),			//써질 인덱스 주소 6 씩 쓴다.
					_colors,
					_scales,
					drawParticleNum	 //인덱스 때문에 현재까지 그려진 파티클수도 같이 넘겨야 한다....
				);
				drawParticleNum++;
			}
		}
	}

	device->SetRenderState(D3DRS_LIGHTING, false);		//라이팅을 끈다.
	device->SetRenderState(D3DRS_ZWRITEENABLE, false);	//z 버퍼의 쓰기를 막는다.

	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//알파 블렌딩 세팅
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	//device->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	//device->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE);

	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//Texture 의 값과 Diffuse 여기서는 정점컬러의 알파값 을 섞어 최종 출력을 한다.
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

	//파티클 Texture 세팅
	device->SetTexture(0, _particleTex);

	//파티클 정점 출력
	device->SetFVF(quadParticle_VERTEX::FVF);
	device->DrawIndexedPrimitiveUP(
		D3DPT_TRIANGLELIST,
		0,
		drawParticleNum * 4,			//정점수
		drawParticleNum * 2,			//삼각형 수
		_particleIndex,
		D3DFMT_INDEX32,
		_particleVerticles,
		sizeof(quadParticle_VERTEX));

	//파티클 그리고 난후 후처리
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
	this->_cameraTransform = pCameraTrans;		//카메라 소트 기억
	this->_isCameraSort = bSort;
}

void quadParticleEmitter::startOneParticle()
{
	//라이브 타임 랜덤
	float liveTime = randomFloatRange(_startLiveTimeMin, _startLiveTimeMax);

	D3DXVECTOR3 position = _transform->getWorldPosition();

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

	//벡터 랜덤
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

	//움직임의 회전량 랜덤
	D3DXVECTOR3 rotate;
	rotate.x = randomFloatRange(_startRotateMin.x, _startRotateMax.x);
	rotate.y = randomFloatRange(_startRotateMin.y, _startRotateMax.y);
	rotate.z = randomFloatRange(_startRotateMin.z, _startRotateMax.z);

	//움직임의 회전 가속량 랜덤
	D3DXVECTOR3 rotateAccel;
	rotateAccel.x = randomFloatRange(_startRotateAccelateMin.x, _startRotateAccelateMax.x);
	rotateAccel.y = randomFloatRange(_startRotateAccelateMin.y, _startRotateAccelateMax.y);
	rotateAccel.z = randomFloatRange(_startRotateAccelateMin.z, _startRotateAccelateMax.z);

	//자신의 월드 매트릭스를 가지고 온다.
	if (_isLocal == false)
	{
		D3DXMATRIXA16 matWorld = this->_transform->getFinalMatrix();
		D3DXVec3TransformNormal(&velocity, &velocity, &matWorld);
		D3DXVec3TransformNormal(&accelation, &accelation, &matWorld);
	}

	//스케일도 랜덤
	float scale = randomFloatRange(_startScaleMin, _startScaleMax);

	_paticles[_particleCount]._transform.setRotateLocal(setRotate.x, setRotate.y, setRotate.z);

	//순번대로 발생시킨다
	_paticles[_particleCount].Start(
		liveTime,			//라이브 타입
		&position,			//위치
		&velocity,			//벨로 시티
		&accelation,		//가속
		&rotate,			//회전량
		&rotateAccel,		//회전 가속
		scale				//스케일
	);

	//다음 파티클을 위한 순번 증가
	_particleCount++;
	if (_particleCount == this->_paticleNum)
	{
		_particleCount = 0;
	}
}