#include "Mushroom.h"

void CMushroom::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = -1;

	if (state == MUSHROOM_INVISIBLE)
		aniId = ID_ANI_MUSHROOM_INVISIBLE;
	else if (state == MUSHROOM_APPEAR)
		aniId = ID_ANI_MUSHROOM;

	animations->Get(aniId)->Render(x, y);
	RenderBoundingBox();
}

void CMushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}
void CMushroom::SetState(int state)
{
	CGameObject::SetState(state);
}

void CMushroom::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - MUSHROOM_BBOX_WIDTH / 2;
	t = y - MUSHROOM_BBOX_HEIGHT / 2;
	r = l + MUSHROOM_BBOX_WIDTH;
	b = t + MUSHROOM_BBOX_HEIGHT;
}