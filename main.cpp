#define _CRT_SECURE_NO_WARNINGS

//�w�b�_�t�@�C���ǂݍ���
#include "game.h"		//�Q�[���S�̂̃w�b�_�t�@�C��
#include "keyboard.h"	//�L�[�{�[�h�̏���
#include "FPS.h"		//FPS�̏���

//�\���̂̒�`

//�^�C�g���摜�̍\����
struct IMAGE
{
	int handle = -1;	//�摜�̃n���h��(�Ǘ��ԍ�)
	char path[255];		//�摜�̏ꏊ(�p�X)

	int x;				//X�ʒu
	int y;				//Y�ʒu
	int width;			//��
	int height;			//����

	int speed = 1;		//�ړ����x

	BOOL IsDraw = FALSE;//�摜���`��ł���H
};

//�L�����N�^�̍\����
struct CHARACTOR
{
	IMAGE img;		//�摜�\����
	int speed = 1;	//�ړ����x
	RECT coll;		//�����蔻��̗̈�i�l�p�j
};



//����̍\����
struct MOVIE
{
	int handle = -1;	//����̃n���h��
	char path[255];		//����̃p�X

	int x;				//X�ʒu
	int y;				//Y�ʒu
	int width;			//��
	int height;			//����

	int Volume = 255;	//�{�����[��(�ŏ�)0�`255(�ő�)�@
};

//���y�̍\����
struct AUDIO
{
	int handle = -1;		//���y�̃n���h��
	char path[255];			//���y�̃p�X

	int Volume = -1;		//�{�����[���iMIN �O�@�`�@�Q�T�TMAX�j
	int playType = -1;		// BGM or SE
};




//�O���[�o���ϐ�
//�V�[�����Ǘ�����ϐ�
GAME_SCENE GameScene;		//���݂̃Q�[���̃V�[��
GAME_SCENE OldGameScene;	//�O��̃Q�[���̃V�[��
GAME_SCENE NextGameScene;	//���̃Q�[���̃V�[��



//�v���C�w�i�̓���
MOVIE playMovie;

//�v���C���[
CHARACTOR player;

//�S�[��
CHARACTOR Goal;

//�摜��ǂݍ���
IMAGE TitleLogo;
IMAGE TitleEnter;
IMAGE EndClear;


//���y
AUDIO TitleBGM;
AUDIO PlayBGM;
AUDIO EndBGM;
AUDIO Himei;

//���ʉ�
AUDIO PlayerSE;

//��ʂ̐؂�ւ�
BOOL IsFadeOut = FALSE;		//�t�F�[�h�A�E�g
BOOL IsFadeIn = FALSE;		//�t�F�[�h�C��

int fadeTimeMill = 2000;					//�؂�ւ��~���b
int fadeTimeMax = fadeTimeMill / 1000 * 60;	//�~���b���t���[���b�ɕϊ�

//�t�F�[�h�A�E�g
int fadeOutCntInit = 0;				//�����l
int fadeOutCnt = fadeOutCntInit;	//�t�F�[�h�A�E�g�̃J�E���^
int fadeOutCntMax = fadeTimeMax;	//�t�F�[�h�A�E�g�̃J�E���^MAX

//�t�F�[�h�C��
int fadeInCntInit = fadeTimeMax;	//�����l
int fadeInCnt = fadeInCntInit;		//�t�F�[�h�A�E�g�̃J�E���^
int fadeInCntMax = fadeTimeMax;		//�t�F�[�h�A�E�g�̃J�E���^MAX

//PushEnter���_��
int PushEnterCnt = 0;
const int PushEnterCntMax = 60;
BOOL PushEnterBrink = FALSE;

//Himei�̃J�E���g
int HimeiCnt = 0;
const int HimeiCntMax =0;
BOOL HimeiBrink = FALSE;


//�v���g�^�C�v�錾
VOID Title(VOID);		//�^�C�g�����
VOID TitleProc(VOID);	//�^�C�g�����(����)
VOID TitleDraw(VOID);	//�^�C�g�����(�`��)

VOID Play(VOID);		//�v���C���
VOID PlayProc(VOID);	//�v���C���(����)
VOID PlayDraw(VOID);	//�v���C���(�`��)

VOID End(VOID);			//�G���h���
VOID EndProc(VOID);		//�G���h���(����)
VOID EndDraw(VOID);		//�G���h���(�`��)

VOID Change(VOID);		//�؂�ւ����
VOID ChangeProc(VOID);	//�؂�ւ����(����)
VOID ChangeDraw(VOID);	//�؂�ւ����(�`��)

VOID ChangeScene(GAME_SCENE scene);	//�V�[���؂�ւ�

VOID CollUpdatePlayer(CHARACTOR* chara);	//�����蔻��̗̈���X�V
VOID CollUpdate(CHARACTOR* chara);			//�����蔻��

BOOL OnCollRect(RECT a, RECT b);			//��`�Ƌ�`�̓����蔻��

BOOL GameLoad(VOID);	//�Q�[���̃f�[�^��ǂݍ���

BOOL LoadGraph(IMAGE* image, const char* path);

VOID GameInit(VOID);	//�Q�[���̃f�[�^�̏�����

// �v���O������ WinMain ����n�܂�܂�
//Windows�̃v���O���~���O���@ = (WinAPI)�œ����Ă���I
//DxLib�́ADirectX�Ƃ����A�Q�[���v���O���~���O�����񂽂�Ɏg����d�g��
int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	SetOutApplicationLogValidFlag(FALSE);				//Log.txt���o�͂��Ȃ�
	ChangeWindowMode(TRUE);								//�E�B���h�E���[�h�ɐݒ�
	SetMainWindowText(GAME_TITLE);						//�E�B���h�E�̃^�C�g���̕���
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);	//�E�B���h�E�̉𑜓x��ݒ�
	SetWindowSize(GAME_WIDTH, GAME_HEIGHT);				//�E�B���h�E�̑傫����ݒ�
	SetBackgroundColor(255, 255, 255);					//�f�t�H���g�̔w�i�̐F
	SetWindowIconID(GAME_ICON_ID);						//�A�C�R���t�@�C����Ǎ�
	SetWindowStyleMode(GAME_WINDOW_BAR);				//�E�B���h�E�o�[�̏��
	SetWaitVSyncFlag(TRUE);								//�f�B�X�v���C�̐���������L���ɂ���
	SetAlwaysRunFlag(TRUE);								//�E�B���h�E�������ƃA�N�e�B�u�ɂ���

	//�c�w���C�u��������������
	if (DxLib_Init() == -1)
	{
		// �G���[���N�����璼���ɏI��
		return -1;
	}

	//�_�u���o�b�t�@�����O�L����
	SetDrawScreen(DX_SCREEN_BACK);

	//�ŏ��̃V�[���́A�^�C�g����ʂ���
	GameScene = GAME_SCENE_TITLE;

	//�Q�[���S�̂̏�����

	//�Q�[���ǂݍ���
	if (!GameLoad())
	{
		//�f�[�^�̓ǂݍ��݂Ɏ��s�����Ƃ�
		DxLib_End();	//DxLib�I��
		return -1;		//�ُ�I��
	}

	//�Q�[���̏�����
	GameInit();

	//�������[�v
	while (1)
	{
		if (ProcessMessage() != 0) { break; }	//���b�Z�[�W���󂯎�葱����
		if (ClearDrawScreen() != 0) { break; }	//��ʂ���������

		//�L�[�{�[�h���͂̍X�V
		AllKeyUpdate();

		//FPS�l�̍X�V
		FPSUpdate();

		//ESC�L�[�ŋ����I��
		if (KeyClick(KEY_INPUT_ESCAPE) == TRUE) { break; }

		//�ȑO�̃V�[�����擾
		if (GameScene != GAME_SCENE_CHANGE)
		{
			OldGameScene = GameScene;
		}

		//�V�[�����Ƃɏ������s��
		switch (GameScene)
		{
		case GAME_SCENE_TITLE:
			Title();			//�^�C�g�����
			break;
		case GAME_SCENE_PLAY:
			Play();				//�v���C���
			break;
		case GAME_SCENE_END:
			End();				//�G���h���
			break;
		case GAME_SCENE_CHANGE:
			Change();			//�؂�ւ����
			break;
		default:
			break;
		}

		//�V�[����؂�ւ���
		if (OldGameScene != GameScene)
		{
			//���݂̃V�[�����؂�ւ���ʂłȂ��Ƃ�
			if (GameScene != GAME_SCENE_CHANGE)
			{
				NextGameScene = GameScene;		//���̃V�[����ۑ�
				GameScene = GAME_SCENE_CHANGE;	//��ʐ؂�ւ��V�[���ɕς���
			}
		}

		//FPS�l��`��
		FPSDraw();

		//FPS�l��҂�
		FPSWait();

		ScreenFlip();	//�_�u���o�b�t�@�����O������ʂ�`��
	}

	//�I���Ƃ��̏���
	DeleteGraph(playMovie.handle);	//������������ォ��폜
	DeleteGraph(player.img.handle);		//�摜���������ォ��폜
	DeleteGraph(Goal.img.handle);		//�摜���������ォ��폜

	//�I���Ƃ��̏���
	DeleteSoundMem(TitleBGM.handle);	//���y���������ォ��폜
	DeleteSoundMem(PlayBGM.handle);		//���y���������ォ��폜
	DeleteSoundMem(EndBGM.handle);		//���y���������ォ��폜


	DeleteSoundMem(PlayerSE.handle);		//SE���������ォ��폜

	DeleteGraph(TitleLogo.handle);		  //�摜���������ォ��폜
	DeleteGraph(TitleEnter.handle);		  //�摜���������ォ��폜
	DeleteGraph(EndClear.handle);		  //�摜���������ォ��폜
	DeleteGraph(Himei.handle);			  //�摜���������ォ��폜
										
	//�c�w���C�u�����g�p�̏I������
	DxLib_End();

	return 0;	// �\�t�g�̏I�� 
}

/// <summary>
/// �Q�[���̃f�[�^��ǂݍ���
/// </summary>
/// <returns>�ǂݍ��߂���TRUE / �ǂݍ��߂Ȃ�������FALSE</returns>
BOOL GameLoad(VOID)
{
	//�v���C����̔w�i��ǂݍ���
	strcpyDx(playMovie.path, ".\\Movie\\PlayMovie.mp4");	//�p�X�̃R�s�[
	playMovie.handle = LoadGraph(playMovie.path);			//����̓ǂݍ���

	//���悪�ǂݍ��߂Ȃ������Ƃ��́A�G���[(-1)������
	if (playMovie.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//���C���̃E�B���h�E�n���h��
			playMovie.path,			//���b�Z�[�W�{��
			"����ǂݍ��݃G���[�I",		//���b�Z�[�W�^�C�g��
			MB_OK					//�{�^��
		);

		return FALSE;	//�ǂݍ��ݎ��s
	}

	//�摜�̕��ƍ������擾
	GetGraphSize(playMovie.handle, &playMovie.width, &playMovie.height);

	//����̃{�����[��
	playMovie.Volume = 255;





	//�v���C���[�̉摜��ǂݍ���
	strcpyDx(player.img.path, ".\\Image\\player2.png");	//�p�X�̃R�s�[
	player.img.handle = LoadGraph(player.img.path);	//�摜�̓ǂݍ���

	//�摜���ǂݍ��߂Ȃ������Ƃ��́A�G���[(-1)������
	if (player.img.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//���C���̃E�B���h�E�n���h��
			player.img.path,			//���b�Z�[�W�{��
			"�摜�ǂݍ��݃G���[�I",		//���b�Z�[�W�^�C�g��
			MB_OK					//�{�^��
		);

		return FALSE;	//�ǂݍ��ݎ��s
	}

	//�摜�̕��ƍ������擾
	GetGraphSize(player.img.handle, &player.img.width, &player.img.height);

	/*
	//�����蔻����X�V����
	CollUpdatePlayer(&player);	//�v���C���[�̓����蔻��̃A�h���X
	*/



	//�S�[���̉摜��ǂݍ���
	strcpyDx(Goal.img.path, ".\\Image\\Goal.png");	//�p�X�̃R�s�[
	Goal.img.handle = LoadGraph(Goal.img.path);	//�摜�̓ǂݍ���

	//�摜���ǂݍ��߂Ȃ������Ƃ��́A�G���[(-1)������
	if (Goal.img.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//���C���̃E�B���h�E�n���h��
			Goal.img.path,				//���b�Z�[�W�{��
			"�摜�ǂݍ��݃G���[�I",		//���b�Z�[�W�^�C�g��
			MB_OK					//�{�^��
		);

		return FALSE;	//�ǂݍ��ݎ��s
	}



	//�摜�̕��ƍ������擾
	GetGraphSize(Goal.img.handle, &Goal.img.width, &Goal.img.height);




	/// <summary>
	/// �^�C�g�����y���������ɓǂݍ���
	/// </summary>
	/// <param name="audio">Audio�\���̕ϐ��̃A�h���X</param>
	/// <param name="path">Audio�̉��y�p�X</param>
	/// <param name="volume">�{�����[��</param>
	/// <param name="playType">DX_PLAYTYPE_LOOP or DX_PLAYTYPE_BACK</param>
	/// <returns></returns>

		//title���y�̓ǂݍ���
	strcpyDx(TitleBGM.path, ".\\Audio\\opening.mp3");	//�p�X�̃R�s�[
	TitleBGM.handle = LoadSoundMem(TitleBGM.path);	//���y�̓ǂݍ���

	if (TitleBGM.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//���C���̃E�B���h�E�n���h��
			TitleBGM.path,				//���b�Z�[�W�{��
			"���y�ǂݍ��݃G���[�I",		//���b�Z�[�W�^�C�g��
			MB_OK					//�{�^��
		);

		return FALSE;	//�ǂݍ��ݎ��s
	}

	TitleBGM.playType = DX_PLAYTYPE_LOOP;	 //���y�����[�v������
	TitleBGM.Volume = 255;					 //MAX���Q�T�T



	//play���y�̓ǂݍ���
	strcpyDx(PlayBGM.path, ".\\Audio\\plaing.mp3");	//�p�X�̃R�s�[
	PlayBGM.handle = LoadSoundMem(PlayBGM.path);	//���y�̓ǂݍ���

	if (PlayBGM.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//���C���̃E�B���h�E�n���h��
			PlayBGM.path,				//���b�Z�[�W�{��
			"���y�ǂݍ��݃G���[�I",		//���b�Z�[�W�^�C�g��
			MB_OK					//�{�^��
		);

		return FALSE;	//�ǂݍ��ݎ��s
	}

	PlayBGM.playType = DX_PLAYTYPE_LOOP;	 //���y�����[�v������
	PlayBGM.Volume = 255;					 //MAX���Q�T�T



	//end���y�̓ǂݍ���
	strcpyDx(EndBGM.path, ".\\Audio\\ending.mp3");	//�p�X�̃R�s�[
	EndBGM.handle = LoadSoundMem(EndBGM.path);	//���y�̓ǂݍ���

	if (EndBGM.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//���C���̃E�B���h�E�n���h��
			EndBGM.path,				//���b�Z�[�W�{��
			"���y�ǂݍ��݃G���[�I",		//���b�Z�[�W�^�C�g��
			MB_OK					//�{�^��
		);

		return FALSE;	//�ǂݍ��ݎ��s
	}

	EndBGM.playType = DX_PLAYTYPE_LOOP;	 //���y�����[�v������
	EndBGM.Volume = 255;					 //MAX���Q�T�T

		//���ʉ��̓ǂݍ���
	strcpyDx(PlayerSE.path, ".\\Audio\\Finger.mp3");	//�p�X�̃R�s�[
	PlayerSE.handle = LoadSoundMem(PlayerSE.path);	//���y�̓ǂݍ���

	if (PlayerSE.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//���C���̃E�B���h�E�n���h��
			PlayerSE.path,				//���b�Z�[�W�{��
			"���y�ǂݍ��݃G���[�I",		//���b�Z�[�W�^�C�g��
			MB_OK					//�{�^��
		);

		return FALSE;	//�ǂݍ��ݎ��s
	}

	PlayerSE.playType = DX_PLAYTYPE_BACK;	 //���y�����[�v������
	PlayerSE.Volume = 255;					 //MAX���Q�T�T

	//���ʉ��̓ǂݍ���
	strcpyDx(Himei.path, ".\\Audio\\himei.mp3");	//�p�X�̃R�s�[
	Himei.handle = LoadSoundMem(Himei.path);	//���y�̓ǂݍ���

	if (Himei.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//���C���̃E�B���h�E�n���h��
			Himei.path,				//���b�Z�[�W�{��
			"���y�ǂݍ��݃G���[�I",		//���b�Z�[�W�^�C�g��
			MB_OK					//�{�^��
		);

		return FALSE;	//�ǂݍ��ݎ��s
	}

	Himei.playType = DX_PLAYTYPE_NORMAL;	 //���y
	Himei.Volume = 255;					 //MAX���Q�T�T


		//titleLogo�̓ǂݍ���
	strcpyDx(TitleLogo.path, ".\\image\\Title.png");	//�p�X�̃R�s�[
	TitleLogo.handle = LoadGraph(TitleLogo.path);	//�摜�̓ǂݍ���]

	//�摜���ǂݍ��߂Ȃ������Ƃ��́A�G���[(-1)������
	if (TitleLogo.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//���C���̃E�B���h�E�n���h��
			TitleLogo.path,				//���b�Z�[�W�{��
			"�摜�ǂݍ��݃G���[�I",		//���b�Z�[�W�^�C�g��
			MB_OK					//�{�^��
		);
		return FALSE;
		
	}
		//�摜�̕��ƍ������擾
		GetGraphSize(TitleLogo.handle, &TitleLogo.width, &TitleLogo.height);


		//titleEnter�̓ǂݍ���
		strcpyDx(TitleEnter.path, ".\\image\\Enter.png");	//�p�X�̃R�s�[
		TitleEnter.handle = LoadGraph(TitleEnter.path);	//�摜�̓ǂݍ���]

		//�摜���ǂݍ��߂Ȃ������Ƃ��́A�G���[(-1)������
		if (TitleEnter.handle == -1)
		{
			MessageBox(
				GetMainWindowHandle(),	//���C���̃E�B���h�E�n���h��
				TitleEnter.path,				//���b�Z�[�W�{��
				"�摜�ǂݍ��݃G���[�I",		//���b�Z�[�W�^�C�g��
				MB_OK					//�{�^��
			);
			return FALSE;	//�ǂݍ��ݎ��s
		}
	
	//�摜�̕��ƍ������擾
	GetGraphSize(TitleEnter.handle, &TitleEnter.width, &TitleEnter.height);

	//EndClear�̓ǂݍ���
	strcpyDx(EndClear.path, ".\\image\\End.png");	//�p�X�̃R�s�[
	EndClear.handle = LoadGraph(EndClear.path);	//�摜�̓ǂݍ���]

	//�摜���ǂݍ��߂Ȃ������Ƃ��́A�G���[(-1)������
	if (EndClear.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//���C���̃E�B���h�E�n���h��
			EndClear.path,				//���b�Z�[�W�{��
			"�摜�ǂݍ��݃G���[�I",		//���b�Z�[�W�^�C�g��
			MB_OK					//�{�^��
		);
		return FALSE;	//�ǂݍ��ݎ��s
	}

	//�摜�̕��ƍ������擾
	GetGraphSize(EndClear.handle, &EndClear.width, &EndClear.height);
	return TRUE;	//�S�ēǂݍ��݂��I
}


/// <summary>
/// �Q�[���f�[�^��������
/// </summary>
/// <param name=""></param>
VOID GameInit(VOID)
{
	//�v���C���[��������
	player.img.x = GAME_WIDTH / 2 - player.img.width / 2;	//������
	player.img.y = GAME_HEIGHT / 2 - player.img.height / 2;	//������
	player.speed = 500;		//�X�s�[�h
	player.img.IsDraw = TRUE;	//�`��ł���I

	//�����蔻����X�V����
	CollUpdatePlayer(&player);	//�v���C���[�̓����蔻��̃A�h���X

	//�S�[����������
	Goal.img.x = GAME_WIDTH - Goal.img.width;
	Goal.img.y = 0;
	Goal.speed = 500;	//�X�s�[�h
	Goal.img.IsDraw = TRUE;	//�`��ł���I

	//�����蔻����X�V����
	CollUpdate(&Goal);	//�v���C���[�̓����蔻��̃A�h���X

	//�^�C�g�����S�̈ʒu�����߂�
	TitleLogo.x = 100;		//��������
	TitleLogo.y = 100;		//��������

	//pushEnter�̈ʒu�����߂�
	TitleEnter.x = GAME_WIDTH / 2 - TitleEnter.width / 2;		//��������
	TitleEnter.y = GAME_HEIGHT / 2 + 200 - TitleEnter.height;	

	//�N���A���S�̈ʒu�����߂�
	EndClear.x = GAME_WIDTH / 2 - EndClear.width / 2;		//��������
	EndClear.y = GAME_HEIGHT / 2 - EndClear.height / 2;		//��������
}

/// <summary>
/// �V�[����؂�ւ���֐�
/// </summary>
/// <param name="scene">�V�[��</param>
VOID ChangeScene(GAME_SCENE scene)
{
	GameScene = scene;	//�V�[����؂�ւ�
	IsFadeIn = FALSE;	//�t�F�[�h�C�����Ȃ�
	IsFadeOut = TRUE;	//�t�F�[�h�A�E�g����

	return;
}

/// <summary>
/// �^�C�g�����
/// </summary>
VOID Title(VOID)
{
	TitleProc();	//����
	TitleDraw();	//�`��

	return;
}

/// <summary>
/// �^�C�g����ʂ̏���
/// </summary>
VOID TitleProc(VOID)
{

	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//BGM���~�߂�
		 StopSoundMem(TitleBGM.handle);

		//�V�[���؂�ւ�
		//���̃V�[���̏������������ōs���Ɗy

		//�Q�[���̏�����
		GameInit();

		//�v���C��ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_PLAY);

		return;
	}

	//BGM������Ă��Ȃ��Ƃ�
	if (CheckSoundMem(TitleBGM.handle) == 0)
	{
		//BGM�𗬂�
		PlaySoundMem(TitleBGM.handle, TitleBGM.playType);
	}

	return;
}

/// <summary>
/// �^�C�g����ʂ̕`��
/// </summary>
VOID TitleDraw(VOID)
{
	//�^�C�g�����S�̕`��
	DrawGraph(TitleLogo.x, TitleLogo.y, TitleLogo.handle, TRUE);

		//MAX�l�܂ő҂�
		if (PushEnterCnt < PushEnterCntMax) { PushEnterCnt++; }
		else
		{
			if (PushEnterBrink == TRUE)PushEnterBrink = FALSE;
			else if (PushEnterBrink == FALSE)PushEnterBrink = TRUE;

			PushEnterCnt = 0;	//�J�E���^��������
		}

		if (PushEnterBrink == TRUE)
		{
			//�������ɂ���
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)PushEnterCnt / PushEnterCntMax) * 255);

			//PushEnter�̕`��
			DrawGraph(TitleEnter.x, TitleEnter.y, TitleEnter.handle, TRUE);

			//�������I��
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
		if (PushEnterBrink == FALSE)
		{
			//�������ɂ���
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)(PushEnterCntMax - PushEnterCnt) / PushEnterCntMax) * 255);

			//PushEnter�̕`��
			DrawGraph(TitleEnter.x, TitleEnter.y, TitleEnter.handle, TRUE);

			//�������I��
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
	DrawString(0, 0, "�^�C�g�����", GetColor(0, 0, 0));
	return;
}

/// <summary>
/// �v���C���
/// </summary>
VOID Play(VOID)
{
	PlayProc();	//����
	PlayDraw();	//�`��

	return;
}

/// <summary>
/// �v���C��ʂ̏���
/// </summary>
VOID PlayProc(VOID)
{
	/*
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//�V�[���؂�ւ�
		//���̃V�[���̏������������ōs���Ɗy

		//�G���h��ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_END);
	}
	*/

	//BGM������Ă��Ȃ��Ƃ�
	if (CheckSoundMem(PlayBGM.handle) == 0)
	{
		//BGM�𗬂�
		PlaySoundMem(PlayBGM.handle, PlayBGM.playType);
	}

	//�v���C���[�̑���
	if (KeyDown(KEY_INPUT_W) == TRUE)
	{
		player.img.y -= player.speed * fps.DeltaTime;

		//�����Ƃ��̌��ʉ���ǉ�
		if (CheckSoundMem(PlayerSE.handle) == 0)
		{
			PlaySoundMem(PlayerSE.handle, PlayerSE.playType);
		}
	}
	if (KeyDown(KEY_INPUT_S) == TRUE)
	{
		player.img.y += player.speed * fps.DeltaTime;

		//�����Ƃ��̌��ʉ���ǉ�
		if (CheckSoundMem(PlayerSE.handle) == 0)
		{
			PlaySoundMem(PlayerSE.handle, PlayerSE.playType);
		}
	}

	if (KeyDown(KEY_INPUT_A) == TRUE)
	{
		player.img.x -= player.speed * fps.DeltaTime;

		//�����Ƃ��̌��ʉ���ǉ�
		if (CheckSoundMem(PlayerSE.handle) == 0)
		{
			PlaySoundMem(PlayerSE.handle, PlayerSE.playType);
		}
	}
	if (KeyDown(KEY_INPUT_D) == TRUE)
	{
		player.img.x += player.speed * fps.DeltaTime;

		//�����Ƃ��̌��ʉ���ǉ�
		if (CheckSoundMem(PlayerSE.handle) == 0)
		{
			PlaySoundMem(PlayerSE.handle, PlayerSE.playType);
		}
	}

	//�����蔻����X�V����
	CollUpdatePlayer(&player);

	//�S�[���̓����蔻����X�V����
	CollUpdate(&Goal);

	//�v���C���[���S�[���ɓ��������Ƃ���
	if (OnCollRect(player.coll, Goal.coll) == TRUE)
	{



		//BGM������Ă��Ȃ��Ƃ�
		if (CheckSoundMem(Himei.handle) == 0)
		{
			//BGM�𗬂�
			PlaySoundMem(Himei.handle, Himei.playType);
		}

		//BGM���~�߂�
		StopSoundMem(PlayBGM.handle);

		//MAX�l�܂ő҂�
		

		//BGM���~�߂�
		StopSoundMem(Himei.handle);



		//�G���h��ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_END);

		//�����������I��
		return;
	}

	return;
}

/// <summary>
/// �v���C��ʂ̕`��
/// </summary>
VOID PlayDraw(VOID)
{
	//�w�i�����`��

	//�����A���悪�Đ�����Ă��Ȃ��Ƃ�
	if (GetMovieStateToGraph(playMovie.handle) == 0)
	{
		//�Đ�����
		SeekMovieToGraph(playMovie.handle, 0);	//�V�[�N�o�[���ŏ��ɖ߂�
		PlayMovieToGraph(playMovie.handle);		//������Đ�
	}
	//�����`��(�摜�������L�΂�)
	DrawExtendGraph(0, 0, GAME_WIDTH, GAME_HEIGHT, playMovie.handle, TRUE);

	//�v���C���[��`��
	if (player.img.IsDraw == TRUE)
	{
		//�摜��`��
		DrawGraph(player.img.x, player.img.y, player.img.handle, TRUE);

		//�f�o�b�O�̂Ƃ��́A�����蔻��̗̈��`��
		if (GAME_DEBUG == TRUE)
		{
			//�l�p��`��
			DrawBox(player.coll.left, player.coll.top, player.coll.right, player.coll.bottom,
				GetColor(255, 0, 0), FALSE);
		}
	}

	//�S�[����`��
	if (Goal.img.IsDraw == TRUE)
	{
		//�摜��`��
		DrawGraph(Goal.img.x, Goal.img.y, Goal.img.handle, TRUE);

		//�f�o�b�O�̂Ƃ��́A�����蔻��̗̈��`��
		if (GAME_DEBUG == TRUE)
		{
			//�l�p��`��
			DrawBox(Goal.coll.left, Goal.coll.top, Goal.coll.right, Goal.coll.bottom,
				GetColor(255, 0, 0), FALSE);
		}
	}

	DrawString(0, 0, "�v���C���", GetColor(0, 0, 0));
	return;
}

/// <summary>
/// �G���h���
/// </summary>
VOID End(VOID)
{
	EndProc();	//����
	EndDraw();	//�`��

	return;
}

/// <summary>
/// �G���h��ʂ̏���
/// </summary>
VOID EndProc(VOID)
{
	//BGM������Ă��Ȃ��Ƃ�
	if (CheckSoundMem(EndBGM.handle) == 0)
	{
		//BGM�𗬂�
		PlaySoundMem(EndBGM.handle, EndBGM.playType);
	}

	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//BGM���~�߂�
		StopSoundMem(EndBGM.handle);

		//�V�[���؂�ւ�
		//���̃V�[���̏������������ōs���Ɗy

		//�^�C�g����ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_TITLE);
	}

	return;
}

/// <summary>
/// �G���h��ʂ̕`��
/// </summary>
VOID EndDraw(VOID)
{
	//ENDCLEAR�̕`��
	DrawGraph(EndClear.x, EndClear.y, EndClear.handle, TRUE);

	DrawString(0, 0, "�G���h���", GetColor(0, 0, 0));
	return;
}

/// <summary>
/// �؂�ւ����
/// </summary>
VOID Change(VOID)
{
	ChangeProc();	//����
	ChangeDraw();	//�`��

	return;
}

/// <summary>
/// �؂�ւ���ʂ̏���
/// </summary>
VOID ChangeProc(VOID)
{

	//�t�F�[�h�C��
	if (IsFadeIn == TRUE)
	{
		if (fadeInCnt > fadeInCntMax)
		{
			fadeInCnt--;	//�J�E���^�����炷
		}
		else
		{
			//�t�F�[�h�C���������I�����

			fadeInCnt = fadeInCntInit;	//�J�E���^������
			IsFadeIn = FALSE;			//�t�F�[�h�C�������I��
		}
	}

	//�t�F�[�h�A�E�g
	if (IsFadeOut == TRUE)
	{
		if (fadeOutCnt < fadeOutCntMax)
		{
			fadeOutCnt++;	//�J�E���^�𑝂₷
		}
		else
		{
			//�t�F�[�h�A�E�g�������I�����

			fadeOutCnt = fadeOutCntInit;	//�J�E���^������
			IsFadeOut = FALSE;				//�t�F�[�h�A�E�g�����I��
		}
	}

	//�؂�ւ������I��
	if (IsFadeIn == FALSE && IsFadeOut == FALSE)
	{
		//�t�F�[�h�C�����Ă��Ȃ��A�t�F�[�h�A�E�g�����Ă��Ȃ��Ƃ�
		GameScene = NextGameScene;	//���̃V�[���ɐ؂�ւ�
		OldGameScene = GameScene;	//�ȑO�̃Q�[���V�[���X�V
	}


	return;
}

/// <summary>
/// �؂�ւ���ʂ̕`��
/// </summary>
VOID ChangeDraw(VOID)
{
	//�ȑO�̃V�[����`��
	switch (OldGameScene)
	{
	case GAME_SCENE_TITLE:
		TitleDraw();	//�^�C�g����ʂ̕`��
		break;
	case GAME_SCENE_PLAY:
		PlayDraw();		//�v���C��ʂ̕`��
		break;
	case GAME_SCENE_END:
		EndDraw();		//�G���h��ʂ̕`��
		break;
	default:
		break;
	}

	//�t�F�[�h�C��
	if (IsFadeIn == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeInCnt / fadeInCntMax) * 255);
	}

	//�t�F�[�h�A�E�g
	if (IsFadeOut == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeOutCnt / fadeOutCntMax) * 255);
	}

	//�l�p��`��
	DrawBox(0, 0, GAME_WIDTH, GAME_HEIGHT, GetColor(0, 0, 0), TRUE);

	//�������I��
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawString(0, 0, "�؂�ւ����", GetColor(0, 0, 0));
	return;
}

/// <summary>
/// �����蔻��̗̈�X�V�i�v���C���[�j
/// </summary>
/// <param name="chara">�����蔻��̗̈�</param>
VOID CollUpdatePlayer(CHARACTOR* chara)
{
	chara->coll.left = chara->img.x + 50;					//�����蔻��������
	chara->coll.top = chara->img.y + 30;						//�����蔻��������

	chara->coll.right = chara->img.x + chara->img.width - 60;		//�����蔻��������
	chara->coll.bottom = chara->img.y + chara->img.height - 60;	//�����蔻��������

	return;
}

/// <summary>
/// �����蔻��̗̈�X�V(�S�[��)
/// </summary>
/// <param name="chara">�����蔻��̗̈�</param>
VOID CollUpdate(CHARACTOR* chara)
{
	chara->coll.left = chara->img.x + 70;
	chara->coll.top = chara->img.y + 70;

	chara->coll.right = chara->img.x + chara->img.width - 60;
	chara->coll.bottom = chara->img.y + chara->img.height - 60;

	return;
}

/// <summary>
/// ��`�Ƌ�`�̓����蔻��
/// </summary>
/// <param name="a">��`A</param>
/// <param name="b">��`B</param>
/// <returns>����������TRUE/������Ȃ��Ȃ�FALSE</returns>
BOOL OnCollRect(RECT a, RECT b)
{
	if (
		a.left < b.right &&		//�@��`A�̍���X���W < ��`B�̉E��X���W�@����
		a.right > b.left &&		//�@��`A�̉E��X���W > ��`B�̍���X���W�@����
		a.top  < b.bottom &&	//�@��`A�̏��Y���W < ��`B�̉���Y���W�@����
		a.bottom > b.top		//�@��`A�̉���Y���W > ��`B�̏��Y���W
		)
	{
		//�������Ă���Ƃ�
		return TRUE;
	}
	else
	{
		//�������Ă��Ȃ��Ƃ�
		return FALSE;
	}
}