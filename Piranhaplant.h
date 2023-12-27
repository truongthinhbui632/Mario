#pragma once
#include "GameObject.h"
#include "Mario.h"
#include "Fireball.h"
#include "PlayScene.h"

#define PPLANT_GRAVITY 0.00002f
#define PPLANT_SPEED 0.05f


#define PPLANT_BBOX_WIDTH 16
#define PPLANT_BBOX_HEIGHT 32

#define PPLANT_IDLE 0
#define PPLANT_RIGHT 10
#define PPLANT_LEFT 20
#define ID_ANI_PPLANT_RIGHT 6001
#define ID_ANI_PPLANT_LEFT 6002
#define TIME_FIRE 2000

class CPiranhaplant : public CGameObject
{
protected:
	float ay;
	int y_start;

	RECT boundary;
	//bool isfired;
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
	ULONGLONG cooldown_start;
	ULONGLONG cooldown_time;

	bool isPlayerInRange;
	bool IsOnCoolDown();
	void StartCoolDown();
	bool firstshot;

	CPiranhaplant(float x, float y);

	CFireball* CreateFireball(float x, float y);

	virtual void SetState(int state);
	virtual void ComparePlayerPosToSelf(CMario* mario);
};