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
	LPDIRECT3DTEXTURE9 _blueCrosshair;			//파랑 크로스헤어
	LPDIRECT3DTEXTURE9 _orangeCrosshair;		//주황 크로스헤어
	LPDIRECT3DTEXTURE9 _centerCrosshair;		//센터 크로스헤어
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

