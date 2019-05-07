#pragma once

enum COLLISIONTYPE
{
	COLLISIONTYPE_NONE,
	COLLISIONTYPE_BUTTON,
	COLLISIONTYPE_WALL,
	COLLISIONTYPE_PLAYER
};
class cube : public baseObject
{
private:
	boundBox		_exclusivePlayer;
	bool			_isButtonCollide;			//��ư�� �浹������
	bool			_isWallCollide;				//���� �浹������
	float		    _gravity;					//�߷°�
	COLLISIONTYPE	_collisionType;
public:
	HRESULT init(float x, float y , float z);
	void release(void);
	void update(void);
	void render(void);
//Getter Setter
public:
	boundBox getPlayerBoundBox(void) { return _exclusivePlayer; }
	COLLISIONTYPE getCollisionType(void) { return _collisionType; }
	void setCollisionType(COLLISIONTYPE collisionType) { _collisionType = collisionType; }
/*	bool getIsButtonCollide(void) { return _isButtonCollide; }
	void setIsButtonCollide(bool isButtonCollide) { _isButtonCollide = isButtonCollide; }
	bool getIsWallCollide(void) { return _isWallCollide; }
	void setIsWallCollide(bool isWallCollide) { _isWallCollide = isWallCollide; }*/
	float getGravity(void) { return _gravity; }
	void setGravity(float gravity) { _gravity = gravity; }
	cube() {};
	~cube() {};
};

