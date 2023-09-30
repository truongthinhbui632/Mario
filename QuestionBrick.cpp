#include "QuestionBrick.h"

void CQuestionBrick::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = -1;
	if (state == ID_ANI_QUESTIONBRICK)
		aniId = ID_ANI_QUESTIONBRICK;
	else if (state == ID_ANI_QUESTIONBRICKBREAK)
		aniId = ID_ANI_QUESTIONBRICKBREAK;
	animations->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}

void CQuestionBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}
void CQuestionBrick::SetState(int state)
{
	CGameObject::SetState(state);
}

void CQuestionBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - BRICK_BBOX_WIDTH / 2;
	t = y - BRICK_BBOX_HEIGHT / 2;
	r = l + BRICK_BBOX_WIDTH;
	b = t + BRICK_BBOX_HEIGHT;
}