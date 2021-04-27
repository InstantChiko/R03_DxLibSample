//�w�b�_�[�t�@�C���̓ǂݍ���
#include "DxLib.h"	//DxLib���g�����͕K�v
#include <stdio.h>

//�}�N����`
#define GAME_TITLE	"�Q�[���^�C�g��"	//�Q�[���^�C�g���i����܂�����j
#define GAME_WIDTH	1280				//�Q�[����ʂ̕��i�E�B�h�X�j
#define GAME_HEIGHT	720					//�Q�[����ʂ̍����i�n�C�g�j
#define GAME_COLOR	32					//�Q�[���̐F��


// �v���O������ WinMain ����n�܂�܂�
//Windows�̃v���O�������@�iWinAPI�j�œ����Ă���
//DxLib�́ADirectX�Ƃ����A�Q�[���v���O���~���O���ȒP�Ɏg����d�g��

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ChangeWindowMode(TRUE);					//�E�B���h�E���[�h�ɐݒ�
	SetMainWindowText("����܂�����");	//�E�B���h�E�̃^�C�g���̕���
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);	//�E�B���h�E�̉𑜓x��ݒ�
	SetWindowSize(GAME_WIDTH, GAME_HEIGHT);		//�E�B���h�E�̂̑傫����ݒ�
	SetBackgroundColor(255, 255,255);			//�f�t�H���g�̔w�i�F
	

	// �c�w���C�u��������������
	if (DxLib_Init() == -1)		
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}

	//�������[�v
	while (1)
	{
		if (CheckHitKeyAll() != 0)
		{
			break;	//�������[�v�𔲂���
		}
		//���b�Z�[�W���󂯎�葱����
		if (ProcessMessage() != 0)	//-1�̂Ƃ��A�G���[��E�B���h�E������ꂽ
		{
			break;	//�������[�v�𔲂���
		}
	}

	// �c�w���C�u�����g�p�̏I������
	DxLib_End();
	return 0;				// �\�t�g�̏I�� 
}