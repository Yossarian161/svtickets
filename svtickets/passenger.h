#ifndef _passenger_infomation_hpp
#define _passenger_infomation_hpp

#include <string>
#include <vector>
#include <sstream>

/**
 *	联系人信息
 */
class contacts_datum
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
	std::string isUserSelf;

	std::string get_passenger_id_status();
	
	std::string print_test()
	{
		std::ostringstream osstr;
		osstr << code << "\t" << passenger_name << "\t"
			//<< passenger_id_type_name << "\t"
			<< passenger_id_no << "\t" << mobile_no << "\t"
			;
		return osstr.str();
	}

};

/**
 *	乘客资料信息
 */
class passenger_datum
{
public:
	void set_passenger_datum(contacts_datum& contacts, const std::string& ttype = "1" ,const std::string& stype = "YZ");

	void set_passenger_name(const std::string& pname);
	void set_passenger_id_type(const std::string& ptype);
	void set_passenger_id_type_name(const std::string& pid_tname);
	void set_passenger_id_no(const std::string& pid);
	void set_seat_type(const std::string& stype);
	void set_passenger_type(const std::string& ptype);
	void set_mobile_no(const std::string& mobile);
	void set_ticket_type(const std::string& ttype = "1");

	std::string get_passenger_name();
	std::string get_passenger_id_type();
	std::string get_passenger_id_type_name();
	std::string get_passenger_id_no();

	std::string get_seat_type();
	std::string get_seat_type_code();
	std::string get_seat_type_name();

	std::string get_passenger_type();
	std::string get_mobile_no();
	std::string get_ticket_type();
	std::string get_ticket_type_name();

private:
	std::string passenger_name;
	std::string passenger_id_type;
	std::string passenger_id_type_name;
	std::string passenger_id_no;
	std::string passenger_type;
	std::string mobile_no;
	/**
	 *	席别
	 */
	std::string seat_type;
	/** 票种
	 *	ticket_type: "1":"成人票","2":"孩票","3":"学生票","4":"伤残军人票"
	 */
	std::string ticket_type;
};

/**
 *	乘客数据管理 
 */
class passenger_dto
{
public:
	/**
	 *	whole passenger operate
	 */
	// 向联系人列表插入联系人信息
	void contacts_datum_push(contacts_datum& contacts);
	// 清空联系人列表
	void contacts_datum_clear();
	
	/**
	 *	selected passenger operate
	 */
	// 清空已选乘客列表
	void passenger_datum_selected_clear();
	// 添加联系人到已选乘客列表
	void passenger_datum_selected(std::vector<passenger_datum>& selected_list);
	void passenger_datum_selected_push(passenger_datum& passenger);
	void passenger_datum_selected_push(unsigned int index);
	// 从已选乘客列表中删除某个联系人
	void passenger_datum_selected_pop(unsigned int index);

	void reset_passenger_seat_type(const std::string& seat_type);
	
	// 判定联系人是否已被选择，已选：return true;else false
	bool passenger_selected_judge(contacts_datum& contacts);
	
	// 获取联系人列表
	std::vector<contacts_datum>& get_contacts_datum_list();
	// 获取乘客列表
	std::vector<passenger_datum>& get_passenger_datum_list();
	
	// 
	std::string get_passenger_ticket_str();
	std::string get_old_passenger_str();

private:
	// 联系人资料 列表
	std::vector<contacts_datum> contacts_datum_list;

	// 已选乘客 列表
	std::vector<passenger_datum> passenger_datum_list;
};

#endif