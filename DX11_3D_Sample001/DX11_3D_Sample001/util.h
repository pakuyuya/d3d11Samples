#pragma once
#include <Windows.h>
#include <stdio.h>
#include <d3d11.h>
#include <D3DCompiler.h>
#include <string.h>

template<class T> void SafeRelease(T*& p){
	if (p){
		(p)->Release();
		p = NULL;
	}
}

template<class T>
class RELEASE_OBJ{
	T* m_p;
public:
	RELEASE_OBJ():m_p(NULL){}
	RELEASE_OBJ(T*& p) : m_p(p){}
	~RELEASE_OBJ(){
		if (!m_p){
			m_p->Release();
			m_p = NULL;
		}
	}
	
	RELEASE_OBJ& operator=(T*& p){
		m_p = p;
	}

	T& operator->(){
		return *m_p;
	}
};

namespace{
	inline void GetLastWindowsError(char buf[], int arySize = 128){
		int errCode = GetLastError();
		FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, NULL, errCode, 0, buf, arySize, NULL);
	}
	inline void ShowError(char *file, int line, char *msg = "No message"){
		char szMsg[4096];
		sprintf_s(szMsg, "[***************Error***************]\n[Source] %s:%d\n[MSG] %s", file, line, msg);

		MessageBoxA(NULL, szMsg, "ERROR", MB_OK);
	}
}

inline HRESULT CompileShaderFromFile
(
TCHAR*      szFileName,
LPCSTR      szEntryPoint,
LPCSTR      szShaderModel,
ID3DBlob**  ppBlobOut
)
{
	HRESULT hr;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

#if defined(NDEBUG) || defined(_NDEBUG)
	dwShaderFlags |= D3DCOMPILE_OPTIMIZATION_LEFEL3;
#endif

	ID3DBlob* pErrorBlob = NULL;

	hr = D3DCompileFromFile(
		szFileName,
		NULL,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		szEntryPoint,
		szShaderModel,
		dwShaderFlags,
		0,
		ppBlobOut,
		&pErrorBlob
		);

	if (FAILED(hr))
	{
		if (pErrorBlob != NULL)
		{
			ShowError(__FILE__, __LINE__, (char*)pErrorBlob->GetBufferPointer());
		}
	}

	if (pErrorBlob)
	{
		pErrorBlob->Release();
		pErrorBlob = NULL;
	}

	return hr;
}
void GetReturnCodeMessage(HRESULT hr, char *buf)
{
	if (hr == D3D11_ERROR_FILE_NOT_FOUND){ strcpy_s(buf, 128, "D3D11_ERROR_FILE_NOT_FOUND"); }
	else if (hr == D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS){ strcpy_s(buf, 128, "D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS"); }
	else if (hr == D3D11_ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS){ strcpy_s(buf, 128, "D3D11_ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS"); }
	else if (hr == D3D11_ERROR_DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD){ strcpy_s(buf, 128, "D3D11_ERROR_DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD"); }
	else if (hr == DXGI_ERROR_INVALID_CALL){ strcpy_s(buf, 128, "DXGI_ERROR_INVALID_CALL"); }
	else if (hr == DXGI_ERROR_WAS_STILL_DRAWING){ strcpy_s(buf, 128, "replaced with DXGI_ERROR_WAS_STILL_DRAWING"); }
	else if (hr == E_FAIL){ strcpy_s(buf, 128, "E_FAIL"); }
	else if (hr == E_INVALIDARG){ strcpy_s(buf, 128, "E_INVALIDARG"); }
	else if (hr == E_OUTOFMEMORY){ strcpy_s(buf, 128, "E_OUTOFMEMORY"); }
	else if (hr == E_NOTIMPL){ strcpy_s(buf, 128, "E_NOTIMPL"); }
	else if (hr == S_FALSE){ strcpy_s(buf, 128, "S_FALSE"); }
	else if (hr == S_OK){ strcpy_s(buf, 128, "S_OK"); }
	else{ strcpy_s(buf, 128, "other error"); }
}