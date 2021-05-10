#pragma once
//�w�b�_�t�@�C����ǂݍ���
#include "DxLib.h"

//�L�[�{�[�h�̃w�b�_�t�@�C��

//�}�N����`
#define KEY_KIND_MAX	256


//�}�N����`
struct KEYBOARD
{
	char TempKeyState[KEY_KIND_MAX];	//���͏�Ԃ��Ǘ�
	int AllKeyState[KEY_KIND_MAX];
	int OldAllKeyState[KEY_KIND_MAX];
};

//�O���̃O���[�o���ϐ�
extern KEYBOARD keyboard;

//�v���g�^�C�v�錾
extern VOID MuAllKeyDownUpdate(VOID);	//���ׂẴL�[�̏�Ԃ��擾����
extern BOOL KeyDown(int KYE_INPUT_);	//����̃L�[�������Ă��邩�H
extern BOOL KeyUp(int KYE_INPUT_);		//����̃L�[���グ�Ă��邩�H
extern BOOL KeyClick(int KYE_INPUT_);	//����̃L�[���N���b�N�������H
extern BOOL KeyDownKeep(int KYE_INPUT_,int millSec);	//����̃L�[���w��̃~���b���������Ă��邩�H