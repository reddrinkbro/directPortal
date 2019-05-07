#pragma once
#include "gameNode.h"
#include "tutorialScene_1.h"
#include "tutorialScene_2.h"
#include "stage1Scene.h"
#include "stage2Scene.h"
#include "stage3Scene.h"
#include "mainScene.h"
#include "endingScene.h"
class mainGame : public gameNode
{
public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);

	mainGame() {}
	~mainGame() {}
};