#include "Piranhaplant.h"
#include "debug.h"
#include "PanelPlatform.h"

CPiranhaplant::CPiranhaplant(float x, float y) :CGameObject(x, y)
{
	this->ax = 0;
	this->ay = PPLANT_GRAVITY;
	state = PPLANT_RIGHT;
	//rect để check player 
	boundary.left = x - PPLANT_BBOX_WIDTH * 5.0f;
	boundary.top = y - PPLANT_BBOX_HEIGHT * 4.0;
	boundary.right = x + PPLANT_BBOX_WIDTH * 5.0f;
	boundary.bottom = y + PPLANT_BBOX_HEIGHT * 4.0f;

	_isPlayerInRange = false;
	objectType = 9;
}

CFireball* CPiranhaplant::CreateFireball(float x, float y)
{
	CFireball* obj = new CFireball(x, y);
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
		DebugOut(L">>> di xuong : %f >>> \n", y);
	}
}

void CPiranhaplant::ComparePlayerPosToSelf(CMario* mario)
{
	if (mario->GetX() >= boundary.left &&
		mario->GetY() >= boundary.top &&
		mario->GetX()<= boundary.right &&
		mario->GetY() <= boundary.bottom)
	{
		_isPlayerInRange = true;
	}
	else {
		_isPlayerInRange = false;
	}
}

void CPiranhaplant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (y > 145)
	{
		//DebugOut(L">>> Y luc doi chieu : %f >>> \n", y);
		vy += ay * dt;
	}
	else
	{
		//DebugOut(L">>> ban o vi tri : %f >>> \n", y);
		vy -= ay * dt;
	}

	//for (UINT i = 0; i < coObjects->size(); i++)
	//{
		//if(coObjects->at(i)==)
	//}
	//CMario* mario = dynamic_cast<CMario*>(e->obj);
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
}
