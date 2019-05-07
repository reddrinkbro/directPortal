#pragma once

class bluePortal : public baseObject
{
private:
	LPDIRECT3DTEXTURE9 _bluePortal;			//�Ķ���Ż �ؽ���
	LPD3DXEFFECT		_effect;			//���̴� ����Ʈ
	transform			_playerPos;			//��Ż �̵��� �÷��̾ ���� ��ġ
	bool				_isSet;				//�ʾȿ� ��Ż�� ���õǾ��ִ���
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
	LPDIRECT3DTEXTURE9  _orangePortal;		//��Ȳ��Ż �ؽ���
	LPD3DXEFFECT		_effect;			//���̴� ����Ʈ
	transform			_playerPos;			//��Ż �̵��� �÷��̾ ���� ��ġ
	bool				_isSet;				//�ʾȿ� ��Ż�� ���õǾ��ִ���
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
