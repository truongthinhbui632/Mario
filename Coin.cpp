#include "Coin.h"

void CCoin::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = -1;

	if (state == COIN_INVISIBLE)
		aniId = ID_ANI_COIN_INVISIBLE;
	else if (state == COIN_APPEAR)
		aniId = ID_ANI_COIN_APPEAR;
	else if (state == COIN_NORMAL)
		aniId = ID_ANI_COIN;

	animations->Get(aniId)->Render(x, y);
	RenderBoundingBox();
}
void CCoin::OnNoCollision(DWORD dt)
{
	y += vy * dt;
}

void CCoin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if ((state == COIN_APPEAR) && (GetTickCount64() - coin_delete > COIN_TIMEOUT))
	{
		isDeleted = true;
		return;
	}
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CCoin::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - COIN_BBOX_WIDTH / 2;
	t = y - COIN_BBOX_HEIGHT / 2;
	r = l + COIN_BBOX_WIDTH - 1;
	b = t + COIN_BBOX_HEIGHT - 1;
}

void CCoin::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case COIN_INVISIBLE:
		vy = 0;
		break;
	case COIN_NORMAL:
		vy = 0;
		break;
	case COIN_APPEAR:
		vy = -COIN_DEFLECT_SPEED;
		coin_delete = GetTickCount64();
		break;
	}
}