// svhttp_basic.cpp : 定义控制台应用程序的入口点。
//
#ifndef _12306_help_hpp_
#define _12306_help_hpp_

#include "svlogger.h"
#include "svhttp.h"
#include "bencode.h"
#include <vector>

//using std::string;
using namespace svhttp;

class left_ticket_dto
{
public:
	inline void set_from_station(std::string sfrom){ form_station = sfrom; }
	inline void set_to_station(std::string sto) { to_station = sto; }
	inline void set_train_date(std::string tdate) { train_date = tdate; }
	inline void set_purpose_codes(bool sf2 = false) { 
		if (sf2)  // 学生票
		{	
			purpose_codes = "0X00";
		}
		else  // 成人票
		{
			purpose_codes = "ADULT";
		}
	}

	// 车票日期
	inline std::string get_train_date(){ return train_date; }
	// 构造查询字符串
	inline std::string get_query_string()
	{
		std::ostringstream osstr;
		osstr << "leftTicketDTO.train_date=" << train_date
			<< "&leftTicketDTO.from_station=" << form_station
			<< "&leftTicketDTO.to_station=" << to_station
			<< "&purpose_codes=" << purpose_codes;

		return osstr.str();
	}

	inline std::string get_query_surplus_string()
	{
		std::ostringstream osstr;
		osstr << "purpose_codes=" << purpose_codes
			<< "&queryDate=" << train_date
			<< "&from_station=" << form_station
			<< "&to_station=" << to_station;
			
		return osstr.str();
	}

private:
	std::string form_station;
	std::string to_station;
	std::string train_date;
	std::string purpose_codes;
};

class train_data
{
public:
	std::string train_no;
	std::string station_train_code;
	std::string start_station_telecode;
	std::string start_station_name;
	std::string end_station_telecode;
	std::string end_station_name;
	std::string from_station_telecode;
	std::string from_station_name;
	std::string to_station_telecode;
	std::string to_station_name;
	std::string start_time;
	std::string arrive_time;
	std::string day_difference;
	std::string train_class_name;
	std::string lishi;
	std::string canWebBuy;
	std::string lishiValue;
	std::string yp_info;
	std::string control_train_day;
	std::string start_train_date;
	std::string seat_feature;
	std::string yp_ex;
	std::string train_seat_feature;
	std::string seat_types;
	std::string location_code;
	std::string from_station_no;
	std::string to_station_no;
	int control_day;
	std::string sale_time;
	std::string is_support_card;
	std::string yz_num;
	std::string rz_num;
	std::string yw_num;
	std::string rw_num;
	std::string gr_num;
	std::string zy_num;
	std::string ze_num;
	std::string tz_num;
	std::string gg_num;
	std::string yb_num;
	std::string wz_num;
	std::string qt_num;
	std::string swz_num;

	std::string secretStr;
	std::string buttonTextInfo;

	std::string train_date;

	std::string train_data_print()
	{
		std::ostringstream osstr;
		osstr << station_train_code << " \t"
			<< from_station_name << "\t"
			<< start_time << "\t"
			<< to_station_name << "\t"
			<< arrive_time << "\t"
			<< lishi << "\t"
			<< canWebBuy << "\t"
			<< buttonTextInfo << "\t"
			;

		return osstr.str();
	}
};

class station_info
{
public:
	std::string arrive_time;
	std::string stat_name;
	std::string start_time;
	std::string stopover_time;
	std::string station_no;
	bool isEnabled;
};

class stop_station
{
public:
	stop_station(){ stop_station_list.clear(); }
	~stop_station(){ stop_station_list.clear(); }

	void set_start_station_name(std::string val)	{ start_station_name = val; }
	void set_station_train_code(std::string val)	{ station_train_code = val; }
	void set_train_class_name(std::string val)		{ train_class_name = val; }
	void set_service_type(std::string val)			{ service_type = val; }
	void set_end_station_name(std::string val)		{ end_station_name = val; }
	
	void clear_stop_station_list()
	{
		stop_station_list.clear();
	};
	void push_stop_station_info(station_info& info)
	{
		stop_station_list.push_back(info);
	}
	void set_stop_station_list(std::vector<station_info>& list)
	{
		stop_station_list = list;
	}

	std::vector<station_info>& get_stop_station_list()
	{
		return stop_station_list;
	}

	std::string get_start_station_name()	{ return start_station_name; }
	std::string get_station_train_code()	{ return station_train_code; }
	std::string get_train_class_name()		{ return train_class_name; }
	std::string get_service_type()			{ return service_type; }
	std::string get_end_station_name()		{ return end_station_name; }

private:
	std::string start_station_name;
	std::string station_train_code;
	std::string train_class_name;	//
	std::string service_type;		// 0: 无空调 other: 有空调
	std::string end_station_name;	

	std::vector<station_info> stop_station_list;
};

#ifndef _station_name_class
#define _station_name_class
class station_name
{
public:
	std::string stat_spell_logo;
	std::string stat_cn;
	std::string stat_code;
	std::string stat_spell_full;
	std::string stat_spell_logo2;
	std::string stat_id;

	inline bool parse_string(std::string stat_str)
	{
		if (stat_str.empty())
		{
			return false;
		}
		// bjb|北京北|VAP|beijingbei|bjb|0
		int x_pos, d_pos = 0;
		x_pos = stat_str.find("|");
		stat_spell_logo = stat_str.substr(0, x_pos);
		d_pos = stat_str.find("|", x_pos+1);
		stat_cn = stat_str.substr(x_pos+1, d_pos-x_pos-1);
		x_pos = stat_str.find("|", d_pos+1);
		stat_code = stat_str.substr(d_pos+1, x_pos-d_pos-1);
		d_pos = stat_str.find("|", x_pos+1);
		stat_spell_full = stat_str.substr(x_pos+1, d_pos-x_pos-1);
		x_pos = stat_str.find("|", d_pos+1);
		stat_spell_logo2 = stat_str.substr(d_pos+1, x_pos-d_pos-1);
		d_pos = stat_str.find("|", x_pos+1);
		stat_id = stat_str.substr(x_pos+1, d_pos-x_pos-1);

#if 0
		SVLOGGER_DBG << stat_cn << "\t" << stat_spell_logo << "\t"
			<< stat_spell_full << "\t" << stat_spell_logo2 << "\t"
			<< stat_code << "\t" << stat_id << "\t";
#endif

		return true;
	}
};
#endif

/**
 *	乘客信息
 */
class pssenger_dto
{
public:
	std::string code;
	std::string passenger_name;
	std::string sex_code;
	std::string sex_name;
	std::string born_date;
	std::string country_code;
	std::string passenger_id_type_code;
	std::string passenger_id_type_name;
	std::string passenger_id_no;
	std::string passenger_type;
	std::string passenger_flag;
	std::string passenger_type_name;
	std::string mobile_no;
	std::string phone_no;
	std::string email;
	std::string address;
	std::string postalcode;
	std::string first_letter;
	std::string recordCount;
	std::string total_times;
	std::string index_id;
private:
};

class ticket_manage
{
public:
	ticket_manage()
	{
		_error_buf = "";
		train_data_vec.clear();
	}
	~ticket_manage()
	{
		train_data_vec.clear();
	}
	// 登录初始化<获取dynamicJs请求地址,获取动态key>
	bool login_init();
	// 请求dynamic url地址，获取dynamicJs，从中提取动态key
	bool get_dynamic_key(const std::string& dynamic_url);

	// 验证码刷新
	bool passcode_reflush(std::string img_path = "data/pass.png");
	// 验证码校验
	bool passcode_verify(std::string passcode_str);
	// 登录验证
	bool login_verify(const std::string& user_name, const std::string& passwd, const std::string& passcode);

	//退出登录
	bool login_out();

	// otn/modifyUser/initQueryUserInfo
	bool init_user_info();

	bool update_user_login();

	// 获取车站名称 列表 <车站输入控件值>
	bool get_station_name();
	std::vector<station_name>& get_station_name_list(){ return station_name_vec; }

	// 查询火车票数据 <预订>
	// --test version
	bool query_train_data(left_ticket_dto& ticket_dto);
	std::vector<train_data>& get_train_data_list() { return train_data_vec; }
	
	// 余票查询
	bool query_train_data_surplus(left_ticket_dto& ticket_dto);
	std::vector<train_data>& get_train_data_surplus_list() { return train_data_surplus_vec; }

	// 查询列车停靠站 信息
	bool query_stop_station(unsigned int index);
	stop_station& get_stop_station(){ return m_stop_station; };

	/**
	 *	获取乘客信息
	 */
	bool get_passenger_dtos();

	/**
	 *	get 系列函数
	 */
	std::string get_username_cn(){ return user_name_cn; }
	std::string get_user_card_id(){ return user_card_id; }

	std::string get_error_buffer(){ return _error_buf; };
private:
	svhttp::http_client _client;
	std::string _error_buf;

	// 12306 dynamic key&value
	std::string dynamic_key;
	std::string dynamic_value;
	
	// 12306用户信息
	std::string user_name;
	std::string user_name_cn;
	std::string user_card_type;
	std::string user_card_id;
	std::string user_card_status;

	// 车票数据 <预订数据>
	std::vector<train_data> train_data_vec;
	// 余票查询数据
	std::vector<train_data> train_data_surplus_vec;
	// 车站名 <出发|到达站 输入控件值>
	std::vector<station_name> station_name_vec;
	// 列车停靠站
	stop_station m_stop_station;
};


#endif