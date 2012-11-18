#pragma once

#include <Windows.h>
#include <d3dx9.h>
#include <d3d9.h>
#include <d3dx9tex.h>

#include "standard_include.h"

namespace direct_x_settings
{
	namespace data_struct
	{
		struct tlvertex
		{
			float x_, y_, z_;	//位置情報
			float rhw_;			//頂点変換値
			D3DCOLOR color_;	//頂点カラー
			float tu_, tv_;		//テクスチャ座標

		};

		struct graphic_information
		{
			LPDIRECT3DTEXTURE9 tex_;
			int width_;
			int height_;
		};

		struct square
		{
			std::array< tlvertex, 4 > vertex_;
			LPDIRECT3DTEXTURE9 tex_;	//LPDIRECT3DTEXTURE自体がポインタ
		};

		extern std::vector< square > vertex;
		extern std::vector< graphic_information> texture;//画像を保存


	}
}