#include "Game.h"
#include "Direct3D9.h"
using namespace gameNS;

int currentBullet1;//仮
int currentBullet2;//仮
float FrameTime = 0.0f;//仮

Game::Game()
{
	sceneName = "Scene -Game-";
	nextScene = SceneList::NONE_SCENE;
	currentBullet1 = 0;
	currentBullet2 = 0;
}

Game::~Game()
{

}

void Game::initialize(Direct3D9* direct3D9,Input* _input) {
	//Input
	input = _input;
	//camera
	camera = new Camera[NUM_PLAYER];
	for (int i = 0; i < NUM_PLAYER; i++)
	{
		camera[i].initialize(WINDOW_WIDTH / 2, WINDOW_HEIGHT);
		camera[i].setTarget(player[i].getPosition());
		camera[i].setTargetX(&player[i].getAxisX()->direction);
		camera[i].setTargetY(&player[i].getAxisY()->direction);
		camera[i].setTargetZ(&player[i].getAxisZ()->direction);
		camera[i].setRelative(CAMERA_RELATIVE_QUATERNION[i]);
		camera[i].setGaze(D3DXVECTOR3(0, 0, 0));
		camera[i].setRelativeGaze(D3DXVECTOR3(0, 10, 0));
		camera[i].setUpVector(D3DXVECTOR3(0, 1, 0));
	}

	//light
	light = new Light;
	light->initialize(direct3D9);

	//コロニー
	colony[0].initialize(direct3D9->device, (LPSTR)"planet.x", &D3DXVECTOR3(150, 0, 300));
	colony[1].initialize(direct3D9->device, (LPSTR)"planet.x", &D3DXVECTOR3(-150, 0, 300));
	//フィールド
	field.initialize(direct3D9->device, (LPSTR)"planet.x", &(D3DXVECTOR3)PLANET_POSITION);

	for (int i = 0; i < NUM_PLAYER; i++)
	{//プレイヤーの初期化
		player[i].initialize(direct3D9->device, (LPSTR)"Toon_6Color.x", &(D3DXVECTOR3)gameNS::PLAYER_POSITION[i]);
		hp[i].initialize(direct3D9->device, i);
		sp[i].initialize(direct3D9->device, i);
		colonyHp[i].initialize(direct3D9->device, i);
		missileInfomation[i].initialize(direct3D9->device, i);
		weaponInfomation[i].initialize(direct3D9->device, i);
		//重力線を作成
		D3DXVECTOR3 gravityDirection;
		between2VectorDirection(&gravityDirection, *player[i].getPosition(), *field.getPosition());
		player[i].getGravityRay()->initialize(*player[i].getPosition(), gravityDirection);
		if (player[i].getGravityRay()->rayIntersect(*field.getMesh(), field.getMatrixWorld()))
		{//重力線上にポリゴンが衝突していた場合、ポリゴン法線を重力方向とし、姿勢を法線と一致させる。
			player[i].postureControl(player[i].getAxisY()->direction, player[i].getGravityRay()->normal, 1.0f);
			player[i].setGravity(-player[i].getGravityRay()->normal, playerNS::GRAVITY_FORCE);
		}
		else 
		{//衝突ポリゴンが存在しない場合は、重力線をそのまま重力方向とし、姿勢を重力線と一致させる。
			player[i].postureControl(player[i].getAxisY()->direction, gravityDirection, 1.0f);
			player[i].setGravity(gravityDirection, playerNS::GRAVITY_FORCE);
		}

	}

	for (int i = 0; i < NUM_MAGNET; i++)
	{//磁石
		if (NUM_MAGNET / 2 > i) {
			magnet[i].initialize(direct3D9->device,2.0f);
		}
		else {
			magnet[i].initialize(direct3D9->device,-2.0f);
		}
	}

	for (int i = 0; i < NUM_BULLET; i++)
	{//バレットの初期化
		bullet1[i].initialize(direct3D9->device, (LPSTR)"bullet.x", &D3DXVECTOR3(0,0,0));
		bullet2[i].initialize(direct3D9->device, (LPSTR)"bullet.x", &D3DXVECTOR3(0,0,0));
		//bullet[i].anyAxisRotation(D3DXVECTOR3(0, 1, 0), (float)(rand() % 360));
	}


	text.initialize(direct3D9->device,10,10, 0xff00ff00);
	text2.initialize(direct3D9->device,11,11, 0xff0000ff);


	//ガラクタ
	for (int i = 0; i < JUNK_MAX; i++)
	{
		D3DXVECTOR3 position((float)(rand()%100-50), (float)(rand()%100-50), (float)(rand()%100-50));

		D3DXVec3Normalize(&position, &position);
		position *= 100;
		junk[i].initialize(direct3D9->device, (LPSTR)"starRegularPolyhedron.x", &position);
	}
	//ポイントスプライト
	pointSprite.initilaize(direct3D9->device);
	//インスタンスプレーン
	plane.createPositionSpherical(10000, 300.0f);
	plane.initialize(direct3D9->device);
}

float difference = 1.0f;

void Game::update(float frameTime) {
	
	sceneTimer += frameTime;
	FrameTime = frameTime;
	if (frameTime > 0.03)return;//フレーム時間が約30FPS時の時の時間より長い場合は、処理落ち（更新しない）※吹っ飛ぶため

	for (int i = 0; i < NUM_PLAYER; i++)
	{//プレイヤーの更新
		player[i].setSpeed(D3DXVECTOR3(0, 0, 0));
		if (input->getController()[PLAYER1]->wasButton(virtualControllerNS::UP))
			difference += 0.01f;
		if(input->getController()[PLAYER1]->wasButton(virtualControllerNS::DOWN))
			difference -= 0.01f;

		bool onJump = false;
		switch (i)
		{
		case PLAYER1:
			if (input->isKeyDown('W')) {
				player[i].move(D3DXVECTOR2(0,-1), camera[i].getDirectionX(), camera[i].getDirectionZ());
			}
			if (input->isKeyDown('S')) {
				player[i].move(D3DXVECTOR2(0,1), camera[i].getDirectionX(), camera[i].getDirectionZ());
			}
			if (input->isKeyDown('A')) {
				player[i].move(D3DXVECTOR2(-1,0), camera[i].getDirectionX(), camera[i].getDirectionZ());
			}
			if (input->isKeyDown('D'))
			{
				player[i].move(D3DXVECTOR2(1,0), camera[i].getDirectionX(), camera[i].getDirectionZ());
			}
			if (input->wasKeyPressed(VK_SPACE)||input->getController()[i]->wasButton(virtualControllerNS::B))
			{
				onJump = true;
			}
			if (input->wasKeyPressed('R'))
			{
				player[i].reset();
			}
			break;
		case PLAYER2:
			if (input->isKeyDown(VK_UP)) {
				player[i].move(D3DXVECTOR2(0,-1), camera[i].getDirectionX(), camera[i].getDirectionZ());
			}
			if (input->isKeyDown(VK_DOWN)) {
				player[i].move(D3DXVECTOR2(0,1), camera[i].getDirectionX(), camera[i].getDirectionZ());
			}
			if (input->isKeyDown(VK_LEFT)) {
				player[i].move(D3DXVECTOR2(-1,0), camera[i].getDirectionX(), camera[i].getDirectionZ());
			}
			if (input->isKeyDown(VK_RIGHT))
			{
				player[i].move(D3DXVECTOR2(1,0), camera[i].getDirectionX(), camera[i].getDirectionZ());
			}
			if (input->wasKeyPressed(VK_RETURN)|| input->getController()[i]->wasButton(virtualControllerNS::B))
			{
				onJump = true;
			}
			break;
		}

		//コントローラスティックによる移動
		if (input->getController()[i]->checkConnect()) {
			player[i].move(input->getController()[i]->getLeftStick()*0.001f, camera[i].getDirectionX(), camera[i].getDirectionZ());
		}

		//フィールド補正
		if (player[i].getReverseAxisY()->rayIntersect(*field.getMesh(), field.getMatrixWorld()) &&
			player[i].getRadius() >= (player[i].getReverseAxisY()->distance -difference))
		{
			//めり込み補正
			//現在位置+ 垂直方向*(めり込み距離)
			player[i].setPosition(*player[i].getPosition() + player[i].getAxisY()->direction*(player[i].getRadius() - player[i].getReverseAxisY()->distance));

			//移動ベクトルのスリップ（面方向へのベクトル成分の削除）
			player[i].setSpeed(player[i].getReverseAxisY()->slip(player[i].getSpeed(), player[i].getReverseAxisY()->normal));
			//Ray moveRay;//移動ベクトルを使ってレイを作成
			//moveRay.initialize(*player[i].getPosition(), player[i].getSpeed());
			//if(moveRay.rayIntersect(*field.getMesh(),field.getMatrixWorld()) && player[i].getRadius() > moveRay.distance)
			//{//二重チェック
			//	player[i].setSpeed(moveRay.slip(player[i].getSpeed(),moveRay.normal));
			//}
			if(onJump)player[i].jump();
		}
		else {
			player[i].setGravity(-player[i].getAxisY()->direction, playerNS::GRAVITY_FORCE);
		}

		player[i].update(frameTime);
	}

	//バレットの発射
	//動的生成にしたい
	if (input->getMouseLButton() || input->getController()[PLAYER1]->wasButton(virtualControllerNS::R1))
	{
		bullet1[currentBullet1].setPosition(*player[PLAYER1].getPosition());
		//Y軸方向への成分を削除する
		D3DXVECTOR3 front = slip(camera[PLAYER1].getDirectionZ(), player[PLAYER1].getAxisY()->direction);
		D3DXVec3Normalize(&front, &front);//正規化
		bullet1[currentBullet1].addSpeed(front*0.2);//速度を加算
		bullet1[currentBullet1].setQuaternion(player[PLAYER1].getQuaternion());
		bullet1[currentBullet1].postureControl(player[PLAYER1].getAxisZ()->direction, front,1.0f);
		bullet1[currentBullet1].activation();
		bullet1[currentBullet1].Object::update();
		currentBullet1++;
		if (currentBullet1 >= NUM_BULLET)currentBullet1 = 0;
	}
	if (input->getMouseRButton() || input->getController()[PLAYER2]->wasButton(virtualControllerNS::R1))
	{
		bullet2[currentBullet2].setPosition(*player[PLAYER2].getPosition());
		bullet2[currentBullet2].addSpeed(player[PLAYER2].getAxisZ()->direction*0.2);
		bullet2[currentBullet2].setQuaternion(player[PLAYER2].getQuaternion());
		currentBullet2++;
		if (currentBullet2 >= NUM_BULLET)currentBullet2 = 0;
	}

	for (int i = 0; i < NUM_BULLET; i++)
	{//バレットの更新
		bullet1[i].update(frameTime,*field.getMesh(),field.getMatrixWorld());
		bullet2[i].update(frameTime,*field.getMesh(),field.getMatrixWorld());
	}

	//カメラの回転

	//1Pカメラ制御
	camera[PLAYER1].rotation(D3DXVECTOR3(0, 1, 0), (float)(input->getMouseRawX()));
	camera[PLAYER1].rotation(camera->getHorizontalAxis(), (float)(input->getMouseRawY()));

	if (input->isKeyDown('G')) {
		//camera[PLAYER1].rotation(D3DXVECTOR3(0, 1, 0), -1.0f);
	};
	if (input->isKeyDown('J')) {
		camera[PLAYER1].rotation(D3DXVECTOR3(0, 1, 0), 1.0f);
	};

	if (input->getController()[PLAYER1]->checkConnect()) {
		camera[PLAYER1].rotation(D3DXVECTOR3(0, 1, 0), input->getController()[PLAYER1]->getRightStick().x*0.001f);
		camera[PLAYER1].rotation(camera->getHorizontalAxis(), input->getController()[PLAYER1]->getRightStick().y*0.001f);
	}
	//2Pをロックオン
	if (GetAsyncKeyState(VK_LCONTROL) & 0x8000) {
		camera[PLAYER1].lockOn(*player[PLAYER2].getPosition(),frameTime);
	}

	//2Pカメラ制御
	if (input->getMouseLButton()) {
		camera[PLAYER2].rotation(D3DXVECTOR3(0, 1, 0), (float)(input->getMouseRawX()));
	};

	if (input->getController()[PLAYER2]->checkConnect()) {
		camera[PLAYER2].rotation(D3DXVECTOR3(0, 1, 0), input->getController()[PLAYER2]->getRightStick().x*0.001f);
		camera[PLAYER2].rotation(D3DXVECTOR3(0, 0, 1), input->getController()[PLAYER2]->getRightStick().y*0.001f);
	}

	for (int i = 0; i < NUM_PLAYER; i++)
	{//カメラの更新
		camera[i].setUpVector(player[i].getAxisY()->direction);
		camera[i].update();
	}

	for (int i = 0; i < NUM_PLAYER; i++)
	{
		hp[i].update(player[i].getHp(),player[i].getMaxHp());
		sp[i].update(player[i].getSp(), player[i].getMaxSp());
		colonyHp[i].update();
		missileInfomation[i].update();
		weaponInfomation[i].update();
	}

	// コロニーアップデート
	colony[0].update();

	for (int i = 0; i < JUNK_MAX; i++)
	{
		// ガラクタアップデート
		junk[i].update(frameTime, *field.getMesh(), field.getMatrixWorld(),*field.getPosition());
	}

	if (input->getController()[PLAYER1]->wasButton(virtualControllerNS::A))
	{
		magnet[NUM_MAGNET - 1].reverseAmount();
	}

	magnet[NUM_MAGNET-1].setPosition(*player[PLAYER1].getPosition());
	magnet[1].setPosition(*player[PLAYER2].getPosition());

	//磁石の更新
	for (int i = 0; i < NUM_MAGNET; i++)
	{
		for (int j = 0; j < NUM_MAGNET; j++)
		{
			if(j != i)
				magnet[i].calculationMagneticeForce(magnet[j]);
		}
	}
	for (int i = 0; i < NUM_MAGNET; i++)
	{
		magnet[i].update();
	}
}

void Game::render(Direct3D9* direct3D9) {

	//1Pカメラ・ウィンドウ
	direct3D9->device->SetTransform(D3DTS_VIEW, &camera[0].view);
	direct3D9->device->SetTransform(D3DTS_PROJECTION, &camera[0].projection);
	direct3D9->changeViewport1PWindow();
	render3D(direct3D9,camera[0]);

	//2Pカメラ・ウィンドウ
	direct3D9->device->SetTransform(D3DTS_VIEW, &camera[1].view);
	direct3D9->device->SetTransform(D3DTS_PROJECTION, &camera[1].projection);
	direct3D9->changeViewport2PWindow();
	render3D(direct3D9,camera[1]);
	//UI
	//direct3D9->device->SetTransform(D3DTS_VIEW, &camera[2].view);
	//direct3D9->device->SetTransform(D3DTS_PROJECTION, &camera[2].projection);
	direct3D9->changeViewportFullWindow();
	renderUI(direct3D9->device);
}

void Game::render3D(Direct3D9* direct3D9,Camera currentCamera) {
	for (int i = 0; i < NUM_PLAYER; i++)
	{//プレイヤーの描画
		player[i].toonRender(direct3D9->device,currentCamera.view,currentCamera.projection,currentCamera.position);
	}
	for (int i = 0; i < NUM_BULLET; i++)
	{//バレットの描画
		bullet1[i].render(direct3D9->device,currentCamera.view,currentCamera.projection,currentCamera.position);
		bullet2[i].render(direct3D9->device,currentCamera.view,currentCamera.projection,currentCamera.position);
	}

	direct3D9->device->SetRenderState(D3DRS_LIGHTING, false);

	for (int i = 0; i < NUM_COLONY; i++)
	{
		colony[i].render(direct3D9->device, currentCamera.view, currentCamera.projection, currentCamera.position);
	}

	direct3D9->device->SetRenderState(D3DRS_LIGHTING, true);

	for (int i = 0; i < JUNK_MAX; i++)
	{
		junk[i].render(direct3D9->device, currentCamera.view, currentCamera.projection, currentCamera.position);
	}

	//フィールドの描画
	field.render(direct3D9->device, currentCamera.view, currentCamera.projection, currentCamera.position);
	for (int i = 0; i < NUM_MAGNET; i++)
	{//
		magnet[i].render(direct3D9->device, currentCamera.view, currentCamera.projection, currentCamera.position);
	}
	//ポイントスプライトの描画
	pointSprite.render(direct3D9->device,currentCamera.position);
	//プレーンの描画
	plane.render(direct3D9->device, currentCamera.view, currentCamera.projection, currentCamera.position);

#ifdef _DEBUG
	Ray debugRay;
	//法線
	debugRay.color = D3DXCOLOR(128, 0, 128, 255);
	debugRay.update(*player[PLAYER1].getPosition(), player[PLAYER1].getReverseAxisY()->normal);
	debugRay.render(direct3D9->device, 100.0f);
#endif
}

bool onUI = true;

void Game::renderUI(LPDIRECT3DDEVICE9 device) {
	text.print(50, 200,
		"difference:%.3f\n",difference);
	text.print(10, 150,
		"\
		sceneTime:%.2f\n\
		frameTime:%.2f\n\
		",
		sceneTimer, FrameTime);
	text.print(10, 10, "acceleration(\t%.2f,\t%.2f,\t%.2f)",
		player[PLAYER1].getAcceleration().x,
		player[PLAYER1].getAcceleration().y,
		player[PLAYER1].getAcceleration().z);
	text.print(10, 30, "gravity(\t%.2f,\t%.2f,\t%.2f)", 
		player[PLAYER1].getGravity().x,
		player[PLAYER1].getGravity().y,
		player[PLAYER1].getGravity().z);
	text.print(10, 50, "quaternion(\t%.2f,\t%.2f,\t%.2f,\t%.2f)", 
		player[PLAYER1].getQuaternion().x,
		player[PLAYER1].getQuaternion().y,
		player[PLAYER1].getQuaternion().z,
		player[PLAYER1].getQuaternion().w);
	text.print(10, 70, "normal(\t%.2f,\t%.2f,\t%.2f)", 
		player[PLAYER1].getReverseAxisY()->normal.x,
		player[PLAYER1].getReverseAxisY()->normal.y,
		player[PLAYER1].getReverseAxisY()->normal.z);
	text.print(10, 90, "cameraRelative(\t%.2f,\t%.2f,\t%.2f,\t%.2f)", 
		camera[PLAYER1].relativeQuaternion.x,
		camera[PLAYER1].relativeQuaternion.y,
		camera[PLAYER1].relativeQuaternion.z,
		camera[PLAYER1].relativeQuaternion.w);
	text.print(10, 110, "input(x:%d,y:%d,rowX%d,rowY%d)", 
		input->getMouseX(),
		input->getMouseY(),
		input->getMouseRawX(),
		input->getMouseRawY());

	text.print(WINDOW_WIDTH / 2, 10, 
		"magnetPosition(%.02f,%.02f,%.02f)\n\
		 magnetSpeed(%.02f,%.02f,%.02f)\n\
		",
		magnet[0].getPosition()->x,magnet[0].getPosition()->y,magnet[0].getPosition()->z,
		magnet[0].getSpeed().x,magnet[0].getSpeed().y,magnet[0].getSpeed().z
	);

	text2.print(10, 450,
		"1P:Controller\n\
		isA:%d wasA:%d\n\
		isB:%d wasB:%d\n\
		isX:%d wasX:%d\n\
		isY:%d wasY:%d\n\
		isUP:%d wasUP:%d\n\
		isRIGHT:%d wasRIGHT:%d\n\
		isDOWN:%d wasDOWN:%d\n\
		isLEFT:%d wasLEFT:%d\n\
		isR1:%d wasR1:%d\n\
		isR2:%d wasR2:%d\n\
		isR3:%d wasR3:%d\n\
		isL1:%d wasL1:%d\n\
		isL2:%d wasL2:%d\n\
		isL3:%d wasL3:%d\n\
		isHOME:%d wasHOME:%d\n\
		isCAPTURE:%d wasCAPTURE:%d\n\
		isSPECIAL_MAIN:%d wasSPECIAL_MAIN:%d\n\
		isSPECIAL_SUB:%d wasSPECIAL_SUB:%d\n\
		LeftStick:(%.2f,%.2f)\n\
		RightStick:(%.2f,%.2f)\n\
		",
		input->getController()[inputNS::DINPUT_1P]->isButton(virtualControllerNS::A),input->getController()[inputNS::DINPUT_1P]->wasButton(virtualControllerNS::A),
		input->getController()[inputNS::DINPUT_1P]->isButton(virtualControllerNS::B),input->getController()[inputNS::DINPUT_1P]->wasButton(virtualControllerNS::B),
		input->getController()[inputNS::DINPUT_1P]->isButton(virtualControllerNS::X),input->getController()[inputNS::DINPUT_1P]->wasButton(virtualControllerNS::X),
		input->getController()[inputNS::DINPUT_1P]->isButton(virtualControllerNS::Y),input->getController()[inputNS::DINPUT_1P]->wasButton(virtualControllerNS::Y),
		input->getController()[inputNS::DINPUT_1P]->isButton(virtualControllerNS::UP),input->getController()[inputNS::DINPUT_1P]->wasButton(virtualControllerNS::UP),
		input->getController()[inputNS::DINPUT_1P]->isButton(virtualControllerNS::RIGHT),input->getController()[inputNS::DINPUT_1P]->wasButton(virtualControllerNS::RIGHT),
		input->getController()[inputNS::DINPUT_1P]->isButton(virtualControllerNS::DOWN),input->getController()[inputNS::DINPUT_1P]->wasButton(virtualControllerNS::DOWN),
		input->getController()[inputNS::DINPUT_1P]->isButton(virtualControllerNS::LEFT),input->getController()[inputNS::DINPUT_1P]->wasButton(virtualControllerNS::LEFT),
		input->getController()[inputNS::DINPUT_1P]->isButton(virtualControllerNS::R1),input->getController()[inputNS::DINPUT_1P]->wasButton(virtualControllerNS::R1),
		input->getController()[inputNS::DINPUT_1P]->isButton(virtualControllerNS::R2),input->getController()[inputNS::DINPUT_1P]->wasButton(virtualControllerNS::R2),
		input->getController()[inputNS::DINPUT_1P]->isButton(virtualControllerNS::R3),input->getController()[inputNS::DINPUT_1P]->wasButton(virtualControllerNS::R3),
		input->getController()[inputNS::DINPUT_1P]->isButton(virtualControllerNS::L1),input->getController()[inputNS::DINPUT_1P]->wasButton(virtualControllerNS::L1),
		input->getController()[inputNS::DINPUT_1P]->isButton(virtualControllerNS::L2),input->getController()[inputNS::DINPUT_1P]->wasButton(virtualControllerNS::L2),
		input->getController()[inputNS::DINPUT_1P]->isButton(virtualControllerNS::L3),input->getController()[inputNS::DINPUT_1P]->wasButton(virtualControllerNS::L3),
		input->getController()[inputNS::DINPUT_1P]->isButton(virtualControllerNS::HOME),input->getController()[inputNS::DINPUT_1P]->wasButton(virtualControllerNS::HOME),
		input->getController()[inputNS::DINPUT_1P]->isButton(virtualControllerNS::CAPTURE),input->getController()[inputNS::DINPUT_1P]->wasButton(virtualControllerNS::CAPTURE),
		input->getController()[inputNS::DINPUT_1P]->isButton(virtualControllerNS::SPECIAL_MAIN),input->getController()[inputNS::DINPUT_1P]->wasButton(virtualControllerNS::SPECIAL_MAIN),
		input->getController()[inputNS::DINPUT_1P]->isButton(virtualControllerNS::SPECIAL_SUB),input->getController()[inputNS::DINPUT_1P]->wasButton(virtualControllerNS::SPECIAL_SUB),
		input->getController()[inputNS::DINPUT_1P]->getLeftStick().x,input->getController()[inputNS::DINPUT_1P]->getLeftStick().y,
		input->getController()[inputNS::DINPUT_1P]->getRightStick().x,input->getController()[inputNS::DINPUT_1P]->getRightStick().y
	);
	text2.print(WINDOW_WIDTH/2.0f, 450,
		"2P:Controller\n\
		isA:%d wasA:%d\n\
		isB:%d wasB:%d\n\
		isX:%d wasX:%d\n\
		isY:%d wasY:%d\n\
		isUP:%d wasUP:%d\n\
		isRIGHT:%d wasRIGHT:%d\n\
		isDOWN:%d wasDOWN:%d\n\
		isLEFT:%d wasLEFT:%d\n\
		isR1:%d wasR1:%d\n\
		isR2:%d wasR2:%d\n\
		isR3:%d wasR3:%d\n\
		isL1:%d wasL1:%d\n\
		isL2:%d wasL2:%d\n\
		isL3:%d wasL3:%d\n\
		isHOME:%d wasHOME:%d\n\
		isCAPTURE:%d wasCAPTURE:%d\n\
		isSPECIAL_MAIN:%d wasSPECIAL_MAIN:%d\n\
		isSPECIAL_SUB:%d wasSPECIAL_SUB:%d\n\
		LeftStick:(%.2f,%.2f)\n\
		RightStick:(%.2f,%.2f)\n\
		",
		input->getController()[inputNS::DINPUT_2P]->isButton(virtualControllerNS::A),input->getController()[inputNS::DINPUT_2P]->wasButton(virtualControllerNS::A),
		input->getController()[inputNS::DINPUT_2P]->isButton(virtualControllerNS::B),input->getController()[inputNS::DINPUT_2P]->wasButton(virtualControllerNS::B),
		input->getController()[inputNS::DINPUT_2P]->isButton(virtualControllerNS::X),input->getController()[inputNS::DINPUT_2P]->wasButton(virtualControllerNS::X),
		input->getController()[inputNS::DINPUT_2P]->isButton(virtualControllerNS::Y),input->getController()[inputNS::DINPUT_2P]->wasButton(virtualControllerNS::Y),
		input->getController()[inputNS::DINPUT_2P]->isButton(virtualControllerNS::UP),input->getController()[inputNS::DINPUT_2P]->wasButton(virtualControllerNS::UP),
		input->getController()[inputNS::DINPUT_2P]->isButton(virtualControllerNS::RIGHT),input->getController()[inputNS::DINPUT_2P]->wasButton(virtualControllerNS::RIGHT),
		input->getController()[inputNS::DINPUT_2P]->isButton(virtualControllerNS::DOWN),input->getController()[inputNS::DINPUT_2P]->wasButton(virtualControllerNS::DOWN),
		input->getController()[inputNS::DINPUT_2P]->isButton(virtualControllerNS::LEFT),input->getController()[inputNS::DINPUT_2P]->wasButton(virtualControllerNS::LEFT),
		input->getController()[inputNS::DINPUT_2P]->isButton(virtualControllerNS::R1),input->getController()[inputNS::DINPUT_2P]->wasButton(virtualControllerNS::R1),
		input->getController()[inputNS::DINPUT_2P]->isButton(virtualControllerNS::R2),input->getController()[inputNS::DINPUT_2P]->wasButton(virtualControllerNS::R2),
		input->getController()[inputNS::DINPUT_2P]->isButton(virtualControllerNS::R3),input->getController()[inputNS::DINPUT_2P]->wasButton(virtualControllerNS::R3),
		input->getController()[inputNS::DINPUT_2P]->isButton(virtualControllerNS::L1),input->getController()[inputNS::DINPUT_2P]->wasButton(virtualControllerNS::L1),
		input->getController()[inputNS::DINPUT_2P]->isButton(virtualControllerNS::L2),input->getController()[inputNS::DINPUT_2P]->wasButton(virtualControllerNS::L2),
		input->getController()[inputNS::DINPUT_2P]->isButton(virtualControllerNS::L3),input->getController()[inputNS::DINPUT_2P]->wasButton(virtualControllerNS::L3),
		input->getController()[inputNS::DINPUT_2P]->isButton(virtualControllerNS::HOME),input->getController()[inputNS::DINPUT_2P]->wasButton(virtualControllerNS::HOME),
		input->getController()[inputNS::DINPUT_2P]->isButton(virtualControllerNS::CAPTURE),input->getController()[inputNS::DINPUT_2P]->wasButton(virtualControllerNS::CAPTURE),
		input->getController()[inputNS::DINPUT_2P]->isButton(virtualControllerNS::SPECIAL_MAIN),input->getController()[inputNS::DINPUT_2P]->wasButton(virtualControllerNS::SPECIAL_MAIN),
		input->getController()[inputNS::DINPUT_2P]->isButton(virtualControllerNS::SPECIAL_SUB),input->getController()[inputNS::DINPUT_2P]->wasButton(virtualControllerNS::SPECIAL_SUB),
		input->getController()[inputNS::DINPUT_2P]->getLeftStick().x,input->getController()[inputNS::DINPUT_2P]->getLeftStick().y,
		input->getController()[inputNS::DINPUT_2P]->getRightStick().x,input->getController()[inputNS::DINPUT_2P]->getRightStick().y
	);
	// αブレンドを行う
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	// αソースカラーの指定
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	// αデスティネーションカラーの指定
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	if (input->wasKeyPressed('U'))onUI = !onUI;
	if (onUI) {
		for (int i = 0; i < NUM_PLAYER; i++)
		{
			hp[i].render(device);
			sp[i].render(device);
			colonyHp[i].render(device);
			missileInfomation[i].render(device);
			weaponInfomation[i].render(device);
		}
	}

}

void Game::collisions() {
	for (int i = 0; i < NUM_BULLET; i++)
	{	//バレット1<->プレイヤー2
		if (!bullet1[i].getActive())continue;
		if (player[PLAYER2].bodyCollide.collide(
			bullet1[i].bodyCollide.getCenter(), bullet1[i].bodyCollide.getRadius(),
			player[PLAYER2].getMatrixWorld(), bullet1[i].getMatrixWorld()))
		{
			player[PLAYER2].damgae(5);
			bullet1[i].inActivation();
		}
	}
		
	for (int i = 0; i < NUM_BULLET; i++)
	{
		//バレット2<->プレイヤー1
		if (!bullet2[i].getActive())continue;
		if (player[PLAYER1].bodyCollide.collide(
			bullet2[i].bodyCollide.getCenter(), bullet2[i].bodyCollide.getRadius(),
			player[PLAYER1].getMatrixWorld(), bullet2[i].getMatrixWorld()))
		{
			player[PLAYER1].damgae(5);
			bullet2[i].inActivation();
			
		}
	}

	//ネストが深い！！
	for (int i = 0; i < NUM_PLAYER; i++)
	{
		for (int k = 0; k < JUNK_MAX; k++)
		{
			if (between2VectorLength(*player[i].getPosition(), *junk[k].getPosition()) <= junkNS::HEAD_DISTANCE)
			{
				if (player[i].bodyCollide.collide(junk[k].bodyCollide.getCenter(),junk[k].getRadius(),
					player[i].getMatrixWorld(),junk[k].getMatrixWorld()))
				{//プレイヤーとガラクタが衝突したら
					junk[k].inActivation();
				}
				junk[k].headPosition(*player[i].getPosition());
			}
		}
	}


}

void Game::AI() {

}

void Game::uninitialize() {
	SAFE_DELETE(light);
	SAFE_DELETE_ARRAY(camera);
	for (int i = 0; i < NUM_PLAYER; i++)
	{
		hp[i].uninitialize();
		sp[i].uninitialize();
		colonyHp[i].uninitialize();
		missileInfomation[i].uninitialize();
		weaponInfomation[i].uninitialize();
	}
}