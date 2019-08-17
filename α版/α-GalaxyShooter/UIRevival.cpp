//======================================================================================================================================================
// Document
//======================================================================================================================================================
// UIRevival.cpp
// HAL東京 GP-12A-332 09 亀岡竣介
// 2019/08/07
//======================================================================================================================================================
#include "UIRevival.h"
//======================================================================================================================================================
// Using Declaration
// using宣言
//======================================================================================================================================================
using namespace uiRevivalNS;
//======================================================================================================================================================
// Global Variable
// グローバル変数
//======================================================================================================================================================
int UIRevival::instanceIndex = 0;		//	インスタンスインデックス
LPDIRECT3DTEXTURE9 UIRevival::texture;	//	テクスチャ
//======================================================================================================================================================
// Constructor
// コンストラクタ
//======================================================================================================================================================
UIRevival::UIRevival(void)
{
	// インスタンスインデックスを加算
	instanceIndex++;

	// テクスチャのクリア
	texture = NULL;

	return;
}
//======================================================================================================================================================
// Destructor
// デストラクタ
//======================================================================================================================================================
UIRevival::~UIRevival(void)
{
	// 解放
	release();

	return;
}
//======================================================================================================================================================
// initialize
// 初期化
//======================================================================================================================================================
HRESULT UIRevival::initialize(LPDIRECT3DDEVICE9 _device, int _playerIndex, TextureLoader* _textureLoader)
{
	// ディレクトリ設定
	setVisualDirectory();

	// テクスチャ読み込み
	texture = *_textureLoader->getTexture(textureLoaderNS::TEXTURE_NUMBER::UI_REVIVAL);

	// スプライト初期化
	sprite.initialize
	(
		_device,
		texture,														//	テクスチャ
		spriteNS::CENTER,												//	原点
		WIDTH,															//	横幅
		HEIGHT,															//	高さ
		D3DXVECTOR3														//	座標
		(
			_playerIndex ? POSITION_X_PLAYER_2 : POSITION_X_PLAYER_1,	//	座標 x
			POSITION_Y,													//	座標 y
			0.0f														//	座標 z
		),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),									//	回転
		D3DCOLOR_RGBA(255, 255, 255, 255)								//	色
	);

	return S_OK;
}
//======================================================================================================================================================
// release
// 解放
//======================================================================================================================================================
void UIRevival::release(void)
{
	// スプライトのクリア
	sprite.setTexture(NULL);

	// インスタンスインデックスを減算
	instanceIndex--;

	return;
}
//======================================================================================================================================================
// render
// 描画
//======================================================================================================================================================
void UIRevival::render(LPDIRECT3DDEVICE9 _device)
{
	sprite.render(_device);

	return;
}