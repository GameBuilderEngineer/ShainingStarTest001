//=============================================================================
// SPのUI処理 [SkillPointUI.cpp]
//
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成日 : 2019/6/9
//=============================================================================
#include "SkillPointUI.h"

//*****************************************************************************
// 定数・マクロ
//*****************************************************************************
const static int		WIDTH = 300;						// 横サイズ
const static int		HEIGHT = 28;						// 縦サイズ					
const static float		POSITION_X_PLAYER1 = 40.0f;			// X座標
const static float		POSITION_X_PLAYER2 = POSITION_X_PLAYER1 + WINDOW_WIDTH / 2.0f;
const static float		POSITION_Y = 45.0f;					// Y座標
#define BAR_COLOR		(D3DCOLOR_RGBA(0, 0, 255, 255))		// バーの色
#define DEFAULT_COLOR	(D3DCOLOR_RGBA(255, 255, 255, 255))	// バーの色


//*****************************************************************************
// グローバル変数
//*****************************************************************************
int SkillPointUI::cntUI = -1;
LPDIRECT3DTEXTURE9 SkillPointUI::textureGaugeBase = NULL;	// ゲージベースのテクスチャ
LPDIRECT3DTEXTURE9 SkillPointUI::textureGaugeBar = NULL;	// ゲージバーのテクスチャ
LPDIRECT3DTEXTURE9 SkillPointUI::textureGaugeCover = NULL;	// ゲージカバーのテクスチャ


//=============================================================================
// コンストラクタ
//=============================================================================
SkillPointUI::SkillPointUI(void)
{
	cntUI++;
}


//=============================================================================
// デストラクタ
//=============================================================================
SkillPointUI::~SkillPointUI(void)
{
}


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT SkillPointUI::initialize(LPDIRECT3DDEVICE9 device, int _playerNumber, TextureLoader*textureLoader)
{
	playerNumber = _playerNumber;

	// テクスチャを読み込む
	setVisualDirectory();

	textureGaugeBase = *textureLoader->getTexture(textureLoaderNS::UI_GAUGE_BASE);
	textureGaugeBar = *textureLoader->getTexture(textureLoaderNS::UI_GAUGE_BAR);
	textureGaugeCover = *textureLoader->getTexture(textureLoaderNS::UI_GAUGE_COVER);


	// スプライト初期化
	gaugeBase.initialize(device,
		textureGaugeBase,					// テクスチャ
		spriteNS::TOP_LEFT,					// 原点
		WIDTH,								// 横幅
		HEIGHT,								// 高さ
		D3DXVECTOR3(playerNumber ? POSITION_X_PLAYER2 : POSITION_X_PLAYER1, POSITION_Y, 0.0f),// 座標
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),		// 回転
		DEFAULT_COLOR						// 色
	);
	gaugeBar.initialize(device,
		textureGaugeBar,					// テクスチャ
		spriteNS::TOP_LEFT,					// 原点
		WIDTH,								// 横幅
		HEIGHT,								// 高さ
		D3DXVECTOR3(playerNumber ? POSITION_X_PLAYER2 : POSITION_X_PLAYER1, POSITION_Y, 0.0f),// 座標
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),		// 回転
		BAR_COLOR							// 色
	);
	gaugeCover.initialize(device,
		textureGaugeCover,					// テクスチャ
		spriteNS::TOP_LEFT,					// 原点
		WIDTH,								// 横幅
		HEIGHT,								// 高さ
		D3DXVECTOR3(playerNumber ? POSITION_X_PLAYER2 : POSITION_X_PLAYER1, POSITION_Y, 0.0f),// 座標
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),		// 回転
		DEFAULT_COLOR						// 色
	);

	return S_OK;
}


//=============================================================================
// 終了処理
//=============================================================================
void SkillPointUI::uninitialize(void)
{
	gaugeBase.setTexture(NULL);
	gaugeBar.setTexture(NULL);
	gaugeCover.setTexture(NULL);

	// インスタンスが存在しなければテクスチャ解放
	cntUI--;
	if (cntUI < 0)
	{
		SAFE_RELEASE(textureGaugeBase)
		SAFE_RELEASE(textureGaugeBar)
		SAFE_RELEASE(textureGaugeCover)
	}
}


//=============================================================================
// 更新処理
//=============================================================================
void SkillPointUI::update(int sp, int spMax)
{
	float percentage = (float)sp / (float)spMax;
	gaugeBar.setSize((int)(percentage * WIDTH), HEIGHT);
	gaugeBar.setVertex();
}


//=============================================================================
// 描画処理
//=============================================================================
void SkillPointUI::render(LPDIRECT3DDEVICE9 device)
{
	gaugeBase.render(device);
	gaugeBar.render(device);
	gaugeCover.render(device);
}




