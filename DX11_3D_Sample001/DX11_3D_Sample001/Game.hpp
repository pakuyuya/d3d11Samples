#pragma once
#include<d3d11.h>
#include<windows.h>
#include <DirectXMath.h>

class Game{
private:
	HWND m_hWnd;
	
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	IDXGIDevice1* m_pDXGIDevice;
	IDXGIAdapter* m_pDXGIAdapter;
	IDXGIFactory* m_pDXGIFactory;
	IDXGISwapChain* m_pDXGISwapChain;
	ID3D11Texture2D* m_pBackBuffer;
	ID3D11RenderTargetView* m_pRenderTargetView;
	ID3D11Texture2D* m_pDepthStencilTexture;
	ID3D11DepthStencilView* m_pDepthStencilView;
	ID3D11ShaderResourceView* m_pShaderResourceView;
	D3D11_VIEWPORT m_viewPort;

	ID3D11VertexShader* m_pVertexShader;
	ID3D11InputLayout* m_pInputLayout;
	ID3D11GeometryShader* m_pGeometryShader;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11Buffer* m_pConstantBuffer;

	ID3D11Buffer* m_pVertexBuffer;

	DirectX::XMMATRIX m_View;
	DirectX::XMMATRIX m_Proj;
public:
	Game();
	~Game();

public:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

public:
	HRESULT InitWnd(HINSTANCE hInst);

	void enterWindowLoop();

	void clearDisplay();
	void drawTriangle();

	HRESULT InitD3DObjects();
	HRESULT CompileShaders();
	HRESULT ReleaseLayout();
	HRESULT ReleaseShaders();
	HRESULT ReleaseD3DObjects();

};