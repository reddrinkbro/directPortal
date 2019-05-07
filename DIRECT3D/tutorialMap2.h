#pragma once

class tutorialMap2
{
private:
	baseObject *		 _object;
	lightDirection*		 _light;
	vector<mapBoundBox> _collision;							//맵 바운드 박스
	vector<mapBoundBox> _basicBox;							//맵에 있는 작은 육각형박스 충돌, 충돌시 포탈이 가운데 위치하기 위함
public:
	HRESULT init(void);
	void release(void);
	void render(void);
	void setBoundBox(void);
public:
	vector<mapBoundBox> getBoundBox(void) { return _collision; }
	vector<mapBoundBox> getbasicBox(void) { return _basicBox; }
	baseObject * getObject(void) { return _object; }
	tutorialMap2() {};
	~tutorialMap2() {};
};

