#ifndef _left_ticket_hpp_
#define _left_ticket_hpp_

#include <string>
#include <sstream>

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
	inline std::string get_purpose_codes() { return purpose_codes; }
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
	// 余票查询
	inline std::string get_query_surplus_string()
	{
		std::ostringstream osstr;
		osstr << "purpose_codes=" << purpose_codes
			<< "&queryDate=" << train_date
			<< "&from_station=" << form_station
			<< "&to_station=" << to_station;

		return osstr.str();
	}

	left_ticket_dto():purpose_codes("ADULT"){};
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
	std::string purpose_codes;

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

	std::vector<std::string>& get_train_seat_type_queue()
	{
		if (train_seat_type_queue.size() == 0)
		{
			init_train_seat_queue();
		}
		
		return train_seat_type_queue;
	}
	bool train_exitst_ticket()
	{
		if (train_seat_type_queue.size() == 0)
		{
			init_train_seat_queue();
		}
		return exist_tickets;
	}

	std::string get_seat_num(const std::string& seat_type)
	{
		if (seat_type == "SWZ")
		{
			return swz_num;
		}
		else if (seat_type == "TZ")
		{
			return tz_num;
		}
		else if (seat_type == "ZY")
		{
			return zy_num;
		}
		else if (seat_type == "ZE")
		{
			return ze_num;
		}
		else if (seat_type == "GR")
		{
			return gr_num;
		}
		else if (seat_type == "RW")
		{
			return rw_num;
		}
		else if (seat_type == "YW")
		{
			return yw_num;
		}
		else if (seat_type == "RZ")
		{
			return rz_num;
		}
		else if (seat_type == "YZ")
		{
			return yz_num;
		}
		else if (seat_type == "WZ")
		{
			return wz_num;
		}

		return "--";
	}
private:
	void init_train_seat_queue()
	{
		train_seat_type_queue.clear();

		if (swz_num != "--")
		{
			train_seat_type_queue.push_back("SWZ");
			if (swz_num != "无") exist_tickets = true;
		}
		if (tz_num != "--")
		{
			train_seat_type_queue.push_back("TZ");
			if (tz_num != "无") exist_tickets = true;
		}
		if (zy_num != "--")
		{
			train_seat_type_queue.push_back("ZY");
			if (zy_num != "无") exist_tickets = true;
		}
		if (ze_num != "--")
		{
			train_seat_type_queue.push_back("ZE");
			if (ze_num != "无") exist_tickets = true;
		}
		if (gr_num != "--")
		{
			train_seat_type_queue.push_back("GR");
			if (gr_num != "无") exist_tickets = true;
		}
		if (rw_num != "--")
		{
			train_seat_type_queue.push_back("RW");
			if (rw_num != "无") exist_tickets = true;
		}
		if (yw_num != "--")
		{
			train_seat_type_queue.push_back("YW");
			if (yw_num != "无") exist_tickets = true;
		}
		if (rz_num != "--")
		{
			train_seat_type_queue.push_back("RZ");
			if (rz_num != "无") exist_tickets = true;
		}
		if (yz_num != "--")
		{
			train_seat_type_queue.push_back("YZ");
			if (yz_num != "无") exist_tickets = true;
		}
		if (wz_num != "--")
		{
			train_seat_type_queue.push_back("WZ");
			if (wz_num != "无") exist_tickets = true;
		}
	}

	std::vector<std::string> train_seat_type_queue;
	bool exist_tickets;
};

class ticket_price
{
public:
	std::string train_no;
	std::string yz_price;
	std::string rz_price;
	std::string yw_price;
	std::string rw_price;
	std::string gr_price;
	std::string zy_price;
	std::string ze_price;
	std::string tz_price;
	std::string gg_price;
	std::string yb_price;
	std::string wz_price;
	std::string qt_price;
	std::string swz_price;

	std::string get_ticket_seat_price(const std::string& seat_type)
	{
		if (seat_type == "SWZ")
		{
			return swz_price;
		}
		else if (seat_type == "TZ")
		{
			return tz_price;
		}
		else if (seat_type == "ZY")
		{
			return zy_price;
		}
		else if (seat_type == "ZE")
		{
			return ze_price;
		}
		else if (seat_type == "GR")
		{
			return gr_price;
		}
		else if (seat_type == "RW")
		{
			return rw_price;
		}
		else if (seat_type == "YW")
		{
			return yw_price;
		}
		else if (seat_type == "RZ")
		{
			return rz_price;
		}
		else if (seat_type == "YZ")
		{
			return yz_price;
		}
		else if (seat_type == "WZ")
		{
			return wz_price;
		}

		return "";
	}

};

#endif