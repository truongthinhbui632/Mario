#pragma once
#include "GameObject.h"
#include "Tail.h"

#define KOOPA_GRAVITY 0.002f
#define KOOPA_SPINNING 0.02f
#define KOOPA_WALKING_SPEED 0.02f
#define KOOPA_SPINNING_SPEED 0.1f

#define KOOPA_BBOX_WIDTH 16
#define KOOPA_BBOX_HEIGHT 26
#define KOOPA_BBOX_HEIGHT_DIE 16

#define KOOPA_DIE_TIMEOUT 5000

#define KOOPA_STATE_WALKING_LEFT 100
#define KOOPA_STATE_WALKING_RIGHT 200
#define KOOPA_STATE_DIE 300
#define KOOPA_STATE_SPIN 400

#define ID_ANI_KOOPA_WALKING_LEFT 7000
#define ID_ANI_KOOPA_WALKING_RIGHT 7001
#define ID_ANI_KOOPA_DIE 7002
#define ID_ANI_KOOPA_REVIVE 7003
#define ID_ANI_KOOPA_SPIN 7004

class CKoopa : public CGameObject
{
protected:
	//CTail* tail;
	float ax;
	float ay;

	ULONGLONG shell_start;
	int shell;
	int direction;
	int revive;
	int state_revive;
	int y_save;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return shell; };

	virtual void ChangeDirection();

	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	virtual void OnCollisionWithGoomba(LPCOLLISIONEVENT e);

public:
	CKoopa(float x, float y);
	virtual void SetState(int state);
	virtual void SetIsDeleted(bool xoa);
	virtual void SetStateBeforeShell(int state_revive, int y_save);
	virtual void setDirection(int direction) { this->direction = direction; };

	virtual int getY() { return y; };
};