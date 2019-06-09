#include "Object.h"
#include "Base.h"
Object::Object()
{
	ZeroMemory(this, sizeof(Object));
	D3DXMatrixIdentity(&matrixRotation);
	quaternion = D3DXQUATERNION(0, 0, 0, 1);
	gravity = D3DXVECTOR3(0, 0, 0);
	axisX = D3DXVECTOR3(1, 0, 0);
	axisY = D3DXVECTOR3(0, 1, 0);
	axisZ = D3DXVECTOR3(0, 0, 1);
}

Object::~Object()
{
	
}

HRESULT Object::initialize(LPDIRECT3DDEVICE9 device, LPSTR xFileName, D3DXVECTOR3* _position)
{
	// メッシュの初期位置
	memcpy(position, _position, sizeof(D3DXVECTOR3));
	// Xファイルからメッシュをロードする	
	LPD3DXBUFFER materialBuffer = NULL;
	setVisualDirectory();
	if (FAILED(D3DXLoadMeshFromX(xFileName, D3DXMESH_SYSTEMMEM,
		device, NULL, &materialBuffer, NULL,
		&numMaterials, &mesh)))
	{
		MessageBox(NULL, "Xファイルの読み込みに失敗しました", xFileName, MB_OK);
		return E_FAIL;
	}
	D3DXMATERIAL* materials = (D3DXMATERIAL*)materialBuffer->GetBufferPointer();
	meshMaterials = new D3DMATERIAL9[numMaterials];
	meshTextures = new LPDIRECT3DTEXTURE9[numMaterials];

	for (DWORD i = 0; i < numMaterials; i++)
	{
		meshMaterials[i] = materials[i].MatD3D;
		meshMaterials[i].Ambient = meshMaterials[i].Diffuse;
		meshTextures[i] = NULL;
		if (materials[i].pTextureFilename != NULL &&
			lstrlen(materials[i].pTextureFilename) > 0)
		{
			if (FAILED(D3DXCreateTextureFromFile(device,
				materials[i].pTextureFilename,
				&meshTextures[i])))
			{
				MessageBox(NULL, "テクスチャの読み込みに失敗しました", NULL, MB_OK);
			}
		}
	}
	MFAIL(D3DXCreateTextureFromFile(device, "Shade.bmp", &textureShade), "Shade.bmpの読み込みに失敗しました");
	MFAIL(D3DXCreateTextureFromFile(device, "Outline.bmp", &textureLine), "Outline.bmpの読み込みに失敗しました");
	materialBuffer->Release();
	//シェーダーを読み込む
	setShaderDirectory();
	MFAIL(D3DXCreateEffectFromFile(device, "ToonShader.fx", NULL, NULL, 0, NULL, &effect, NULL), "シェーダーファイル読み込み失敗");

	return S_OK;
}

VOID Object::render(LPDIRECT3DDEVICE9 device,D3DXMATRIX view,D3DXMATRIX projection,D3DXVECTOR3 cameraPositon)
{
	//ワールドトランスフォーム（ローカル座標→ワールド座標への変換）	
	D3DXMatrixTranslation(&matrixPosition, position.x, position.y, position.z);

	//クォータニオン（qtnAttitude）を回転量パラメーターに使用する
	{
		D3DXMatrixRotationQuaternion(&matrixRotation, &quaternion);
	}
	D3DXMatrixMultiply(&matrixWorld, &matrixRotation, &matrixPosition);

	//device->SetTransform(D3DTS_WORLD, &matrixWorld);

	//回転により、ローカル軸を曲げる
	D3DXVec3TransformCoord(&axisX, &D3DXVECTOR3(1, 0, 0), &matrixRotation);
	D3DXVec3TransformCoord(&axisY, &D3DXVECTOR3(0, 1, 0), &matrixRotation);
	D3DXVec3TransformCoord(&axisZ, &D3DXVECTOR3(0, 0, 1), &matrixRotation);

	effect->SetTechnique("ToonShading");
	effect->SetMatrix("mProj", &projection);
	effect->SetMatrix("mView", &view);
	effect->SetMatrix("mWorld", &matrixWorld);
	effect->SetTexture("ShadeTexture", textureShade);
	effect->SetTexture("LineTexture", textureLine);

	effect->SetVector("LightPos", &D3DXVECTOR4(3, 4, -10, 1));
	effect->SetVector("EyePos", (D3DXVECTOR4*)&cameraPositon);
	effect->Begin(NULL, 0);

	// レンダリング			
	for (DWORD i = 0; i < numMaterials; i++)
	{
		effect->BeginPass(0);
		effect->SetFloatArray("Diffuse", (FLOAT*)&meshMaterials[i].Diffuse, 4);
		effect->SetTexture("DecalTexture", meshTextures[i]);
		//device->SetMaterial(&meshMaterials[i]);
		//device->SetTexture(0, meshTextures[i]);
		mesh->DrawSubset(i);
		effect->EndPass();
	}
	effect->End();
}