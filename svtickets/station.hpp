#ifndef _station_infomation_hpp_
#define _station_infomation_hpp_

#include <string>
#include <vector>

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


#endif