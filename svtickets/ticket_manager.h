#ifndef _ticket_manager_hpp_
#define _ticket_manager_hpp_

#include <vector>
#include "svhttp.h"
#include "svlogger.h"
#include "bencode.h"

#include "station.hpp"
#include "left_ticket.hpp"
#include "passenger.h"

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
	// 
	/**
	 *	登录初始化<获取dynamicJs请求地址,获取动态key> --> 用于登录校验
	 *  如已经登录返回1；正常登录成功返回0； 登录失败返回-1；
	 */
	int login_init();
	// 请求dynamic url地址，获取dynamicJs，从中提取动态key
	bool get_dynamic_key(const std::string& dynamic_url);

	// 登录验证码刷新
	bool login_passcode_reflush(std::string img_path = "data/pass.png");
	// 登录验证码校验
	bool login_passcode_verify(std::string passcode_str);
	// 登录验证
	bool login_verify(const std::string& user_name, const std::string& passwd, const std::string& passcode);

	//退出登录
	bool login_out();

	// otn/modifyUser/initQueryUserInfo
	bool init_user_info();
	// 更新用户的登录状态 </...>
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
	bool query_passengers();
	passenger_dto& get_passengers(){ return m_passenger_dto; };
	/**
	 *	预定&提交
	 */
	// 登录用户检查
	bool login_check_user();
	// 车票预定 初始化页面 <获取用于预定车票时需要的dynamicKey&value>
	bool left_ticket_init();
	// 预定 按钮
	bool submit_order_request(train_data& _train_data);
	/**
	 *  获取单程车票的预订确认页面 ../initDC
	 *  解析获取 globalRepeatSubmitToken & dynamicKey=value
	 */ 
	bool confirm_passenger_initdc();
	// 预定确认验证码刷新
	bool passenger_passcode_reflush(std::string img_path = "data/pass.png");
	// 预定确认验证码校验
	bool passenger_passcode_verify(std::string passcode_str);
	// 
	bool check_order_info(std::string spasscode);

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
	std::string repeat_submit_token;
	
	// 12306用户信息
	std::string user_name;
	std::string user_name_cn;
	std::string user_card_type;
	std::string user_card_id;
	std::string user_card_status;

	// 车站名 <出发|到达站 输入控件值>
	std::vector<station_name> station_name_vec;

	// 车票数据 <预订查询数据>
	std::vector<train_data> train_data_vec;
	// 余票查询数据
	std::vector<train_data> train_data_surplus_vec;
	
	// 列车停靠站
	stop_station m_stop_station;

	// 联系人(乘客)管理
	passenger_dto m_passenger_dto;
};

#endif