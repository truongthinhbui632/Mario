#pragma once
#include "GameObject.h"

#define GOOMBA_GRAVITY 0.00002f
#define GOOMBA_WALKING_SPEED 0.05f


#define GOOMBA_BBOX_WIDTH 16
#define GOOMBA_BBOX_HEIGHT 32

#define ID_ANI_GOOMBA_WALKING 6000

class CPiranhaplant : public CGameObject
{
protected:
	float ax;
	float ay;
	int maxheight = 32;
	ULONGLONG die_start;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual void OnNoCollision(DWORD dt);
	//virtual int isNoBypass() { return 0; };
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	//virtual void OnCollisionWithPlatform(LPCOLLISIONEVENT e);

public:
	CPiranhaplant(float x, float y);
	virtual void SetState(int state);
};