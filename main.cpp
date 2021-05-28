
//ヘッダファイル読み込み
#include"game.h"		//ゲーム全体のヘッダファイル
#include "FPS.h"		//FPSの処理
#include "keyboard.h"	//キーボードの処理

//キャラクタ構造体
struct CHARACTOR
{
	int handle = -1;	//画像のハンドル（管理番号）
	char path[255];		//画像の場所（パス）
	int x;				//X位置
	int y;				//Y位置
	int width;			//幅	
	int height;			//高さ

	int speed = 50;		//移動速度

	RECT coll;			//当たり判定の領域（四角）
	BOOL IsDraw = FALSE;//画像が描画できる？
};



//グローバル変数
//シーンを管理する変数
GAME_SCENE GameScene;	//現在のゲームシーン
GAME_SCENE OldGameScene;	//前回のゲームシーン
GAME_SCENE NextGameScene;	//次のゲームシーン

//プレイヤー
CHARACTOR player;

//ゴール
CHARACTOR goal;

//画面の切り替え
BOOL IsFadeOut = FALSE;		//フェードアウト
BOOL IsFadeIn = FALSE;		//フェードイン

//シーン切り替え
int fadeTimeMill = 2000;	//切り替えミリ秒
int fadeTimeMax = fadeTimeMill / 1000 * 60;	//ミリ秒をフレーム秒に変換

//フェードアウト
int fadeOutCntInit = 0;	//初期値
int fadeOutCnt = fadeOutCntInit;	//フェードアウトのカウンタ
int fadeOutCntMax = fadeTimeMax;	//フェードアウトのカウンタMAX

//フェードイン
int fadeInCntInit = fadeTimeMax;	//初期値
int fadeInCnt = fadeInCntInit;	//フェードインのカウンタ
int fadeInCntMax = 0;	//フェードインのカウンタMAX

//プロトタイプ宣言
VOID Title(VOID);			//タイトル画面
VOID TitleProc(VOID);		//タイトル画面（処理）
VOID TitleDraw(VOID);		//タイトル画面（描画）

VOID Play(VOID);			//プレイ画面
VOID PlayProc(VOID);		//プレイ画面（処理）
VOID PlayDraw(VOID);		//プレイ画面（描画）

VOID End(VOID);			//エンド画面
VOID EndProc(VOID);		//エンド画面（処理）
VOID EndDraw(VOID);		//エンド画面（描画）

VOID Change(VOID);			//タイトル画面
VOID ChangeProc(VOID);		//タイトル画面（処理）
VOID ChangeDraw(VOID);		//タイトル画面（描画）

VOID ChangeScene(GAME_SCENE sece);	//シーン切り替え

VOID CollUpdatePlayer(CHARACTOR* chara);	//当たり判定
VOID CollUpdate(CHARACTOR* chara);	//当たり判定

BOOL OnCollRect(RECT a, RECT b);		//矩形と矩形の当たり判定

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
	SetBackgroundColor(255, 255, 255);						//デフォルトの背景色
	SetWindowIconID(GAME_ICON_ID);							//アイコンファイルを読み込み
	SetWindowStyleMode(GAME_WINDOW_BAR);					//ウィンドウバーの状態
	SetWaitVSyncFlag(TRUE);									//ディスプレイの垂直同期を有効にする
	SetAlwaysRunFlag(TRUE);									//ウィンドウをずっとアクティブにする

	// ＤＸライブラリ初期化処理
	if (DxLib_Init() == -1)
	{
		return -1;			// エラーが起きたら直ちに終了
	}


	//ダブルバッファリングを有効化
	SetDrawScreen(DX_SCREEN_BACK);

	//最初のシーンは、タイトル画面から
	GameScene = GAME_SCENE_TITLE;

	//ゲーム全体の初期化
	
	//プレイヤーの画像を読み込み
	strcatDx(player.path, ".\\image\\player2.png");	//パスのコピー
	player.handle = LoadGraph(player.path);	//画像の読み込み
	//画像が読み込めなかったときは、エラー（－1）が入る
	if (player.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//メインのウィンドウハンドル
			player.path,			//メッセージの本文
			"画像読み込みエラー！",//メッセージのタイトル
			MB_OK				//ボタン
		);

		DxLib_End();		//強制終了
		return -1;			//エラー処理
	}
	//画像の幅と高さを取得
	GetGraphSize(player.handle, &player.width, &player.height);

	/*当たり判定を更新する
	CollUpdatePlayer(&player);	//プレイヤーの当たり判定のアドレス
	*/
	//プレイヤーを初期化
	player.x = GAME_WIDTH / 2 - player.width / 2;	//中央寄せ
	player.y = GAME_HEIGHT / 2 - player.height / 2;	//中央寄せ
	player.speed = 500;		//スピード

	player.IsDraw = TRUE;	//描画できる！

	//当たり判定を更新する
	CollUpdatePlayer(&player);	//プレイヤーの当たり判定のアドレス

	//ゴールの画像を読み込み
	strcatDx(goal.path, ".\\image\\goal.png");	//パスのコピー
	goal.handle = LoadGraph(goal.path);	//画像の読み込み
	//画像が読み込めなかったときは、エラー（－1）が入る
	if (goal.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//メインのウィンドウハンドル
			goal.path,			//メッセージの本文
			"画像読み込みエラー！",//メッセージのタイトル
			MB_OK				//ボタン
		);

		DxLib_End();		//強制終了
		return -1;			//エラー処理
	}
	
	//画像の幅と高さを取得
	GetGraphSize(goal.handle, &goal.width, &goal.height);
	
	//ゴールを初期化
	goal.x = GAME_WIDTH - goal.width;	//中央寄せ
	goal.y = 0;	//中央寄せ
	goal.speed = 500;		//スピード
	goal.IsDraw = TRUE;	//描画できる！

	//当たり判定を更新する
	CollUpdate(&goal);	//ゴールの当たり判定のアドレス

	//無限ループ
	while (1)
	{
		//メッセージを受け取り続ける
		if (ProcessMessage() != 0) { break; }	//-1のとき、エラーやウィンドウが閉じられた
		if (ClearDrawScreen() != 0) { break; }	//画面を消去する

		//キーボード入力の更新
		AllKeyUpdate();

		//FPS値を更新
		FPSUpdate();

		//ESCキーで強制終了
		if (KeyClick(KEY_INPUT_ESCAPE) == TRUE) { break; }


		//以前のシーンを取得
		if (GameScene != GAME_SCENE_CHANGE)
		{
			OldGameScene = GameScene;
		}
		//シーンごとに処理を行う

		switch (GameScene)
		{
		case GAME_SCENE_TITLE:
			Title();		//タイトル画面
			break;
		case GAME_SCENE_PLAY:
			Play();		//プレイ画面
			break;
		case GAME_SCENE_END:
			End();		//エンド画面
			break;
		case GAME_SCENE_CHANGE:
			Change();		//切り替え画面
			break;
		default:
			break;
		}

		//シーンを切り替える
		if (OldGameScene != GameScene)
		{
			//現在のシーンが切り替え画面でないとき
			if (GameScene != GAME_SCENE_CHANGE)
			{
				NextGameScene = GameScene;	//次のシーンを保存
				GameScene = GAME_SCENE_CHANGE;	//画面切り替えシーンに帰る
			}
		}



		//FPS値を描画
		FPSDraw();

		//FPS値を待つ
		FPSWait();

		ScreenFlip();		//ダブルバッファリングした画面を描画
	}

	//終わるときの処理
	DeleteGraph(player.handle);	//画像をメモリ上から削除
	DeleteGraph(goal.handle);	//画像をメモリ上から削除

	DxLib_End();		// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}

/// <summary>
/// シーンを切り替える関数
/// </summary>
/// <param name="sene"></param>
VOID ChangeScene(GAME_SCENE scene)
{
	//	プレイシーンへ切り替える

	GameScene = scene;	//切り替え
	IsFadeIn = FALSE;	//フェードインしない
	IsFadeOut = TRUE;	//フェードアウトする

	return;
}


/// <summary>
/// タイトル画面
/// </summary>
VOID Title(VOID)
{
	TitleProc();		//処理
	TitleDraw();		//描画

	return;
}
/// <summary>
/// タイトル画面の処理
/// </summary>
/// <param name=""></param>
VOID TitleProc(VOID)
{
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//シーンの切り替え
		//次のシーンの初期化をここで行うと楽

		//プレイ画面に切り替え
		ChangeScene(GAME_SCENE_PLAY);
	}
	return;
}

/// <summary>
/// タイトル画面の描画
/// </summary>
/// <param name=""></param>
VOID TitleDraw(VOID)
{
	DrawString(0, 0, "タイトル画面", GetColor(0, 0, 0));
	return;
}

/// <summary>
/// プレイ画面
/// </summary>
VOID Play(VOID)
{
	PlayProc();		//処理
	PlayDraw();		//描画

	return;
}
/// <summary>
/// プレイ画面の処理
/// </summary>
/// <param name=""></param>
VOID PlayProc(VOID)
{
	/*if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//シーンの切り替え
		//次のシーンの初期化をここで行うと楽

		//エンド画面に切り替え
		ChangeScene(GAME_SCENE_END);
	}
	*/
	
	//プレイヤーの操作

	if (KeyDown(KEY_INPUT_W) == TRUE)				//「W」のところがUPだと矢印キー
	{
		player.y -= player.speed * fps.DeltaTime;
	}
	if (KeyDown(KEY_INPUT_S) == TRUE)				//「S」のところがDOWNだと矢印キー
	{
		player.y += player.speed * fps.DeltaTime;
	}
	if (KeyDown(KEY_INPUT_A) == TRUE)				//「A」のところがLEFTだと矢印キー
	{
		player.x -= player.speed * fps.DeltaTime;
	}
	if (KeyDown(KEY_INPUT_D) == TRUE)				//「D」のところがRIGHTだと矢印キー	
	{
		player.x += player.speed * fps.DeltaTime;
	}

	//当たり判定を更新する
	CollUpdatePlayer(&player);

	//ゴールの当たり判定を更新する
	CollUpdate(&goal);

	//プレイヤーがゴールに当たった時は
	if (OnCollRect(player.coll, goal.coll) == TRUE)
	{
		//エンド画面に切り替え
		ChangeScene(GAME_SCENE_END);
		return;				//処理終了
	}

	return;
}

/// <summary>
/// プレイ画面の描画
/// </summary>
/// <param name=""></param>
VOID PlayDraw(VOID)
{
	//プレイヤーを描画
	if (player.IsDraw == TRUE)
	{
		//画像を描画
		DrawGraph(player.x, player.y, player.handle, TRUE);

		//デバッグの時は、当たり判定の領域を描画
		if (GAME_DEBUG == TRUE)
		{
			//四角を描画
			DrawBox(player.coll.left, player.coll.top, player.coll.right, player.coll.bottom,
				GetColor(255, 0, 0), FALSE);
		}
	}

	//ゴールを描画
	if (goal.IsDraw == TRUE)
	{
		//画像を描画
		DrawGraph(goal.x, goal.y, goal.handle, TRUE);

		//デバッグの時は、当たり判定の領域を描画
		if (GAME_DEBUG == TRUE)
		{
			//四角を描画
			DrawBox(goal.coll.left, goal.coll.top, goal.coll.right, goal.coll.bottom,
				GetColor(255, 0, 0), FALSE);
		}
	}

	DrawString(0, 0, "プレイ画面", GetColor(0, 0, 0));
	return;
}

/// <summary>
/// エンド画面
/// </summary>
VOID End(VOID)
{
	EndProc();		//処理
	EndDraw();		//描画

	return;
}
/// <summary>
/// エンド画面の処理
/// </summary>
VOID EndProc(VOID)
{

	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//シーンの切り替え
		//次のシーンの初期化をここで行うと楽

		//タイトル画面に切り替え
		ChangeScene(GAME_SCENE_TITLE);
	}
	return;
}

/// <summary>
/// エンド画面の描画
/// </summary>
/// <param name=""></param>
VOID EndDraw(VOID)
{

	DrawString(0, 0, "エンド画面", GetColor(0, 0, 0));
	return;
}

/// <summary>
/// 切り替え画面
/// </summary>
VOID Change(VOID)
{
	ChangeProc();		//処理
	ChangeDraw();		//描画

	return;
}
/// <summary>
/// 切り替え画面の処理
/// </summary>
VOID ChangeProc(VOID)
{

	//フェードイン
	if (IsFadeIn == TRUE)
	{

		if (fadeInCnt > fadeInCntMax)
		{
			fadeInCnt--;		//カウンタを減らす
		}
		else
		{
			//フェードイン処理が終わった
			fadeInCnt = fadeInCntInit;	//カウンタ初期化
			IsFadeIn = FALSE;			//フェードイン終了処理
		}
	}

	//フェードアウト
	if (IsFadeOut == TRUE)
	{

		if (fadeOutCnt < fadeOutCntMax)
		{
			fadeOutCnt++;		//カウンタを増やす
		}
		else
		{
			//フェードアウト処理が終わった
			fadeOutCnt = fadeOutCntInit;	//カウンタ初期化
			IsFadeOut = FALSE;			//フェードアウト終了処理
		}
	}

	//切り替え処理終了
	if (IsFadeIn == FALSE && IsFadeOut == FALSE)
	{
		//フェードインしていない、フェードアウトもしていないとき
		GameScene = NextGameScene;	//次のシーンに切り替え
		OldGameScene = GameScene;	//以前のゲームシーン更新
	}

	return;
}

/// <summary>
/// 切り替え画面の描画
/// </summary>
/// <param name=""></param>
VOID ChangeDraw(VOID)
{
	//以前のシーンを描画
	switch (OldGameScene)
	{
	case GAME_SCENE_TITLE:
		TitleDraw();	//タイトル画面の描画
		break;
	case GAME_SCENE_PLAY:
		PlayDraw();	//プレイ画面の描画
		break;
	case GAME_SCENE_END:
		EndDraw();	//エンド画面の描画
		break;
	default:
		break;
	}

	//フェードイン
	if (IsFadeIn == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeInCnt / fadeInCntMax) * 255);
	}

	//フェードアウト
	if (IsFadeOut == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeOutCnt / fadeOutCntMax) * 255);
	}

	//四角を描画
	DrawBox(0, 0, GAME_WIDTH, GAME_HEIGHT, GetColor(0, 0, 0), TRUE);

	//半透明終了
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawString(0, 0, "切り替え画面", GetColor(255, 0, 0));
	return;
}

/// <summary>
/// 当たり判定の領域更新
/// </summary>
/// <param name="Coll">当たり判定の領域</param>
VOID CollUpdatePlayer(CHARACTOR* chara)
{
	chara->coll.left = chara->x + 20;							 //当たり判定の微調整
	chara->coll.top = chara->y;							 //当たり判定の微調整
	chara->coll.right = chara->x + chara->width - 20;			 //当たり判定の微調整
	chara->coll.bottom = chara->y + chara->height;			 //当たり判定の微調整

	return;
}

/// <summary>
/// 当たり判定の領域
/// </summary>
/// <param name="Coll">当たり判定の領域</param>
VOID CollUpdate(CHARACTOR* chara)
{
	chara->coll.left = chara->x ;							 //当たり判定の微調整
	chara->coll.top = chara->y  ;							 //当たり判定の微調整
	chara->coll.right= chara->x + chara->width ;			 //当たり判定の微調整
	chara->coll.bottom = chara->y + chara->height ;			 //当たり判定の微調整

	return;
}

/// <summary>
/// 矩形と矩形の当たり判定
/// </summary>
/// <param name="a">矩形A</param>
/// <param name="b">矩形B</param>
/// <returns>当たったらTRUE/当たらなかったらFALSE</returns>
BOOL OnCollRect(RECT a, RECT b)
{
	if (
		 a.left < b.right&&		//矩形Aの左辺X座標　＜　矩形Ｂの右辺X座標　かつ
		 a.right > b.left&&		//矩形Aの右辺X座標　＜　矩形Ｂの左辺X座標　かつ
		 a.top < b.bottom&&		//矩形Aの上辺Y座標　＜　矩形Ｂの下辺Y座標　かつ
		 a.bottom > b.top		//矩形Aの下辺Y座標　＜　矩形Ｂの上辺Y座標　
		)
	{
		//当たっているとき
		return TRUE;
	}
	else {
		//当たっていないとき
		return FALSE;
	}
}