#include <iostream>
#include <fstream>
#include "AssetIDs.h"

#include "PlayScene.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "Coin.h"
#include "Platform.h"
#include "MiscObject.h"
#include "PanelPlatform.h"
#include "QuestionBrick.h"
#include "Piranhaplant.h"
#include "Koopa.h"
#include "Fireball.h"
#include "Mushroom.h"

#include "SampleKeyEventHandler.h"

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath):
	CScene(id, filePath)
{
	player = NULL;
	key_handler = new CSampleKeyHandler(this);
}


#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_ASSETS	1
#define SCENE_SECTION_OBJECTS	2

#define ASSETS_SECTION_UNKNOWN -1
#define ASSETS_SECTION_SPRITES 1
#define ASSETS_SECTION_ANIMATIONS 2

#define MAX_SCENE_LINE 1024

void CPlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPTEXTURE tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return; 
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CPlayScene::_ParseSection_ASSETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return;

	wstring path = ToWSTR(tokens[0]);
	
	LoadAssets(path.c_str());
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i+1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

/*
	Parse a line in section [OBJECTS] 
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	// skip invalid lines - an object set must have at least id, x, y
	if (tokens.size() < 2) return;

	int object_type = atoi(tokens[0].c_str());
	float x = (float)atof(tokens[1].c_str());
	float y = (float)atof(tokens[2].c_str());

	CGameObject *obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_MARIO:
		if (player!=NULL) 
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = new CMario(x,y); 
		player = (CMario*)obj;  

		DebugOut(L"[INFO] Player object has been created!\n");
		break;
	case OBJECT_TYPE_GOOMBA: obj = new CGoomba(x,y); break;
	case OBJECT_TYPE_KOOPA:
	{
		obj = new CKoopa(x, y);
		CKoopa* koopa = dynamic_cast<CKoopa*>(obj);
		CTail* tail = koopa->CreateTail(koopa->GetX(), koopa->GetY());
		objects.push_back(tail);
		break;
	}
	case OBJECT_TYPE_BRICK: obj = new CBrick(x,y); break;
	case OBJECT_TYPE_COIN: obj = new CCoin(x, y); break;
	case OBJECT_TYPE_MISC: {
		int id = atoi(tokens[3].c_str());
		obj = new CMiscObject(x, y,id);
		break;
	}
	case OBJECT_TYPE_PLATFORM:
	{

		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int length = atoi(tokens[5].c_str());
		int sprite_begin = atoi(tokens[6].c_str());
		int sprite_middle = atoi(tokens[7].c_str());
		int sprite_end = atoi(tokens[8].c_str());

		obj = new CPlatform(
			x, y,
			cell_width, cell_height, length,
			sprite_begin, sprite_middle, sprite_end
		);

		break;
	}
	case OBJECT_TYPE_PANELPLATFORM:
	{

		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int length = atoi(tokens[5].c_str());
		int sprite_begin = atoi(tokens[6].c_str());
		int sprite_middle = atoi(tokens[7].c_str());
		int sprite_end = atoi(tokens[8].c_str());

		obj = new CPanelPlatform(
			x, y,
			cell_width, cell_height, length,
			sprite_begin, sprite_middle, sprite_end
		);

		break;
	}
	case OBJECT_TYPE_QUESTIONBRICK: obj = new CQuestionBrick(x, y); break;

	case  OBJECT_TYPE_PIRANHAPLANT: obj = new CPiranhaplant(x, y); break;
	case OBJECT_TYPE_MUSHROOM: obj = new CMushroom(x, y); break;
	case OBJECT_TYPE_PORTAL:
	{
		float r = (float)atof(tokens[3].c_str());
		float b = (float)atof(tokens[4].c_str());
		int scene_id = atoi(tokens[5].c_str());
		obj = new CPortal(x, y, r, b, scene_id);
	}
	break;

	default:
		DebugOut(L"[ERROR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);


	objects.push_back(obj);
}

void CPlayScene::LoadAssets(LPCWSTR assetFile)
{
	DebugOut(L"[INFO] Start loading assets from : %s \n", assetFile);

	ifstream f;
	f.open(assetFile);

	int section = ASSETS_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[SPRITES]") { section = ASSETS_SECTION_SPRITES; continue; };
		if (line == "[ANIMATIONS]") { section = ASSETS_SECTION_ANIMATIONS; continue; };
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case ASSETS_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case ASSETS_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading assets from %s\n", assetFile);
}

void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;					

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	
		if (line == "[ASSETS]") { section = SCENE_SECTION_ASSETS; continue; };
		if (line == "[OBJECTS]") { section = SCENE_SECTION_OBJECTS; continue; };
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }	

		//
		// data section
		//
		switch (section)
		{ 
			case SCENE_SECTION_ASSETS: _ParseSection_ASSETS(line); break;
			case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading scene  %s\n", sceneFilePath);
}

void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	CMario* mario = NULL;

	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		CGameObject* object = objects.at(i);
		if (object->GetObjectType()== OBJECT_TYPE_MARIO)
		{
			mario = dynamic_cast<CMario*>(object);
			break;
		}
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		CGameObject* object = objects.at(i);
		
		//objects[i]->Update(dt, &coObjects);

		switch (object->GetObjectType())
		{
		case OBJECT_TYPE_PIRANHAPLANT:
			{
				// tạo fireball
				CPiranhaplant* pplant = dynamic_cast<CPiranhaplant*>(object);
				pplant->ComparePlayerPosToSelf(mario);
				int x_plant = pplant->GetX();
				int y_plant = pplant->GetY();
				if (pplant->isPlayerInRange == true)
				{
					if (x_plant - mario->GetX() < 0.0f)
					{
						pplant->SetState(PPLANT_LEFT);
						if (y_plant >= 126 && y_plant <= 135)
						{
							if (pplant->firstshot == 1)
							{
								objects.push_back(pplant->CreateFireball(x_plant, y_plant - 2));
								pplant->firstshot = 0;
								pplant->StartCoolDown();
							}
							if (pplant->IsOnCoolDown() && GetTickCount64() - pplant->cooldown_start > TIME_FIRE)
							{
								objects.push_back(pplant->CreateFireball(x_plant, y_plant - 2));
								DebugOut(L">>> tao ra fireball %f >>> \n",y_plant);
								pplant->StartCoolDown();
							}
						}
					}
					else
					{
						pplant->SetState(PPLANT_RIGHT);
						if (y_plant >= 126 && y_plant <= 135)
						{
							if (pplant->firstshot == 1)
							{
								objects.push_back(pplant->CreateFireball(x_plant, y_plant - 2));
								pplant->firstshot = 0;
								pplant->StartCoolDown();
							}
							if (pplant->IsOnCoolDown() && GetTickCount64() - pplant->cooldown_start > TIME_FIRE)
							{
								objects.push_back(pplant->CreateFireball(x_plant, y_plant - 2));
								DebugOut(L">>> tao ra fireball: %f >>> \n", pplant->GetY());
								pplant->StartCoolDown();
							}
						}
					}
				}
				break;
			}

		case OBJECT_TYPE_KOOPA:
			{
				CKoopa* koopa = dynamic_cast<CKoopa*>(object);
				if (koopa->GetState() == KOOPA_STATE_WALKING_LEFT || 
					koopa->GetState() == KOOPA_STATE_WALKING_RIGHT || 
					koopa->GetState()==KOOPA_STATE_DIE)
				{
					//CTail* tail = dynamic_cast<CTail*>(FindObject(OBJECT_TYPE_TAIL));
					//tail = koopa->CreateTail(koopa->GetX(), koopa->GetY());
					//if (tail->GetVy() != 0)
						//{
							//koopa->ChangeDirection();
							//tail->Respawn(koopa);
						//}
					/*
					if (tail == NULL)
					{
						tail = koopa->CreateTail(koopa->GetX(),koopa->GetY());
						objects.push_back(tail);
					}
					else
					{
						if (tail->isOnPlatform == false)
						{
							koopa->ChangeDirection();
							tail->Respawn(koopa);
						}
					}
					*/
				}
				else 
				{
					CTail* tail = dynamic_cast<CTail*>(FindObject(OBJECT_TYPE_TAIL));
					if (tail == NULL)
					{
						break;
					}
					else
					{
						tail->Delete();
					}
				}
				break;
			}

		case OBJECT_TYPE_MUSHROOM:
			{
				CMushroom* mushroom = dynamic_cast<CMushroom*>(object);
				//tìm questionbrick
				for (size_t i = 0; i < objects.size(); i++)
				{
					CGameObject* obj = objects.at(i);
					if (obj->GetObjectType() == 8)
					{
						CQuestionBrick*	qbrick = dynamic_cast<CQuestionBrick*>(obj);
						if (qbrick->GetX() == mushroom->GetX())
						{
							if (qbrick->GetState() == QUESTIONBRICK_OFF)
							{
								if(qbrick->GetDirection()== 1 )
									mushroom->SetState(MUSHROOM_APPEAR_LEFT);
								else if(qbrick->GetDirection() == -1)
									mushroom->SetState(MUSHROOM_APPEAR_RIGHT);
							}
						}
					}
				}
				break;
			}
		}
		object->Update(dt, &coObjects);
	}
	for (size_t i = 0; i < objects.size(); i++)
	{
		CGameObject* object = objects.at(i);
		if (object->GetObjectType() == 11)
		{
			//DebugOut(L">>> tao ra tail ");
		}
	}
	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return; 

	// Update camera to follow mario
	float cx, cy;
	player->GetPosition(cx, cy);

	CGame *game = CGame::GetInstance();
	cx -= game->GetBackBufferWidth() / 2;
	cy -= game->GetBackBufferHeight() / 2;

	if (cx < 0) cx = 0;

	CGame::GetInstance()->SetCamPos(cx, 0.0f /*cy*/);

	PurgeDeletedObjects();
}

void CPlayScene::Render()
{
	for (int i = 0; i < objects.size(); i++)
		objects[i]->Render();
}

/*
*	Clear all objects from this scene
*/
void CPlayScene::Clear()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		delete (*it);
	}
	objects.clear();
}

/*
	Unload scene

	TODO: Beside objects, we need to clean up sprites, animations and textures as well 

*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	player = NULL;

	DebugOut(L"[INFO] Scene %d unloaded! \n", id);
}

bool CPlayScene::IsGameObjectDeleted(const LPGAMEOBJECT& o) { return o == NULL; }

void CPlayScene::PurgeDeletedObjects()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		LPGAMEOBJECT o = *it;
		if (o->IsDeleted())
		{
			delete o;
			*it = NULL;
		}
	}

	// NOTE: remove_if will swap all deleted items to the end of the vector
	// then simply trim the vector, this is much more efficient than deleting individual items
	objects.erase(
		std::remove_if(objects.begin(), objects.end(), CPlayScene::IsGameObjectDeleted),
		objects.end());
}

CGameObject* CPlayScene::FindObject(int object_type)
{
	for (size_t i = 0; i < objects.size(); i++)
	{
		CGameObject* object = objects.at(i);
		if (object->GetObjectType() == object_type)
		{
			return object;
		}
	}
	return NULL;
}