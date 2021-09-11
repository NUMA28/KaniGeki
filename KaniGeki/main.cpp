#include "DxLib.h"
#include <math.h>

const char TITLE[] = "蟹撃 - KANIGEKI";

const int WIN_WIDTH = 800; //ウィンドウ横幅
const int WIN_HEIGHT = 600;//ウィンドウ縦幅


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	ChangeWindowMode(TRUE);						//ウィンドウモードに設定
	//ウィンドウサイズを手動では変更できず、かつウィンドウサイズに合わせて拡大できないようにする
	SetWindowSizeChangeEnableFlag(FALSE, FALSE);
	SetMainWindowText(TITLE);					// タイトルを変更
	SetGraphMode(WIN_WIDTH, WIN_HEIGHT, 32);	//画面サイズの最大サイズ、カラービット数を設定（モニターの解像度に合わせる）
	SetWindowSizeExtendRate(1.0);				//画面サイズを設定（解像度との比率で設定）
	SetBackgroundColor(0, 0, 0);		// 画面の背景色を設定する

	//Dxライブラリの初期化
	if (DxLib_Init() == -1) { return -1; }

	//（ダブルバッファ）描画先グラフィック領域は裏面を指定
	SetDrawScreen(DX_SCREEN_BACK);

	//画像などのリソースデータの変数宣言と読み込み
	//自機(プレイヤー)
	int PlayerX = 376;//プレイヤー初期出現X座標
	int PlayerY = 400;//プレイヤー初期出現Y座標
	int PlayerR = 25;//プレイヤー半径
	int PlayerAlive = 1;//プレイヤー存在フラグ(1で生存、0で死亡)
	int PlayerTimer = 0;//プレイヤーのアニメーション用タイマー
	int PlayerHyoji = 1;//プレイヤー正面画像の表示フラグ(1で表示、0で非表示)
	int PlayerLeft = 0;//プレイヤーが左を押したときの反応フラグ
	int PlayerRight = 0;//プレイヤーが右を押したときの反応フラグ

	//弾
	int BulX = 300;//弾1のx座標
	int BulY = 300;//弾1のy座標
	int BulR = 10;//弾1の半径
	int Bullet1Flag = 0;//弾1の存在フラグ

	//弾2
	int Bul2X = 300;//弾2のx座標
	int Bul2Y = 300;//弾2のy座標
	int Bul2R = 10;//弾2の半径
	int Bullet2Flag = 0;//弾2の存在フラグ

	//弾3
	int Bul3X = 300;//弾3のx座標
	int Bul3Y = 300;//弾3のy座標
	int Bul3R = 10;//弾3の半径
	int Bullet3Flag = 0;//弾3の存在フラグ

	//敵
	int EnemyX = 300;//敵1の初期出現X座標
	int EnemyY = 100;//敵1の初期出現Y座標
	int EnemyR = 25;//敵1半径
	int EnemyAlive = 1;//敵1の存在フラグ(1で表示、0非表示)
	int EnemySpeed = 10;//敵1の速度
	int EnemyTimer = 120;//敵1の復活タイマー

	//敵2
	int Enemy2X = 600;//敵2の初期出現X座標
	int Enemy2Y = 200;//敵2の初期出現Y座標
	int Enemy2R = 25;//敵2半径
	int Enemy2Alive = 1;//敵2の存在フラグ(1で表示、0非表示)
	int Enemy2Speed = 7;//敵2の速度
	int Enemy2Timer = 120;//敵2の復活タイマー

	//ゲームオーバー用フラグ
	int SE3 = LoadSoundMem("Sound\\GAME OVER.wav");
	int BGM1 = LoadSoundMem("Sound\\GAME OVER BGM.mp3");

	//ステージスクロール用
	int StageY = 0;
	int BGM2 = LoadSoundMem("Sound\\Main Theme.mp3");

	//ゲームシーン
	int scene = 0;

	//敵
	int graphHandle1 = LoadGraph("Image\\kani.png");//画像ファイル
	int SE1 = LoadSoundMem("Sound\\Explosive.wav");//プレイヤー＆敵の爆発音

	//自機
	int graphHandle2 = LoadGraph("Image\\Shooting Plane.png");//正面画像ファイル
	int graphHandle5 = LoadGraph("Image\\Shooting Plane Left.png");//左傾き画像ファイル
	int graphHandle6 = LoadGraph("Image\\Shooting Plane Right.png");//右傾き画像ファイル

	//プレイヤーの弾
	int graphHandle3 = LoadGraph("Image\\Bullet.png");//画像ファイル
	int SE2 = LoadSoundMem("Sound\\shot sound.wav");//弾の発射音

	//タイトル画面
	int graphHandle8 = LoadGraph("Image\\Title.png");

	//敵の弾
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

	int SCORE = 0;//スコア用

	//背景
	int graphHandle4 = LoadGraph("Image\\Haikei.png");//画像ファイル

	//ゲームループで使う変数の宣言
	char keys[256] = { 0 }; //最新のキーボード情報用
	char oldkeys[256] = { 0 };//1ループ（フレーム）前のキーボード情報

	// ゲームループ
	while (1)
	{
		//最新のキーボード情報だったものは１フレーム前のキーボード情報として保存

		//最新のキーボード情報を取得
		GetHitKeyStateAll(keys);

		//画面クリア
		ClearDrawScreen();
		//---------  ここからプログラムを記述  ----------//

		//Zキーを押したらシーンを0にしてタイトル画面へ


		//更新処理

		switch (scene)
		{
		case 0://タイトル画面
			if (keys[KEY_INPUT_Z] == 1)
			{
				scene = 1;
			}
			DrawGraph(0, 0, graphHandle8, TRUE);
			DrawFormatString(315, 455, GetColor(0, 0, 0), "PUSH Z KEY TO START");
			DrawFormatString(600, 550, GetColor(0, 0, 0), "音楽素材：魔王魂様");
			break;


		case 1://ゲームプレイ画面

			if (CheckSoundMem(BGM2) == 0)
			{
				PlaySoundMem(BGM2, DX_PLAYTYPE_LOOP);
			}

			//自機のコントロール
			if (keys[KEY_INPUT_RIGHT] == 1)//右が押されたとき
			{
				PlayerX = PlayerX + 5;
				PlayerRight = 1;
				PlayerLeft = 0;
			}

			if (keys[KEY_INPUT_RIGHT] == 0)//右が離されたとき
			{
				PlayerRight = 0;
			}

			if (keys[KEY_INPUT_LEFT] == 1)//左が押されたとき
			{
				PlayerX = PlayerX - 5;
				PlayerLeft = 1;
				PlayerRight = 0;
			}

			if (keys[KEY_INPUT_LEFT] == 0)//左が離されたとき
			{
				PlayerLeft = 0;
			}

			if (keys[KEY_INPUT_UP] == 1)//上
			{
				PlayerY = PlayerY - 5;
			}

			if (keys[KEY_INPUT_DOWN] == 1)//下
			{
				PlayerY = PlayerY + 2;
			}

			//弾の発射
			if ((keys[KEY_INPUT_SPACE] == 1) && (PlayerAlive == 1))
			{
				if ((Bullet1Flag == 0) && (Bullet2Flag == 0) && (Bullet3Flag == 0))//弾フラグ0で押されたとき
				{

					BulX = PlayerX;
					BulY = PlayerY - PlayerR;//弾を表示
					Bullet1Flag = 1;//弾フラグを1に(画面内を飛んでいる間)
					PlaySoundMem(SE2, DX_PLAYTYPE_BACK, TRUE);

					Bul2X = PlayerX;
					Bul2Y = PlayerY - PlayerR;//弾2を表示
					Bullet2Flag = 1;

					Bul3X = PlayerX;
					Bul3Y = PlayerY - PlayerR;//弾2を表示
					Bullet3Flag = 1;
				}
			}

			//弾1の挙動
			if (Bullet1Flag == 1)
			{
				BulY = BulY - 12;//弾の速度

				//弾が画面からはみ出した際
				if (BulY <= 0)
				{
					Bullet1Flag = 0;
				}
			}

			//弾2の挙動
			if (Bullet2Flag == 1)
			{
				Bul2Y = Bul2Y - 12;
				Bul2X = Bul2X - 3;

				//弾が画面からはみ出した際
				if (Bul2Y <= 0)
				{
					Bullet2Flag = 0;
				}
			}

			//弾3の挙動
			if (Bullet3Flag == 1)
			{
				Bul3Y = Bul3Y - 12;
				Bul3X = Bul3X + 3;

				//弾が画面からはみ出した際
				if (Bul3Y <= 0)
				{
					Bullet3Flag = 0;
				}
			}

			//敵移動
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

			//敵復活
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

			//敵1の弾のタイマー
			if ((EnemyAlive == 1) && (E1BF == 0) && (Shot1 == 0))
			{
				E1BT = E1BT - 1;

				if (E1BT < 0)
				{
					E1BF = 1;
					E1BT = 10;
				}
			}

			//敵1の弾表示
			if ((E1BF == 1) && (Shot1 == 0))
			{
				Enemy1BulX = EnemyX;
				Enemy1BulY = EnemyY - EnemyR;//弾を表示
				Shot1 = 1;
			}

			//敵1の弾発射
			if (Shot1 == 1)
			{
				Enemy1BulY = Enemy1BulY + 9;
				if (Enemy1BulY > 650)
				{
					E1BF = 0;
					Shot1 = 0;
				}
			}

			//敵2の弾のタイマー
			if ((Enemy2Alive == 1) && (E2BF == 0) && (Shot2 == 0))
			{
				E2BT = E2BT - 1;

				if (E2BT < 0)
				{
					E2BF = 1;
					E2BT = 10;
				}
			}

			//敵2の弾表示
			if ((E2BF == 1) && (Shot2 == 0))
			{
				Enemy2BulX = Enemy2X;
				Enemy2BulY = Enemy2Y - Enemy2R;//弾を表示
				Shot2 = 1;
			}

			//敵2の弾発射
			if (Shot2 == 1)
			{
				Enemy2BulY = Enemy2BulY + 9;
				if (Enemy2BulY > 650)
				{
					E2BF = 0;
					Shot2 = 0;
				}
			}

			//弾1と敵1の当たり判定
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

			//弾1と敵2の当たり判定
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

			//弾2と敵1の当たり判定
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

			//弾2と敵2の当たり判定
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

			//弾3と敵の当たり判定
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

			//弾3と敵2の当たり判定
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

			//敵1の弾と自機の当たり判定
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

			//敵2の弾と自機の当たり判定
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

			//自機と敵1の当たり判定
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

			//自機と敵2の当たり判定
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

			//背景スクロール
			if (scene == 1)
			{
				StageY = StageY + 1;
				if (StageY >= WIN_HEIGHT)
				{
					StageY = 0;
				}
			}

			//自機のアニメーション
			PlayerTimer = PlayerTimer + 50;

			//敵の弾のアニメーション
			EBAT = EBAT + 20;

			//自機のアニメーションのループ
			if (PlayerTimer > 199)
			{
				PlayerTimer = 0;
			}

			//敵の弾のアニメーションのループ
			if (EBAT > 320)
			{
				EBAT = 0;
			}

			//自機の壁の当たり判定
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

			//描画処理

			//背景
			DrawGraph(0, StageY, graphHandle4, TRUE);
			DrawGraph(0, StageY - WIN_HEIGHT, graphHandle4, TRUE);

			//弾
			if (Bullet1Flag == 1)//弾1
			{
				DrawGraph(BulX - BulR, BulY - BulR, graphHandle3, TRUE);
			}

			if (Bullet2Flag == 1)//弾2
			{
				DrawGraph(Bul2X - Bul2R, Bul2Y - Bul2R, graphHandle3, TRUE);
			}

			if (Bullet3Flag == 1)//弾3
			{
				DrawGraph(Bul3X - Bul3R, Bul3Y - Bul3R, graphHandle3, TRUE);
			}

			//敵の弾
			if (E1BF == 1)
			{
				DrawRectGraph(Enemy1BulX, Enemy1BulY, EBAT, 0, 20, 20, graphHandle7, TRUE);
			}

			if (E2BF == 1)
			{
				DrawRectGraph(Enemy2BulX, Enemy2BulY, EBAT, 0, 20, 20, graphHandle7, TRUE);
			}

			//プレイヤー
			if ((PlayerHyoji == 1) && (PlayerAlive == 1))//表示フラグ1で正面画像を表示
			{
				DrawRectGraph(PlayerX - PlayerR, PlayerY - PlayerR, PlayerTimer, 0, 50, 50, graphHandle2, TRUE);
			}

			if ((PlayerLeft == 1) && (PlayerAlive == 1))//左を押したら左傾き画像を表示
			{
				PlayerHyoji = 0;
				DrawGraph(PlayerX - PlayerR, PlayerY - PlayerR, graphHandle5, TRUE);//左傾き画像を表示
			}

			if ((PlayerLeft == 0) && (PlayerAlive == 1))//左を離したら左傾き画像を非表示にし
			{
				PlayerHyoji = 1;//正面画像を表示
			}

			if ((PlayerRight == 1) && (PlayerAlive == 1))
			{
				PlayerHyoji = 0;
				DrawGraph(PlayerX - PlayerR, PlayerY - PlayerR, graphHandle6, TRUE);
			}

			//敵
			if (EnemyAlive == 1)
			{
				DrawGraph(EnemyX - EnemyR, EnemyY - EnemyR, graphHandle1, TRUE);
			}

			if (Enemy2Alive == 1)
			{
				DrawGraph(Enemy2X - Enemy2R, Enemy2Y - Enemy2R, graphHandle1, TRUE);
			}

			//ゲームオーバー移行
			if (PlayerAlive == 0)
			{
				StopSoundMem(BGM2);
				DrawFormatString(350, 270, GetColor(255, 255, 255), "GAME OVER", TRUE);
				DrawFormatString(345, 300, GetColor(255, 255, 255), "SCORE：%d", SCORE);
				DrawFormatString(300, 330, GetColor(255, 255, 255), "PUSH ESC KEY TO END", TRUE);
			}

			//スコア表示
			DrawFormatString(0, 20, GetColor(255, 255, 255), " SCORE = %d", SCORE);
			DrawFormatString(300, 20, GetColor(255, 255, 255), " SHOT THE CULBS !!");

			break;
		}

		//---------  ここまでにプログラムを記述  ---------//
		ScreenFlip();//（ダブルバッファ）裏面
		// 20ミリ秒待機（疑似60FPS）
		WaitTimer(20);
		// Windows システムからくる情報を処理する
		if (ProcessMessage() == -1)
		{
			break;
		}
		// ＥＳＣキーが押されたらループから抜ける
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)
		{
			break;
		}
	}
	//Dxライブラリ終了処理
	DxLib_End();

	return 0;
}