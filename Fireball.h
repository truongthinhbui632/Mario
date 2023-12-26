#pragma once
#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define PIRANHAPLANT_FIREBALL_SPEED 0.05f
#define FIREBALL_GRAVITY 0.002f
#define ID_ANI_FIREBALL 8000

#define	COIN_WIDTH 9

#define COIN_BBOX_WIDTH 9
#define COIN_BBOX_HEIGHT 9

#define FIREBALL_TIMEOUT 8000
class CFireball : public CGameObject {
public:
	float ax;
	float ay;

	ULONGLONG start;

	CFireball(float x, float y);

	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
};