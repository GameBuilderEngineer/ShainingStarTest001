//======================================================================================================================================================
// Document
//======================================================================================================================================================
// UIRecursion.h
// HAL東京 GP-12A-332 09 亀岡竣介
// 2019/07/27
//======================================================================================================================================================
#pragma once
#include "Sprite.h"
#include "AbstractScene.h"
//======================================================================================================================================================
// NameSpace
// 名前空間
//======================================================================================================================================================
namespace uiRecursionNS
{
	const int WIDTH = (280);																//	幅
	const int HEIGHT = (50);																//	高さ
	const int FADE_START_TIME = (200);														//	フェード開始時間
	const float POSITION_X_PLAYER_1 = ((WINDOW_WIDTH / 2) - (uiRecursionNS::WIDTH / 2));	//	座標 x ( Player1 )
	const float POSITION_X_PLAYER_2 = (POSITION_X_PLAYER_1 + (WINDOW_WIDTH / 2.0f));		//	座標 x ( Player2 )
	const float POSITION_Y_POP = (140.0f);													//	Y座標
	const float POSITION_Y_MOVE_1 = (110.0f);												//	Y座標
	const float POSITION_Y_MOVE_2 = (80.0f);												//	Y座標
	const float POSITION_Y_CLOSE = (50.0f);													//	Y座標

	enum TYPE
	{
		BICYCLE,		//	自転車
		BLACK_PHONE,	//	黒電話
		CHEWING_GUM,	//	チューイングガム
		FAN,			//	扇風機
		KENDAMA,		//	けん玉
		NAWATOBI,		//	なわとび
		NINGEN,			//	人間
		RABBIT_DOLL,	//	ウサギのぬいぐるみ
		SOCCER_BALL,	//	サッカーボール
		TELEVISION,		//	テレビ
		XMAS_TREE,		//	クリスマスツリー
		TYPE_MAX		//	最大値
	};

	enum POSITION
	{
		POP,
		MOVE_01,
		MOVE_02,
		CLOSE,
		POSITION_MAX
	};
}
//======================================================================================================================================================
// Class
// クラス
//======================================================================================================================================================
class UIRecursion
{
private:
	static int instanceIndex;											//	インスタンスインデックス
	static LPDIRECT3DTEXTURE9 texture[uiRecursionNS::TYPE::TYPE_MAX];	//	テクスチャ
	Sprite sprite[uiRecursionNS::TYPE::TYPE_MAX];						//	スプライト
	int playerIndex;													//	プレイヤーインデックス
	int destinationIndex[uiRecursionNS::TYPE::TYPE_MAX];

	Input* input;

public:
	UIRecursion(void);
	~UIRecursion(void);
	HRESULT initialize(LPDIRECT3DDEVICE9 _device, int _playerNumber, TextureLoader* _textureLoader, Input* _input);
	void initializeSprite(LPDIRECT3DDEVICE9 _device, int _Index);
	void release(void);
	void update(void);
	void updateMove(void);
	void render(LPDIRECT3DDEVICE9 _device);
};