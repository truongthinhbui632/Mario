#pragma once
#include "GameObject.h"

#define KOOPA_GRAVITY 0.002f
#define KOOPA_WALKING_SPEED 0.05f


#define KOOPA_BBOX_WIDTH 16
#define KOOPA_BBOX_HEIGHT 26
#define KOOPA_BBOX_HEIGHT_DIE 16

#define KOOPA_DIE_TIMEOUT 50000

#define KOOPA_STATE_WALKING_LEFT 100
#define KOOPA_STATE_WALKING_RIGHT 200
#define KOOPA_STATE_DIE 300

#define ID_ANI_KOOPA_WALKING_LEFT 7000
#define ID_ANI_KOOPA_WALKING_RIGHT 7001
#define ID_ANI_KOOPA_DIE 7002

class CKoopa : public CGameObject
{
protected:
	float ax;
	float ay;

	ULONGLONG die_start;

	int shell;
	int revive;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return shell; }

	virtual void ChangeDirection();

	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
	CKoopa(float x, float y);
	virtual void SetState(int state);
};