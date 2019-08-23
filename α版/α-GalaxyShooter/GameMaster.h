//===================================================================================================================================
//【GameMaster.h】
// [作成者] HAL東京GP12A332 11 菅野 樹
// [作成日] 2019/07/31
// [更新日] 2019/08/22
//===================================================================================================================================
#pragma once
#include "Base.h"
#include "StaticMeshLoader.h"
#include "Player.h"

namespace gameMasterNS {
	const float GAME_TIME = 60.0f * 4.0f;	//4分
	const float COUNT_DOWN_TIME = 3.0f;		//3秒

	enum PLAYER_TYPE
	{
		PLAYER_CHARACTER,
		NON_PLAYER_CHARACTER,
		TYPE_NUM
	};

	enum RESULT_LIST
	{
		WIN_1P,
		WIN_2P,
		DRAW,
	};

	const int MODEL_ADAM = staticMeshNS::ADULT;
	const int MODEL_EVE = staticMeshNS::CHILD;

	const int COUNT_DOWN = 3;
	const float COUNT_INTERVAL = 1.0f;

}

struct playerTable
{
	int playerType;		//PC|NPC
	int modelType;		//プレイヤーのモデルタイプ
	int wage;			//チンギン
};

//===================================================================================================================================
//【ゲーム管理クラス】
// [用途]	このクラスは、シーンをまたいで情報の橋渡しを行う。
//			更新や設定は、各シーンで行う。
//===================================================================================================================================
class GameMaster :public Base
{
private:
	float gameTimer;										//ゲーム時間
	float countDownTimer;									//カウントダウン時間
	int count;												//ゲームカウント
	playerTable playerInformation[playerNS::NUM_PLAYER];	//プレイヤー情報

public:
	GameMaster();
	~GameMaster();
	
	//processing
	void initialize();
	void update(float frameTime);

	//operation
	void updateGameTime(float frameTime);					//ゲーム時間の更新
	void gameStartCount(float frameTime);					//ゲーム開始時のカウントダウン
	void gameFinishCount(float frameTime);					//ゲーム終了時のカウントダウン
	void setCountDown();									//カウントダウンのセット
	void resetGameTime();									//ゲーム時間のリセット

	//setter
	void setPlayerCharacter(int playerNo, int playerType, int modelType);	//キャラクター情報をセット
	void setWage(int playerNo, int wage);									//チンギンを加算
	
	//getter
	playerTable* getPlayerInfomation();
	int getResult();
	float getGameTime();
	bool whetherGameOver();
	int getCount();
};

