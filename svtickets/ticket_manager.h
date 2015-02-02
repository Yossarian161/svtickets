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
	 *	��¼��ʼ��<��ȡdynamicJs�����ַ,��ȡ��̬key> --> ���ڵ�¼У��
	 *  ���Ѿ���¼����1��������¼�ɹ�����0�� ��¼ʧ�ܷ���-1��
	 */
	int login_init();
	// ����dynamic url��ַ����ȡdynamicJs��������ȡ��̬key
	bool get_dynamic_key(const std::string& dynamic_url);

	// ��¼��֤��ˢ��
	bool login_passcode_reflush(std::string img_path = "data/pass.png");
	// ��¼��֤��У��
	bool login_passcode_verify(std::string passcode_str);
	// ��¼��֤
	bool login_verify(const std::string& user_name, const std::string& passwd, const std::string& passcode);

	//�˳���¼
	bool login_out();

	// otn/modifyUser/initQueryUserInfo
	bool init_user_info();
	// �����û��ĵ�¼״̬ </...>
	bool update_user_login();

	// ��ȡ��վ���� �б� <��վ����ؼ�ֵ>
	bool get_station_name();
	std::vector<station_name>& get_station_name_list(){ return station_name_vec; }

	// ��ѯ��Ʊ���� <Ԥ��>
	// --test version
	bool query_train_data(left_ticket_dto& ticket_dto);
	std::vector<train_data>& get_train_data_list() { return train_data_vec; }
	
	// ��Ʊ��ѯ
	bool query_train_data_surplus(left_ticket_dto& ticket_dto);
	std::vector<train_data>& get_train_data_surplus_list() { return train_data_surplus_vec; }

	// ��ѯ�г�ͣ��վ ��Ϣ
	bool query_stop_station(unsigned int index);
	stop_station& get_stop_station(){ return m_stop_station; };

	/**
	 *	��ȡ�˿���Ϣ
	 */
	bool query_passengers();
	passenger_dto& get_passengers(){ return m_passenger_dto; };
	/**
	 *	Ԥ��&�ύ
	 */
	// ��¼�û����
	bool login_check_user();
	// ��ƱԤ�� ��ʼ��ҳ�� <��ȡ����Ԥ����Ʊʱ��Ҫ��dynamicKey&value>
	bool left_ticket_init();
	// Ԥ�� ��ť
	bool submit_order_request(train_data& _train_data);
	/**
	 *  ��ȡ���̳�Ʊ��Ԥ��ȷ��ҳ�� ../initDC
	 *  ������ȡ globalRepeatSubmitToken & dynamicKey=value
	 */ 
	bool confirm_passenger_initdc();
	// Ԥ��ȷ����֤��ˢ��
	bool passenger_passcode_reflush(std::string img_path = "data/pass.png");
	// Ԥ��ȷ����֤��У��
	bool passenger_passcode_verify(std::string passcode_str);
	// 
	bool check_order_info(std::string spasscode);

	/**
	 *	get ϵ�к���
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
	
	// 12306�û���Ϣ
	std::string user_name;
	std::string user_name_cn;
	std::string user_card_type;
	std::string user_card_id;
	std::string user_card_status;

	// ��վ�� <����|����վ ����ؼ�ֵ>
	std::vector<station_name> station_name_vec;

	// ��Ʊ���� <Ԥ����ѯ����>
	std::vector<train_data> train_data_vec;
	// ��Ʊ��ѯ����
	std::vector<train_data> train_data_surplus_vec;
	
	// �г�ͣ��վ
	stop_station m_stop_station;

	// ��ϵ��(�˿�)����
	passenger_dto m_passenger_dto;
};

#endif