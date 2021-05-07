//ヘッダーファイルの読み込み
#include "DxLib.h"	//DxLibを使う時は必要
#include <stdio.h>

//マクロ定義
#define GAME_TITLE	"ゲームタイトル"	//ゲームタイトル（お先まっくら）
#define GAME_WIDTH	860				//ゲーム画面の幅（ウィドス）
#define GAME_HEIGHT	720				//ゲーム画面の高さ（ハイト）
#define GAME_COLOR	32					//ゲームの色域

#define GAME_ICON_ID	333				//ゲームのICONのID

#define GAME_WINDOW_BAR 0				//ウィンドウバーの種類

// プログラムは WinMain から始まります
//Windowsのプログラム方法（WinAPI）で動いている
//DxLibは、DirectXという、ゲームプログラミングを簡単に使える仕組み

//ウィンドウの設定はInitの前に書く
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetOutApplicationLogValidFlag(FALSE);					//Log.txtを出力しない
	ChangeWindowMode(TRUE);									//ウィンドウモードに設定
	SetMainWindowText("ゲームタイトル");					//ウィンドウのタイトルの文字
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);		//ウィンドウの解像度を設定
	SetWindowSize(GAME_WIDTH, GAME_HEIGHT);					//ウィンドウのの大きさを設定
	SetBackgroundColor(100,0,150);							//デフォルトの背景色
	SetWindowIconID(GAME_ICON_ID);							//アイコンファイルを読み込み
	SetWindowStyleMode(GAME_WINDOW_BAR);					//ウィンドウバーの状態
	SetWaitVSyncFlag(TRUE);									//ディスプレイの垂直同期を有効にする
	SetAlwaysRunFlag(TRUE);									//ウィンドウをずっとアクティブにする

	// ＤＸライブラリ初期化処理
	if (DxLib_Init() == -1)		
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	//四角の位置を決める
	int X = GAME_WIDTH / 2;
	int Y = GAME_HEIGHT / 2;

	
	//速度
	int Speed =2;
	int XSpeed = Speed;
	int YSpeed = Speed;

//円の半径を決める
	int radius = 50;

	//四角の大きさを決める
	int width = 32; //幅
	int height = 32; //高さ

	SetDrawScreen(DX_SCREEN_BACK);

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

		//画面を消去する
		if (ClearDrawScreen() != 0) { break; }

	//四角を描画
		//DrawBox(
		//	X, Y, X + width, Y + height,
		//	GetColor(255, 255, 0),	//色を取得
		//	TRUE					//塗りつぶし？
		//);
		DrawCircle(X, Y, radius,
			GetColor(0, 255, 0),
			FALSE, 5
		);

		


		X += XSpeed;//四角の位置を右にずらす
	
		Y += YSpeed;//四角の位置を下にずらす

		if (X - radius < 0 || X + radius > GAME_WIDTH) {	//画面の横から出たとき
			//移動速度の符号を反転させる
			XSpeed = -XSpeed; //移動する向きを反転
		if (XSpeed > 0) { XSpeed += 1;}
			else if (XSpeed < 0) { XSpeed -= 1;}
		}	
		if (Y - radius < 0 || Y + radius > GAME_HEIGHT) {	//画面の縦から出たとき
			//移動速度の符号を反転させる
			YSpeed = -YSpeed; //移動する向きを反転
			if (YSpeed > 0) { YSpeed += 1; }
			else if (YSpeed < 0) { YSpeed -= 1; }

			
		}


	//四角を移動
			
			ScreenFlip();		//ダブルバッファリングした画面を描画
			
			}
			
			
		
	

	

	// ＤＸライブラリ使用の終了処理
	DxLib_End();
	return 0;				// ソフトの終了 
}