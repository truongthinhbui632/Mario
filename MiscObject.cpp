#include "MiscObject.h"

#include "Sprite.h"
#include "Sprites.h"

#include "Textures.h"

void CMiscObject::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_MISC)->Render(x, y);
	//RenderBoundingBox();
}

void CMiscObject::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - BBOX_WIDTH / 2;
	t = y - BBOX_HEIGHT / 2;
	r = l + BBOX_WIDTH;
	b = t + BBOX_HEIGHT;
}
