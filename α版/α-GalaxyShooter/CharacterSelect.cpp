//============================================================================================================================================
// Document
//============================================================================================================================================
// CharacterSelect.cpp
// HAL東京 GP-12A-332 09 亀岡竣介
// 2019/08/25
//============================================================================================================================================
#include "CharacterSelect.h"
//============================================================================================================================================
// Using Declaration
// using宣言
//============================================================================================================================================
using namespace characterSelectNS;
//============================================================================================================================================
// Constructor
// コンストラクタ
//============================================================================================================================================
CharacterSelect::CharacterSelect(void)
{
	// 今のシーン( キャラクターセレクト )
	sceneName = ("Scene -CharacterSelect-");

	// 次のシーン( ゲーム )
	nextScene = SceneList::GAME;

	// 初期化
	selectTime = SELECT_TIME;

	return;
}
//============================================================================================================================================
// Destructor
// デストラクタ
//============================================================================================================================================
CharacterSelect::~CharacterSelect(void)
{
	// サウンドの停止
	sound->stop(soundNS::TYPE::BGM_CHARACTER_SELECT);

	return;
}
//============================================================================================================================================
// initialize
// 初期化
//============================================================================================================================================
void CharacterSelect::initialize(Direct3D9* _direct3D9, Input* _input, Sound* _sound, TextureLoader* _textureLoader, StaticMeshLoader* _staticMeshLoader, ShaderLoader* _shaderLoader, TextManager* _textManager)
{
	// Input
	input = _input;
	// Sound
	sound = _sound;
	// TextureLoader
	textureLoader = _textureLoader;
	// StaticMeshLoader
	staticMeshLoader = _staticMeshLoader;
	// ShaderLoader
	shaderLoader = _shaderLoader;
	// テキストマネージャ
	textManager = _textManager;

	// サウンドの再生
	sound->play(soundNS::TYPE::BGM_CHARACTER_SELECT, soundNS::METHOD::LOOP);

	// Camera
	camera = new Camera;
	camera->initialize(WINDOW_WIDTH / 2, WINDOW_HEIGHT);
	camera->setGaze(D3DXVECTOR3(0, 0, 0));
	camera->setPosition(D3DXVECTOR3(0, 0, -1));
	camera->setUpVector(D3DXVECTOR3(0, 1, 0));

	// 画面分割線の初期化
	uiScreenSplitLine.initialize(_direct3D9->device, textureLoader);

	// キャラクターセレクトUIの初期化
	uiCharacterSelect.initialize(_direct3D9->device, _textureLoader);

	return;
}
//============================================================================================================================================
// uninitialize
// 解放
//============================================================================================================================================
void CharacterSelect::uninitialize(void)
{
	uiCharacterSelect.release();

	return;
}
//============================================================================================================================================
// update
// 更新
//============================================================================================================================================
void CharacterSelect::update(float _frameTime)
{
	// カメラ更新
	camera->update();

	// 時間更新
	updateTime(_frameTime);

	// 遷移更新
	updateScene();

	// キャラクターセレクトUI更新
	uiCharacterSelect.update(input, sound);

	return;
}
//============================================================================================================================================
// updateScene
// 更新 - シーン
//============================================================================================================================================
void CharacterSelect::updateScene(void)
{
	// 前の遷移へ戻る
	updateSceneBack();

	// 次の遷移へ進む
	updateSceneNext();

	return;
}
//============================================================================================================================================
// updateSceneBack
// 更新 - シーン( 戻る )
//============================================================================================================================================
void CharacterSelect::updateSceneBack(void)
{
	if (((uiCharacterSelect.getProgressState(inputNS::DINPUT_1P) == uiCharacterSelectNS::PROGRESS_TYPE::CHARACTER_SELECT) &&
		input->wasKeyPressed(VK_BACK)) ||
		input->getController()[inputNS::DINPUT_1P]->wasButton(virtualControllerNS::B))
	{
		// サウンドの再生
		sound->play(soundNS::TYPE::SE_CANCEL, soundNS::METHOD::PLAY);
		// 次の遷移先を設定
		nextScene = (SceneList::TITLE);
		// 遷移
		changeScene(nextScene);

		return;
	}
	else if (((uiCharacterSelect.getProgressState(inputNS::DINPUT_2P) == uiCharacterSelectNS::PROGRESS_TYPE::CHARACTER_SELECT) &&
		input->wasKeyPressed(VK_DELETE)) ||
		input->getController()[inputNS::DINPUT_2P]->wasButton(virtualControllerNS::B))
	{
		// サウンドの再生
		sound->play(soundNS::TYPE::SE_CANCEL, soundNS::METHOD::PLAY);
		// 次の遷移先を設定
		nextScene = (SceneList::TITLE);
		// 遷移
		changeScene(nextScene);

		return;
	}

	return;
}
//============================================================================================================================================
// updateSceneNext
// 更新 - シーン( 進む )
//============================================================================================================================================
void CharacterSelect::updateSceneNext(void)
{
	if ((uiCharacterSelect.getProgressState(inputNS::DINPUT_1P) == uiCharacterSelectNS::PROGRESS_TYPE::WAITING) &&
		(uiCharacterSelect.getProgressState(inputNS::DINPUT_2P) == uiCharacterSelectNS::PROGRESS_TYPE::WAITING))
	{
		// サウンドの再生
		sound->play(soundNS::TYPE::SE_READY, soundNS::METHOD::PLAY);
		// キャラクターの設定
		gameMaster->setPlayerCharacter(playerNS::PLAYER1, gameMasterNS::PLAYER_CHARACTER, uiCharacterSelect.getSelectState(uiCharacterSelectNS::PLAYER_TYPE::PLAYER_1));
		gameMaster->setPlayerCharacter(playerNS::PLAYER2, gameMasterNS::PLAYER_CHARACTER, uiCharacterSelect.getSelectState(uiCharacterSelectNS::PLAYER_TYPE::PLAYER_2));
		// 遷移
		changeScene(nextScene);
	}

	return;
}
//============================================================================================================================================
// updateTime
// 更新 - 時間
//============================================================================================================================================
void CharacterSelect::updateTime(float _frameTime)
{
	// 選択時間の更新
	selectTime -= _frameTime;

	// 時間制限
	if (selectTime <= 0.0f)
	{
		// 遷移
		changeScene(nextScene);
	}

	return;
}
//============================================================================================================================================
// render
// 描画
//============================================================================================================================================
void CharacterSelect::render(Direct3D9* _direct3D9)
{
	// カメラ・ウィンドウ
	_direct3D9->device->SetTransform(D3DTS_VIEW, &camera->view);
	_direct3D9->device->SetTransform(D3DTS_PROJECTION, &camera->projection);
	_direct3D9->changeViewportFullWindow();

	// 3D
	render3D(_direct3D9);

	// 2D
	render2D(_direct3D9->device);

	// Time
	renderTime(gameMaster->getGameTime());

	return;
}
//============================================================================================================================================
// render3D
// 描画 - 3D
//============================================================================================================================================
void CharacterSelect::render3D(Direct3D9* _direct3D9)
{
	return;
}
//============================================================================================================================================
// render2D
// 描画 - 2D
//============================================================================================================================================
void CharacterSelect::render2D(LPDIRECT3DDEVICE9 _device)
{
	// キャラクターセレクトUI
	uiCharacterSelect.render(_device);

	// 画面分割線
	uiScreenSplitLine.render(_device);

	// タイムUI
	uiCharacterSelect.renderTime(_device);

	return;
}
//============================================================================================================================================
// time
// 時間
//============================================================================================================================================
void CharacterSelect::renderTime(float _time)
{
	textManager->text[textManagerNS::TYPE::NEW_RODIN_CHARACTER_SELECT_TIME]->print(POSITION_TIME_X, POSITION_TIME_Y, "%.0f", _time);

	return;
}
//============================================================================================================================================
// None
//============================================================================================================================================
void CharacterSelect::collisions(void)
{
	// None
	return;
}
void CharacterSelect::AI(void)
{
	// None
	return;
}