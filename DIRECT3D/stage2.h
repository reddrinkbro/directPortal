#pragma once
class stage2
{
private:
	baseObject *		_object;
	lightDirection*		_light;
	vector<mapBoundBox> _collision;							//맵 바운드 박스
public:
	HRESULT init(void);
	void release(void);
	void render(void);
	void setBoundBox(void);
public:
	vector<mapBoundBox> getBoundBox(void) { return _collision; }
	baseObject * getObject(void) { return _object; }
	stage2() {};
	~stage2() {};
};

