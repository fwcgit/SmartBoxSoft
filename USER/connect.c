#include "connect.h"
#include "AtParser.h"

u16 net_pos = 0;

const unsigned char *enable_net_table[] = 
{
	(unsigned char *)"AT\r\n",
	(unsigned char *)"AT+CGATT=1\r\n",
	(unsigned char *)"AT+CGDCONT=1,\"IP\",\"CMNET\"\r\n",
	(unsigned char *)"AT+CGACT=1,1\r\n"
};

const unsigned char *connect_server_table[] =
{
	(unsigned char *)"AT+CIPSTART=\"TCP\",\"101.37.106.182\",11111\r\n"
};

void gsm_mode_config(void)
{
	net_pos = 0;
	net_init(0);
	
	enable_net_at();
	
}

void enable_net_at(void)
{
	if(net_pos < 4)
	{
			Send_data(enable_net_table[net_pos],strlen((const char *)enable_net_table[net_pos]));
			net_pos++;
	}
	else
	{
		net_init(1);
		connect_server();
	}
}

void connect_server(void)
{
	Send_data(connect_server_table[0],strlen((const char *)connect_server_table[0]));
}

void close_connect(void)
{
	if(is_network())
	{
		gsm_mode_config();
	}
}

