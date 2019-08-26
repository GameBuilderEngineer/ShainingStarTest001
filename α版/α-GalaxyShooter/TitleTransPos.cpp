//=============================================================================
// タイトル選択位置描画処理 [TitleTransPos.cpp]
// 制作者 飯塚春輝
////=============================================================================
#include "TitleTransPos.h"
//*****************************************************************************
// 定数
//*****************************************************************************
#ifdef _DEBUG
const static int		WIDTH = 600;										// タイトル選択位置画像横サイズ
const static int		HEIGHT = 50;										// タイトル選択位置画像縦サイズ
const static float		POSITION_X = 0.0f;									// タイトル選択位置画像X座標
static float			POSITION_Y = CNT_TITLE_START;						// タイトル選択位置画像Y座標
#else
const static int		WIDTH = 800;										// リリース時タイトル選択位置画像横サイズ
const static int		HEIGHT = 80;										// リリース時タイトル選択位置画像縦サイズ
const static float		POSITION_X = 0.0f;									// リリース時タイトル選択位置画像X座標
static float			POSITION_Y = CNT_RELEASE_START;						// リリース時タイトル選択位置画像Y座標
#endif 
//*****************************************************************************
// グローバル変数
//*****************************************************************************
int TitleTransPos::cntUI = -1;
LPDIRECT3DTEXTURE9 TitleTransPos::titleTransPosTex = NULL;					// タイトル選択位置画像テクスチャ
//=============================================================================
// コンストラクタ
//=============================================================================
TitleTransPos::TitleTransPos()
{
	cntUI++;
}
//=============================================================================
// デストラクタ
//=============================================================================
TitleTransPos::~TitleTransPos()
{
}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT TitleTransPos::initialize(LPDIRECT3DDEVICE9 device, int _playerNumber, TextureLoader*textureLoader)
{
	//	プレイヤーナンバー
	playerNumber = _playerNumber;

	// テクスチャを読み込む
	setVisualDirectory();

	titleTransPosTex = *textureLoader->getTexture(textureLoaderNS::TITLE_POS);

	// タイトル選択位置描画処理初期化
	Sprite::initialize(device,
		titleTransPosTex,								// テクスチャ
		spriteNS::TOP_LEFT,								// 原点
		WIDTH,											// 横幅
		HEIGHT,											// 高さ
		D3DXVECTOR3(POSITION_X, POSITION_Y, 0.0f),		// 座標
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),					// 回転
		TITLE_TRANSPOS_COLOR							// 色
	);

	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void TitleTransPos::uninitialize(void)
{
}
//=============================================================================
// 更新処理
//=============================================================================
void TitleTransPos::update(void)
{
	// タイトル選択位置画像移動処理
	titleTransPosMove();

	// 位置設定
	setVertex();
}
//=============================================================================
// 描画処理
//=============================================================================
void TitleTransPos::render(LPDIRECT3DDEVICE9 device)
{
	// タイトル選択位置描画処理描画
	Sprite::render(device);
}
//=============================================================================
// タイトル選択位置画像移動処理
//=============================================================================
void TitleTransPos::titleTransPosMove(void)
{
	switch (cntTitle)
	{
		// セレクト
	case 0:
#ifdef _DEBUG
		setPosition2(D3DXVECTOR3(0, CNT_TITLE_START, 0));
#else
		setPosition2(D3DXVECTOR3(0, CNT_RELEASE_START, 0));
#endif 
		break;

		// チュートリアル
	case 1:
#ifdef _DEBUG
		setPosition2(D3DXVECTOR3(0, CNT_TITLE_TUTORIAL, 0));
#else
		setPosition2(D3DXVECTOR3(0, CNT_RELEASE_TUTORIAL, 0));
#endif 
		break;

		// 操作方法
	case 2:
#ifdef _DEBUG
		setPosition2(D3DXVECTOR3(0, CNT_TITLE_OPERATION, 0));
#else
		setPosition2(D3DXVECTOR3(0, CNT_RELEASE_OPERATION, 0));
#endif 
		break;

		// クレジット
	case 3:
#ifdef _DEBUG
		setPosition2(D3DXVECTOR3(0, CNT_TITLE_CREDIT, 0));
#else
		setPosition2(D3DXVECTOR3(0, CNT_RELEASE_CREDIT, 0));
#endif 
		break;

		// ゲーム終了
	case 4:
#ifdef _DEBUG
		setPosition2(D3DXVECTOR3(0, CNT_TITLE_END, 0));
#else
		setPosition2(D3DXVECTOR3(0, CNT_RELEASE_END, 0));
#endif 
		break;
	}
}