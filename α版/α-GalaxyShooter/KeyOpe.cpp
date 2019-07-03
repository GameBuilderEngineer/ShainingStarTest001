//=============================================================================
// キーボード操作説明処理 [KeyOpe.cpp]
//=============================================================================
#include "KeyOpe.h"
//*****************************************************************************
// 定数・マクロ
//*****************************************************************************
//マクロ定義
const static int		WIDTH = 1280;								// キーボード操作説明横サイズ
const static int		HEIGHT = 720;								// キーボード操作説明縦サイズ

const static float		POSITION_X = 0.0f;							// キーボード操作説明X座標
const static float		POSITION_Y = 0.0f;							// キーボード操作説明Y座標
//*****************************************************************************
// グローバル変数
//*****************************************************************************
int KeyOpe::cntUI = -1;
LPDIRECT3DTEXTURE9 KeyOpe::textureKeyOpe = NULL;					// キーボード操作説明テクスチャ
//=============================================================================
// コンストラクタ
//=============================================================================
KeyOpe::KeyOpe()
{
	cntUI++;
}

//=============================================================================
// デストラクタ
//=============================================================================
KeyOpe::~KeyOpe()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT KeyOpe::initialize(LPDIRECT3DDEVICE9 device, int _playerNumber)
{
	playerNumber = _playerNumber;

	// テクスチャを読み込む
	setVisualDirectory();
	if (textureKeyOpe == NULL)
	{
		if (FAILED(D3DXCreateTextureFromFile(device, "MissileUI.jpg", &textureKeyOpe)))
		{
			MessageBox(NULL, "テクスチャの読み込みに失敗しました", "MissileUI.jpg", MB_OK);
			return E_FAIL;
		}
	}

	keyOpe.initialize(device,
		textureKeyOpe,								// テクスチャ
		spriteNS::TOP_LEFT,							// 原点
		WIDTH,										// 横幅
		HEIGHT,										// 高さ
		D3DXVECTOR3(POSITION_X, POSITION_Y, 0.0f),	// 座標
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),				// 回転
		KEY_OPE_COLOR								// 色
	);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void KeyOpe::uninitialize(void)
{
	keyOpe.setTexture(NULL);

	// インスタンスが存在しなければテクスチャ解放
	cntUI--;
	if (cntUI < 0)
	{
		SAFE_RELEASE(textureKeyOpe)
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void KeyOpe::update(void)
{
}


//=============================================================================
// 描画処理
//=============================================================================
void KeyOpe::render(LPDIRECT3DDEVICE9 device)
{
	keyOpe.render(device);
}

