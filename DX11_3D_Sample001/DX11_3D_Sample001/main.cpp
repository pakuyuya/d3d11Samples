#include <Windows.h>
#include "Game.hpp"
#include <tchar.h>

int __stdcall _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR pCmd, int nShowWnd)
{
	Game game;
	if (FAILED(game.InitWnd(hInstance)))
		return -1;

	if (FAILED(game.InitD3DObjects()))
		return -1;

	if (FAILED(game.CompileShaders()))
		return -1;

	game.enterWindowLoop();
	
	return 0;
}