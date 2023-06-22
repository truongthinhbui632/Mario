#pragma once
#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#define ID_ANI_MISC 70000
#define	WIDTH 16
#define BBOX_WIDTH 16
#define BBOX_HEIGHT 16
class CMiscObject : public CGameObject{
public:
	CMiscObject(float x, float y) :CGameObject(x, y) {
	}
	int IsBlocking() { return 0; };
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};
