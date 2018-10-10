#include "Matching_Cross.h"
#include "GameEngine.h"

#pragma warning (disable:4996)



int TileMatch(int g_iScore);
void FillTiles();			
void DrawTime(HDC hDC);		
void DrawScore(HDC hDC);	
void DrawHighScore(HDC hDC);
void DrawMatch(HDC hDC);	
void OnGameOver();			

extern void StopWave();
extern void PlayWave(); 
BOOL GameInitialize(HINSTANCE hInstance)	
{
	g_pGame = new GameEngine(hInstance,TEXT("Matching_Cross"), 
		TEXT("Matching_Cross"),IDI_Matching_Cross,IDI_Matching_Cross_SM,376,480);
	if(g_pGame==NULL)
		return FALSE;

	g_pGame->SetFrameRate(1);

	g_hInstance = hInstance;

	return TRUE;
}

void GameStart(HWND hWindow)
{
	srand(GetTickCount());	

	tick_cnt = GetTickCount();

	tick_start = tick_cnt;	

	g_hOffscreenDC = CreateCompatibleDC(GetDC(hWindow)); 

	HDC hDC = GetDC(hWindow);

	g_pTiles[0] = new Bitmap(hDC, IDB_TILE1, g_hInstance);
	g_pTiles[1] = new Bitmap(hDC, IDB_TILE2, g_hInstance);
	g_pTiles[2] = new Bitmap(hDC, IDB_TILE3, g_hInstance);
	g_pTiles[3] = new Bitmap(hDC, IDB_TILE4, g_hInstance);
	g_pTiles[4] = new Bitmap(hDC, IDB_TILE5, g_hInstance);
	g_pTiles[5] = new Bitmap(hDC, IDB_TILE6, g_hInstance);
	g_pTiles[6] = new Bitmap(hDC, IDB_TILE7, g_hInstance);
	g_pTiles[7] = new Bitmap(hDC, IDB_TILE8, g_hInstance);
	g_pTiles[8] = new Bitmap(hDC, IDB_TILE9, g_hInstance);
	g_pTiles[9] = new Bitmap(hDC, IDB_TILE10, g_hInstance);
	g_pTiles[10] = new Bitmap(hDC, IDB_TILE11, g_hInstance);
	g_pTiles[11] = new Bitmap(hDC, IDB_TILE12, g_hInstance);
	g_pTiles[12] = new Bitmap(hDC, IDB_TILE13, g_hInstance);
	g_pTiles[13] = new Bitmap(hDC, IDB_TILE14, g_hInstance);
	g_pTiles[14] = new Bitmap(hDC, IDB_TILE15, g_hInstance);
	g_pTiles[15] = new Bitmap(hDC, IDB_TILE16, g_hInstance);

	g_pOTiles[0] = new Bitmap(hDC, IDB_OTILE1, g_hInstance);
	g_pOTiles[1] = new Bitmap(hDC, IDB_OTILE2, g_hInstance);
	g_pOTiles[2] = new Bitmap(hDC, IDB_OTILE3, g_hInstance);
	g_pOTiles[3] = new Bitmap(hDC, IDB_OTILE4, g_hInstance);
	g_pOTiles[4] = new Bitmap(hDC, IDB_OTILE5, g_hInstance);
	g_pOTiles[5] = new Bitmap(hDC, IDB_OTILE6, g_hInstance);
	g_pOTiles[6] = new Bitmap(hDC, IDB_OTILE7, g_hInstance);
	g_pOTiles[7] = new Bitmap(hDC, IDB_OTILE1, g_hInstance);

	g_pNPCBitmap[0] = new Bitmap(hDC, IDB_NPC1, g_hInstance);
	g_pNPCBitmap[1] = new Bitmap(hDC, IDB_NPC2, g_hInstance);
	g_pNPCBitmap[2] = new Bitmap(hDC, IDB_NPC3, g_hInstance);
	g_pNPCBitmap[3] = new Bitmap(hDC, IDB_NPC4, g_hInstance);
	g_pNPCBitmap[4] = new Bitmap(hDC, IDB_NPC5, g_hInstance);


	g_pBG[0] = new Bitmap(hDC, IDB_MAINBG, g_hInstance);

	RECT rcBounds = { 0, 0, 376, 480 };

	g_pNPCSprite1 = new Sprite(g_pNPCBitmap[0], rcBounds, BA_STOP);
	g_pNPCSprite1->SetPosition(0, 1);
	g_pNPCSprite1->SetVelocity(0, 0);
	g_pNPCSprite1->SetZOrder(2);
	g_pGame->AddSprite(g_pNPCSprite1);

	g_pNPCSprite2 = new Sprite(g_pNPCBitmap[1], rcBounds, BA_STOP);
	g_pNPCSprite2->SetPosition(1, 1);
	g_pNPCSprite2->SetVelocity(0, 0);
	g_pNPCSprite2->SetZOrder(2);

	g_pNPCSprite3 = new Sprite(g_pNPCBitmap[2], rcBounds, BA_STOP);
	g_pNPCSprite3->SetPosition(60, 423);
	g_pNPCSprite3->SetVelocity(0, 0);
	g_pNPCSprite3->SetZOrder(2);
	g_pGame->AddSprite(g_pNPCSprite3);

	g_pNPCSprite4 = new Sprite(g_pNPCBitmap[3], rcBounds, BA_STOP);
	g_pNPCSprite4->SetPosition(200, 423);
	g_pNPCSprite4->SetVelocity(0, 0);
	g_pNPCSprite4->SetZOrder(2);
	
	g_pNPCSprite5 = new Sprite(g_pNPCBitmap[4], rcBounds, BA_STOP);
	g_pNPCSprite5->SetPosition(130, 250);
	g_pNPCSprite5->SetVelocity(0, 0);
	g_pNPCSprite5->SetZOrder(2);

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			if (j == 0)
				g_bTileStates[i][j] = false;
			else
				g_bTileStates[i][j] = true;
			g_iTiles[i][j] = 0;
		}
	}

	for (int i = 0; i < 2; i++)
	{
		g_iOrder[i] = 0;
	}

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			g_iTiles[i][j] = (rand() % 7) + 1;
		}
	}

	for (int i = 0; i < 2; i++)
	{
		if (g_iOrder[0] == g_iOrder[1])
			g_iOrder[0] = (g_iOrder[1] + 1) % 7 + 1;
		else
			g_iOrder[i] = (rand() % 7) + 1;
	}

	g_ptTile1.x = g_ptTile1.y = -1;
	g_ptTile2.x = g_ptTile2.y = -1;

	g_iHighScore = GetProfileInt("Matching_Cross", "highscore", 0);
	g_iScore = 0;
	g_match = 0;

	g_bGameOver = false;
}

void GameEnd()
{
	for (int i = 0; i < 8; i++)
	{
		delete g_pTiles[i];
		delete g_pOTiles[i];
	}

	delete g_pBG[0];

	for (int i = 0; i < 4; i++)
		delete g_pNPCBitmap[i];

	g_pGame->CleanupSprites();

	delete g_pGame;
}

void GameRestart()
{
	g_newStart = true;
	GameStart(g_pGame->GetWindow());
}

void GameActivate(HWND hWindow)
{
}

void GameDeactivate(HWND hWindow)
{
}

void GamePaint(HDC hDC)
{
	HDC hTempDC;

#ifdef	USE_DOUBLE_BUFFERING 
	HBITMAP hBitmap, hOldBitmap = NULL;
	hTempDC = CreateCompatibleDC(hDC);

	hBitmap = CreateCompatibleBitmap(hDC, g_pGame->GetWidth(),
		g_pGame->GetHeight());
	hOldBitmap = (HBITMAP)SelectObject(hTempDC, hBitmap);

	switch (delay)
	{
	case 0:
		g_pGame->AddSprite(g_pNPCSprite3);
		delay++;
		break;
	case 1:
		g_pGame->AddSprite(g_pNPCSprite4);
		delay++;
		break;

	case 2:
		delay = 0;
		break;
	}
#else
	hTempDC = hDC;
#endif

	int iTileWidth = g_pTiles[1]->GetWidth();
	int iTileHeight = g_pTiles[1]->GetHeight();

	int dx = 60;
	int dy = 86;

	g_pBG[0]->Draw(hTempDC, 0, 0, TRUE);

	g_pGame->DrawSprites(hTempDC);

	for (int i = 0; i < 5; i++)
	{
		for (int j = 1; j < 6; j++)
		{
			if (g_bTileStates[i][j] || 
				((i == g_ptTile1.x) && (j == g_ptTile1.y)) || 
				((i == g_ptTile2.x) && (j == g_ptTile2.y)))	
			{
				if (g_iTiles[i][j] != 0)
				{
					if (i == s_x&&j == s_y)
					{
						switch (g_iTiles[i][j])
						{
						case 0:
							dd = 8;
							break;
						case 1:
							dd = 9;
							break;
						case 2:
							dd = 10;
							break;
						case 3:
							dd = 11;
							break;
						case 4:
							dd = 12;
							break;
						case 5:
							dd = 13;
							break;
						case 6:
							dd = 14;
							break;
						case 7:
							dd = 15;
							break;
						}
						g_pTiles[dd]->Draw(hTempDC, dx + (i*iTileWidth), dy + (j*iTileHeight), TRUE);
					}
					else
						g_pTiles[g_iTiles[i][j]]->Draw(hTempDC, dx + (i * iTileWidth),
						dy + (j * iTileHeight), TRUE);
				}
			}
		}
	}

	for (int i = 0; i < 2; i++)
	{
		g_pOTiles[g_iOrder[i]]->Draw(hTempDC, (i * 138) + iTileWidth + 70, 423, TRUE);
	}

	DrawMatch(hTempDC);
	DrawTime(hTempDC);
	DrawScore(hTempDC);
	DrawHighScore(hTempDC);

	HBRUSH NBrush, OBrush;

	RECT rect;
	rect.left = (g_x*g_pTiles[1]->GetWidth()) + 60;
	rect.top = (g_y*g_pTiles[1]->GetWidth()) + 87;
	rect.right = (g_x*g_pTiles[1]->GetWidth()) + 60 + 50;
	rect.bottom = (g_y*g_pTiles[1]->GetWidth()) + 87 + 50;

	RECT rect2;
	rect.left = (g_x*g_pTiles[1]->GetWidth()) + 60 - 1;
	rect.top = (g_y*g_pTiles[1]->GetWidth()) + 87 - 1;
	rect.right = (g_x*g_pTiles[1]->GetWidth()) + 60 + 50 + 1;
	rect.bottom = (g_y*g_pTiles[1]->GetWidth()) + 87 + 50 + 1;

	NBrush = CreateSolidBrush(RGB(0, 0, 255));
	OBrush = (HBRUSH)SelectObject(hTempDC, NBrush);

	if (g_RectState == TRUE)
	{
		FrameRect(hTempDC, &rect, NBrush);
		FrameRect(hTempDC, &rect2, NBrush);
	}

#ifdef USE_DOUBLE_BUFFERING
	BitBlt(hDC, 0, 0, g_pGame->GetWidth(), g_pGame->GetHeight(),
		hTempDC, 0, 0, SRCCOPY);
	if (hOldBitmap != NULL)
		SelectObject(hTempDC, hOldBitmap);
	if (hBitmap != NULL)
		DeleteObject(hBitmap);
	DeleteDC(hTempDC);
#endif
}

void GameCycle() //
{
	if (!g_bGameOver)
	{
		HWND hWindow = g_pGame->GetWindow();
		HDC hDC = GetDC(hWindow);

		GamePaint(hDC);

		tick_cnt = GetTickCount();
		tick_now = tick_cnt;
		sec = (2 - ((tick_now - tick_start) / 1000));

		g_iScore = TileMatch(g_iScore);
		FillTiles();

		g_pGame->AddSprite(g_pNPCSprite1);

		if (sec <= 0)
		{
			OnGameOver();
			return;
		}
		InvalidateRect(g_pGame->GetWindow(), NULL, FALSE);
	}
}

void HandleKeys()
{
}

void MouseButtonDown(int x, int y, BOOL bLeft) //마우스 왼쪽 버튼 클릭시
{
	int dx = 60; 
	int dy = 87;

	int iTileX=0;//각각의 타일의 배열의 위치를 저장할 변수
	int iTileY=0;
	//타일의 범위 내에 있을 경우
	if (((x - dx) >= 0) && ((y - dy) >= g_pTiles[1]->GetWidth()) && //x,y 시작 부분
		((x - dx) <= g_pTiles[1]->GetWidth() * 5) &&//가로 끝
		((y - dy) <= g_pTiles[1]->GetWidth() * 6))	//세로 끝
	{
		iTileX = (x - dx) / (g_pTiles[1]->GetWidth() / 1);//타일맵안에서의 x,y 좌표
		iTileY = (y - dy) / (g_pTiles[1]->GetHeight() / 1);
	}

	if (g_ptTile1.x == -1) //선택을 하지 않았을 경우
	{
		g_ptTile1.x = iTileX;
		g_ptTile1.y = iTileY; //타일좌표 대입
	}

	else if ((iTileX != g_ptTile1.x) || (iTileY != g_ptTile1.y))//선택한 타일이 첫번째 선택한 타일과 다른경우(첫번째 두번째 타일이 같지 않은 경우)
	{
		if (g_ptTile2.x == -1)//두번째(바꿀 타일)를 선택하지 않았을 경우
		{
			g_ptTile2.x = iTileX;
			g_ptTile2.y = iTileY; 
			if ((((g_ptTile1.x == (((x - dx) + (g_pTiles[1]->GetWidth() / 1)) / (g_pTiles[1]->
				GetWidth() / 1)))
				|| (g_ptTile1.x == (((x - dx) - (g_pTiles[1]->GetWidth() / 1)) / (g_pTiles[1]->
				GetWidth() / 1)))))//
				&& (g_ptTile1.y == g_ptTile2.y)
				|| (((g_ptTile1.y == (((y - dy) + (g_pTiles[1]->GetHeight() / 1)) / (g_pTiles[1]->
				GetHeight() / 1)))
				|| (((g_ptTile1.y == (((y - dy) + (g_pTiles[1]->GetHeight() / 1)) / (g_pTiles[1]->
				GetHeight() / 1)))
				&& (g_ptTile1.x == g_ptTile2.x))))))
			{
				g_tmpTile = g_iTiles[g_ptTile1.x][g_ptTile1.y];
				g_iTiles[g_ptTile1.x][g_ptTile1.y] = g_iTiles[g_ptTile2.x][g_ptTile2.y];
				g_iTiles[g_ptTile2.x][g_ptTile2.y] = g_tmpTile;

				g_iScore = TileMatch(g_iScore);
				FillTiles();

				g_ptTile1.x = g_ptTile1.y = g_ptTile2.x = g_ptTile2.y = -1;
			}
		}
		else
		{
			g_ptTile1.x = g_ptTile1.y = g_ptTile2.x = g_ptTile2.y = -1;
		}
	}
	InvalidateRect(g_pGame->GetWindow(), NULL, FALSE);
}

void MouseButtonUp(int x, int y, BOOL bLeft)
{

}
			
void MouseMove(int x, int y)
{
	int dx = 60;
	int dy = 87;

	int iTileX;
	int iTileY;

	if (((x - dx) >= 0) && ((y - dy) >= g_pTiles[1]->GetWidth()) &&
		((x - dx) <= g_pTiles[1]->GetWidth() * 5) && ((y - dy) <= g_pTiles[1]->GetWidth() * 6))
	{
		g_RectState = TRUE;
		iTileX = (x - dx) / (g_pTiles[1]->GetWidth() / 1);
		iTileY = (y - dy) / (g_pTiles[1]->GetHeight() / 1);
		s_x = iTileX;
		s_y = iTileY;

		g_x = iTileX;
		g_y = iTileY;
		g_pGame->AddSprite(g_pNPCSprite1);
	}
	else
		g_RectState = FALSE;

	InvalidateRect(g_pGame->GetWindow(), NULL, FALSE);
}

int TileMatch(int g_iScore)
{

	int cnt = 0;
	int g_iMatchOrder = 0;
	bool recheck = false;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 1; j < 6; j++)
		{
			if ((g_iTiles[i][j] != 0) && (g_iTiles[i + 1][j] != 0) &&
				(g_iTiles[i][j] == g_iTiles[i + 1][j]))
			{
				for (int k = 1; ((i + k) < 5); k++)
				{
					if (g_iTiles[i][j] == g_iTiles[i + k][j])
					{
						cnt++;
					}
					else
						break;
				}
				if (cnt >= 2)
				{
					g_pGame->AddSprite(g_pNPCSprite2);

					for (int k = 0; k <= cnt; k++)
					{
						if (g_iTiles[i + k][j] == g_iOrder[0])
						{
							g_iMatchOrder = 1;
							g_iScore = g_iScore + 100;
							g_iOrder[0] = (rand() % 7) + 1;
							if (g_iOrder[0] == g_iOrder[1])
								g_iOrder[0] = (g_iOrder[1] + 1) % 7 + 1;
						}
						else if (g_iTiles[i + k][j] == g_iOrder[1])
						{
							g_iMatchOrder = 1;
							g_iScore = g_iScore + 100;
							g_iOrder[1] = (rand() % 7) + 1;
							if (g_iOrder[0] == g_iOrder[1])
								g_iOrder[1] = (g_iOrder[0] + 1) % 7 + 1;
						}

						g_pGame->AddSprite(g_pNPCSprite5);

						g_iTiles[i + k][j] = 0;
					}

					if (g_iMatchOrder == 1)
					{
						PlaySound(MAKEINTRESOURCE(IDW_MATCHORDER), g_hInstance, SND_ASYNC | SND_RESOURCE);
				
						g_iMatchOrder = 0;
					}
					else
					{
						PlaySound((LPCSTR)IDW_MATCH, g_hInstance, SND_ASYNC | SND_RESOURCE);
					}

					if (cnt >= 2)
					{
						g_match++;
						g_iScore = g_iScore + 10;
					}
					else if (cnt >= 3)
					{
						g_match++;
						g_iScore = g_iScore + 30;
					}
					else
					{
						g_match++;
						g_iScore = g_iScore + 50;
					}
					recheck = true;
				}
				cnt = 0;
			}
		}
	}

	for (int j = 1; j < 4; j++)
	{
		for (int i = 0; i < 5; i++)
		{
			if ((g_iTiles[i][j] != 0) && (g_iTiles[i][j + 1] != 0 &&
				(g_iTiles[i][j] == g_iTiles[i][j + 1])))
			{
				for (int k = 1; ((j + k) < 6); k++)
				{
					if (g_iTiles[i][j] == g_iTiles[i][j + k])
					{
						cnt++;
					}
					else
						break;
				}
				if (cnt >= 2)
				{
					g_pGame->AddSprite(g_pNPCSprite2);

					for (int k = 0; k <= cnt; k++)
					{
						if (g_iTiles[i][j + k] == g_iOrder[0])
						{
							g_iMatchOrder = 1;
							g_iScore = g_iScore + 100;
							g_iOrder[0] = (rand() % 7) + 1;
							if (g_iOrder[0] == g_iOrder[1])
								g_iOrder[0] = (g_iOrder[1] + 1) % 7 + 1;

						}
						else if (g_iTiles[i][j + k] == g_iOrder[1])
						{
							g_iMatchOrder = 1;
							g_iScore = g_iScore + 100;
							g_iOrder[1] = (rand() % 7) + 1;
							if (g_iOrder[0] == g_iOrder[1])
								g_iOrder[1] = (g_iOrder[0] + 1) % 7 + 1;
						}
						g_iTiles[i][j + k] = 0;
					}
					if (g_iMatchOrder == 1)
					{
						PlaySound((LPCSTR)IDW_MATCHORDER, g_hInstance, SND_ASYNC | SND_RESOURCE);
						g_iMatchOrder = 0;
					}
					else
					{
						PlaySound((LPCSTR)IDW_MATCH, g_hInstance, SND_ASYNC | SND_RESOURCE);
					}

					if (cnt >= 2)
					{
						g_match++;
						g_iScore = g_iScore + 10;
					}
					else if (cnt >= 3)
					{
						g_match++;
						g_iScore = g_iScore + 30;
					}
					else
					{
						g_match++;
						g_iScore = g_iScore + 50;
					}
					recheck = true;
				}
				cnt = 0;
			}
		}
	}

	if (g_iScore > g_iHighScore)
	{
		char szHighScore[32];
		sprintf(szHighScore, "%d", g_iScore);

		WriteProfileString("Matching_Cross", "highscore", szHighScore);
	}

	if (recheck == true)
	{
		recheck = false;
		TileMatch(g_iScore);
	}
	return g_iScore;
}

void FillTiles()	
{
	int empty = 0;

	for (int j = 5; j > 0; j--)
	for (int i = 0; i<5; i++)
	{
		if (g_iTiles[i][j] == 0)
		{
			for (int k = 0; ((j - k) >0); k++)
			{
				if (g_iTiles[i][j - k] == 0)
				{
					empty++;
				}
			}
		}

		if (empty != 0) 
		{
			for (int k = 0; (j - k) > 0; k++)
			{
				if ((j - k - empty) < 0)
				{
					g_iTiles[i][j - k] = g_iTiles[i][0];
				}
				else
				{
					g_iTiles[i][j - k] = g_iTiles[i][j - k - empty];
				}
				g_iTiles[i][0] = (rand() % 7) + 1; 
			}
		}
		empty = 0;

	}
}

void DrawTime(HDC hDC)
{
	char szTime[32];
	char szName[6];

	RECT rect = { g_pGame->GetWidth() / 2 + 285, 90, g_pGame->GetWidth() / 2 - 15, 120 };
	RECT rect2 = { g_pGame->GetWidth() / 2 + 285, 65, g_pGame->GetWidth() / 2 - 15, 88 };

	SetTextColor(hDC, RGB(0, 0, 0));
	SetBkMode(hDC, TRANSPARENT);

	sprintf(szTime, "- %d - \r\n", sec);
	DrawText(hDC, szTime, strlen(szTime), &rect, DT_CENTER);
	sprintf(szName, "Time");
	DrawText(hDC, szName, strlen(szName), &rect2, DT_CENTER);
}

void DrawScore(HDC hDC)
{
	char szScore[32];
	char szName[6];

	RECT rect = { g_pGame->GetWidth() / 2 + 35, 38, g_pGame->GetWidth() - 85, 58 };
	SetTextColor(hDC, RGB(0, 0, 0));
	SetBkMode(hDC, TRANSPARENT);

	sprintf(szName, "Score");
	DrawText(hDC, szName, strlen(szName), &rect, DT_LEFT);
	sprintf(szScore, "%d\r\n", g_iScore);
	DrawText(hDC, szScore, strlen(szScore), &rect, DT_RIGHT);
}

void DrawHighScore(HDC hDC)
{
	char szName[6];
	char szScore[32];

	RECT rect = { 115, 38, g_pGame->GetWidth() / 2 - 20, 58 };
	SetTextColor(hDC, RGB(0, 0, 0));
	SetBkMode(hDC, TRANSPARENT);

	sprintf(szName, "H");
	DrawText(hDC, szName, strlen(szName), &rect, DT_LEFT);
	sprintf(szScore, "%d\r\n", g_iHighScore);
	DrawText(hDC, szScore, strlen(szScore), &rect, DT_RIGHT);
}

void DrawMatch(HDC hDC)
{
	char szName[6];
	char szmatch[32];

	RECT rect = { 115, 58, g_pGame->GetWidth() / 2 - 20, 88 };
	SetTextColor(hDC, RGB(0, 0, 0));
	SetBkMode(hDC, TRANSPARENT);

	sprintf(szName, "M");
	DrawText(hDC, szName, strlen(szName), &rect, DT_LEFT);
	sprintf(szmatch, "%d\r\n", g_match);
	DrawText(hDC, szmatch, strlen(szmatch), &rect, DT_RIGHT);
}

void OnGameOver()
{
	BOOL bHighestRecord = FALSE;
	if (g_iScore > g_iHighScore)
	{
		char szHighScore[32];
		sprintf(szHighScore, "%d", g_iScore);
		WriteProfileString("Matching_Cross", "highscore", szHighScore);
		bHighestRecord = TRUE;
	}
	StopWave();
	char szMessage[256];
	if (bHighestRecord)
	{
		PlaySound((LPCSTR)IDW_CONG, g_hInstance, SND_ASYNC | SND_RESOURCE);
		sprintf(szMessage, "축하합니다! %d점으로 최고 기록을 갱신하였습니다!"\
			"\r\n한판 더 하시겠습니까?", g_iScore);
	}
	else
	{
		PlaySound((LPCSTR)IDW_END, g_hInstance, SND_ASYNC | SND_RESOURCE);
		sprintf(szMessage, "한판 더 하시겠습니까?", g_iScore);
	}

	int nRet = MessageBox(g_pGame->GetWindow(), szMessage, "Game Over", MB_YESNO);

	if (nRet == IDYES)
	{
		GameRestart();
		PlayWave();
	}
	else
		PostQuitMessage(0);
}