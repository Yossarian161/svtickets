1.���㽫Ҫʹ��GDI+�Ĺ����У���ɳ�ʼ�������� 

�����´������StdAfx.hͷ�ļ��У� 

//�����ͷ�ļ� 
#include <comdef.h> //��ʼ��һ��com�� 

#ifndef ULONG_PTR 
#define ULONG_PTR unsigned long* 
#include "GdiPlus.h " 
using namespace Gdiplus; 
#endif 
//���� 

2.�ڳ����ʼ�����ּ��� 

//gdi+�õ����������� 
GdiplusStartupInput m_gdiplusStartupInput; 
ULONG_PTR m_pGdiToken; 
//װ��gdi+ 
GdiplusStartup(&m_pGdiToken,&m_gdiplusStartupInput,NULL); 

3.�ڳ����˳�����ǰ���� 

//ж��gdi+ 
GdiplusShutdown(m_pGdiToken); 

4.��Project-> stting-> Link-> Object/libary�м���gdiplus.lib 
������Ϳ�����VC6.0��ʹ��GDI+��