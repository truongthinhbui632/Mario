#pragma once
#include "Brick.h"
#define ID_ANI_QUESTIONBRICK 10001
#define ID_ANI_QUESTIONBRICKBREAK 10002
class CQuestionBrick : public CBrick {
public:
	CQuestionBrick(float x, float y) : CBrick(x, y) { 
		state = ID_ANI_QUESTIONBRICK;
	}
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void SetState(int state);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};