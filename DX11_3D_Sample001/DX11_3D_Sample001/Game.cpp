#include "Game.hpp"
#include "util.h"
#include <tchar.h>
#include <stdio.h>
#include <d3d11.h>

#include "structs.h"

// 定数群
namespace{
	const int WND_W = 640;
	const int WND_H = 480;
}

Game::Game()
	: m_hWnd(NULL)
	, m_pDXGIDevice(NULL)
	, m_pDXGIAdapter(NULL)
	, m_pDXGIFactory(NULL)
	, m_pDXGISwapChain(NULL)
	, m_pBackBuffer(NULL)
	, m_pRenderTargetView(NULL)
	, m_pDepthStencilView(NULL)
	, m_pInputLayout(NULL)
	, m_pVertexShader(NULL)
{
}

Game::~Game()
{
	ReleaseShaders();
	ReleaseD3DObjects();
}

LRESULT CALLBACK Game::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	switch (msg){
	case WM_PAINT:
		{
			hdc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
		}
		break;
	case WM_CLOSE:
		PostMessage(hWnd, WM_DESTROY, 0, 0);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return(DefWindowProc(hWnd, msg, wParam, lParam));
}



HRESULT Game::InitWnd(HINSTANCE hInst)
{
	// ウィンドウクラス登録
	TCHAR szWndClsName[] = _T("DX11_3D_SAMPLE001");

	WNDCLASS wcex;
	ZeroMemory(&wcex, sizeof(wcex));

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInst;
	wcex.hIcon = NULL;
	wcex.hCursor = NULL;
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWndClsName;

	if (!RegisterClass(&wcex)){
		char errMsg[128];
		GetLastWindowsError(errMsg);
		ShowError(__FILE__, __LINE__, errMsg);
		return -1;
	}
	//ウィンドウ生成
	const TCHAR szTitle[] = _T("DX11_D3_Sample001");
	HWND hWnd;

	hWnd = CreateWindow(
		szWndClsName,
		szTitle,
		WS_POPUP | WS_VISIBLE | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		0,
		0,
		WND_W,
		WND_H,
		NULL,
		NULL,
		hInst,
		NULL
	);
	if (!hWnd){
		char errMsg[128];
		int errCode = GetLastError();
		FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, NULL, errCode, 0, errMsg, ARRAYSIZE(errMsg), NULL);
		ShowError(__FILE__, __LINE__, errMsg);
		return -1;
	}

	m_hWnd = hWnd;

	UpdateWindow(m_hWnd);
	ShowWindow(m_hWnd, SW_SHOW);

	return 0;
}

void Game::enterWindowLoop()
{
	MSG hMsg;

	while (true){
		if (PeekMessage(&hMsg, NULL, 0, 0, PM_REMOVE)){
			if (hMsg.message == WM_QUIT){
				return;
			}
			TranslateMessage(&hMsg);
			DispatchMessage(&hMsg);
		}
		else{
			clearDisplay();
			drawTriangle();
		}
	}
}

void Game::clearDisplay()
{
//	float clearColor[] = { 255.0f, 255.0f, 255.0f };
	float clearColor[] = { 128.0f, 128.0f, 128.0f };
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, clearColor);
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void Game::drawTriangle()
{
	m_pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	m_pDeviceContext->GSSetShader(m_pGeometryShader, NULL, 0);
	m_pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);

	ConstantBufferForPerFrame cb;
	cb.world = DirectX::XMMatrixIdentity();
	cb.view = DirectX::XMMatrixIdentity();
	cb.proj = DirectX::XMMatrixIdentity();

	m_pDeviceContext->UpdateSubresource(m_pConstantBuffer, 0, NULL, &cb, 0, 0);
	m_pDeviceContext->GSSetConstantBuffers(0, 1, &m_pConstantBuffer);

	m_pDeviceContext->Draw(3, 0);

	m_pDXGISwapChain->Present(0, 0);
}


HRESULT Game::InitD3DObjects()
{
	HRESULT hr;

	hr = D3D11CreateDevice(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		0,
		NULL,
		0,
		D3D11_SDK_VERSION,
		&m_pDevice,
		NULL,
		&m_pDeviceContext
	);

	if (FAILED(hr)){
		ShowError(__FILE__, __LINE__);
		return hr;
	}

	hr = m_pDevice->QueryInterface(__uuidof(IDXGIDevice1), (void**)&m_pDXGIDevice);

	if (FAILED(hr)){
		ShowError(__FILE__, __LINE__);
		return hr;
	}
	hr = m_pDXGIDevice->GetAdapter(&m_pDXGIAdapter);

	if (FAILED(hr)){
		ShowError(__FILE__, __LINE__);
		return hr;
	}
	hr = m_pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&m_pDXGIFactory);

	if (FAILED(hr)){
		ShowError(__FILE__, __LINE__);
		return hr;
	}
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = 2;
	swapChainDesc.BufferDesc.Width = WND_W;
	swapChainDesc.BufferDesc.Height = WND_H;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;;
//	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
//	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.OutputWindow = m_hWnd;
	swapChainDesc.SampleDesc.Count = 4;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = TRUE;
//	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
//	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	hr = m_pDXGIFactory->CreateSwapChain(m_pDevice, &swapChainDesc, &m_pDXGISwapChain);
	if (FAILED(hr)){
		ShowError(__FILE__, __LINE__);
		return hr;
	}

	hr = m_pDXGISwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&m_pBackBuffer);
	if (FAILED(hr)){
		char buf[512];
		GetReturnCodeMessage(hr, buf);
		ShowError(__FILE__, __LINE__, buf);
		return hr;
	}
	hr = m_pDevice->CreateRenderTargetView(m_pBackBuffer, NULL, &m_pRenderTargetView);

	if (FAILED(hr)){
		ShowError(__FILE__, __LINE__);
		return hr;
	}

	DXGI_FORMAT textureFormat = DXGI_FORMAT_R24G8_TYPELESS;
	DXGI_FORMAT resourceFormat = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;

	int multiSampleCount = 1;

	D3D11_TEXTURE2D_DESC td;
	ZeroMemory(&td, sizeof(td));
	td.Width = WND_W;
	td.Height = WND_H;
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.Format = textureFormat;
	td.SampleDesc.Count = 4;
	td.SampleDesc.Quality = 0;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	td.CPUAccessFlags = 0;
	td.MiscFlags = 0;

	hr = m_pDevice->CreateTexture2D(&td, NULL, &m_pDepthStencilTexture);
	if (FAILED(hr)){
		ShowError(__FILE__, __LINE__);
		return hr;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory(&dsvd, sizeof(dsvd));
	dsvd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	if (multiSampleCount == 0)
	{
		dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dsvd.Texture2D.MipSlice = 0;
	}
	else
	{
		dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	}

	hr = m_pDevice->CreateDepthStencilView(m_pDepthStencilTexture, &dsvd, &m_pDepthStencilView);
	if (FAILED(hr)){
		ShowError(__FILE__, __LINE__);
		return hr;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
	ZeroMemory(&srvd, sizeof(srvd));
	srvd.Format = resourceFormat;

	if (multiSampleCount == 0)
	{
		srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvd.Texture2D.MostDetailedMip = 0;
		srvd.Texture2D.MipLevels = 1;
	}
	else
	{
		srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
	}

	hr = m_pDevice->CreateShaderResourceView(m_pDepthStencilTexture, &srvd, &m_pShaderResourceView);
	if (FAILED(hr))
	{
		ShowError(__FILE__, __LINE__);
		return hr;
	}

	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);
	
	m_viewPort.TopLeftX = 0;
	m_viewPort.TopLeftY = 0;
	m_viewPort.Width = WND_W;
	m_viewPort.Height = WND_H;
	m_viewPort.MinDepth = 0.0f;
	m_viewPort.MaxDepth = 1.0f;

	m_pDeviceContext->RSSetViewports(1, &m_viewPort);


	if (FAILED(hr)){
		ShowError(__FILE__, __LINE__);
		return hr;
	}

	return S_OK;
}

HRESULT Game::CompileShaders(){
	ID3DBlob* pVSBlob = NULL;
	HRESULT hr;
	
	hr = CompileShaderFromFile(_T("./Simple.fx"), "VSFunc", "vs_4_0", &pVSBlob);

	if (FAILED(hr))
	{
		ShowError(__FILE__, __LINE__);
		return E_FAIL;
	}

	hr = m_pDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &m_pVertexShader);
	if (FAILED(hr))
	{
		SafeRelease(pVSBlob);
		ShowError(__FILE__, __LINE__);
		return E_FAIL;
	}

	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	UINT numElements = ARRAYSIZE(layout);

	hr = m_pDevice->CreateInputLayout(
		layout,
		numElements,
		pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(),
		&m_pInputLayout
		);
	
	SafeRelease(pVSBlob);

	if (FAILED(hr))
	{
		ShowError(__FILE__, __LINE__);
		return E_FAIL;
	}

	m_pDeviceContext->IASetInputLayout(m_pInputLayout);

	{
		ID3DBlob* pGSBlob = NULL;

		hr = CompileShaderFromFile(_T("Simple.fx"), "GSFunc", "gs_4_0", &pGSBlob);

		if (FAILED(hr))
		{
			ShowError(__FILE__, __LINE__);
			return E_FAIL;
		}

		hr = m_pDevice->CreateGeometryShader(
			pGSBlob->GetBufferPointer(),
			pGSBlob->GetBufferSize(),
			NULL,
			&m_pGeometryShader
			);

		SafeRelease(pGSBlob);
		if (FAILED(hr))
		{
			ShowError(__FILE__, __LINE__);
			return E_FAIL;
		}
	}
	{
		ID3DBlob* pPSBlob = NULL;
		hr = CompileShaderFromFile(_T("Simple.fx"), "PSFunc", "ps_4_0", &pPSBlob);
		if (FAILED(hr))
		{
			ShowError(__FILE__, __LINE__);
			return E_FAIL;
		}

		hr = m_pDevice->CreatePixelShader(
			pPSBlob->GetBufferPointer(),
			pPSBlob->GetBufferSize(),
			NULL,
			&m_pPixelShader
		);

		SafeRelease(pPSBlob);
		if (FAILED(hr))
		{
			ShowError(__FILE__, __LINE__);
			return E_FAIL;
		}
	}

	CustomVertex vertices[] = {
		DirectX::XMFLOAT3(0.0f, 0.5f, 0.0f),
		DirectX::XMFLOAT3(0.5f, -0.5f, 0.0f),
		DirectX::XMFLOAT3(-0.5f, -0.5f, 0.0f),
	};

	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(CustomVertex) * ARRAYSIZE(vertices);
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA initData;
		ZeroMemory(&initData, sizeof(initData));
		initData.pSysMem = vertices;

		HRESULT hr;
		hr = m_pDevice->CreateBuffer(&bd, &initData, &m_pVertexBuffer);
		if (FAILED(hr))
		{
			ShowError(__FILE__, __LINE__);
			return hr;
		}

		UINT stride = sizeof(CustomVertex);
		UINT offset = 0;
		m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

		m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(ConstantBufferForPerFrame);
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = 0;

		hr = m_pDevice->CreateBuffer(&bd, NULL, &m_pConstantBuffer);
		if (FAILED(hr))
		{
			ShowError(__FILE__, __LINE__);
			return hr;
		}
	}

	{
		DirectX::XMVECTOR camPos = DirectX::XMVectorSet(0.0f, 0.0f, -0.75f, 0.0f);
		DirectX::XMVECTOR camTarget = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		DirectX::XMVECTOR camUpward = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

		m_View = DirectX::XMMatrixLookAtLH(camPos, camTarget, camUpward);

		FLOAT aspectRatio = (FLOAT)WND_W / (FLOAT)WND_H;
	}
	return hr;
}

HRESULT Game::ReleaseShaders()
{
	SafeRelease(m_pConstantBuffer);
	SafeRelease(m_pPixelShader);
	SafeRelease(m_pGeometryShader);
	SafeRelease(m_pInputLayout);
	SafeRelease(m_pVertexShader);

	return S_OK;
}

HRESULT Game::ReleaseD3DObjects()
{

	SafeRelease(m_pShaderResourceView);
	SafeRelease(m_pDepthStencilView);
	SafeRelease(m_pDepthStencilTexture);
	SafeRelease(m_pBackBuffer);
	SafeRelease(m_pDXGISwapChain);
	SafeRelease(m_pDXGIFactory);
	SafeRelease(m_pDXGIAdapter);
	SafeRelease(m_pDXGIDevice);
	SafeRelease(m_pDeviceContext);
	SafeRelease(m_pDevice);

	return S_OK;
}