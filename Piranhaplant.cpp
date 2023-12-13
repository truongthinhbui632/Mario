#include "Piranhaplant.h"
#include "debug.h"
#include "PanelPlatform.h"

CPiranhaplant::CPiranhaplant(float x, float y) :CGameObject(x, y)
{
	this->ax = 0;
	this->ay = GOOMBA_GRAVITY;
}

void CPiranhaplant::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - GOOMBA_BBOX_WIDTH/2;
	top = y - GOOMBA_BBOX_HEIGHT/2;
	right = left + GOOMBA_BBOX_WIDTH;
	bottom = top + GOOMBA_BBOX_HEIGHT;
}

void CPiranhaplant::OnNoCollision(DWORD dt)
{
	y -= vy * dt;
};

void CPiranhaplant::OnCollisionWith(LPCOLLISIONEVENT e)
{	
	if (dynamic_cast<CPlatform*>(e->obj))
	{
		DebugOut(L">>> va cham voi panel >>> \n");
		vy = -vy;
	}
	if (dynamic_cast<CPanelPlatform*>(e->obj))
	{
		//DebugOut(L">>> va cham voi panel >>> \n");
	}
	if (e->ny <= 0)
	{
		vy = -vy;
	}
}

void CPiranhaplant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (y > 145) vy +=ay*dt;
	else vy -= ay * dt;
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CPiranhaplant::Render()
{
	int aniId = ID_ANI_GOOMBA_WALKING;

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	RenderBoundingBox();
}

void CPiranhaplant::SetState(int state)
{
}
