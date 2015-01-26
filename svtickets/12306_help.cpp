#include "12306_help.h"

#include <iostream>
#include <sstream>
#include "jsoncpp/include/json.h"
#include "bencode.h"
#include "dynamic_encrypt.hpp"


// ��¼��ʼ��<��ȡdynamicJs�����ַ,��ȡ��̬key>
bool ticket_manage::login_init()
{
	static const bool status = false;
	if (!status)
	{
		//	_client.set_verbose();
		_client.set_option(CURLOPT_FOLLOWLOCATION, true);
		_client.set_cookies("data/cookie.txt");
		_client.enable_accept_encoding(true);
		/*���ô���*/
		//_client.set_proxy(std::string("10.1.100.33:808"));
	}

	/**
	 *	��ȡdynamicjs ����url
	 */
	std::string url = "https://kyfw.12306.cn/otn/login/init";
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
		
		// ����dynamicJs��ַ����ȡdynamic key
		if(!_client.open(dynamic_url))
		{
			_error_buf = _client.get_error_buffer();
			return false;
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

		passcode_reflush();

		return true;
	}
	
	//SVLOGGER_ERR << reponse_str;

	passcode_reflush();

	return false;
}

bool ticket_manage::passcode_reflush(std::string img_path)
{
	request_header opts;
	opts.insert(std::string("Referer: https://kyfw.12306.cn/otn/login/init#"));
	opts.insert(std::string("Accept-Language: zh-cn"));
	opts.insert(std::string("User-Agent: Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 5.1; Trident/4.0; .NET4.0C; .NET4.0E)"));
//	opts.insert(std::string("Accept: image/png, image/svg+xml, image/*;q=0.8, */*;q=0.5"));
	opts.insert(std::string("Connection: Keep-Alive"));
	opts.insert(std::string("x-requested-with: XMLHttpRequest"));
	opts.insert(std::string("Cache-Control: no-cache"));
	opts.insert(std::string("Content-Type: application/x-www-form-urlencoded; charset=UTF-8"));
	_client.set_headers(opts);
	
	// ���������
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

bool ticket_manage::passcode_verify(std::string passcode_str)
{
	std::string post_str;
	post_str = "randCode=" + passcode_str;
	post_str += "&rand=sjrand&randCode_validate=";

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
		_error_buf = "����json���ݽ���ʧ��";
		SVLOGGER_FILE << recv_str;
		return false;
	}

	if (reader_object["status"].asBool())
	{
		if (reader_object["data"]["result"].asString() == "1")
		{
			SVLOGGER_INFO << "��֤�ɹ�";
			return true;
		}
		else
		{
			if (reader_object["data"]["msg"].asString() == "randCodeError")
			{
				_error_buf = "��֤�����";
			}
			return false;
		}
	}

	return false;
}

bool ticket_manage::login_verify(const std::string& username, const std::string& passwd, const std::string& passcode)
{
	if (!passcode_verify(passcode))
	{
		return false;
	}

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
		_error_buf = "����json���ݽ���ʧ��";
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
			SVLOGGER_DBG << "��¼�ɹ�";
			
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

	x_pos = reponse_str.find("����");
	if (x_pos != -1)
	{
		x_pos = reponse_str.find("class=\"con\"", x_pos);
		d_pos = reponse_str.find("</div>", x_pos+12);

		user_name_cn = reponse_str.substr(x_pos+12, d_pos-x_pos-12);
		SVLOGGER_INFO << user_name_cn;

		x_pos = reponse_str.find("֤������");
		x_pos = reponse_str.find("class=\"con\"", x_pos);
		d_pos = reponse_str.find("</div>", x_pos+12);
		
		user_card_type = reponse_str.substr(x_pos+12, d_pos-x_pos-12);
		SVLOGGER_INFO << user_card_type;


		x_pos = reponse_str.find("֤������");
		x_pos = reponse_str.find("class=\"con\"", x_pos);
		d_pos = reponse_str.find("</div>", x_pos+12);
		
		user_card_id = reponse_str.substr(x_pos+12, d_pos-x_pos-12);
		SVLOGGER_INFO << user_card_id;


		x_pos = reponse_str.find("����״̬");
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
	// ������վ��������
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
	//_client.set_verbose(true);

// 	std::string _url = "https://kyfw.12306.cn/otn/leftTicket/log?leftTicketDTO.train_date=2015-02-17&leftTicketDTO.from_station=SZQ&leftTicketDTO.to_station=BJP&purpose_codes=ADULT";
// 	if (!_client.open(_url))
// 	{
// 		_error_buf = _client.get_error_buffer();
// 		return false;
// 	}
// 
// 	std::string _reponse_str = _client.read_some();
// 	//convert_str("utf-8", "gbk", _reponse_str);
// 	SVLOGGER_INFO << _reponse_str;

	std::string url = "https://kyfw.12306.cn/otn/leftTicket/queryT?"
		+ ticket_dto.get_query_string();
		//"leftTicketDTO.train_date=2015-02-17&leftTicketDTO.from_station=SZQ&leftTicketDTO.to_station=BJP&purpose_codes=ADULT";

	request_header opts;
	opts.insert(std::string("Referer:https://kyfw.12306.cn/otn/leftTicket/init"));
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
		_error_buf = "����json���ݽ���ʧ��";
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
		_error_buf = "����json���ݽ���ʧ��";
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
			t_data.buttonTextInfo = (left_new_dto["note"].asString().empty())?"Ԥ��":left_new_dto["note"].asString();

			// 
			t_data.train_date = ticket_dto.get_train_date();

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
		_error_buf = "����json���ݽ���ʧ��";
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


