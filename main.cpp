//�w�b�_�[�t�@�C���̓ǂݍ���
#include "DxLib.h"	//DxLib���g�����͕K�v
#include <stdio.h>

//�}�N����`
#define GAME_TITLE	"�Q�[���^�C�g��"	//�Q�[���^�C�g���i����܂�����j
#define GAME_WIDTH	860				//�Q�[����ʂ̕��i�E�B�h�X�j
#define GAME_HEIGHT	720				//�Q�[����ʂ̍����i�n�C�g�j
#define GAME_COLOR	32					//�Q�[���̐F��

#define GAME_ICON_ID	333				//�Q�[����ICON��ID

#define GAME_WINDOW_BAR 0				//�E�B���h�E�o�[�̎��

// �v���O������ WinMain ����n�܂�܂�
//Windows�̃v���O�������@�iWinAPI�j�œ����Ă���
//DxLib�́ADirectX�Ƃ����A�Q�[���v���O���~���O���ȒP�Ɏg����d�g��

//�E�B���h�E�̐ݒ��Init�̑O�ɏ���
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetOutApplicationLogValidFlag(FALSE);					//Log.txt���o�͂��Ȃ�
	ChangeWindowMode(TRUE);									//�E�B���h�E���[�h�ɐݒ�
	SetMainWindowText("�Q�[���^�C�g��");					//�E�B���h�E�̃^�C�g���̕���
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);		//�E�B���h�E�̉𑜓x��ݒ�
	SetWindowSize(GAME_WIDTH, GAME_HEIGHT);					//�E�B���h�E�̂̑傫����ݒ�
	SetBackgroundColor(100,0,150);							//�f�t�H���g�̔w�i�F
	SetWindowIconID(GAME_ICON_ID);							//�A�C�R���t�@�C����ǂݍ���
	SetWindowStyleMode(GAME_WINDOW_BAR);					//�E�B���h�E�o�[�̏��
	SetWaitVSyncFlag(TRUE);									//�f�B�X�v���C�̐���������L���ɂ���
	SetAlwaysRunFlag(TRUE);									//�E�B���h�E�������ƃA�N�e�B�u�ɂ���

	// �c�w���C�u��������������
	if (DxLib_Init() == -1)		
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}

	//�l�p�̈ʒu�����߂�
	int X = GAME_WIDTH / 2;
	int Y = GAME_HEIGHT / 2;

	
	//���x
	int Speed =2;
	int XSpeed = Speed;
	int YSpeed = Speed;

//�~�̔��a�����߂�
	int radius = 50;

	//�l�p�̑傫�������߂�
	int width = 32; //��
	int height = 32; //����

	SetDrawScreen(DX_SCREEN_BACK);

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

		//��ʂ���������
		if (ClearDrawScreen() != 0) { break; }

	//�l�p��`��
		//DrawBox(
		//	X, Y, X + width, Y + height,
		//	GetColor(255, 255, 0),	//�F���擾
		//	TRUE					//�h��Ԃ��H
		//);
		DrawCircle(X, Y, radius,
			GetColor(0, 255, 0),
			FALSE, 5
		);

		


		X += XSpeed;//�l�p�̈ʒu���E�ɂ��炷
	
		Y += YSpeed;//�l�p�̈ʒu�����ɂ��炷

		if (X - radius < 0 || X + radius > GAME_WIDTH) {	//��ʂ̉�����o���Ƃ�
			//�ړ����x�̕����𔽓]������
			XSpeed = -XSpeed; //�ړ���������𔽓]
		if (XSpeed > 0) { XSpeed += 1;}
			else if (XSpeed < 0) { XSpeed -= 1;}
		}	
		if (Y - radius < 0 || Y + radius > GAME_HEIGHT) {	//��ʂ̏c����o���Ƃ�
			//�ړ����x�̕����𔽓]������
			YSpeed = -YSpeed; //�ړ���������𔽓]
			if (YSpeed > 0) { YSpeed += 1; }
			else if (YSpeed < 0) { YSpeed -= 1; }

			
		}


	//�l�p���ړ�
			
			ScreenFlip();		//�_�u���o�b�t�@�����O������ʂ�`��
			
			}
			
			
		
	

	

	// �c�w���C�u�����g�p�̏I������
	DxLib_End();
	return 0;				// �\�t�g�̏I�� 
}