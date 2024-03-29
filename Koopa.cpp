#include "Koopa.h"
#include "Goomba.h"
#include "Mario.h"
#include "QuestionBrick.h"
#include "debug.h"

CKoopa::CKoopa(float x, float y) :CGameObject(x, y)
{
	this->ax = 0;
	this->ay = KOOPA_GRAVITY;

	shell_start = -1;
	SetState(KOOPA_STATE_WALKING_LEFT);
	shell = 0;
	direction = 1;
	revive = 0;
	state_revive = KOOPA_STATE_WALKING_LEFT;
	y_save = y;
	objectType = 3;
}

void CKoopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == KOOPA_STATE_DIE || state == KOOPA_STATE_SPIN)
	{
		left = x - KOOPA_BBOX_WIDTH / 2;
		top = y - KOOPA_BBOX_HEIGHT_DIE / 2;
		//top = y + (KOOPA_BBOX_HEIGHT - KOOPA_BBOX_HEIGHT_DIE) / 2 - KOOPA_BBOX_HEIGHT_DIE / 2;
		right = left + KOOPA_BBOX_WIDTH;
		bottom = top + KOOPA_BBOX_HEIGHT_DIE;
	}
	else
	{
		left = x - KOOPA_BBOX_WIDTH / 2;
		top = y - KOOPA_BBOX_HEIGHT / 2;
		right = left + KOOPA_BBOX_WIDTH-1;
		bottom = top + KOOPA_BBOX_HEIGHT-1;
	}
}

void CKoopa::OnNoCollision(DWORD dt)
{
	if (vy == 0)
	{
		x += vx * dt;
	}
	else 
	{
		if (state == KOOPA_STATE_WALKING_LEFT || state == KOOPA_STATE_WALKING_RIGHT)
		{
			ChangeDirection();
		}
		else
		{
			x += vx * dt; y += vy * dt;
		}
	}
};

void CKoopa::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;

	if (dynamic_cast<CKoopa*>(e->obj)) return;
	if (dynamic_cast<CGoomba*>(e->obj))
	{
		OnCollisionWithGoomba(e);
	};
	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		if (state != KOOPA_STATE_SPIN)
		{
			ChangeDirection();
		}
		else
		{
			if (dynamic_cast<CQuestionBrick*>(e->obj))
				OnCollisionWithQuestionBrick(e);
			vx = -vx;
		}
	}
}

void CKoopa::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
	if (state == KOOPA_STATE_SPIN)
	{
		if (goomba->GetState() != GOOMBA_STATE_DIE)
		{
			goomba->SetState(GOOMBA_STATE_DIE);
		}
	}
}

void CKoopa::OnCollisionWithQuestionBrick(LPCOLLISIONEVENT e)
{
	CQuestionBrick* qbrick = dynamic_cast<CQuestionBrick*>(e->obj);
	int idvc = qbrick->GetState();
	if (e->nx != 0)
	{
		DebugOut(L">>> Va cham gach >>> \n");
		DebugOut(L">>> state: %d >>> \n", idvc);
		if (idvc == QUESTIONBRICK_ON)
		{
			qbrick->SetState(QUESTIONBRICK_OFF);
			idvc = qbrick->GetState();
			DebugOut(L">>> state sau va cham : %d >>> \n", idvc);
		}
		if (nx > 0)
		{
			int direction = -1;
			qbrick->SetDirection(direction);
		}
	}
}
void CKoopa::ChangeDirection() 
{
	if (state == KOOPA_STATE_WALKING_LEFT)
	{
		SetState(KOOPA_STATE_WALKING_RIGHT);
		//DebugOut(L">>> di phai >>> \n");
	}
	else if (state == KOOPA_STATE_WALKING_RIGHT)
	{
		SetState(KOOPA_STATE_WALKING_LEFT);
		//DebugOut(L">>> di trai >>> \n");
	}
}

void CKoopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	/*if (state != KOOPA_STATE_DIE)
	{
		vy += ay * dt;
		vx += ax * dt;
		CGameObject::Update(dt, coObjects);
		CCollision::GetInstance()->Process(this, dt, coObjects);
	}*/
	if (state != KOOPA_STATE_DIE)
	{
		vy += ay * dt;
		vx += ax * dt;
		CGameObject::Update(dt, coObjects);
		CCollision::GetInstance()->Process(this, dt, coObjects);
	}
	if ((state == KOOPA_STATE_DIE) && (GetTickCount64() - shell_start > KOOPA_DIE_TIMEOUT-1000))
	{
		revive = 1;
	}
	if ((state == KOOPA_STATE_DIE) && (GetTickCount64() - shell_start > KOOPA_DIE_TIMEOUT))
	{
		y = y_save;
		SetState(state_revive);
		revive = 0;
		return;
	}

}


void CKoopa::Render()
{
	int aniId = ID_ANI_KOOPA_WALKING_LEFT;

	if (state == KOOPA_STATE_WALKING_LEFT)
	{
		aniId = ID_ANI_KOOPA_WALKING_LEFT;
	}

	else if (state == KOOPA_STATE_WALKING_RIGHT)
	{
		aniId = ID_ANI_KOOPA_WALKING_RIGHT;
	}

	else if (state == KOOPA_STATE_SPIN)
	{
		aniId = ID_ANI_KOOPA_SPIN;
	}

	else if (state == KOOPA_STATE_DIE)
	{
		if (revive == 1)
			aniId = ID_ANI_KOOPA_REVIVE;
		else
			aniId = ID_ANI_KOOPA_DIE;
	}

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	RenderBoundingBox();
}

void CKoopa::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPA_STATE_DIE:
		shell_start = GetTickCount64();
		y += (KOOPA_BBOX_HEIGHT - KOOPA_BBOX_HEIGHT_DIE) / 2;
		shell = 1;
		break;
	case KOOPA_STATE_WALKING_LEFT:
		vx = -KOOPA_WALKING_SPEED;
		break;
	case KOOPA_STATE_WALKING_RIGHT:
		vx = KOOPA_WALKING_SPEED;
		break;
	case KOOPA_STATE_SPIN:
		vx = KOOPA_SPINNING_SPEED*direction;
		y += (KOOPA_BBOX_HEIGHT - KOOPA_BBOX_HEIGHT_DIE) / 2 - 7;
		//ax = KOOPA_SPINNING;
		break;
	}

}

void CKoopa::SetIsDeleted(bool xoa) 
{
	isDeleted = xoa;
}

void CKoopa::SetStateBeforeShell(int state_revive, int y_save)
{
	this->state_revive = state_revive;
	this->y_save = y_save;
}

CTail* CKoopa::CreateTail(float x, float y) 
{
	CTail* tail= NULL;
	if (this->GetState() == KOOPA_STATE_WALKING_LEFT)
	{
		tail = new CTail(x - 5, y+14);
		tail->SetVx(-KOOPA_WALKING_SPEED);
	}
	else if (this->GetState() == KOOPA_STATE_WALKING_RIGHT)
	{
		tail = new CTail(x + 21, y+14);
		tail->SetVx(KOOPA_WALKING_SPEED);
	}
	return tail;
}