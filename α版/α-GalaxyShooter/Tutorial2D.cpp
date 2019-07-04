//=============================================================================
// Tutorialの描画処理 [Tutorial2D.cpp]
////=============================================================================
#include "Tutorial2D.h"

//*****************************************************************************
// 定数・マクロ
//*****************************************************************************
//マクロ定義
const static int		WIDTH = WINDOW_WIDTH;						// チュートリアル2D横サイズ
const static int		HEIGHT = WINDOW_HEIGHT;						// チュートリアル2D縦サイズ

const static float		POSITION_X = 0.0f;							// チュートリアル2DX座標
const static float		POSITION_Y = 0.0f;							// チュートリアル2DY座標

//*****************************************************************************
// グローバル変数
//*****************************************************************************
int Tutorial2D::cntUI = -1;
LPDIRECT3DTEXTURE9 Tutorial2D::textureTutorial2D = NULL;			// チュートリアル2Dテクスチャ

//=============================================================================
// コンストラクタ
//=============================================================================
Tutorial2D::Tutorial2D()
{
	cntUI++;
}

//=============================================================================
// デストラクタ
//=============================================================================
Tutorial2D::~Tutorial2D()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT Tutorial2D::initialize(LPDIRECT3DDEVICE9 device, int _playerNumber, TextureLoader*textureLoader)
{
	playerNumber = _playerNumber;

	// テクスチャを読み込む
	setVisualDirectory();

	textureTutorial2D = *textureLoader->getTexture(textureLoaderNS::TUTORIAL);

	tutorial2D.initialize(device,
		textureTutorial2D,							// テクスチャ
		spriteNS::TOP_LEFT,							// 原点
		WIDTH,										// 横幅
		HEIGHT,										// 高さ
		D3DXVECTOR3(POSITION_X, POSITION_Y, 0.0f),	// 座標
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),				// 回転
		TUTORIAL2D_COLOR
	);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void Tutorial2D::uninitialize(void)
{
	tutorial2D.setTexture(NULL);

	// インスタンスが存在しなければテクスチャ解放
	cntUI--;
	if (cntUI < 0)
	{
		SAFE_RELEASE(textureTutorial2D)
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void Tutorial2D::update(void)
{
}


//=============================================================================
// 描画処理
//=============================================================================
void Tutorial2D::render(LPDIRECT3DDEVICE9 device)
{
	tutorial2D.render(device);
}

