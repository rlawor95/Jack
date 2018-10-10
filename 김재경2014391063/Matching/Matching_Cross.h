#pragma once

#include "GameEngine.h"
#include <windows.h>
#include <stdio.h>
#include "resource.h"
#include "Bitmap.h"
//#include "Sprite.h"

#define USE_DOUBLE_BUFFERING

HINSTANCE   g_hInstance;			
GameEngine* g_pGame;				
HDC         g_hOffscreenDC;		
HBITMAP     g_hOffscreenBitmap;		
Bitmap*     g_pTiles[17];			
Bitmap*		g_pOTiles[8];			
Bitmap*		g_pBG[1];				
Bitmap*		g_pNPCBitmap[4];		

Sprite* g_pNPCSprite1;				
Sprite* g_pNPCSprite2;				
Sprite* g_pNPCSprite3;
Sprite* g_pNPCSprite4;
Sprite* g_pNPCSprite5;
	

int         g_iTiles[5][6];			
int			g_iOrder[2];			
int			g_tmpTile;				
int         g_iScore;				
int			g_iHighScore;			
int			g_x,g_y;				
int			delay;					
int			time=0;
int			g_match;
int			s_x;
int			s_y;
int			dd;

long 		tick_cnt;				
long		tick_start;				
long		tick_now;				

long		sec;					
POINT       g_ptTile1, g_ptTile2, g_ptTile_tmp;
BOOL        g_bTileStates[5][6];	
BOOL		g_bGameOver;			
BOOL		g_RectState=FALSE;		
bool		g_newStart=true;		
//int i=0;						

int g_ItemScore=0;					
