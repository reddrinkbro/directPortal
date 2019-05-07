#pragma once

class bluePortal : public baseObject
{
private:
	LPDIRECT3DTEXTURE9 _bluePortal;			//파랑포탈 텍스쳐
	LPD3DXEFFECT		_effect;			//쉐이더 이펙트
	transform			_playerPos;			//포탈 이동시 플레이어가 있을 위치
	bool				_isSet;				//맵안에 포탈이 세팅되어있는지
public:
	HRESULT init(void);
	void release(void);
	void render(void);
public:
	transform	getPlayerPos(void) { return _playerPos; }
	bool getIsSet(void) { return _isSet; }
	void setIsSet(bool isSet) { _isSet = isSet; }
	bluePortal() {};
	~bluePortal() {};
};

class orangePortal : public baseObject
{
private:
	LPDIRECT3DTEXTURE9  _orangePortal;		//주황포탈 텍스쳐
	LPD3DXEFFECT		_effect;			//쉐이더 이펙트
	transform			_playerPos;			//포탈 이동시 플레이어가 있을 위치
	bool				_isSet;				//맵안에 포탈이 세팅되어있는지
public:
	HRESULT init(void);
	void release(void);
	void render(void);
public:
	transform getPlayerPos(void) { return _playerPos; }
	bool getIsSet(void) { return _isSet; }
	void setIsSet(bool isSet) { _isSet = isSet; }
	orangePortal() {};
	~orangePortal() {};
};
