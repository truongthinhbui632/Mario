#pragma once
#include "Brick.h"
#define ID_ANI_QUESTIONBRICK 10001
class CQuestionBrick : public CBrick {
public:
	CQuestionBrick(float x, float y) : CBrick(x, y) {}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};