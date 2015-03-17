#pragma once
#include <DirectXMath.h>

struct CustomVertex
{
	DirectX::XMFLOAT3 position;
};
struct ConstantBufferForPerFrame
{
	DirectX::XMMATRIX world;        //!< ワールド行列です.
	DirectX::XMMATRIX view;         //!< ビュー行列です.
	DirectX::XMMATRIX proj;         //!< 射影行列です.
};