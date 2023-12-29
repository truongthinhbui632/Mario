#pragma once
#include "Brick.h"

#define QUESTIONBRICK_ON 10
#define QUESTIONBRICK_OFF 20
#define ID_ANI_QUESTIONBRICK 10001
#define ID_ANI_QUESTIONBRICKBREAK 10002

class CQuestionBrick : public CBrick {
public:
	int direction;
	CQuestionBrick(float x, float y) : CBrick(x, y) { 
		state = QUESTIONBRICK_ON;
		direction = 1;
		objectType = 8;
	}
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	void SetState(int state);
	void SetDirection(int direction) { this->direction = direction; };
	int GetDirection() { return this->direction; };
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};