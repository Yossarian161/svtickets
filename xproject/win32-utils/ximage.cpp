#include "ximage.hpp"

xImage* xImage::_x_image = NULL;

//根据图片的宽度和高度更新窗口客户区的大小
void xImage::set_window_size(CWnd* pwnd,int width,int height)
{
	HWND hWnd = pwnd->m_hWnd;
	RECT rcWindow,rcClient;
	int border_width,border_height;

	GetWindowRect(hWnd,&rcWindow);
	GetClientRect(hWnd,&rcClient);

	border_width = (rcWindow.right-rcWindow.left) - (rcClient.right-rcClient.left);
	border_height = (rcWindow.bottom-rcWindow.top) - (rcClient.bottom-rcClient.top);

	SetWindowPos(hWnd,0,0,0,border_width+width,border_height+height,SWP_NOMOVE|SWP_NOZORDER);
}

void xImage::draw_image(CWnd* hWnd, wchar_t* file, bool adjust_size)
{
	CDC* hdc;
	int width,height;

	//加载图像
	Image image(file);
	if(image.GetLastStatus() != 0){
		//MessageBox(hWnd,L"图片无效!",NULL,MB_OK);
		return;
	}

	//取得宽度和高度
	width = image.GetWidth();
	height = image.GetHeight();

	//更新窗口大小
	if (adjust_size)
	{
		set_window_size(hWnd,width,height);
	}

	hdc = hWnd->GetDC();

	//绘图
	Graphics graphics(hdc->m_hDC);
	graphics.DrawImage(&image,0,0,width,height);

	hWnd->ReleaseDC(hdc);

	return;
}

