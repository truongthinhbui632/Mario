#pragma once
#include "GameObject.h"

#define MUSHROOM_INVISIBLE 10
#define MUSHROOM_APPEAR 20

#define ID_ANI_MUSHROOM_INVISIBLE 100000
#define ID_ANI_MUSHROOM 100001

#define MUSHROOM_BBOX_WIDTH 16
#define MUSHROOM_BBOX_HEIGHT 16

class CMushroom : public CGameObject {
public:
	CMushroom(float x, float y) : CGameObject(x, y) {
		state = MUSHROOM_INVISIBLE;
		objectType = 12;
	}
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void SetState(int state);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};