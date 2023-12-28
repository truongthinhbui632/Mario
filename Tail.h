#pragma once
#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "Platform.h"
#include "PanelPlatform.h"

#define ID_ANI_TAIL 90001
#define TAIL_GRAVITY 0.002f

#define	TAIL_WIDTH 4
#define TAIL_BBOX_WIDTH 4
#define TAIL_BBOX_HEIGHT 4
class CTail : public CGameObject {
public:
	bool isOnPlatform;
	float y_start;
	float ay;

	CTail(float x, float y);

	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

	virtual int GetVy() { return this->vy; };
	virtual void SetVx(float x) {  this->vx = x; };

	virtual void Respawn(CGameObject* koopa);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};