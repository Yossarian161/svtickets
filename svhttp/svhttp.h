#ifndef _svhttp_client_h
#define _svhttp_client_h

#include <string>
#include <vector>

#include "curl/curl.h"
#include "svlogger.h"

namespace svhttp
{
	class http_options
	{
	public:
		http_options()
		{ _options.clear(); }

		~http_options()
		{ _options.clear(); }

		void insert(std::string &str_option)
		{
			_options.push_back(str_option);
		}
		//bool insert(std::vector<std::string>& list_option);

		std::vector<std::string> option_all() { return _options; };

		// 得到Header字符串.
		std::string header_string() const
		{
			std::string str;
			for (std::vector<std::string>::const_iterator f = _options.begin(); f != _options.end(); f++)
			{
				str += (*f + "\r\n");
			}
			return str;
		}

	private:
		std::vector<std::string> _options;
	};

	typedef http_options request_header;
	typedef http_options reponse_header;

	class http_proxy
	{
	public:
		void set_proxy_type(curl_proxytype &type){ _type = type; };
		curl_proxytype get_proxy_type(){ return _type; }

		void set_hostname(const std::string& hostname){ _hostname = hostname; }
		void set_port(const std::string& port) { _port = port; }
		void set_username(const std::string &username) { _username = username; }
		void set_password(const std::string &password) { _password = password; }

		std::string get_hostname() { return _hostname; }
		std::string get_port() { return _port; }
		std::string get_username() { return _username; }
		std::string get_password() { return _password; }

	private:
		curl_proxytype _type;

		std::string _hostname;
		std::string _port;

		std::string _username;
		std::string _password;
	};

	class http_client
	{
	public:
		http_client();
		~http_client();

		template <class T>
		bool set_option(CURLoption option, T data)
		{
			return CURLE_OK == curl_easy_setopt(_curl, option, data);
		}

		bool set_headers(request_header& request_opt);
		void set_cookies(const std::string& cookies_file);
		void set_ssl_verification(const std::string& ssl_ca_file);
		
		bool set_proxy(http_proxy &proxy);
		bool set_proxy(std::string &single_proxy);

		bool set_post_fields(const std::string& post_str);

		bool open(const std::string& url);

		std::string read_some() 
		{ 
			return std::string(_reponse_stream.begin(), _reponse_stream.end()); 
		};
		void read_to_file(const std::string &file_path)
		{
			svlogger::save_file(file_path, std::string(_reponse_stream.begin(), _reponse_stream.end()));

		}
		std::string read_reponse_header() 
		{ 
			return std::string(_reponse_header_stream.begin(), _reponse_header_stream.end()); 
		};
	
		void set_timeout_connect(int val)
		{
			_timeout_connect = val;
		};
		void set_timeout_read(int val)
		{
			_timeout_read = val;
		};
		int get_timeout_connect()
		{
			return _timeout_connect;
		};
		int get_timeout_read()
		{
			return _timeout_read;
		};

		std::string get_error_buffer()
		{
			return ((!_error_buffer)?"":_error_buffer);
		}

		std::string get_version();
		std::string str_escape(const std::string& str_in)
		{
			return curl_easy_escape(_curl, str_in.c_str(), str_in.length());
		}
		std::string str_unescape(const std::string& str_in)
		{
			int len;
			char *lp_str_out = curl_easy_unescape(_curl, str_in.c_str(), str_in.length(), &len);
			return std::string(lp_str_out, len);
		}

		// 开启运行日志
		bool set_verbose(bool val = true){ return set_option(CURLOPT_VERBOSE, val?1L:0L); }

		void enable_accept_encoding(bool val){ _accept_encoding = val; }
	private:
		CURL *_curl;
		curl_slist *_headers;
		
		std::vector<char> _reponse_stream;
		std::vector<char> _reponse_header_stream;

		std::string _cookie_file;
		std::string _ssl_ca_file;

		bool init();
		static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream);
		static size_t write_header_data(void *ptr, size_t size, size_t nmemb, void *stream);
		
		char _error_buffer[CURL_ERROR_SIZE];
		int _timeout_connect;
		int _timeout_read;

		bool _accept_encoding;
	};

	
}	//namespace svhttp


#endif