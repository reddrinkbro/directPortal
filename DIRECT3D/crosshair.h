#pragma once
enum CROSSHAIRSTATE
{
	CROSSHAIRSTATE_NONE,
	CROSSHAIRSTATE_ONLYBLUE,
	CROSSHAIRSTATE_ALL
};
class crosshair
{
private:
	LPDIRECT3DTEXTURE9 _blueCrosshair;			//�Ķ� ũ�ν����
	LPDIRECT3DTEXTURE9 _orangeCrosshair;		//��Ȳ ũ�ν����
	LPDIRECT3DTEXTURE9 _centerCrosshair;		//���� ũ�ν����
	CROSSHAIRSTATE	   _state;
public:
	HRESULT init(void);
	void release(void);
	void render(void);
public:
	void setState(CROSSHAIRSTATE state) { _state = state; }
	crosshair() {};
	~crosshair() {};
};

