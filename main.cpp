//ヘッダーファイルの読み込み
#include "DxLib.h"	//DxLibを使う時は必要
#include <stdio.h>

//マクロ定義
#define GAME_TITLE	"ゲームタイトル"	//ゲームタイトル（お先まっくら）
#define GAME_WIDTH	1280				//ゲーム画面の幅（ウィドス）
#define GAME_HEIGHT	720					//ゲーム画面の高さ（ハイト）
#define GAME_COLOR	32					//ゲームの色域


// プログラムは WinMain から始まります
//Windowsのプログラム方法（WinAPI）で動いている
//DxLibは、DirectXという、ゲームプログラミングを簡単に使える仕組み

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ChangeWindowMode(TRUE);					//ウィンドウモードに設定
	SetMainWindowText("お先まっくら");	//ウィンドウのタイトルの文字
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);	//ウィンドウの解像度を設定
	SetWindowSize(GAME_WIDTH, GAME_HEIGHT);		//ウィンドウのの大きさを設定
	SetBackgroundColor(255, 255,255);			//デフォルトの背景色
	

	// ＤＸライブラリ初期化処理
	if (DxLib_Init() == -1)		
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	//無限ループ
	while (1)
	{
		if (CheckHitKeyAll() != 0)
		{
			break;	//無限ループを抜ける
		}
		//メッセージを受け取り続ける
		if (ProcessMessage() != 0)	//-1のとき、エラーやウィンドウが閉じられた
		{
			break;	//無限ループを抜ける
		}
	}

	// ＤＸライブラリ使用の終了処理
	DxLib_End();
	return 0;				// ソフトの終了 
}