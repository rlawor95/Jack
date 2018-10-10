#pragma once

#include <windows.h>

class Bitmap
{
protected:
  HBITMAP m_hBitmap;
  int     m_iWidth, m_iHeight;	

  void Free();

public:
  Bitmap();//持失切
  Bitmap(HDC hDC, LPTSTR szFileName);//持失切
  Bitmap(HDC hDC, UINT uiResID, HINSTANCE hInstance);//持失切
  Bitmap(HDC hDC, int iWidth, int iHeight, COLORREF crColor = RGB(0, 0, 0));//持失切
  virtual ~Bitmap();//社瑚切

  BOOL Create(HDC hDC, LPTSTR szFileName);							
  BOOL Create(HDC hDC, UINT uiResID, HINSTANCE hInstance);
  BOOL Create(HDC hDC, int iWidth, int iHeight, COLORREF crColor);
  void Draw(HDC hDC, int x, int y, BOOL bTrans = FALSE,
    COLORREF crTransColor = RGB(255, 0, 255));
  int  GetWidth() { return m_iWidth; };
  int  GetHeight() { return m_iHeight; };
};
