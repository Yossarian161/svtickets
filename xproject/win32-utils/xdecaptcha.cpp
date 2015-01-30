#include "xdecaptcha.h"

/**
 *	xdecaptcha 类实现
 */
xdecaptcha* xdecaptcha::x_decaptcha = NULL;

xdecaptcha* xdecaptcha::get_instance()
{
	if (!x_decaptcha)
	{
		x_decaptcha = new xdecaptcha;
	}
	return x_decaptcha;
}

xdecaptcha::xdecaptcha()
	:m_cds_index(0)
{
	load_andivcode_dll();

	m_cds_index = load_cds_from_file(std::string("decaptcha.cds"));
}

xdecaptcha::~xdecaptcha()
{
	if (m_hInst)
	{
		FreeLibrary(m_hInst);
	}
}
// 加载dll文件
void xdecaptcha::load_andivcode_dll(std::string& dll_path)
{
	m_hInst = LoadLibraryA(dll_path.c_str()); // 加载 dll
	if (!m_hInst)
	{
		_error_buf = "load library <decaptcha.dll> failed.";
	}
}

// 载入识别库
/**
 *	int LoadCdsFromBuffer(//成功返回当前识别库文件索引，失败返回-1
 *  char[] FileBuffer, //识别库文件二进制数据
 *  int FileBufLen);	//识别库文件数据尺寸
 */
int xdecaptcha::load_cds_from_buffer(std::string& str_buffer)
{
	typedef int (CALLBACK* lpLoadCds)(const char[], int);
	lpLoadCds load_from_buffer = (lpLoadCds)GetProcAddress(m_hInst, "LoadCdsFromBuffer");

	int index = load_from_buffer(str_buffer.c_str(), str_buffer.length());

	if (index == -1)//返回-1说明载入识别库出错
	{
		_error_buf = "load cds file error.";
	}

	return index;
}
/**
 *  int LoadCdsFromFile(//成功返回当前识别库文件索引，失败返回-1
 *  char[] FilePath);//识别库文件所在路径
 */
int xdecaptcha::load_cds_from_file(std::string& file_path)
{
	typedef int (CALLBACK* lpLoadCds)(const char[]);
	lpLoadCds load_from_file = (lpLoadCds)GetProcAddress(m_hInst, "LoadCdsFromFile");

	int index = load_from_file(file_path.c_str());

	if (index == -1)//返回-1说明载入识别库出错
	{
		_error_buf = "load cds file error.";
	}

	return index;
}

// 识别验证码

/**
 *  bool GetVcodeFromFile( //能识别返回真，否则返回假
 *  int CdsFileIndex ,//识别库文件索引
 *  char[] FilePath , //验证码文件所在路径
 *  char[] Vcode);   //返回的已识别验证码文本
 */
std::string xdecaptcha::get_vcode_from_file( std::string& file_path, int cds_idx, int code_len )
{
	std::string _result_str;
	typedef bool (CALLBACK* lpGetVCode)(int,const char[], char[]);
	lpGetVCode getvcode_from_file = (lpGetVCode)GetProcAddress(m_hInst, "GetVcodeFromFile");

	//char result[5];	// 验证码长度+1
	char* result = (char*)malloc(code_len+1);
	memset(result, 0, code_len+1);
	bool bret = getvcode_from_file((cds_idx==0)?m_cds_index:cds_idx, file_path.c_str(), result);

	if (!bret)//返回-1说明载入识别库出错
	{
		_error_buf = "获取验证码失败！";
		_result_str = "";
	}
	else
	{
		_result_str = result;
	}
	free(result);
	
	return _result_str;
}

/**
 *  bool GetVcodeFromBuffer(  //能识别返回真，否则返回假
 *  int CdsFileIndex ,//识别库文件索引
 *  char[] ImgBuffer , //验证码图像二进制数据
 *  int ImgBufLen ,//验证码图像尺寸
 *  char[] Vcode);//返回的已识别验证码文本
 */
std::string xdecaptcha::get_vcode_from_buffer( std::string& str_buffer, int cds_idx /*= 0*/, int code_len /*= 4*/ )
{
	std::string _result_str;
	typedef bool (CALLBACK* lpGetVCode)(int,const char[], int, char[]);
	lpGetVCode getvcode_from_buffer = (lpGetVCode)GetProcAddress(m_hInst, "GetVcodeFromBuffer");

	//char result[5];	// 验证码长度+1
	char* result = (char*)malloc(code_len+1);
	memset(result, 0, code_len+1);
	bool bret = getvcode_from_buffer((cds_idx==0)?m_cds_index:cds_idx, str_buffer.c_str(), str_buffer.length(), result);

	if (!bret)//返回-1说明载入识别库出错
	{
		_error_buf = "获取验证码失败！";
		_result_str = "";
	}
	else
	{
		_result_str = result;
	}
	free(result);

	return _result_str;
}

/**
 *  bool GetVcodeFromFile( //能识别返回真，否则返回假
 *  int CdsFileIndex ,//识别库文件索引
 *  char[] ImgURL ,  //验证码下载地址
 *  char[] Vcode);   //返回的已识别验证码文本
 */
std::string xdecaptcha::get_vcode_from_url( std::string& url_path, int cds_idx /*= 0*/, int code_len /*= 4*/ )
{
	std::string result_str;
	typedef bool (CALLBACK* lpGetVCode)(int, const char[], char[]);
	lpGetVCode getvcode_from_url = (lpGetVCode)GetProcAddress(m_hInst, "GetVcodeFromURL");

	//char result[5];	// 验证码长度+1
	char* result = (char*)malloc(code_len+1);
	memset(result, 0, code_len+1);
	bool bret = getvcode_from_url((cds_idx==0)?m_cds_index:cds_idx, url_path.c_str(), result);

	if (!bret)//返回-1说明载入识别库出错
	{
		_error_buf = "获取验证码失败！";
		result_str = "";
	}
	else
	{
		result_str = result;
	}
	free(result);

	return result_str;
}


