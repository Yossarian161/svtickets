#include "passenger.h"

/**
 *	contacts_datum <��ϵ������>��ʵ��
 */

std::string contacts_datum::get_passenger_id_status()
{
	std::string id_status_str;	// ��ݺ���״̬ 
	if (passenger_id_type_code == "2")
	{
		// ����վ����֧��һ���������֤�������Ϊ�����������֤.
		id_status_str = "δͨ��";
	}
	else
	{
		if (total_times == "95" || total_times == "97")
		{
			id_status_str = "��ͨ��";
		}
		else
		{
			if (total_times == "93" || total_times == "99")
			{
				if (passenger_id_type_code == "1")
				{
					id_status_str = "��ͨ��";
				}
				else
				{
					id_status_str = "Ԥͨ��";
				}
			}
			else
			{
				if (total_times == "94" || total_times == "96")
				{
					id_status_str = "δͨ��";
				}
				else
				{
					if (total_times == "92" || total_times == "98")
					{
						if (passenger_id_type_code == "B" || passenger_id_type_code == "G" || passenger_id_type_code == "C" )
						{
							id_status_str = "�뱨�� ";
						}
						else
						{
							id_status_str = "������ ";
						}
					}
					else
					{
						id_status_str = "�뱨�� ";
					}
				}
			}
		}
	}

	return id_status_str;
}


/**
 *	contacts_datum <��ϵ������>��ʵ��
 */
void passenger_datum::set_passenger_datum( contacts_datum& contacts, const std::string& ttype, const std::string& stype)
{
	set_passenger_name(contacts.passenger_name);
	set_passenger_id_type(contacts.passenger_id_type_code);
	set_passenger_id_type_name(contacts.passenger_id_type_name);
	set_passenger_id_no(contacts.passenger_id_no);
	set_passenger_type(contacts.passenger_type);
	set_mobile_no(contacts.mobile_no);

	set_seat_type(stype);
	set_ticket_type(ttype);
}

void passenger_datum::set_passenger_name( const std::string& pname )
{
	passenger_name = pname;
}
void passenger_datum::set_passenger_id_type(const std::string& ptype)
{
	passenger_type = ptype;
}
void passenger_datum::set_passenger_id_type_name( const std::string& pid_tname )
{
	passenger_id_type_name = pid_tname;
}
void passenger_datum::set_passenger_id_no( const std::string& pid )
{
	passenger_id_no = pid;
}
void passenger_datum::set_seat_type( const std::string& stype )
{
	seat_type = stype;
}
void passenger_datum::set_passenger_type( const std::string& ptype )
{
	passenger_type = ptype;
}
void passenger_datum::set_mobile_no( const std::string& mobile )
{
	mobile_no = mobile;
}
void passenger_datum::set_ticket_type( const std::string& ttype )
{
	ticket_type = ttype;
}

std::string passenger_datum::get_passenger_name()
{
	return passenger_name;
}
std::string passenger_datum::get_passenger_id_type()
{
	return passenger_id_type;
}
std::string passenger_datum::get_passenger_id_type_name()
{
	return passenger_id_type_name;
}
std::string passenger_datum::get_passenger_id_no()
{
	return passenger_id_no;
}
std::string passenger_datum::get_seat_type()
{
	return seat_type;
}
std::string passenger_datum::get_seat_type_code()
{
	std::string seat_code;
	if (seat_type == "ZY")
	{
		seat_code = "M";
	}
	else if (seat_type == "ZE")
	{
		seat_code = "O";
	}
	else if (seat_type == "SWZ")
	{
		seat_code = "9";
	}
	else if (seat_type == "TZ")
	{
		seat_code = "P";
	}
	else if (seat_type == "YZ")
	{
		seat_code = "1";
	}
	else if (seat_type == "RZ")
	{
		seat_code = "2";
	}
	else if (seat_type == "YW")
	{
		seat_code = "3";
	}
	else if (seat_type == "RW")
	{
		seat_code = "4";
	}
	else if (seat_type == "GR")
	{
		seat_code = "6";
	}
	else if (seat_type == "WZ")
	{
		seat_code = "WZ";
	}
	else if (seat_type == "SRRB")
	{
		seat_code = "F";
	}
	else if (seat_type == "YYRW")
	{
		seat_code = "A";
	}

	return seat_code;
}
std::string passenger_datum::get_seat_type_name()
{
	std::string seat_name;
	if (seat_type == "ZY")
	{
		seat_name = "һ����";
	}
	else if (seat_type == "ZE")
	{
		seat_name = "������";
	}
	else if (seat_type == "SWZ")
	{
		seat_name = "������";
	}
	else if (seat_type == "TZ")
	{
		seat_name = "�ص���";
	}
	else if (seat_type == "YZ")
	{
		seat_name = "Ӳ��";
	}
	else if (seat_type == "RZ")
	{
		seat_name = "����";
	}
	else if (seat_type == "YW")
	{
		seat_name = "Ӳ��";
	}
	else if (seat_type == "RW")
	{
		seat_name = "����";
	}
	else if (seat_type == "GR")
	{
		seat_name = "�߼�����";
	}
	else if (seat_type == "SRRB")
	{
		seat_name = "����";
	}
	else if (seat_type == "YYRW")
	{
		seat_name = "�߼�����";
	}
	else if (seat_type == "WZ")
	{
		seat_name = "����";
	}

	return seat_name;
}

std::string passenger_datum::get_passenger_type()
{
	return passenger_type;
}
std::string passenger_datum::get_mobile_no()
{
	return mobile_no;
}
std::string passenger_datum::get_ticket_type()
{
	return ticket_type;
}

std::string passenger_datum::get_ticket_type_name()
{
	std::string ttname;
	//ticket_type: "1":"����Ʊ","2":"��Ʊ","3":"ѧ��Ʊ","4":"�˲о���Ʊ"
	if (ticket_type == "1")
	{
		ttname = "����Ʊ";
	}
	else if (ticket_type == "2")
	{
		ttname = "��ͯƱ";
	}
	else if (ticket_type == "3")
	{
		ttname = "ѧ��Ʊ";
	}
	else if (ticket_type == "4")
	{
		ttname = "�˲о���Ʊ";
	}

	return ttname;
}


/**
 *	�˿����ݹ��� ��ʵ��
 */

void passenger_dto::contacts_datum_push( contacts_datum& contacts )
{
	contacts_datum_list.push_back(contacts);
}

std::vector<contacts_datum>& passenger_dto::get_contacts_datum_list()
{
	return contacts_datum_list;
}

void passenger_dto::contacts_datum_clear()
{
	contacts_datum_list.clear();
}

void passenger_dto::passenger_datum_selected( std::vector<passenger_datum>& selected_list )
{
	passenger_datum_list.clear();
	passenger_datum_list = selected_list;
}

void passenger_dto::passenger_datum_selected_clear()
{
	passenger_datum_list.clear();
}

void passenger_dto::passenger_datum_selected_push( passenger_datum& passenger )
{
	passenger_datum_list.push_back(passenger);
}

void passenger_dto::passenger_datum_selected_push( unsigned int index )
{
	if (index < contacts_datum_list.size() )
	{
		passenger_datum passenger;
		passenger.set_passenger_datum(contacts_datum_list[index]);
		passenger_datum_list.push_back(passenger);
	}
}

void passenger_dto::passenger_datum_selected_pop( unsigned int index )
{
	contacts_datum contacts = contacts_datum_list[index];

	std::vector<passenger_datum>::iterator iter = passenger_datum_list.begin();
	for (iter; iter < passenger_datum_list.end(); ++iter)
	{
		passenger_datum passenger = *iter;
		if (passenger.get_passenger_id_no() == contacts.passenger_id_no 
			&& passenger.get_ticket_type() == contacts.passenger_type)
		{
			passenger_datum_list.erase(iter);
			return ;
		}

	}
}

bool passenger_dto::passenger_selected_judge( contacts_datum& contacts )
{
	std::vector<passenger_datum>::iterator iter = passenger_datum_list.begin();
	for (iter; iter < passenger_datum_list.end(); ++iter)
	{
		passenger_datum passenger = *iter;
		if (passenger.get_passenger_id_no() == contacts.passenger_id_no 
			&& passenger.get_ticket_type() == contacts.passenger_type)
		{
			return true;
		}
	}

	return false;
}

std::vector<passenger_datum>& passenger_dto::get_passenger_datum_list()
{
	return passenger_datum_list;
}

std::string passenger_dto::get_passenger_ticket_str()
{
	std::string ticket_str;
	std::vector<passenger_datum>::iterator iter = passenger_datum_list.begin();

	std::ostringstream osstr_ticket;

	for (iter; iter != passenger_datum_list.end(); ++iter)
	{
		if (iter != passenger_datum_list.begin())
		{
			osstr_ticket << "_";
		}
		passenger_datum& passenger = *iter;
		
		osstr_ticket << passenger.get_seat_type_code() << ",0," 
			<< passenger.get_ticket_type() <<  "," 
			<< passenger.get_passenger_name() << ","  
			<< passenger.get_passenger_id_type() << ","
			<< passenger.get_passenger_id_no() << ","
			<< (passenger.get_mobile_no().empty()? "" : passenger.get_mobile_no()) << ",N";
	}

	return osstr_ticket.str();
}

std::string passenger_dto::get_old_passenger_str()
{
	std::string ticket_str;
	std::vector<passenger_datum>::iterator iter = passenger_datum_list.begin();

	std::ostringstream osstr_ticket;

	for (iter; iter != passenger_datum_list.end(); ++iter)
	{
		passenger_datum& passenger = *iter;

		osstr_ticket << passenger.get_passenger_name() << ","  
			<< passenger.get_passenger_id_type() << ","
			<< passenger.get_passenger_id_no() << ","
			<< passenger.get_passenger_type() << "_";
	}

	return osstr_ticket.str();
}
