//===================================================================================================================================
//【StaticMeshLoader.h】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/05/16
// [更新日]2019/08/12
//===================================================================================================================================
#pragma once
#include "Base.h"

namespace staticMeshNS {
	enum MESH_NUMBER {
		GUN,
		BULLET,
		CUBE,
		SAMPLE_HIERARCHY_MESH,
		MAGNET_S,
		MAGNET_N,
		PLANET,
		SAMPLE_ROBOT001,
		SAMPLE_ROBOT002,
		SAMPLE_SKINNED_MESH,
		STAR,
		STAR_REGULAR_POLYHEDRON,
		STAR_REGULAR_POLYHEDRON_X10,
		STAR_REGULAR_POLYHEDRON_X100,
		SAMPLE_STATIC_MESH,
		SAMPLE_TOON_MESH,
		MEMORY_PILE,
		WASUREMONO_BUNNY,
		WASUREMONO_PAPER,
		WASUREMONO_REDBULL,
		WASUREMONO_SHOES,
		WASUREMONO_TEAPOT,
		WASUREMONO_LIGHT,
		WASUREMONO_BALL,
		WASUREMONO_SCISSORS,
		WASUREMONO_PLAYSTATION,
		WASUREMONO_HAT,
		MESH_NUM,
	};
}

struct StaticMesh
{
	LPDIRECT3DVERTEXBUFFER9			vertexBuffer;		//頂点情報バッファ
	LPDIRECT3DINDEXBUFFER9			indexBuffer;		//インデックスバッファ
	LPD3DXMESH						mesh;				// メッシュ情報
	D3DMATERIAL9*					materials;			// マテリアル情報格納先
	LPD3DXBUFFER					bufferMaterial;		// メッシュのマテリアルバッファ
	LPDIRECT3DTEXTURE9*				textures;			// テクスチャ情報格納先
	DWORD							numMaterial;		// マテリアル情報の数
	D3DVERTEXELEMENT9				vertexElement[65];	// 頂点属性
	LPDIRECT3DVERTEXDECLARATION9	declaration;		// 頂点宣言
	DWORD							numBytesPerVertex;	// 頂点サイズ
	D3DXATTRIBUTERANGE*				attributeTable;		// 属性テーブル
	DWORD							attributeTableSize;
};

class StaticMeshLoader :public Base
{
private:
	const char* fileName[staticMeshNS::MESH_NUM];
public:
	StaticMesh	staticMesh[staticMeshNS::MESH_NUM];
	StaticMeshLoader();
	~StaticMeshLoader();

	HRESULT load(LPDIRECT3DDEVICE9 device);
	void release(void);
};