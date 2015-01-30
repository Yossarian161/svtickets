#ifndef _xdecaptcha_hpp_
#define _xdecaptcha_hpp_

#include <windows.h>
#include <string>

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