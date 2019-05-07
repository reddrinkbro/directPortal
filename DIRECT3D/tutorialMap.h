#pragma once

class tutorialMap 
{
private:
	baseObject *		_object;
	lightDirection*		_light;
	vector<mapBoundBox> _collision;							//�� �ٿ�� �ڽ�
	vector<mapBoundBox>	_stairsCollision;					//��ܺκ�
public:
	HRESULT init(void);
	void release(void);
	void render(void);
	void setBoundBox(void);
public:
	vector<mapBoundBox> getBoundBox(void) { return _collision; }
	vector<mapBoundBox> getStairsCollision(void) { return _stairsCollision; }
	baseObject * getObject(void) { return _object; }
	tutorialMap();
	~tutorialMap();
};

