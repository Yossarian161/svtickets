#ifndef _http_options_hpp_svhttp_
#define _http_options_hpp_svhttp_

#include <string>
#include <vector>


namespace svhttp
{
	/**
	 *	http header options.
	 */ 
	class http_options
	{
	public:
		http_options()
		{ 
			_options.clear(); 
		}
		~http_options()
		{ 
			_options.clear(); 
		}

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
		void set_proxy_type(curl_proxytype &type){ proxy_type = type; };
		curl_proxytype get_proxy_type(){ return proxy_type; }

		void set_hostname(const std::string& hostname){ _hostname = hostname; }
		void set_port(const std::string& port) { _port = port; }
		void set_username(const std::string &username) { _username = username; }
		void set_password(const std::string &password) { _password = password; }

		std::string get_hostname() { return _hostname; }
		std::string get_port() { return _port; }
		std::string get_username() { return _username; }
		std::string get_password() { return _password; }

	private:
		curl_proxytype proxy_type;

		std::string _hostname;
		std::string _port;

		std::string _username;
		std::string _password;
	};


}	//namespace svhttp


#endif