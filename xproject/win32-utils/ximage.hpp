#ifndef _gdiplus_ximage_hpp
#define _gdiplus_ximage_hpp

#include <afxwin.h>

#if _MSC_VER > 1600

#include <GdiPlus.h>
#pragma comment(lib,"gdiplus.lib")

#else

#include <comdef.h> //初始化一下com口 

#ifndef ULONG_PTR 
#define ULONG_PTR unsigned long*
#endif

#include "GdiPlus.h"
#pragma comment(lib,"GdiPlus.lib")

#endif

using namespace Gdiplus;

class xImage
{
public:
	static xImage* get_instance()
	{
		if (!_x_image)
		{
			_x_image = new xImage;
		}
		return _x_image;
	}
	void draw_image(CWnd* hWnd, wchar_t* file, bool adjust_size=false);
private:
	xImage(){
		GdiplusStartupInput m_gdiplusStartupInput; 
		GdiplusStartup(&m_pGdiToken,&m_gdiplusStartupInput,NULL);
	};
	~xImage(){ GdiplusShutdown(m_pGdiToken); }

	void set_window_size(CWnd* pwnd,int width,int height);
	

	ULONG_PTR m_pGdiToken; 

	static xImage* _x_image;
};

/*
 * 	ximage 浣跨敤鑼冧緥锛?
 *	xImage::get_instance()->draw_image(GetDlgItem(IDC_STATIC_P), L"pass.jpg");
 *
 */


#endif