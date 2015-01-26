1.在你将要使用GDI+的工程中，完成初始化工作： 

将以下代码加入StdAfx.h头文件中： 

//加入的头文件 
#include <comdef.h> //初始化一下com口 

#ifndef ULONG_PTR 
#define ULONG_PTR unsigned long* 
#include "GdiPlus.h " 
using namespace Gdiplus; 
#endif 
//结束 

2.在程序初始化部分加入 

//gdi+用到的两个变量 
GdiplusStartupInput m_gdiplusStartupInput; 
ULONG_PTR m_pGdiToken; 
//装载gdi+ 
GdiplusStartup(&m_pGdiToken,&m_gdiplusStartupInput,NULL); 

3.在程序退出部分前加入 

//卸载gdi+ 
GdiplusShutdown(m_pGdiToken); 

4.在Project-> stting-> Link-> Object/libary中加入gdiplus.lib 
这样你就可以在VC6.0中使用GDI+了