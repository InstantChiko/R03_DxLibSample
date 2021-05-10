#pragma once
//ヘッダファイルを読み込み
#include "DxLib.h"

//キーボードのヘッダファイル

//マクロ定義
#define KEY_KIND_MAX	256


//マクロ定義
struct KEYBOARD
{
	char TempKeyState[KEY_KIND_MAX];	//入力状態を管理
	int AllKeyState[KEY_KIND_MAX];
	int OldAllKeyState[KEY_KIND_MAX];
};

//外部のグローバル変数
extern KEYBOARD keyboard;

//プロトタイプ宣言
extern VOID MuAllKeyDownUpdate(VOID);	//すべてのキーの状態を取得する
extern BOOL KeyDown(int KYE_INPUT_);	//特定のキーを押しているか？
extern BOOL KeyUp(int KYE_INPUT_);		//特定のキーを上げているか？
extern BOOL KeyClick(int KYE_INPUT_);	//特定のキーをクリックしたか？
extern BOOL KeyDownKeep(int KYE_INPUT_,int millSec);	//特定のキーを指定のミリ秒押し続けているか？