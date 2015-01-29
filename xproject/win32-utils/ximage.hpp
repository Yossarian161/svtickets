#ifndef _gdiplus_ximage_hpp
#define _gdiplus_ximage_hpp

#include <afxwin.h>
#include <windows.h>
#include <string>

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

/*
 * 	xImage 使用范例
 *	xImage::get_instance()->draw_image(GetDlgItem(IDC_STATIC_P), L"pass.jpg");
 */
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


/** 
 *	xdecaptcha 
 *  验证码自动识别库
 */
class xdecaptcha
{
public:
	static xdecaptcha* get_instance();
	
	/** 加载识别库
	 *	parameter_1: cds file path or contents. 
	 */
	int load_cds_from_file(std::string& file_path);
	int load_cds_from_buffer(std::string& str_buffer);

	/** 获取验证码
	 *	parameter_1: image (file/url) address or content.
	 *	cds_idx: cds文件索引 (由load_cds_xx 系列函数获得 ) 
	 *  code_len： 验证码长度（设定默认验证码长度=4）
	 */
	std::string get_vcode_from_file(std::string& file_path, int cds_idx = 0, int code_len = 4);
	std::string get_vcode_from_buffer(std::string& str_buffer, int cds_idx = 0, int code_len = 4);
	std::string get_vcode_from_url(std::string& url_path, int cds_idx = 0, int code_len = 4);

private:
	xdecaptcha();
	~xdecaptcha();

	// 动态加载识别库dll 
	void load_andivcode_dll(std::string& dll_path = std::string("decaptcha.dll"));

	// --------------------------------
	static xdecaptcha* x_decaptcha;
	HINSTANCE m_hInst;
	std::string _error_buf;
	int m_cds_index;
};

#endif