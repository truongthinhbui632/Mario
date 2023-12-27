#include "Piranhaplant.h"
#include "debug.h"
#include "PanelPlatform.h"

CPiranhaplant::CPiranhaplant(float x, float y) :CGameObject(x, y)
{
	ay = PPLANT_GRAVITY;
	y_start = y;
	state = PPLANT_RIGHT;
	//rect để check player 
	boundary.left = x - PPLANT_BBOX_WIDTH * 5.0f;
	boundary.top = y - PPLANT_BBOX_HEIGHT * 4.0;
	boundary.right = x + PPLANT_BBOX_WIDTH * 5.0f;
	boundary.bottom = y + PPLANT_BBOX_HEIGHT * 4.0f;

	cooldown_start = 10;
	cooldown_time = 500;
	firstshot = 1;
	isPlayerInRange = false;
	objectType = 9;
}

CFireball* CPiranhaplant::CreateFireball(float x, float y)
{
	CFireball* obj = new CFireball(x, y);
	if (state == PPLANT_LEFT)
	{
		obj->SetState(FIREBALL_RIGHT);
	}
	else
	{
		obj->SetState(FIREBALL_LEFT);
	}
	return obj;
}

void CPiranhaplant::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - PPLANT_BBOX_WIDTH/2;
	top = y - PPLANT_BBOX_HEIGHT/2;
	right = left + PPLANT_BBOX_WIDTH;
	bottom = top + PPLANT_BBOX_HEIGHT;
}

void CPiranhaplant::OnNoCollision(DWORD dt)
{
	y += vy * dt;
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
		DebugOut(L">>> di xuong : %f >>> \n", y);
	}
}

bool CPiranhaplant::IsOnCoolDown()
{
	return cooldown_start != 0;
}

void CPiranhaplant::StartCoolDown() {
	cooldown_start = GetTickCount64();
}

void CPiranhaplant::ComparePlayerPosToSelf(CMario* mario)
{
	if (mario->GetX() >= boundary.left &&
		mario->GetY() >= boundary.top &&
		mario->GetX()<= boundary.right &&
		mario->GetY() <= boundary.bottom)
	{
		isPlayerInRange = true;
	}
	else {
		isPlayerInRange = false;
	}
}

void CPiranhaplant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (y > 145)
	{
		vy -= ay * dt;
	}
	else
	{
		vy += ay * dt;
	}
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CPiranhaplant::Render()
{
	int aniId = ID_ANI_PPLANT_RIGHT;

	if (state == PPLANT_RIGHT)
		aniId = ID_ANI_PPLANT_RIGHT;
	else if (state == PPLANT_LEFT)
		aniId = ID_ANI_PPLANT_LEFT;

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	RenderBoundingBox();
}

void CPiranhaplant::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
		case PPLANT_LEFT:
			ay = PPLANT_GRAVITY;
			break;
		case PPLANT_RIGHT:
			ay = PPLANT_GRAVITY;
			break;
		case PPLANT_IDLE:
			break;
	}
}
