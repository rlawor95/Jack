#pragma comment(lib,"msimg32.lib") 

#include "GameEngine.h"

#include "resource.h"

GameEngine *GameEngine ::m_pGameEngine = NULL;

#include <dsound.h>
#include <mmsystem.h>	

LPDIRECTSOUND lpDirectSound;
LPDIRECTSOUNDBUFFER dsbSound;

DSBUFFERDESC DSB_desc;

bool DSoundInit(HWND);
void ReleaseDSound();
bool LoadWave(char *);
void PlayWave();
void StopWave();


bool DSoundInit(HWND hwnd)
{
	if(DirectSoundCreate (NULL, &lpDirectSound, NULL) != DS_OK)
	{
		return (false);
	}
	if(lpDirectSound -> SetCooperativeLevel(hwnd, DSSCL_NORMAL) != DS_OK)
	{
		return (false);
	}

	return (true);
}

void ReleaseDSound()
{
	if(dsbSound != NULL)
	{
		dsbSound -> Release();
		dsbSound = NULL;
	}

	if(lpDirectSound != NULL)
	{
		lpDirectSound -> Release();
		lpDirectSound = NULL;
	}
}

//bool LoadWave(char *filename)
//{
//	HMMIO wavefile;
//	wavefile = mmioOpen(filename, 0, MMIO_READ | MMIO_ALLOCBUF);
//	if(wavefile == NULL)
//	{
//		return (false);
//	}
//
//	MMCKINFO parent;
//	memset(&parent, 0, sizeof(MMCKINFO));
//	parent.fccType = mmioFOURCC('W','A','V','E');
//	mmioDescend(wavefile, &parent, 0, MMIO_FINDRIFF);
//
//	MMCKINFO child;
//	memset(&child, 0, sizeof(MMCKINFO));
//	child.fccType = mmioFOURCC('f','m','t',' ');
//	mmioDescend(wavefile, &child, &parent, 0);
//
//	WAVEFORMATEX wavefmt;
//	mmioRead(wavefile, (char*)&wavefmt, sizeof(wavefmt));
//	if(wavefmt.wFormatTag != WAVE_FORMAT_PCM)
//	{
//		return (false);
//	}
//
//	mmioAscend(wavefile, &child, 0);
//	child.ckid = mmioFOURCC('d','a','t','a');
//	mmioDescend(wavefile, &child, &parent, MMIO_FINDCHUNK);
//
//	DSBUFFERDESC bufdesc;
//	memset(&bufdesc, 0, sizeof(DSBUFFERDESC));
//	bufdesc.dwSize = sizeof(DSBUFFERDESC);
//
//	bufdesc.dwFlags = 0;
//	bufdesc.dwBufferBytes = child.cksize;
//	bufdesc.lpwfxFormat = &wavefmt;
//	if((lpDirectSound ->CreateSoundBuffer(&bufdesc, &dsbSound, NULL)) != DS_OK)
//	{
//		return (false);
//	}
//
//	void *write1 = 0, *write2 = 0;
//	unsigned long length1, length2;
//	dsbSound->Lock(0, child.cksize, &write1, &length1, &write2, &length2, 0);
//	if (write1 > 0)
//		mmioRead(wavefile, (char*)write1, length1);
//	if(write2 > 0)
//		mmioRead(wavefile, (char*)write2, length2);
//	dsbSound->Unlock(write1, length1, write2, length2);
//
//	mmioClose(wavefile, 0);
//
//	return(true);
//}

bool LoadWave(char *filename)
{
	HMMIO wavefile;
	MMCKINFO parent;	 
	MMCKINFO child;	 
	
	if (!(wavefile=mmioOpen(filename, 0, MMIO_READ | MMIO_ALLOCBUF)))
	{
		return (false);
	}

	memset(&parent, 0, sizeof(MMCKINFO));
	parent.fccType = mmioFOURCC('W', 'A', 'V', 'E');
	if (mmioDescend(wavefile, &parent, 0, MMIO_FINDRIFF))
	{
		mmioClose(wavefile, 0);
		return false;
	}

	memset(&child, 0, sizeof(MMCKINFO));
	child.fccType = mmioFOURCC('f', 'm', 't', ' ');
	if (mmioDescend(wavefile, &child, &parent, MMIO_FINDCHUNK))
	{
		mmioClose(wavefile, 0);
		return false;
	}

	WAVEFORMATEX wavefmt;
	mmioRead(wavefile, (char*)&wavefmt, sizeof(wavefmt));
	if (wavefmt.wFormatTag != WAVE_FORMAT_PCM)
	{
		return (false);
	}

	mmioAscend(wavefile, &child, 0);
	child.ckid = mmioFOURCC('d', 'a', 't', 'a');
	if(mmioDescend(wavefile, &child, &parent, MMIO_FINDCHUNK))
		return false;

	DSBUFFERDESC bufdesc;
	memset(&bufdesc, 0, sizeof(DSBUFFERDESC));
	bufdesc.dwSize = sizeof(DSBUFFERDESC);

	bufdesc.dwFlags = 0;
	bufdesc.dwBufferBytes = child.cksize;
	bufdesc.lpwfxFormat = &wavefmt;
	if ((lpDirectSound->CreateSoundBuffer(&bufdesc, &dsbSound, NULL)) != DS_OK)
	{
		return (false);
	}

	void *write1 = 0, *write2 = 0;
	unsigned long length1, length2;
	dsbSound->Lock(0, child.cksize, &write1, &length1, &write2, &length2, 0);
	if (write1 > 0)
		mmioRead(wavefile, (char*)write1, length1);
	if (write2 > 0)
		mmioRead(wavefile, (char*)write2, length2);
	dsbSound->Unlock(write1, length1, write2, length2);

	mmioClose(wavefile, 0);

	return(true);
}

void PlayWave()
{
	dsbSound->SetCurrentPosition(0);
	dsbSound->Play(0,0,DSBPLAY_LOOPING);
}

void StopWave()
{
}

///////////////////////////////////////////////

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	MSG			msg;
	static int	iTickTrigger = 0;
	int			iTickCount;
		
	if(GameInitialize(hInstance))
	{
		if (!GameEngine::GetEngine()->Initialize(iCmdShow))
			return FALSE;

		while(TRUE)
		{
			if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if(msg.message == WM_QUIT)
					break;
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				if(!GameEngine::GetEngine() ->GetSleep())
				{
					iTickCount = GetTickCount();
					if(iTickCount > iTickTrigger)
					{
						iTickTrigger = iTickCount + GameEngine::GetEngine()->GetFrameDelay();
						//�ð��� ���߱� ���� ġƮŰ�� ���� HanldeKeys ����
						//HandleKeys();
						GameCycle();
					}
				}
			}
		}
		return (int)msg.wParam;
	}
	GameEnd();

	StopWave();
	ReleaseDSound();

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return GameEngine::GetEngine()->HandleEvent(hWindow, msg, wParam, lParam);
}

BOOL GameEngine::CheckSpriteCollision(Sprite* pTestSprite)
{
	vector<Sprite*>::iterator siSprite;
	for (siSprite = m_vSprites.begin(); siSprite != m_vSprites.end(); siSprite++)
	{
		if (pTestSprite == (*siSprite))
			continue;
	}
	return FALSE;
}


GameEngine::GameEngine(HINSTANCE hInstance, LPTSTR szWindowClass,
					   LPTSTR szTitle, WORD wlcon, WORD wSmallIcon, int iWidth, int iHeight)
{
	m_pGameEngine = this;
	m_hInstance = hInstance;
	m_hWindow = NULL;
	if(lstrlen(szWindowClass) > 0)
		lstrcpy(m_szWindowClass, szWindowClass);
	if(lstrlen(m_szTitle) >0)
		lstrcpy(m_szTitle, szTitle);
	m_wlcon = wlcon;
	m_wSmallIcon = wSmallIcon;
	m_iWidth = iWidth;
	m_iHeight = iHeight;
	m_iFrameDelay = 50;	
	m_bSleep = TRUE;
	m_uiJoystickID = 0;
	m_vSprites.reserve(100);
}

GameEngine::~GameEngine()
{
}


BOOL GameEngine::Initialize(int iCmdShow)
{
	WNDCLASSEX		wndclass;

	wndclass.cbSize		= sizeof(wndclass);
	wndclass.style			= CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc		= WndProc;
	wndclass.cbClsExtra	  =  0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = m_hInstance;
	wndclass.hIcon = LoadIcon(m_hInstance, MAKEINTRESOURCE(IDI_Matching_Cross));
	wndclass.hIconSm = LoadIcon(m_hInstance, MAKEINTRESOURCE(IDI_Matching_Cross_SM));
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);	
	wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = m_szWindowClass;

	if(!RegisterClassEx(&wndclass))
		return FALSE;

	int iWindowWidth = m_iWidth + GetSystemMetrics(SM_CXFIXEDFRAME) * 2,
		iWindowHeight = m_iHeight + GetSystemMetrics(SM_CYFIXEDFRAME) * 2 +
		GetSystemMetrics(SM_CYCAPTION);
	if(wndclass.lpszMenuName != NULL)
		iWindowHeight += GetSystemMetrics(SM_CYMENU);
	int iXWindowPos = (GetSystemMetrics(SM_CXSCREEN) - iWindowWidth) /2,
		iYWindowPos = (GetSystemMetrics(SM_CYSCREEN) - iWindowHeight) /2;

	m_hWindow = CreateWindow(m_szWindowClass, m_szTitle, WS_POPUPWINDOW |
		WS_CAPTION | WS_MINIMIZEBOX, iXWindowPos, iYWindowPos, iWindowWidth,
		iWindowHeight, NULL, NULL, m_hInstance, NULL);
	if(!m_hWindow)
		return FALSE;
	
	ShowWindow(m_hWindow, iCmdShow);
	UpdateWindow(m_hWindow);

	return TRUE;
}

LRESULT GameEngine::HandleEvent(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_CREATE:
		SetWindow(hWindow);
		GameStart(hWindow);
		
		//sound
		DSoundInit(hWindow);
		LoadWave("BGM.wav");
		PlayWave();
		return 0;

	case WM_SETFOCUS:
		GameActivate(hWindow);
		SetSleep(FALSE);
		return 0;

	case WM_KILLFOCUS:
		GameDeactivate(hWindow);
		SetSleep(TRUE);
		return 0;

	case WM_PAINT:
		HDC			hDC;
		PAINTSTRUCT	ps;
		hDC = BeginPaint(hWindow, &ps);

		GamePaint(hDC);

		EndPaint(hWindow, &ps);
		return 0;

	case WM_KEYDOWN:
		if (wParam == 'R') GameStart(GetWindow());	
		if (wParam == 'C') HandleKeys();			
		break;

	case WM_LBUTTONDOWN:
		MouseButtonDown(LOWORD(lParam), HIWORD(lParam), TRUE);
		return 0;

	case WM_LBUTTONUP:
		MouseButtonUp(LOWORD(lParam), HIWORD(lParam), TRUE);
		return 0;

	case WM_RBUTTONDOWN:
		MouseButtonDown(LOWORD(lParam), HIWORD(lParam), FALSE);
		return 0;
			
	case WM_RBUTTONUP:
		MouseButtonUp(LOWORD(lParam), HIWORD(lParam), FALSE);
		return 0;

	case WM_MOUSEMOVE:
		MouseMove(LOWORD(lParam), HIWORD(lParam));
		return 0;

	case WM_DESTROY:
		GameEnd();
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWindow, msg, wParam, lParam);
}

void GameEngine::ErrorQuit(LPTSTR szErrorMsg)
{
	MessageBox(GetWindow(), szErrorMsg, TEXT("Critical Error"), MB_OK | MB_ICONERROR);
	PostQuitMessage(0);
}

BOOL GameEngine::InitJoystick()
{
	/*
	//Make sure joystick driver is present
	JOYINFO jiInfo;
	if (joyGetPos(JOYSTICKID1, &jiInfo) != JOYERR_UNPLUGGED)
		m_uiJoystickID = JOYSTICKID1;
	else
		return FALSE;

	//Calculate the trip values
	JOYCAPS jcCaps;
	joyGetDevCaps(m_uiJoystickID, &jcCaps, sizeof(JOYCAPS));
	DWORD dwXCenter = ((DWORD)jcCaps.wXmin + jcCaps.wXmax) / 2;
	DWORD dwYCenter = ((DWORD)jcCaps.wYmin + jcCaps.wYmax) / 2;
	m_rcJoystickTrip.left = (jcCaps.wXmin + (WORD)dwXCenter) / 2;
	m_rcJoystickTrip.right = (jcCaps.wXmax + (WORD)dwXCenter) / 2;
	m_rcJoystickTrip.top = (jcCaps.wYmin + (WORD)dwYCenter) / 2;
	m_rcJoystickTrip.bottom = (jcCaps.wYmax + (WORD)dwYCenter) / 2;
	*/

	return TRUE;
}

void GameEngine::AddSprite(Sprite* pSprite)
{
	if(pSprite != NULL)																		//���ڷ� ���� ����� ���Դ����� ���� ����ó�� 
	{
		if(m_vSprites.size() > 0)															//���;ȿ� ���� ����ִ����� ���� ����ó�� 
		{
			vector<Sprite*>::iterator siSprite;												// ��������Ʈ���� ���ͷ����� ��ü����

			for(siSprite = m_vSprites.begin(); siSprite != m_vSprites.end(); siSprite++)		//���� ù��° ���� ������ ���� ��ȸ�� 
				if(pSprite->GetZOrder() < (*siSprite) -> GetZOrder())						//pSprite�� ���̰��� *siSprite�� ���̰����� ���� ���
				{
					m_vSprites.insert(siSprite, pSprite);									//siSprite�� pSprite �߰�
					return;																	//������ �����ν� �ش� for�� ����(AddSprite �Լ� ����)
				}
		}
		m_vSprites.push_back(pSprite);														//������ �������������� �ǵڷ� �ٴ´�.
	}
}

void GameEngine::DelSprite(Sprite* pSprite)
{
	if(pSprite != NULL)
	{
		if(m_vSprites.size() > 0)
		{
			vector<Sprite*>::iterator siSprite;													//�ݺ��� ����
			for(siSprite = m_vSprites.begin(); siSprite != m_vSprites.end(); siSprite++)		//�ݺ���
			{
				delete(*siSprite);
				m_vSprites.erase(siSprite);
				return;
			}
		}
		m_vSprites.push_back(pSprite);
	}
}


void GameEngine::DrawSprites(HDC hDC)
{
	vector<Sprite*>::iterator siSprite;
	for (siSprite = m_vSprites.begin(); siSprite != m_vSprites.end(); siSprite++)
		(*siSprite)->Draw(hDC);
}

void GameEngine::UpdateSprites()
{
	if (m_vSprites.size() >= (m_vSprites.capacity() / 2))	
		m_vSprites.reserve(m_vSprites.capacity() * 2);	

	RECT			rcOldSpritePos;	
	SPRITEACTION	saSpriteAction;
	vector<Sprite*>::iterator siSprite;
	for (siSprite = m_vSprites.begin(); siSprite != m_vSprites.end(); siSprite++)	//������ ��������Ʈ ��ǥ����, ������ ������Ű�� ���� ���
	{
		rcOldSpritePos = (*siSprite)->GetPosition();								//*siSprite ��ġ�� �޾ƿ� rcOldSpritePos�� ����
		saSpriteAction = (*siSprite)->Update();										//*siSprite ������Ʈ�Լ��� ���� saSpriteAction�� ����

		if (saSpriteAction&SA_KILL)													//saSpriteAction�� 8��Ʈ������ �ؼ� ���� ���� 1(SA_KILL)�� ��
		{																			//SA_KILL�� 0x0001L�̴�. = 1�� Long�� ������ ���
			delete(*siSprite);														//*siSprite�� ���� ����
			m_vSprites.erase(siSprite);
			siSprite--;
			continue;
		}

		if (CheckSpriteCollision(*siSprite))
			(*siSprite)->SetPosition(rcOldSpritePos);
	}
}

//��������Ʈ�� ���� �����ϴ� �Լ�
void GameEngine::CleanupSprites()
{
	vector<Sprite*>::iterator siSprite;//�ݺ��� ����
	for (siSprite = m_vSprites.begin(); siSprite != m_vSprites.end(); siSprite++)
	{		
		delete(*siSprite);//*siSprite ���� ����
		m_vSprites.erase(siSprite);//���� ��������Ʈ���� siSprite ����
		siSprite--;//siSprite�� ���ҽ�Ű�鼭 ������ ������������ ��� ����	
	}
}

//������������ � ��������Ʈ�� �ִ��� �˷��ִ� �Լ�
Sprite* GameEngine::IsPointInSprite(int x, int y)
{
	vector<Sprite*>::reverse_iterator siSprite;												//�� �ݺ��� ����
	for (siSprite = m_vSprites.rbegin(); siSprite != m_vSprites.rend();siSprite++)			//�ڿ������� �տ�
	if (!(*siSprite)->IsHidden() && (*siSprite)->IsPointInside(x,y))				//*siSprite�� �������� �����鼭 ����Ʈ�ȿ� ���� ���
		return (*siSprite);															//*siSprite�� �����Ѵ�.

	return NULL;																		//if���� �ش����� ���� ��� NULL�� �����Ѵ�.
}