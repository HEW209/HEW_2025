//main.cpp
#include <Windows.h>
#include <iostream>
#pragma comment(lib, "winmm.lib")

#include "Config.h"
#include <DXGameFrame.h>
#include "SampleScene.h"
#include "GridDebugScene.h"


#include"PlayerMoveDebugScene.h"


//ウィンドウプロシージャ
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lPalam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	WNDCLASSEX wcex;	//ウィンドウクラス
	HWND hWnd;			//ウィンドウハンドル
	MSG message;		//メッセージ

	srand(time(0));

	//----------------------------------------
	//		ウィンドウクラス情報を設定
	//----------------------------------------
	ZeroMemory(&wcex, sizeof(wcex));
	wcex.hInstance = hInstance;									//インスタンスハンドル
	wcex.lpszClassName = "Class Name";							//ウィンドウクラス名
	wcex.lpfnWndProc = WndProc;									//ウィンドウプロシージャへのポインタ
	wcex.style = CS_HREDRAW | CS_VREDRAW;						//ウィンドウの挙動
	wcex.cbSize = sizeof(WNDCLASSEX);							//構造体のサイズ
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);				//アプリアイコン
	wcex.hIconSm = wcex.hIcon;									//アプリアイコン(小)
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);					//カーソルアイコン
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	//背景の色


	//---------------------------------------
	//		ウィンドウクラス情報の登録
	//---------------------------------------
	if (!RegisterClassEx(&wcex))
	{
		Debug::ErrorMessage("ウィンドウクラスの登録に失敗しました");
		return 0;
	}


	//-----------------------------
	//		ウィンドウの作成
	//-----------------------------
	hWnd = CreateWindowEx(
		WS_EX_OVERLAPPEDWINDOW,				//ウィンドウの見た目(Exからの追加スタイル)
		wcex.lpszClassName,					//ウィンドウクラス名
		"タイトル",							//ウィンドウのタイトル
		WS_CAPTION | WS_SYSMENU,			//ウィンドウの見た目(スタイル)
		CW_USEDEFAULT, CW_USEDEFAULT,		//ウィンドウの位置
		WindowWidth, WindowHeight,			//ウィンドウのサイズ
		HWND_DESKTOP,						//親ウィンドウ
		NULL,								//子ウィンドウ又はメニュー
		hInstance, NULL
	);

	//エラーチェック
	if (hWnd == NULL) {
		Debug::ErrorMessage("ウィンドウの生成に失敗しました");
		return 0;
	}


	//-----------------------------
	//		ウィンドウの表示
	//-----------------------------
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);


	//---------------------------------
	//		Direct3Dクラスの作成
	//---------------------------------
	if (FAILED(Direct3D::Instance().Init(hWnd, WindowWidth, WindowHeight, false)))
	{
		Debug::ErrorMessage("Direct3Dの初期化に失敗しました");
		return 0;
	}

	//-----------------------
	//		シーンの作成
	//-----------------------
	//SceneManager::Init(std::make_unique<SampleScene>());
	SceneManager::Init(std::make_unique<GridDebugScene>());

	//------------------------
	//		FPS固定の準備
	//------------------------
	timeBeginPeriod(1);				//分解能 (１ミリ秒)

	int nExecLastTime;				//最終実行時間
	int nCrrentTime;				//現在時間

	nExecLastTime = nCrrentTime = timeGetTime();

	//-----------------------------
	//		メッセージループ
	//-----------------------------
	while (1)
	{
		if (PeekMessage(&message, NULL, 0, 0, PM_NOREMOVE))
		{
			if (!GetMessage(&message, NULL, 0, 0))
			{
				break;
			}
			else
			{
				TranslateMessage(&message);
				DispatchMessage(&message);
			}
		}
		else
		{
			//---------------------
			//		FPSを固定
			//---------------------
			//現在の時間を取得
			nCrrentTime = timeGetTime();

			//FPS固定
			if (nCrrentTime - nExecLastTime < 1000.0f / FPS)
			{
				continue;
			}

			//最終実行時間を更新
			nExecLastTime = nCrrentTime;


			//----------------------------------
			//			ゲームの処理
			//----------------------------------
			InputManager::Update();
			SceneManager::Execute();
		}
	}


	//----------------------------------
	//			リソースの解放
	//----------------------------------
	SceneManager::Uninit();
	return 0;
}


//ウィンドウプロシージャ
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lPalam)
{
	switch (message)
	{
	case WM_CLOSE:
		if (IDNO == MessageBox(hWnd, "終了しますか？", "終了確認", MB_YESNO))
		{
			return 0;
		}
		break;

	case WM_DESTROY:
		//スレッドの終了をシステムに伝える
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lPalam);
}