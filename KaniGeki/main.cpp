#include "DxLib.h"
#include <math.h>

const char TITLE[] = "�I�� - KANIGEKI";

const int WIN_WIDTH = 800; //�E�B���h�E����
const int WIN_HEIGHT = 600;//�E�B���h�E�c��


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	ChangeWindowMode(TRUE);						//�E�B���h�E���[�h�ɐݒ�
	//�E�B���h�E�T�C�Y���蓮�ł͕ύX�ł����A���E�B���h�E�T�C�Y�ɍ��킹�Ċg��ł��Ȃ��悤�ɂ���
	SetWindowSizeChangeEnableFlag(FALSE, FALSE);
	SetMainWindowText(TITLE);					// �^�C�g����ύX
	SetGraphMode(WIN_WIDTH, WIN_HEIGHT, 32);	//��ʃT�C�Y�̍ő�T�C�Y�A�J���[�r�b�g����ݒ�i���j�^�[�̉𑜓x�ɍ��킹��j
	SetWindowSizeExtendRate(1.0);				//��ʃT�C�Y��ݒ�i�𑜓x�Ƃ̔䗦�Őݒ�j
	SetBackgroundColor(0, 0, 0);		// ��ʂ̔w�i�F��ݒ肷��

	//Dx���C�u�����̏�����
	if (DxLib_Init() == -1) { return -1; }

	//�i�_�u���o�b�t�@�j�`���O���t�B�b�N�̈�͗��ʂ��w��
	SetDrawScreen(DX_SCREEN_BACK);

	//�摜�Ȃǂ̃��\�[�X�f�[�^�̕ϐ��錾�Ɠǂݍ���
	//���@(�v���C���[)
	int PlayerX = 376;//�v���C���[�����o��X���W
	int PlayerY = 400;//�v���C���[�����o��Y���W
	int PlayerR = 25;//�v���C���[���a
	int PlayerAlive = 1;//�v���C���[���݃t���O(1�Ő����A0�Ŏ��S)
	int PlayerTimer = 0;//�v���C���[�̃A�j���[�V�����p�^�C�}�[
	int PlayerHyoji = 1;//�v���C���[���ʉ摜�̕\���t���O(1�ŕ\���A0�Ŕ�\��)
	int PlayerLeft = 0;//�v���C���[�������������Ƃ��̔����t���O
	int PlayerRight = 0;//�v���C���[���E���������Ƃ��̔����t���O

	//�e
	int BulX = 300;//�e1��x���W
	int BulY = 300;//�e1��y���W
	int BulR = 10;//�e1�̔��a
	int Bullet1Flag = 0;//�e1�̑��݃t���O

	//�e2
	int Bul2X = 300;//�e2��x���W
	int Bul2Y = 300;//�e2��y���W
	int Bul2R = 10;//�e2�̔��a
	int Bullet2Flag = 0;//�e2�̑��݃t���O

	//�e3
	int Bul3X = 300;//�e3��x���W
	int Bul3Y = 300;//�e3��y���W
	int Bul3R = 10;//�e3�̔��a
	int Bullet3Flag = 0;//�e3�̑��݃t���O

	//�G
	int EnemyX = 300;//�G1�̏����o��X���W
	int EnemyY = 100;//�G1�̏����o��Y���W
	int EnemyR = 25;//�G1���a
	int EnemyAlive = 1;//�G1�̑��݃t���O(1�ŕ\���A0��\��)
	int EnemySpeed = 10;//�G1�̑��x
	int EnemyTimer = 120;//�G1�̕����^�C�}�[

	//�G2
	int Enemy2X = 600;//�G2�̏����o��X���W
	int Enemy2Y = 200;//�G2�̏����o��Y���W
	int Enemy2R = 25;//�G2���a
	int Enemy2Alive = 1;//�G2�̑��݃t���O(1�ŕ\���A0��\��)
	int Enemy2Speed = 7;//�G2�̑��x
	int Enemy2Timer = 120;//�G2�̕����^�C�}�[

	//�Q�[���I�[�o�[�p�t���O
	int SE3 = LoadSoundMem("Sound\\GAME OVER.wav");
	int BGM1 = LoadSoundMem("Sound\\GAME OVER BGM.mp3");

	//�X�e�[�W�X�N���[���p
	int StageY = 0;
	int BGM2 = LoadSoundMem("Sound\\Main Theme.mp3");

	//�Q�[���V�[��
	int scene = 0;

	//�G
	int graphHandle1 = LoadGraph("Image\\kani.png");//�摜�t�@�C��
	int SE1 = LoadSoundMem("Sound\\Explosive.wav");//�v���C���[���G�̔�����

	//���@
	int graphHandle2 = LoadGraph("Image\\Shooting Plane.png");//���ʉ摜�t�@�C��
	int graphHandle5 = LoadGraph("Image\\Shooting Plane Left.png");//���X���摜�t�@�C��
	int graphHandle6 = LoadGraph("Image\\Shooting Plane Right.png");//�E�X���摜�t�@�C��

	//�v���C���[�̒e
	int graphHandle3 = LoadGraph("Image\\Bullet.png");//�摜�t�@�C��
	int SE2 = LoadSoundMem("Sound\\shot sound.wav");//�e�̔��ˉ�

	//�^�C�g�����
	int graphHandle8 = LoadGraph("Image\\Title.png");

	//�G�̒e
	int Enemy1BulX = 300;
	int Enemy1BulY = 300;
	int Enemy1BulR = 10;
	int E1BF = 0;
	int Shot1 = 0;
	int Enemy2BulX = 300;
	int Enemy2BulY = 300;
	int Enemy2BulR = 10;
	int E2BF = 0;
	int Shot2 = 0;
	int graphHandle7 = LoadGraph("Image\\Enemy Bullet.png");
	int E1BT = 60;
	int E2BT = 60;
	int EBAT = 0;

	int SCORE = 0;//�X�R�A�p

	//�w�i
	int graphHandle4 = LoadGraph("Image\\Haikei.png");//�摜�t�@�C��

	//�Q�[�����[�v�Ŏg���ϐ��̐錾
	char keys[256] = { 0 }; //�ŐV�̃L�[�{�[�h���p
	char oldkeys[256] = { 0 };//1���[�v�i�t���[���j�O�̃L�[�{�[�h���

	// �Q�[�����[�v
	while (1)
	{
		//�ŐV�̃L�[�{�[�h��񂾂������̂͂P�t���[���O�̃L�[�{�[�h���Ƃ��ĕۑ�

		//�ŐV�̃L�[�{�[�h�����擾
		GetHitKeyStateAll(keys);

		//��ʃN���A
		ClearDrawScreen();
		//---------  ��������v���O�������L�q  ----------//

		//Z�L�[����������V�[����0�ɂ��ă^�C�g����ʂ�


		//�X�V����

		switch (scene)
		{
		case 0://�^�C�g�����
			if (keys[KEY_INPUT_Z] == 1)
			{
				scene = 1;
			}
			DrawGraph(0, 0, graphHandle8, TRUE);
			DrawFormatString(315, 455, GetColor(0, 0, 0), "PUSH Z KEY TO START");
			DrawFormatString(600, 550, GetColor(0, 0, 0), "���y�f�ށF�������l");
			break;


		case 1://�Q�[���v���C���

			if (CheckSoundMem(BGM2) == 0)
			{
				PlaySoundMem(BGM2, DX_PLAYTYPE_LOOP);
			}

			//���@�̃R���g���[��
			if (keys[KEY_INPUT_RIGHT] == 1)//�E�������ꂽ�Ƃ�
			{
				PlayerX = PlayerX + 5;
				PlayerRight = 1;
				PlayerLeft = 0;
			}

			if (keys[KEY_INPUT_RIGHT] == 0)//�E�������ꂽ�Ƃ�
			{
				PlayerRight = 0;
			}

			if (keys[KEY_INPUT_LEFT] == 1)//���������ꂽ�Ƃ�
			{
				PlayerX = PlayerX - 5;
				PlayerLeft = 1;
				PlayerRight = 0;
			}

			if (keys[KEY_INPUT_LEFT] == 0)//���������ꂽ�Ƃ�
			{
				PlayerLeft = 0;
			}

			if (keys[KEY_INPUT_UP] == 1)//��
			{
				PlayerY = PlayerY - 5;
			}

			if (keys[KEY_INPUT_DOWN] == 1)//��
			{
				PlayerY = PlayerY + 2;
			}

			//�e�̔���
			if ((keys[KEY_INPUT_SPACE] == 1) && (PlayerAlive == 1))
			{
				if ((Bullet1Flag == 0) && (Bullet2Flag == 0) && (Bullet3Flag == 0))//�e�t���O0�ŉ����ꂽ�Ƃ�
				{

					BulX = PlayerX;
					BulY = PlayerY - PlayerR;//�e��\��
					Bullet1Flag = 1;//�e�t���O��1��(��ʓ�����ł����)
					PlaySoundMem(SE2, DX_PLAYTYPE_BACK, TRUE);

					Bul2X = PlayerX;
					Bul2Y = PlayerY - PlayerR;//�e2��\��
					Bullet2Flag = 1;

					Bul3X = PlayerX;
					Bul3Y = PlayerY - PlayerR;//�e2��\��
					Bullet3Flag = 1;
				}
			}

			//�e1�̋���
			if (Bullet1Flag == 1)
			{
				BulY = BulY - 12;//�e�̑��x

				//�e����ʂ���͂ݏo������
				if (BulY <= 0)
				{
					Bullet1Flag = 0;
				}
			}

			//�e2�̋���
			if (Bullet2Flag == 1)
			{
				Bul2Y = Bul2Y - 12;
				Bul2X = Bul2X - 3;

				//�e����ʂ���͂ݏo������
				if (Bul2Y <= 0)
				{
					Bullet2Flag = 0;
				}
			}

			//�e3�̋���
			if (Bullet3Flag == 1)
			{
				Bul3Y = Bul3Y - 12;
				Bul3X = Bul3X + 3;

				//�e����ʂ���͂ݏo������
				if (Bul3Y <= 0)
				{
					Bullet3Flag = 0;
				}
			}

			//�G�ړ�
			if (EnemyAlive == 1)
			{
				EnemyX = EnemyX + EnemySpeed;
				if (EnemyX > WIN_WIDTH - EnemyR)
				{
					EnemySpeed = -10;
				}

				if (EnemyX < 0 + EnemyR)
				{
					EnemySpeed = 10;
				}
			}

			if (Enemy2Alive == 1)
			{
				Enemy2X = Enemy2X + Enemy2Speed;
				if (Enemy2X > WIN_WIDTH - Enemy2R)
				{
					Enemy2Speed = -7;
				}

				if (Enemy2X < 0 + Enemy2R)
				{
					Enemy2Speed = 7;
				}
			}

			//�G����
			if (EnemyAlive == 0)
			{
				EnemyTimer = EnemyTimer - 1;
				if (EnemyTimer < 0)
				{
					EnemyTimer = 120;
					EnemyAlive = 1;
				}
			}

			if (Enemy2Alive == 0)
			{
				Enemy2Timer = Enemy2Timer - 1;
				if (Enemy2Timer < 0)
				{
					Enemy2Timer = 120;
					Enemy2Alive = 1;
				}
			}

			//�G1�̒e�̃^�C�}�[
			if ((EnemyAlive == 1) && (E1BF == 0) && (Shot1 == 0))
			{
				E1BT = E1BT - 1;

				if (E1BT < 0)
				{
					E1BF = 1;
					E1BT = 10;
				}
			}

			//�G1�̒e�\��
			if ((E1BF == 1) && (Shot1 == 0))
			{
				Enemy1BulX = EnemyX;
				Enemy1BulY = EnemyY - EnemyR;//�e��\��
				Shot1 = 1;
			}

			//�G1�̒e����
			if (Shot1 == 1)
			{
				Enemy1BulY = Enemy1BulY + 9;
				if (Enemy1BulY > 650)
				{
					E1BF = 0;
					Shot1 = 0;
				}
			}

			//�G2�̒e�̃^�C�}�[
			if ((Enemy2Alive == 1) && (E2BF == 0) && (Shot2 == 0))
			{
				E2BT = E2BT - 1;

				if (E2BT < 0)
				{
					E2BF = 1;
					E2BT = 10;
				}
			}

			//�G2�̒e�\��
			if ((E2BF == 1) && (Shot2 == 0))
			{
				Enemy2BulX = Enemy2X;
				Enemy2BulY = Enemy2Y - Enemy2R;//�e��\��
				Shot2 = 1;
			}

			//�G2�̒e����
			if (Shot2 == 1)
			{
				Enemy2BulY = Enemy2BulY + 9;
				if (Enemy2BulY > 650)
				{
					E2BF = 0;
					Shot2 = 0;
				}
			}

			//�e1�ƓG1�̓����蔻��
			int dEBx = EnemyX - BulX;
			int dEBy = EnemyY - BulY;
			int a = dEBx * dEBx + dEBy * dEBy;
			int b = sqrt(a);

			if ((EnemyAlive == 1) && (Bullet1Flag == 1))
			{
				if (b < BulR + EnemyR)
				{
					EnemyAlive = 0;
					Bullet1Flag = 0;
					SCORE = SCORE + 100;
					PlaySoundMem(SE1, DX_PLAYTYPE_BACK, TRUE);
				}
			}

			//�e1�ƓG2�̓����蔻��
			int dE2Bx = Enemy2X - BulX - 10;
			int dE2By = Enemy2Y - BulY - 10;
			int c = dE2Bx * dE2Bx + dE2By * dE2By;
			int d = sqrt(c);

			if ((Enemy2Alive == 1) && (Bullet1Flag == 1))
			{
				if (d < BulR + Enemy2R)
				{
					Enemy2Alive = 0;
					Bullet1Flag = 0;
					SCORE = SCORE + 100;
					PlaySoundMem(SE1, DX_PLAYTYPE_BACK, TRUE);
				}
			}

			//�e2�ƓG1�̓����蔻��
			int dEB2x = EnemyX - Bul2X - 10;
			int dEB2y = EnemyY - Bul2Y - 10;
			int e = dEB2x * dEB2x + dEB2y * dEB2y;
			int f = sqrt(e);

			if ((EnemyAlive == 1) && (Bullet2Flag == 1))
			{
				if (f < Bul2R + EnemyR)
				{
					EnemyAlive = 0;
					Bullet2Flag = 0;
					SCORE = SCORE + 100;
					PlaySoundMem(SE1, DX_PLAYTYPE_BACK, TRUE);
				}
			}

			//�e2�ƓG2�̓����蔻��
			int dE2B2x = Enemy2X - Bul2X - 10;
			int dE2B2y = Enemy2Y - Bul2Y - 10;
			int g = dE2B2x * dE2B2x + dE2B2y * dE2B2y;
			int h = sqrt(g);

			if ((Enemy2Alive == 1) && (Bullet2Flag == 1))
			{
				if (h < Bul2R + Enemy2R)
				{
					Enemy2Alive = 0;
					Bullet2Flag = 0;
					SCORE = SCORE + 100;
					PlaySoundMem(SE1, DX_PLAYTYPE_BACK, TRUE);
				}
			}

			//�e3�ƓG�̓����蔻��
			int dEB3x = EnemyX - Bul3X;
			int dEB3y = EnemyY - Bul3Y;
			int i = dEB3x * dEB3x + dEB3y * dEB3y;
			int j = sqrt(i);

			if ((EnemyAlive == 1) && (Bullet3Flag == 1))
			{
				if (j < Bul3R + EnemyR)
				{
					EnemyAlive = 0;
					Bullet3Flag = 0;
					SCORE = SCORE + 100;
					PlaySoundMem(SE1, DX_PLAYTYPE_BACK, TRUE);
				}
			}

			//�e3�ƓG2�̓����蔻��
			int dE2B3x = Enemy2X - Bul3X;
			int dE2B3y = Enemy2Y - Bul3Y;
			int k = dE2B3x * dE2B3x + dE2B3y * dE2B3y;
			int l = sqrt(k);

			if ((Enemy2Alive == 1) && (Bullet3Flag == 1))
			{
				if (l < Bul3R + Enemy2R)
				{
					Enemy2Alive = 0;
					Bullet3Flag = 0;
					SCORE = SCORE + 100;
					PlaySoundMem(SE1, DX_PLAYTYPE_BACK, TRUE);
				}
			}

			//�G1�̒e�Ǝ��@�̓����蔻��
			int dE1BPx = Enemy1BulX - PlayerX;
			int dE1BPy = Enemy1BulY - PlayerY;
			int m = dE1BPx * dE1BPx + dE1BPy * dE1BPy;
			int n = sqrt(m);

			if (PlayerAlive == 1)
			{
				if (n < Enemy1BulR + PlayerR)
				{
					PlayerAlive = 0;
					ChangeVolumeSoundMem(255 * 40 / 100, BGM1);
					PlaySoundMem(SE1, DX_PLAYTYPE_BACK, TRUE);
					PlaySoundMem(BGM1, DX_PLAYTYPE_LOOP, TRUE);
				}
			}

			//�G2�̒e�Ǝ��@�̓����蔻��
			int dE2BPx = Enemy2BulX - PlayerX;
			int dE2BPy = Enemy2BulY - PlayerY;
			int o = dE2BPx * dE2BPx + dE2BPy * dE2BPy;
			int p = sqrt(o);

			if (PlayerAlive == 1)
			{
				if (p < Enemy2BulR + PlayerR)
				{
					PlayerAlive = 0;
					ChangeVolumeSoundMem(255 * 40 / 100, BGM1);
					PlaySoundMem(SE1, DX_PLAYTYPE_BACK, TRUE);
					PlaySoundMem(BGM1, DX_PLAYTYPE_LOOP, TRUE);
				}
			}

			//���@�ƓG1�̓����蔻��
			int EPx = EnemyX - PlayerX;
			int EPy = EnemyY - PlayerY;
			int q = EPx * EPx + EPy * EPy;
			int r = sqrt(q);

			if ((EnemyAlive == 1) && (PlayerAlive == 1))
			{
				if (r < PlayerR + EnemyR)
				{
					ChangeVolumeSoundMem(255 * 40 / 100, BGM1);
					PlaySoundMem(SE1, DX_PLAYTYPE_BACK, TRUE);
					PlaySoundMem(BGM1, DX_PLAYTYPE_LOOP, TRUE);
					PlayerAlive = 0;
				}
			}

			//���@�ƓG2�̓����蔻��
			int E2Px = Enemy2X - PlayerX;
			int E2Py = Enemy2Y - PlayerY;
			int s = E2Px * E2Px + E2Py * E2Py;
			int t = sqrt(s);

			if ((Enemy2Alive == 1) && (PlayerAlive == 1))
			{
				if (t < PlayerR + Enemy2R)
				{
					PlaySoundMem(SE1, DX_PLAYTYPE_BACK, TRUE);
					ChangeVolumeSoundMem(255 * 40 / 100, BGM1);
					PlaySoundMem(BGM1, DX_PLAYTYPE_LOOP);
					PlayerAlive = 0;
				}
			}

			//�w�i�X�N���[��
			if (scene == 1)
			{
				StageY = StageY + 1;
				if (StageY >= WIN_HEIGHT)
				{
					StageY = 0;
				}
			}

			//���@�̃A�j���[�V����
			PlayerTimer = PlayerTimer + 50;

			//�G�̒e�̃A�j���[�V����
			EBAT = EBAT + 20;

			//���@�̃A�j���[�V�����̃��[�v
			if (PlayerTimer > 199)
			{
				PlayerTimer = 0;
			}

			//�G�̒e�̃A�j���[�V�����̃��[�v
			if (EBAT > 320)
			{
				EBAT = 0;
			}

			//���@�̕ǂ̓����蔻��
			if (PlayerX < 24)
			{
				PlayerX = 24;
			}

			if (PlayerX > 775)
			{
				PlayerX = 775;
			}

			if (PlayerY < 45)
			{
				PlayerY = 45;
			}

			if (PlayerY > 575)
			{
				PlayerY = 575;
			}

			//�`�揈��

			//�w�i
			DrawGraph(0, StageY, graphHandle4, TRUE);
			DrawGraph(0, StageY - WIN_HEIGHT, graphHandle4, TRUE);

			//�e
			if (Bullet1Flag == 1)//�e1
			{
				DrawGraph(BulX - BulR, BulY - BulR, graphHandle3, TRUE);
			}

			if (Bullet2Flag == 1)//�e2
			{
				DrawGraph(Bul2X - Bul2R, Bul2Y - Bul2R, graphHandle3, TRUE);
			}

			if (Bullet3Flag == 1)//�e3
			{
				DrawGraph(Bul3X - Bul3R, Bul3Y - Bul3R, graphHandle3, TRUE);
			}

			//�G�̒e
			if (E1BF == 1)
			{
				DrawRectGraph(Enemy1BulX, Enemy1BulY, EBAT, 0, 20, 20, graphHandle7, TRUE);
			}

			if (E2BF == 1)
			{
				DrawRectGraph(Enemy2BulX, Enemy2BulY, EBAT, 0, 20, 20, graphHandle7, TRUE);
			}

			//�v���C���[
			if ((PlayerHyoji == 1) && (PlayerAlive == 1))//�\���t���O1�Ő��ʉ摜��\��
			{
				DrawRectGraph(PlayerX - PlayerR, PlayerY - PlayerR, PlayerTimer, 0, 50, 50, graphHandle2, TRUE);
			}

			if ((PlayerLeft == 1) && (PlayerAlive == 1))//�����������獶�X���摜��\��
			{
				PlayerHyoji = 0;
				DrawGraph(PlayerX - PlayerR, PlayerY - PlayerR, graphHandle5, TRUE);//���X���摜��\��
			}

			if ((PlayerLeft == 0) && (PlayerAlive == 1))//���𗣂����獶�X���摜���\���ɂ�
			{
				PlayerHyoji = 1;//���ʉ摜��\��
			}

			if ((PlayerRight == 1) && (PlayerAlive == 1))
			{
				PlayerHyoji = 0;
				DrawGraph(PlayerX - PlayerR, PlayerY - PlayerR, graphHandle6, TRUE);
			}

			//�G
			if (EnemyAlive == 1)
			{
				DrawGraph(EnemyX - EnemyR, EnemyY - EnemyR, graphHandle1, TRUE);
			}

			if (Enemy2Alive == 1)
			{
				DrawGraph(Enemy2X - Enemy2R, Enemy2Y - Enemy2R, graphHandle1, TRUE);
			}

			//�Q�[���I�[�o�[�ڍs
			if (PlayerAlive == 0)
			{
				StopSoundMem(BGM2);
				DrawFormatString(350, 270, GetColor(255, 255, 255), "GAME OVER", TRUE);
				DrawFormatString(345, 300, GetColor(255, 255, 255), "SCORE�F%d", SCORE);
				DrawFormatString(300, 330, GetColor(255, 255, 255), "PUSH ESC KEY TO END", TRUE);
			}

			//�X�R�A�\��
			DrawFormatString(0, 20, GetColor(255, 255, 255), " SCORE = %d", SCORE);
			DrawFormatString(300, 20, GetColor(255, 255, 255), " SHOT THE CULBS !!");

			break;
		}

		//---------  �����܂łɃv���O�������L�q  ---------//
		ScreenFlip();//�i�_�u���o�b�t�@�j����
		// 20�~���b�ҋ@�i�^��60FPS�j
		WaitTimer(20);
		// Windows �V�X�e�����炭�������������
		if (ProcessMessage() == -1)
		{
			break;
		}
		// �d�r�b�L�[�������ꂽ�烋�[�v���甲����
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)
		{
			break;
		}
	}
	//Dx���C�u�����I������
	DxLib_End();

	return 0;
}