#include "Mushroom.h"

void CMushroom::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = -1;

	if (state == MUSHROOM_INVISIBLE)
		aniId = ID_ANI_MUSHROOM_INVISIBLE;
	else if (state == MUSHROOM_APPEAR_RIGHT || state == MUSHROOM_APPEAR_LEFT)
		aniId = ID_ANI_MUSHROOM;

	animations->Get(aniId)->Render(x, y);
	RenderBoundingBox();
}

void CMushroom::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CMushroom::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

void CMushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vx += ax * dt;
	vy += ay * dt;
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CMushroom::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case MUSHROOM_INVISIBLE:
		vx = 0;
		vy = 0;
		break;
	case MUSHROOM_APPEAR_RIGHT:
		vx = MUSHROOM_MOVE_SPEED;
		y -= 2;
		ay = MUSHROOM_GRAVITY;
		break;
	case MUSHROOM_APPEAR_LEFT:
		vx = -MUSHROOM_MOVE_SPEED;
		y -= 2;
		ay = MUSHROOM_GRAVITY;
		break;
	}
}

void CMushroom::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - MUSHROOM_BBOX_WIDTH / 2;
	t = y - MUSHROOM_BBOX_HEIGHT / 2;
	r = l + MUSHROOM_BBOX_WIDTH - 1;
	b = t + MUSHROOM_BBOX_HEIGHT - 1;
}