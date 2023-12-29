#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define COIN_DEFLECT_SPEED  0.07f

#define COIN_INVISIBLE 10
#define COIN_APPEAR 20
#define COIN_NORMAL 30

#define ID_ANI_COIN 11000
#define ID_ANI_COIN_INVISIBLE 11001
#define ID_ANI_COIN_APPEAR 11002

#define	COIN_WIDTH 10
#define COIN_BBOX_WIDTH 10
#define COIN_BBOX_HEIGHT 16
#define COIN_TIMEOUT 500

class CCoin : public CGameObject {
public:
	float ay;
	ULONGLONG coin_delete;
	CCoin(float x, float y) : CGameObject(x, y) {
		ay = 0;
		state = COIN_NORMAL;
		coin_delete = -1;
		objectType = 4;
	}
	virtual int IsCollidable() { return 1; };
	virtual void OnNoCollision(DWORD dt);
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void SetState(int state);
	int IsBlocking() { return 0; }
};