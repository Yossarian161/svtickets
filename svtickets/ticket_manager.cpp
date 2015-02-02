#include <iostream>
#include <sstream>
#include "jsoncpp/include/json.h"
#include "bencode.h"

#include "ticket_manager.h"
#include "dynamic_encrypt.hpp"

using namespace svhttp;

// 登录初始化<获取dynamicJs请求地址,获取动态key>
int ticket_manage::login_init()
{
	static const bool status = false;
	if (!status)
	{
		_client.set_verbose();
		_client.set_option(CURLOPT_FOLLOWLOCATION, true);
		_client.set_cookies("data/cookie.txt");
		_client.enable_accept_encoding(true);
		/*设置代理*/
		//_client.set_proxy(std::string("10.1.100.33:808"));
	}

	/**
	 *	获取dynamicjs 请求url
	 */
	std::string url = "https://kyfw.12306.cn/otn/login/init";
	if(!_client.open(url))
	{
		_error_buf = _client.get_error_buffer();
		return -1;
	}
	
	std::string reponse_str = _client.read_some();
	convert_str("utf-8", "gbk", reponse_str);

	int x_pos,d_pos;
	
	// 判断是否已登录
	x_pos = reponse_str.find("sessionInit = '");
	d_pos = reponse_str.find("'", x_pos+15);
	std::string sub_str = reponse_str.substr(x_pos+15, d_pos-x_pos-15);
	if (!sub_str.empty())	// cookies已经登录
	{
		SVLOGGER_INFO << sub_str;
		return 1;
	}

	// 走正常登录流程
	x_pos = reponse_str.find("dynamicJs/");
	if (x_pos != -1)
	{
		d_pos = reponse_str.find("\"", x_pos+10);
		std::string dynamic_url = "https://kyfw.12306.cn/otn/dynamicJs/" + reponse_str.substr(x_pos+10, d_pos-x_pos-10);
		SVLOGGER_INFO << dynamic_url;
		
		// 请求dynamicJs地址，获取dynamic key
		if(!_client.open(dynamic_url))
		{
			_error_buf = _client.get_error_buffer();
			return -1;
		}

		//SVLOGGER_INFO << _client.read_some();
		reponse_str = _client.read_some();
		convert_str("utf-8", "gbk", reponse_str);

		x_pos = reponse_str.find("key='");
		d_pos = reponse_str.find("'", x_pos+5);

		dynamic_key = reponse_str.substr(x_pos+5, d_pos-x_pos-5);
		SVLOGGER_DBG << dynamic_key;

		dynamic_value = encode32(bin216(base32_encrypt("1111", dynamic_key)));
		SVLOGGER_DBG << dynamic_value;

		login_passcode_reflush();

		return 0;
	}
	
	//SVLOGGER_ERR << reponse_str;

	login_passcode_reflush();

	return -1;
}

bool ticket_manage::login_passcode_reflush(std::string img_path)
{
	request_header opts;
	opts.insert(std::string("Referer: https://kyfw.12306.cn/otn/login/init#"));
	opts.insert(std::string("User-Agent: Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 5.1; Trident/4.0; .NET4.0C; .NET4.0E)"));
	opts.insert(std::string("Accept: image/png, image/svg+xml, image/*;q=0.8, */*;q=0.5"));
	opts.insert(std::string("Connection: Keep-Alive"));
	_client.set_headers(opts);
	
	// 生成随机数
	std::ostringstream oss_rand;
	oss_rand << rand() << time(0) << rand();
	std::string str_rand = oss_rand.str();
	if (str_rand.length() > 17)
	{
		str_rand = str_rand.substr(0,(rand()%7==1?17:16));
	}
	SVLOGGER_INFO << str_rand;
	std::ostringstream str_url;
	str_url << "https://kyfw.12306.cn/otn/passcodeNew/getPassCodeNew?module=login&rand=sjrand&0."
		<< str_rand;
	
	if(!_client.open(str_url.str()))
	{
		_error_buf = _client.get_error_buffer();
		return false;
	}
	
	_client.read_to_file(img_path);
	
	return true;
}

bool ticket_manage::login_passcode_verify(std::string passcode_str)
{
	std::string post_str;
	post_str = "randCode=" + passcode_str;
	post_str += "&rand=sjrand&randCode_validate=";
	convert_str("gbk", "utf-8", post_str);
	_client.set_post_fields(post_str);
	
	std::string str_url = "https://kyfw.12306.cn/otn/passcodeNew/checkRandCodeAnsyn";

	if(!_client.open(str_url))
	{
		_error_buf = _client.get_error_buffer();
		return false;
	}

	std::string recv_str = _client.read_some();

	Json::Value reader_object;
    Json::Reader reader;

	if (!reader.parse(recv_str, reader_object))
    {
		_error_buf = "返回json数据解析失败";
		SVLOGGER_FILE << recv_str;
		return false;
	}

	if (reader_object["status"].asBool())
	{
		if (reader_object["data"]["result"].asString() == "1")
		{
			SVLOGGER_INFO << "验证成功";
			return true;
		}
		else
		{
			if (reader_object["data"]["msg"].asString() == "randCodeError")
			{
				_error_buf = "验证码错误";
			}
			return false;
		}
	}

	return false;
}

bool ticket_manage::login_verify(const std::string& username, const std::string& passwd, const std::string& passcode)
{
	if (!login_passcode_verify(passcode))
	{
		return false;
	}

	request_header opts;
	opts.insert(std::string("Referer: https://kyfw.12306.cn/otn/login/init"));
	opts.insert(std::string("Content-Type: application/x-www-form-urlencoded; charset=UTF-8"));
	opts.insert(std::string("Accept-Encoding: gzip, deflate"));
	opts.insert(std::string("User-Agent: Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 5.1; Trident/4.0; .NET4.0C; .NET4.0E; .NET CLR 2.0.50727)"));
	opts.insert(std::string("Connection: Keep-Alive"));
	_client.set_headers(opts);

	std::ostringstream post_strem;
	post_strem << "loginUserDTO.user_name=" << _client.str_escape(username)
		<< "&userDTO.password=" << passwd
		<< "&randCode=" << passcode
		<< "&randCode_validate=&" << _client.str_escape(dynamic_key) 
		<< "=" << _client.str_escape(dynamic_value)
		<< "&myversion=undefined";

	std::string post_str = post_strem.str();
	convert_str("gbk", "utf-8", post_str);
	SVLOGGER_INFO << post_str;
	_client.set_post_fields(post_str);
	
	std::ostringstream str_url;
	str_url << "https://kyfw.12306.cn/otn/login/loginAysnSuggest";
	
	if(!_client.open(str_url.str()))
	{
		_error_buf = _client.get_error_buffer();
		return false;
	}

	std::string recv_str = _client.read_some();
	convert_str("utf-8", "gbk", recv_str);
	SVLOGGER_DBG << recv_str;

	Json::Reader reader;
	Json::Value reader_object;

	if (!reader.parse(recv_str, reader_object))
	{
		_error_buf = "返回json数据解析失败";
		SVLOGGER_FILE << recv_str;
		return false;
	}
	
	//SVLOGGER_INFO << reader_object["data"].asString();
	if (!reader_object["status"].asBool() )
	{
		Json::Value msg = reader_object["messages"];

		_error_buf =  msg[(unsigned int)0].asString();
		
		//init_user_info();

		return false;
	}
	else
	{
		if (reader_object["data"]["loginCheck"].asString() == "Y")
		{
			SVLOGGER_DBG << "登录成功";
			
			//update_user_login();
			user_name = username;
			init_user_info();
			
			return true;
		}
		else
		{
			Json::Value msg = reader_object["messages"];
			int idx = 0;
			_error_buf =  msg[idx].asString();
			return false;
		}
	}

	return false;
}

bool ticket_manage::login_out()
{
	std::string url = "https://kyfw.12306.cn/otn/login/loginOut";
	if(!_client.open(url))
	{
		_error_buf = _client.get_error_buffer();
		return false;
	}
	return true;
}

bool ticket_manage::init_user_info()
{
	std::string url = "https://kyfw.12306.cn/otn/modifyUser/initQueryUserInfo";
	if(!_client.open(url))
	{
		_error_buf = _client.get_error_buffer();
		return false;
	}

	std::string reponse_str = _client.read_some();
	convert_str("utf-8", "gbk", reponse_str);
	//SVLOGGER_DBG << reponse_str;
	int x_pos, d_pos;

	x_pos = reponse_str.find("姓名");
	if (x_pos != -1)
	{
		x_pos = reponse_str.find("class=\"con\"", x_pos);
		d_pos = reponse_str.find("</div>", x_pos+12);

		user_name_cn = reponse_str.substr(x_pos+12, d_pos-x_pos-12);
		SVLOGGER_INFO << user_name_cn;

		x_pos = reponse_str.find("证件类型");
		x_pos = reponse_str.find("class=\"con\"", x_pos);
		d_pos = reponse_str.find("</div>", x_pos+12);
		
		user_card_type = reponse_str.substr(x_pos+12, d_pos-x_pos-12);
		SVLOGGER_INFO << user_card_type;


		x_pos = reponse_str.find("证件号码");
		x_pos = reponse_str.find("class=\"con\"", x_pos);
		d_pos = reponse_str.find("</div>", x_pos+12);
		
		user_card_id = reponse_str.substr(x_pos+12, d_pos-x_pos-12);
		SVLOGGER_INFO << user_card_id;


		x_pos = reponse_str.find("核验状态");
		x_pos = reponse_str.find("class=\"con\"", x_pos);
		x_pos = reponse_str.find(">", x_pos+12);
		d_pos = reponse_str.find("</div>", x_pos+1);
		
		user_card_status = reponse_str.substr(x_pos+1, d_pos-x_pos-1);
		SVLOGGER_INFO << user_card_status;
	}

	return true;
}


bool ticket_manage::update_user_login()
{
	_client.set_post_fields("_json_att:");

	std::string url = "https://kyfw.12306.cn/otn/login/userLogin";
	if(!_client.open(url))
	{
		_error_buf = _client.get_error_buffer();
		return false;
	}
	
	std::string reponse_str = _client.read_some();
	convert_str("utf-8", "gbk", reponse_str);

	SVLOGGER_DBG << reponse_str;
	return true;
}

bool ticket_manage::get_station_name()
{
	request_header opts;
	opts.insert(std::string("Referer:https://kyfw.12306.cn/otn/leftTicket/init"));
	opts.insert(std::string("Accept-Encoding:gzip,deflate,sdch"));
	_client.set_headers(opts);

	std::string url = "https://kyfw.12306.cn/otn/resources/js/framework/station_name.js?station_version=1.8257";

	if (!_client.open(url))
	{
		_error_buf = _client.get_error_buffer();
		return false;
	}

	std::string reponse_str = _client.read_some();
	convert_str("utf-8", "gbk", reponse_str);

	//SVLOGGER_DBG << reponse_str;
	// 解析车站名称数据
	std::string stat_substr = "";
	int x_pos = reponse_str.find("@");
	int d_pos = reponse_str.find("@", x_pos+1);
	while (x_pos != -1)
	{
		if (d_pos == -1)
			stat_substr = reponse_str.substr(x_pos+1, reponse_str.length()-x_pos-3);
		else
			stat_substr = reponse_str.substr(x_pos+1, d_pos-x_pos-1);

		station_name stat_name;
		stat_name.parse_string(stat_substr);

		station_name_vec.push_back(stat_name);

		if (d_pos == -1)
		{
			break;
		}
		x_pos = d_pos;
		d_pos = reponse_str.find("@", x_pos+1);
	}

	return true;
}

bool ticket_manage::query_train_data(left_ticket_dto& ticket_dto)
{
#if 0	// 查询前先查询log，此处为了节省时间，暂先屏蔽。
	request_header opts1;
	opts1.insert(std::string("Referer:https://kyfw.12306.cn/otn/leftTicket/init"));
	opts1.insert(std::string("Accept-Encoding:gzip,deflate,sdch"));
	opts1.insert(std::string("Connection: Keep-Alive"));
	_client.set_headers(opts1);

	std::string _url = "https://kyfw.12306.cn/otn/leftTicket/log?"
		+ ticket_dto.get_query_string();

	if (!_client.open(_url))
	{
		_error_buf = _client.get_error_buffer();
		return false;
	}

	std::string _reponse_str = _client.read_some();
	//convert_str("utf-8", "gbk", _reponse_str);
	SVLOGGER_INFO << _reponse_str;
#endif

	std::string url = "https://kyfw.12306.cn/otn/leftTicket/queryT?"
		+ ticket_dto.get_query_string();

	request_header opts;
	opts.insert(std::string("Referer:https://kyfw.12306.cn/otn/leftTicket/init"));
	opts.insert(std::string("Accept-Encoding:gzip,deflate,sdch"));
	opts.insert(std::string("Connection: Keep-Alive"));
	opts.insert(std::string("If-Modified-Since: 0"));
	_client.set_headers(opts);

	if (!_client.open(url))
	{
		_error_buf = _client.get_error_buffer();
		return false;
	}

	std::string reponse_str = _client.read_some();
	convert_str("utf-8", "gbk", reponse_str);
	SVLOGGER_FILE << reponse_str;

	Json::Reader reader;
	Json::Value reader_object;

	if (!reader.parse(reponse_str, reader_object))
	{
		_error_buf = "返回json数据解析失败";
		SVLOGGER_DBG << reponse_str;
		return false;
	}

	if (!reader_object["status"].asBool() == true)
	{
		Json::Value msg = reader_object["messages"];
		_error_buf =  msg[(unsigned int)0].asString();
		return false;
	}
	else
	{
		train_data_vec.clear();

		Json::Value train_data_object = reader_object["data"];
		for (unsigned int idx = 0; idx < train_data_object.size(); ++idx)
		{
			train_data t_data;
			Json::Value train_record = train_data_object[idx];
			Json::Value left_new_dto = train_record["queryLeftNewDTO"];

			// parse data/queryLeftNewDTO
			t_data.train_no = left_new_dto["train_no"].asString();
			t_data.station_train_code = left_new_dto["station_train_code"].asString();
			t_data.start_station_telecode = left_new_dto["start_station_telecode"].asString();
			t_data.start_station_name = left_new_dto["start_station_name"].asString();
			t_data.end_station_telecode = left_new_dto["end_station_telecode"].asString();
			t_data.end_station_name = left_new_dto["end_station_name"].asString();
			t_data.from_station_telecode = left_new_dto["from_station_telecode"].asString();
			t_data.from_station_name = left_new_dto["from_station_name"].asString();
			t_data.to_station_telecode = left_new_dto["to_station_telecode"].asString();
			t_data.to_station_name = left_new_dto["to_station_name"].asString();
			t_data.start_time = left_new_dto["start_time"].asString();
			t_data.arrive_time = left_new_dto["arrive_time"].asString();
			t_data.day_difference = left_new_dto["day_difference"].asString();
			t_data.train_class_name = left_new_dto["train_class_name"].asString();
			t_data.lishi = left_new_dto["lishi"].asString();
			t_data.canWebBuy = left_new_dto["canWebBuy"].asString();
			t_data.lishiValue = left_new_dto["lishiValue"].asString();
			t_data.yp_info = left_new_dto["yp_info"].asString();
			t_data.control_train_day = left_new_dto["control_train_day"].asString();
			t_data.start_train_date = left_new_dto["start_train_date"].asString();
			t_data.seat_feature = left_new_dto["seat_feature"].asString();
			t_data.yp_ex = left_new_dto["yp_ex"].asString();
			t_data.train_seat_feature = left_new_dto["train_seat_feature"].asString();
			t_data.seat_types = left_new_dto["seat_types"].asString();
			t_data.location_code = left_new_dto["location_code"].asString();
			t_data.from_station_no = left_new_dto["from_station_no"].asString();
			t_data.to_station_no = left_new_dto["to_station_no"].asString();
			// control_day [Int]
			t_data.control_day = left_new_dto["control_day"].asInt();
			t_data.sale_time = left_new_dto["sale_time"].asString();
			t_data.is_support_card = left_new_dto["is_support_card"].asString();
			t_data.yz_num = left_new_dto["yz_num"].asString();
			t_data.rz_num = left_new_dto["rz_num"].asString();
			t_data.yw_num = left_new_dto["yw_num"].asString();
			t_data.rw_num = left_new_dto["rw_num"].asString();
			t_data.gr_num = left_new_dto["gr_num"].asString();
			t_data.zy_num = left_new_dto["zy_num"].asString();
			t_data.ze_num = left_new_dto["ze_num"].asString();
			t_data.tz_num = left_new_dto["tz_num"].asString();
			t_data.gg_num = left_new_dto["gg_num"].asString();
			t_data.yb_num = left_new_dto["yb_num"].asString();
			t_data.wz_num = left_new_dto["wz_num"].asString();
			t_data.qt_num = left_new_dto["qt_num"].asString();
			t_data.swz_num = left_new_dto["swz_num"].asString();

			// parse data/..
			t_data.secretStr = train_record["secretStr"].asString();
			t_data.buttonTextInfo = train_record["buttonTextInfo"].asString();

			// 
			t_data.train_date = ticket_dto.get_train_date();
			t_data.purpose_codes = ticket_dto.get_purpose_codes();

			SVLOGGER_DBG << t_data.train_data_print();

			train_data_vec.push_back(t_data);
		}
	}

	return true;
}

bool ticket_manage::query_train_data_surplus( left_ticket_dto& ticket_dto )
{
	std::string url = "https://kyfw.12306.cn/otn/lcxxcx/query?"
		+ ticket_dto.get_query_surplus_string();

	request_header opts;
	opts.insert(std::string("Referer:https://kyfw.12306.cn/otn/lcxxcx/init"));
	opts.insert(std::string("Accept-Encoding:gzip,deflate,sdch"));
	_client.set_headers(opts);

	if (!_client.open(url))
	{
		_error_buf = _client.get_error_buffer();
		return false;
	}

	std::string reponse_str = _client.read_some();
	convert_str("utf-8", "gbk", reponse_str);
	//SVLOGGER_DBG << reponse_str;

	Json::Reader reader;
	Json::Value reader_object;

	if (!reader.parse(reponse_str, reader_object))
	{
		_error_buf = "返回json数据解析失败";
		SVLOGGER_DBG << reponse_str;
		return false;
	}

	if (!reader_object.isMember("status"))
	{
		_error_buf = "返回数据错误";
		SVLOGGER_DBG << reponse_str;
		return false; 
	}

	if (!reader_object["status"].asBool() == true)
	{
		Json::Value msg = reader_object["messages"];
		_error_buf =  msg[(unsigned int)0].asString();
		return false;
	}
	else
	{
		train_data_surplus_vec.clear();

		Json::Value train_data_object = reader_object["data"]["datas"];
		for (unsigned int idx = 0; idx < train_data_object.size(); ++idx)
		{
			train_data t_data;
			Json::Value left_new_dto = train_data_object[idx];

			// parse data/data
			t_data.train_no = left_new_dto["train_no"].asString();
			t_data.station_train_code = left_new_dto["station_train_code"].asString();
			t_data.start_station_telecode = left_new_dto["start_station_telecode"].asString();
			t_data.start_station_name = left_new_dto["start_station_name"].asString();
			t_data.end_station_telecode = left_new_dto["end_station_telecode"].asString();
			t_data.end_station_name = left_new_dto["end_station_name"].asString();
			t_data.from_station_telecode = left_new_dto["from_station_telecode"].asString();
			t_data.from_station_name = left_new_dto["from_station_name"].asString();
			t_data.to_station_telecode = left_new_dto["to_station_telecode"].asString();
			t_data.to_station_name = left_new_dto["to_station_name"].asString();
			t_data.start_time = left_new_dto["start_time"].asString();
			t_data.arrive_time = left_new_dto["arrive_time"].asString();
			t_data.day_difference = left_new_dto["day_difference"].asString();
			t_data.train_class_name = left_new_dto["train_class_name"].asString();
			t_data.lishi = left_new_dto["lishi"].asString();
			t_data.canWebBuy = left_new_dto["canWebBuy"].asString();
			t_data.lishiValue = left_new_dto["lishiValue"].asString();
			t_data.yp_info = left_new_dto["yp_info"].asString();
			t_data.control_train_day = left_new_dto["control_train_day"].asString();
			t_data.start_train_date = left_new_dto["start_train_date"].asString();
			t_data.seat_feature = left_new_dto["seat_feature"].asString();
			t_data.yp_ex = left_new_dto["yp_ex"].asString();
			t_data.train_seat_feature = left_new_dto["train_seat_feature"].asString();
			t_data.seat_types = left_new_dto["seat_types"].asString();
			t_data.location_code = left_new_dto["location_code"].asString();
			t_data.from_station_no = left_new_dto["from_station_no"].asString();
			t_data.to_station_no = left_new_dto["to_station_no"].asString();
			// control_day [Int]
			t_data.control_day = left_new_dto["control_day"].asInt();
			t_data.sale_time = left_new_dto["sale_time"].asString();
			t_data.is_support_card = left_new_dto["is_support_card"].asString();
			t_data.yz_num = left_new_dto["yz_num"].asString();
			t_data.rz_num = left_new_dto["rz_num"].asString();
			t_data.yw_num = left_new_dto["yw_num"].asString();
			t_data.rw_num = left_new_dto["rw_num"].asString();
			t_data.gr_num = left_new_dto["gr_num"].asString();
			t_data.zy_num = left_new_dto["zy_num"].asString();
			t_data.ze_num = left_new_dto["ze_num"].asString();
			t_data.tz_num = left_new_dto["tz_num"].asString();
			t_data.gg_num = left_new_dto["gg_num"].asString();
			t_data.yb_num = left_new_dto["yb_num"].asString();
			t_data.wz_num = left_new_dto["wz_num"].asString();
			t_data.qt_num = left_new_dto["qt_num"].asString();
			t_data.swz_num = left_new_dto["swz_num"].asString();

			t_data.secretStr = "";
			t_data.buttonTextInfo = (left_new_dto["note"].asString().empty())?"预订":left_new_dto["note"].asString();

			// 
			t_data.train_date = ticket_dto.get_train_date();
			t_data.purpose_codes = ticket_dto.get_purpose_codes();

			SVLOGGER_DBG << t_data.train_data_print();

			train_data_surplus_vec.push_back(t_data);
		}
	}
	return true;
}

bool ticket_manage::query_stop_station(unsigned int index)
{
	train_data _train = (train_data_vec.size()<index)?train_data_surplus_vec[index]:train_data_vec[index];

	std::ostringstream osstr;
	osstr << "https://kyfw.12306.cn/otn/czxx/queryByTrainNo?train_no=" << _train.train_no
		<< "&from_station_telecode=" << _train.from_station_telecode
		<< "&to_station_telecode= " << _train.to_station_telecode
		<< "&depart_date=" << _train.train_date;

	request_header opts;
	opts.insert(std::string("Referer:https://kyfw.12306.cn/otn/leftTicket/init"));
	opts.insert(std::string("Accept-Encoding:gzip,deflate,sdch"));
	_client.set_headers(opts);
	
	if (!_client.open(osstr.str()))
	{
		_error_buf = _client.get_error_buffer();
		return false;
	}
	
	std::string reponse_str = _client.read_some();
	convert_str("utf-8", "gbk", reponse_str);
	
	Json::Reader reader;
	Json::Value reader_object;
	
	if (!reader.parse(reponse_str, reader_object))
	{
		_error_buf = "返回json数据解析失败";
		SVLOGGER_DBG << reponse_str;
		return false;
	}
	
	if (!reader_object["status"].asBool() == true)
	{
		Json::Value msg = reader_object["messages"];
		_error_buf =  msg[(unsigned int)0].asString();
		return false;
	}
	else
	{
		m_stop_station.clear_stop_station_list();

		Json::Value train_list = reader_object["data"]["data"];
		for (unsigned int idx = 0; idx < train_list.size(); ++idx)
		{
			Json::Value t_info = train_list[idx];
			if (idx == 0)
			{
				m_stop_station.set_start_station_name(t_info["start_station_name"].asString());
				m_stop_station.set_station_train_code(t_info["station_train_code"].asString());
				m_stop_station.set_train_class_name(t_info["train_class_name"].asString());
				m_stop_station.set_service_type(t_info["service_type"].asString());
				m_stop_station.set_end_station_name(t_info["end_station_name"].asString());
			}

			station_info stat_info;
			stat_info.arrive_time = t_info["arrive_time"].asString();
			stat_info.stat_name = t_info["station_name"].asString();
			stat_info.start_time = t_info["start_time"].asString();
			stat_info.stopover_time = t_info["stopover_time"].asString();
			stat_info.station_no = t_info["station_no"].asString();
			stat_info.isEnabled = t_info["arrive_time"].asBool();

			m_stop_station.push_stop_station_info(stat_info);
		}
	}
	return true;
}

bool ticket_manage::query_passengers()
{
	/**
	 *	发包获取联系人总页数
	 */
	std::string url = "https://kyfw.12306.cn/otn/passengers/init";

	_client.set_post_fields(std::string("_json_att="));
	
	if (!_client.open(url))
	{
		_error_buf = _client.get_error_buffer();
		return false;
	}
	
	std::string rep_str = _client.read_some();
	convert_str("utf-8", "gbk", rep_str);
	//SVLOGGER_DBG << rep_str;
	
	int x_pos = rep_str.find("totlePage");
	int d_pos = rep_str.find(";", x_pos);
	rep_str = rep_str.substr(x_pos+12, d_pos-x_pos-12);
	SVLOGGER_DBG << "总页码:" << rep_str << "页";

	/**
	 *	根据获取到的总页数，一次性查询所有联系人信息
	 */
	url = "https://kyfw.12306.cn/otn/passengers/query";

	request_header opts;
	opts.insert(std::string("Referer: https://kyfw.12306.cn/otn/passengers/init"));
	opts.insert(std::string("Accept-Encoding: gzip, deflate"));
	opts.insert(std::string("Connection: Keep-Alive"));
	opts.insert(std::string("Content-Type: application/x-www-form-urlencoded; charset=UTF-8"));
	_client.set_headers(opts);

	std::ostringstream osstr;
	osstr << "pageIndex=1&pageSize=" << atoi(rep_str.c_str())*10;
	std::string post_str = osstr.str();
	// 必须转换成 utf-8 格式,否则查询返回失败
	convert_str("gbk", "utf-8", post_str);
	_client.set_post_fields(post_str);

	if (!_client.open(url))
	{
		_error_buf = _client.get_error_buffer();
		return false;
	}

	std::string reponse_str = _client.read_some();
	convert_str("utf-8", "gbk", reponse_str);
	//SVLOGGER_DBG << reponse_str;

	Json::Reader reader;
	Json::Value reader_object;

	if (!reader.parse(reponse_str, reader_object))
	{
		_error_buf = "返回json数据解析失败";
		SVLOGGER_DBG << reponse_str;
		return false;
	}

	if (!reader_object["status"].asBool() == true)
	{
		Json::Value msg = reader_object["messages"];
		_error_buf =  msg[(unsigned int)0].asString();
		return false;
	}
	else
	{
		m_passenger_dto.contacts_datum_clear();

		Json::Value pax_data_obj = reader_object["data"]["datas"];
		if (pax_data_obj.size() == 0)
		{  // 查询失败.
			_error_buf = reader_object["data"]["message"].asString();
			SVLOGGER_ERR << _error_buf;
			return false;
		}

		for (unsigned int idx = 0; idx < pax_data_obj.size(); ++idx)
		{
			contacts_datum contacts;
			Json::Value pax_dto_obj = pax_data_obj[idx];
			
			contacts.code = pax_dto_obj["code"].asString();
			contacts.passenger_name = pax_dto_obj["passenger_name"].asString();
			contacts.sex_code = pax_dto_obj["sex_code"].asString();
			contacts.sex_name = pax_dto_obj["sex_name"].asString();
			contacts.born_date = pax_dto_obj["born_date"].asString();
			contacts.country_code = pax_dto_obj["country_code"].asString();
			contacts.passenger_id_type_code = pax_dto_obj["passenger_id_type_code"].asString();
			contacts.passenger_id_type_name = pax_dto_obj["passenger_id_type_name"].asString();
			contacts.passenger_id_no = pax_dto_obj["passenger_id_no"].asString();
			contacts.passenger_type = pax_dto_obj["passenger_type"].asString();
			contacts.passenger_flag = pax_dto_obj["passenger_flag"].asString();
			contacts.passenger_type_name = pax_dto_obj["passenger_type_name"].asString();
			contacts.mobile_no = pax_dto_obj["mobile_no"].asString();
			contacts.phone_no = pax_dto_obj["phone_no"].asString();
			contacts.email = pax_dto_obj["email"].asString();
			contacts.address = pax_dto_obj["address"].asString();
			contacts.postalcode = pax_dto_obj["postalcode"].asString();
			contacts.first_letter = pax_dto_obj["first_letter"].asString();
			contacts.recordCount = pax_dto_obj["recordCount"].asString();
			contacts.isUserSelf = pax_dto_obj["isUserSelf"].asString();
			contacts.total_times = pax_dto_obj["total_times"].asString();

			SVLOGGER_DBG << contacts.print_test();

			m_passenger_dto.contacts_datum_push(contacts);
		}
	}
	return true;
}

bool ticket_manage::login_check_user()
{
	std::string url = "https://kyfw.12306.cn/otn/login/checkUser";

	request_header opts;
	opts.insert(std::string("Referer: https://kyfw.12306.cn/otn/leftTicket/init"));
	opts.insert(std::string("Accept-Encoding: gzip, deflate"));
	opts.insert(std::string("Connection: Keep-Alive"));
	opts.insert(std::string("Content-Type: application/x-www-form-urlencoded; charset=UTF-8"));
	_client.set_headers(opts);

	std::string post_str = "_json_att=";
	convert_str("gbk", "utf-8", post_str);
	_client.set_post_fields(post_str);

	if (!_client.open(url))
	{
		_error_buf = _client.get_error_buffer();
		return false;
	}

	std::string reponse_str = _client.read_some();
	convert_str("utf-8", "gbk", reponse_str);

	Json::Reader reader;
	Json::Value reader_object;

	if (!reader.parse(reponse_str, reader_object))
	{
		_error_buf = "返回json数据解析失败";
		SVLOGGER_DBG << reponse_str;
		return false;
	}

	if (!reader_object["status"].asBool() == true)
	{
		Json::Value msg = reader_object["messages"];
		_error_buf =  msg[(unsigned int)0].asString();
		return false;
	}

	return true;
}

bool ticket_manage::left_ticket_init()
{
	/**
	 *	获取dynamicjs 请求url
	 */
	std::string url = "https://kyfw.12306.cn/otn/leftTicket/init";
	if(!_client.open(url))
	{
		_error_buf = _client.get_error_buffer();
		return false;
	}
	
	std::string reponse_str = _client.read_some();
	convert_str("utf-8", "gbk", reponse_str);

	int x_pos,d_pos;
	x_pos = reponse_str.find("dynamicJs/");
	if (x_pos != -1)
	{
		d_pos = reponse_str.find("\"", x_pos+10);
		std::string dynamic_url = "https://kyfw.12306.cn/otn/dynamicJs/" + reponse_str.substr(x_pos+10, d_pos-x_pos-10);
		SVLOGGER_INFO << dynamic_url;
		
		// 请求dynamicJs地址，获取dynamic key
		if(!_client.open(dynamic_url))
		{
			_error_buf = _client.get_error_buffer();
			return false;
		}

		reponse_str = _client.read_some();
		convert_str("utf-8", "gbk", reponse_str);

		x_pos = reponse_str.find("key='");
		d_pos = reponse_str.find("'", x_pos+5);

		dynamic_key = reponse_str.substr(x_pos+5, d_pos-x_pos-5);
		SVLOGGER_DBG << dynamic_key;

		dynamic_value = encode32(bin216(base32_encrypt("1111", dynamic_key)));
		SVLOGGER_DBG << dynamic_value;
	}
	return true;
}


bool ticket_manage::submit_order_request( train_data& _train_data )
{
	if (!login_check_user())
	{
		// 用户登录验证失败<用户未登录>
		return false;
	}

	if (!left_ticket_init())
	{
		return false;
	}

	std::string url = "https://kyfw.12306.cn/otn/leftTicket/submitOrderRequest";

	request_header opts;
	opts.insert(std::string("Referer: https://kyfw.12306.cn/otn/leftTicket/init"));
	opts.insert(std::string("Accept-Encoding: gzip, deflate"));
	opts.insert(std::string("Connection: Keep-Alive"));
	opts.insert(std::string("Content-Type: application/x-www-form-urlencoded; charset=UTF-8"));
	_client.set_headers(opts);

	std::ostringstream osstr;
	osstr << _client.str_escape(dynamic_key) << "=" << _client.str_escape(dynamic_value)
		<< "&myversion=undefined&secretStr=" << _train_data.secretStr
		<< "&train_date=" << _train_data.train_date
		<< "&back_train_date=" << svlogger::get_now_time(2)
		<< "&tour_flag=dc&purpose_codes=" << _train_data.purpose_codes
		<< "&query_from_station_name=" << _train_data.from_station_name
		<< "&query_to_station_name=" << _train_data.to_station_name
		<< "&undefined";

	SVLOGGER_DBG << osstr.str();

	std::string post_str = osstr.str();
	convert_str("gbk", "utf-8", post_str);
	_client.set_post_fields(post_str);

	if (!_client.open(url))
	{
		_error_buf = _client.get_error_buffer();
		return false;
	}

	std::string reponse_str = _client.read_some();
	convert_str("utf-8", "gbk", reponse_str);
	SVLOGGER_DBG << reponse_str;

	Json::Reader reader;
	Json::Value reader_object;

	if (!reader.parse(reponse_str, reader_object))
	{
		_error_buf = "返回json数据解析失败";
		SVLOGGER_DBG << reponse_str;
		return false;
	}

	if (!reader_object["status"].asBool() == true)
	{
		Json::Value msg = reader_object["messages"];
		_error_buf =  msg[(unsigned int)0].asString();
		return false;
	}

	/**
	 *	initDc
	 */

	return true;
}


bool ticket_manage::confirm_passenger_initdc()
{
	std::string url = "https://kyfw.12306.cn/otn/confirmPassenger/initDc";

	std::string post_str = "_json_att=";
	convert_str("gbk", "utf-8", post_str);
	_client.set_post_fields(post_str);

	if(!_client.open(url))
	{
		_error_buf = _client.get_error_buffer();
		return false;
	}
	
	std::string reponse_str = _client.read_some();
	convert_str("utf-8", "gbk", reponse_str);

	int x_pos,d_pos;
	
	// 解析 globalRepeatSubmitToken
	x_pos = reponse_str.find("globalRepeatSubmitToken");
	x_pos = reponse_str.find("'", x_pos);
	d_pos = reponse_str.find("';", x_pos+1);
	repeat_submit_token = reponse_str.substr(x_pos+1, d_pos-x_pos-1);
	SVLOGGER_DBG << "globalRepeatSubmitToken=" << repeat_submit_token;

	// 解析 dynamicJs地址,并请求获取dynamicJs内容
	x_pos = reponse_str.find("dynamicJs/");
	if (x_pos != -1)
	{
		d_pos = reponse_str.find("\"", x_pos+10);
		std::string dynamic_url = "https://kyfw.12306.cn/otn/dynamicJs/" + reponse_str.substr(x_pos+10, d_pos-x_pos-10);
		SVLOGGER_INFO << dynamic_url;
		
		// 请求dynamicJs地址，获取dynamic key
		if(!_client.open(dynamic_url))
		{
			_error_buf = _client.get_error_buffer();
			return false;
		}

		reponse_str = _client.read_some();
		convert_str("utf-8", "gbk", reponse_str);

		x_pos = reponse_str.find("key='");
		d_pos = reponse_str.find("'", x_pos+5);

		dynamic_key = reponse_str.substr(x_pos+5, d_pos-x_pos-5);
		SVLOGGER_DBG << dynamic_key;

		dynamic_value = encode32(bin216(base32_encrypt("1111", dynamic_key)));
		SVLOGGER_DBG << dynamic_value;
	}

	return true;
}

// 预确认 订单 验证码刷新
bool ticket_manage::passenger_passcode_reflush(std::string img_path)
{
	std::ostringstream str_url;
	str_url << "https://kyfw.12306.cn/otn/passcodeNew/getPassCodeNew?module=passenger&rand=randp&";
	
	if(!_client.open(str_url.str()))
	{
		_error_buf = _client.get_error_buffer();
		return false;
	}
	
	_client.read_to_file(img_path);
	
	return true;
}

// 预确认 订单 验证码验证
bool ticket_manage::passenger_passcode_verify(std::string passcode_str)
{
	request_header opts;
	opts.insert(std::string("Referer: https://kyfw.12306.cn/otn/confirmPassenger/initDc"));
	_client.set_headers(opts);

	std::ostringstream osstr;
	osstr << "randCode=" << passcode_str << "&rand=randp&_json_att=&REPEAT_SUBMIT_TOKEN=" << repeat_submit_token;

	std::string post_str = osstr.str();
	convert_str("gbk", "utf-8", post_str);
	_client.set_post_fields(post_str);
	
	std::string str_url = "https://kyfw.12306.cn/otn/passcodeNew/checkRandCodeAnsyn";
	
	if(!_client.open(str_url))
	{
		_error_buf = _client.get_error_buffer();
		return false;
	}
	
	std::string recv_str = _client.read_some();
	
	Json::Value reader_object;
    Json::Reader reader;
	
	if (!reader.parse(recv_str, reader_object))
    {
		_error_buf = "返回json数据解析失败";
		SVLOGGER_FILE << recv_str;
		return false;
	}
	
	if (reader_object["status"].asBool())
	{
		if (reader_object["data"]["result"].asString() == "1")
		{
			SVLOGGER_INFO << "验证成功";
			return true;
		}
		else
		{
			if (reader_object["data"]["msg"].asString() == "randCodeError")
			{
				_error_buf = "验证码错误";
			}
			return false;
		}
	}
	
	return false;
}

bool ticket_manage::check_order_info(std::string spasscode)
{
	std::string url = "https://kyfw.12306.cn/otn/confirmPassenger/checkOrderInfo";
	
	request_header opts;
	opts.insert(std::string("Referer: https://kyfw.12306.cn/otn/confirmPassenger/initDc"));
	opts.insert(std::string("Accept-Encoding: gzip, deflate"));
	opts.insert(std::string("Connection: Keep-Alive"));
	opts.insert(std::string("Content-Type: application/x-www-form-urlencoded; charset=UTF-8"));
	_client.set_headers(opts);
	
	std::string ticket_str = m_passenger_dto.get_passenger_ticket_str();
	convert_str("gbk", "utf-8", ticket_str);
	std::string old_pstr = m_passenger_dto.get_old_passenger_str();
	convert_str("gbk", "utf-8", old_pstr);

	std::ostringstream osstr;
	osstr << "cancel_flag=2&bed_level_order_num=000000000000000000000000000000&passengerTicketStr="
		<< _client.str_escape(ticket_str)
		<< "&oldPassengerStr=" << _client.str_escape(old_pstr)
		<< "&tour_flag=dc&randCode=" << spasscode
		<< "&" << _client.str_escape(dynamic_key) << "=" << _client.str_escape(dynamic_value)
		<< "&_json_att=&REPEAT_SUBMIT_TOKEN=" << repeat_submit_token;

	std::string post_str = osstr.str();
	/*convert_str("gbk", "utf-8", post_str);*/
	_client.set_post_fields(post_str);
	
	if (!_client.open(url))
	{
		_error_buf = _client.get_error_buffer();
		return false;
	}
	
	std::string reponse_str = _client.read_some();
	convert_str("utf-8", "gbk", reponse_str);
	
	Json::Reader reader;
	Json::Value reader_object;
	
	if (!reader.parse(reponse_str, reader_object))
	{
		_error_buf = "返回json数据解析失败";
		SVLOGGER_DBG << reponse_str;
		return false;
	}
	
	if (!reader_object["status"].asBool() == true)
	{
		Json::Value msg = reader_object["messages"];
		_error_buf =  msg[(unsigned int)0].asString();
		return false;
	}
	
	return true;
}