#pragma once
#include "GameObject.h"

#define MUSHROOM_GRAVITY 0.002f
#define MUSHROOM_MOVE_SPEED 0.05f

#define MUSHROOM_INVISIBLE 10
#define MUSHROOM_APPEAR_RIGHT 20
#define MUSHROOM_APPEAR_LEFT 30

#define ID_ANI_MUSHROOM_INVISIBLE 100000
#define ID_ANI_MUSHROOM 100001

#define MUSHROOM_BBOX_WIDTH 16
#define MUSHROOM_BBOX_HEIGHT 16

class CMushroom : public CGameObject {
public:
	float ax;
	float ay;
	int collidable;

	CMushroom(float x, float y) : CGameObject(x, y) {
		state = MUSHROOM_INVISIBLE;
		ax = 0;
		ay = 0;
		collidable = 1;
		objectType = 12;
	}
	virtual int IsCollidable() { return 1; };
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	
	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);

	int IsBlocking() { return 0; }

	void SetState(int state);
	void SetIsCollidable(int collidable) { this->collidable = collidable; };
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};