//=============================================================================
// Pose描画ヘッダー [Pose.h]
// 制作者 飯塚春輝
//=============================================================================
#pragma once
#include "AbstractScene.h"
#include "Sprite.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define POSE_COLOR		(D3DCOLOR_RGBA(255, 255, 255, 255))	// ポーズの色
//*****************************************************************************
// クラス定義
//*****************************************************************************
class Pose :public Sprite
{
private:
	// Data
	int playerNumber;								// プレイヤー番号
	static int cntUI;								// インスタンスの数を数える
	static LPDIRECT3DTEXTURE9 texturePose;			// テクスチャ

public:
	Pose(void);
	~Pose(void);
	HRESULT initialize(LPDIRECT3DDEVICE9 device, int _playerNumber, TextureLoader*textureLoader);
	void uninitialize(void);
	void update(void);
	void render(LPDIRECT3DDEVICE9 device);
	bool poseon;									// ポーズフラグ
};

