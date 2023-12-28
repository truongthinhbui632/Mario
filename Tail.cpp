#include "Tail.h"
#include "debug.h"
CTail::CTail(float x, float y) : CGameObject(x, y)
{
	y_start = y;
	this->ay = TAIL_GRAVITY;
	objectType = 11;
	vy = ay;
	isOnPlatform = false;
}

void CTail::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CTail::Respawn(CGameObject* koopa)
{
	if (koopa->GetState() == 100)
	{
		x = x - 20;
	}
	else if (koopa->GetState() == 200)
	{
		x = x + 20;
	}
	vx = -vx;
}
void CTail::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->ny != 0 )
	{
		vy = 0;
		isOnPlatform = true;
	}
}

void CTail::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x ;
	t = y ;
	r = l + TAIL_BBOX_WIDTH ;
	b = t + TAIL_BBOX_HEIGHT ;
}

void CTail::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy = ay * dt;
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CTail::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_TAIL)->Render(x, y);

	//RenderBoundingBox();
}


