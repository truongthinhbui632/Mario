#pragma once
#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#define ID_ANI_TAIL 90001

#define	TAIL_WIDTH 4
#define TAIL_BBOX_WIDTH 4
#define TAIL_BBOX_HEIGHT 4
class CTail : public CGameObject {
public:
	bool isOnPlatform;

	CTail(float x, float y);

	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
};