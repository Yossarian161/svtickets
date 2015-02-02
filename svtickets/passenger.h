#ifndef _passenger_infomation_hpp
#define _passenger_infomation_hpp

#include <string>
#include <vector>
#include <sstream>

/**
 *	��ϵ����Ϣ
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
 *	�˿�������Ϣ
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
	 *	ϯ��
	 */
	std::string seat_type;
	/** Ʊ��
	 *	ticket_type: "1":"����Ʊ","2":"��Ʊ","3":"ѧ��Ʊ","4":"�˲о���Ʊ"
	 */
	std::string ticket_type;
};

/**
 *	�˿����ݹ��� 
 */
class passenger_dto
{
public:
	/**
	 *	whole passenger operate
	 */
	// ����ϵ���б������ϵ����Ϣ
	void contacts_datum_push(contacts_datum& contacts);
	// �����ϵ���б�
	void contacts_datum_clear();
	
	/**
	 *	selected passenger operate
	 */
	// �����ѡ�˿��б�
	void passenger_datum_selected_clear();
	// �����ϵ�˵���ѡ�˿��б�
	void passenger_datum_selected(std::vector<passenger_datum>& selected_list);
	void passenger_datum_selected_push(passenger_datum& passenger);
	void passenger_datum_selected_push(unsigned int index);
	// ����ѡ�˿��б���ɾ��ĳ����ϵ��
	void passenger_datum_selected_pop(unsigned int index);

	void reset_passenger_seat_type(const std::string& seat_type);
	
	// �ж���ϵ���Ƿ��ѱ�ѡ����ѡ��return true;else false
	bool passenger_selected_judge(contacts_datum& contacts);
	
	// ��ȡ��ϵ���б�
	std::vector<contacts_datum>& get_contacts_datum_list();
	// ��ȡ�˿��б�
	std::vector<passenger_datum>& get_passenger_datum_list();
	
	// 
	std::string get_passenger_ticket_str();
	std::string get_old_passenger_str();

private:
	// ��ϵ������ �б�
	std::vector<contacts_datum> contacts_datum_list;

	// ��ѡ�˿� �б�
	std::vector<passenger_datum> passenger_datum_list;
};

#endif