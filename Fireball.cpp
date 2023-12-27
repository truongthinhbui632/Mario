#include "Fireball.h"

CFireball::CFireball(float x, float y) : CGameObject(x, y) 
{
	state = FIREBALL_LEFT;
	vx = PIRANHAPLANT_FIREBALL_SPEED;
	ay = FIREBALL_GRAVITY;
	objectType = 10;
	start = GetTickCount64();
}
void CFireball::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_FIREBALL)->Render(x, y);

	//RenderBoundingBox();
}

void CFireball::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CFireball::OnCollisionWith(LPCOLLISIONEVENT e)
{

}

void CFireball::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	if(GetTickCount64() - start > FIREBALL_TIMEOUT)
	{
		isDeleted = true;
		return;
	}
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CFireball::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - COIN_BBOX_WIDTH / 2;
	t = y - COIN_BBOX_HEIGHT / 2;
	r = l + COIN_BBOX_WIDTH;
	b = t + COIN_BBOX_HEIGHT;
}

void CFireball::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case FIREBALL_LEFT:
		vx = -vx;
		break;
	case FIREBALL_RIGHT:
		break;
	}

}