#include "MiscObject.h"

#include "Sprite.h"
#include "Sprites.h"

#include "Textures.h"

void CMiscObject::Render()
{
	CSprites* misc = CSprites::GetInstance();
	misc->Get(this->id_misc)->Draw(x, y);
}

void CMiscObject::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = 0;
	t = 0;
	r = 0;
	b =	0;
}
