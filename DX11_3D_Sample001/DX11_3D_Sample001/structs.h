#pragma once
#include <DirectXMath.h>

struct CustomVertex
{
	DirectX::XMFLOAT3 position;
};
struct ConstantBufferForPerFrame
{
	DirectX::XMMATRIX world;        //!< ���[���h�s��ł�.
	DirectX::XMMATRIX view;         //!< �r���[�s��ł�.
	DirectX::XMMATRIX proj;         //!< �ˉe�s��ł�.
};