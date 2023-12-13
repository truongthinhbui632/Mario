#pragma once
#include "Platform.h";

class CPanelPlatform : public CPlatform
{
protected:
	int length;				// Unit: cell 
	float cellWidth;
	float cellHeight;
	int spriteIdBegin, spriteIdMiddle, spriteIdEnd;
public:
	CPanelPlatform(float x, float y,
		float cell_width, float cell_height, int length,
		int sprite_id_begin, int sprite_id_middle, int sprite_id_end) :CPlatform(x, y, cell_width, 
			cell_height, length, sprite_id_begin, sprite_id_middle, sprite_id_end)
	{
		this->length = length;
		this->cellWidth = cell_width;
		this->cellHeight = cell_height;
		this->spriteIdBegin = sprite_id_begin;
		this->spriteIdMiddle = sprite_id_middle;
		this->spriteIdEnd = sprite_id_end;
	}

	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsDirectionColliable(float nx, float ny);
	int isNoBypass() { return 0; };
	int isBypass(LPGAMEOBJECT src_obj);
	void RenderBoundingBox();
};
