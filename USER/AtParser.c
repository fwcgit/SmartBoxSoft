#include "AtParser.h"
#include <ctype.h>

char is_creg = 0;
u8 is_net_init = 0;
u8 is_gsm_ready = 0;
u8 is_connect_server = 0;
u8 read_cmd = 1;

u16 strToInt(unsigned char *str)
{
	u16 i = 0;
	u16 reg = 0;
	
	while(*(str+i) != ',')
	{
		if(*(str+i) >= '0' && *(str+i) <= '9')
		{
			reg = reg * 10 + *(str+i) - '0';
		}
		i++;
	}
	
	return reg;
}
u8 is_network(void)
{
	return is_creg == 1 || is_creg == 5 ? 0 : 1;
}

u8 gsm_ready(void)
{
	return is_gsm_ready;
}

void at_creg_par(unsigned char *atcmd,u16 len)
{
	u16 i = 0;
	u8 reg = 0;
	
	while(i < (len -2))
	{
		if(*(atcmd+i) >= '0' && *(atcmd+i) <= '9')
		{
			reg = reg * 10 + *(atcmd+1) - '0';
		}
		
		if(*(atcmd+i) == ',' || *(atcmd+i) == '\0')
			break;
		
		i++;
	}
	
	is_creg = reg;
}

u8 mem_cmp(unsigned char *dst,unsigned char *src,u16 len)
{
	u16 i = 0;
	
	while(i < len)
	{
		if(*(src+i) != *(dst+i) || *(src+i) == 0 || *(dst+i) == 0)
			return 1;
		
		i++;
	}
	
	return 0;
}

unsigned char* check_valid(unsigned char *atcmd,u16 *len)
{
	u16 i = 0;
	if(*len < 4) return atcmd;
	
	while( i < *len)
	{
			if(!(*(atcmd+i) == '\r' || *(atcmd+i) == '\n' || *(atcmd+i) == ' '))
			{
				*len = *len - i;
				return atcmd+i;
			}
			
			i++;
	}
	
	 return atcmd;
}

void net_init(u8 val)
{
	is_net_init = val;
}

u8 is_server_connect(void)
{
	return is_connect_server;
}

void read_at_cmd(u8 cmd)
{

}

void at_parser(unsigned char *atcmd,u16 len)
{
	
	//printf("%s ","at_parser");
	
	if(len == 2) return;
	
	if(*(atcmd+(len-2)) == '\r' && *(atcmd+(len-1)) == '\n')
	{
		
		atcmd = check_valid(atcmd,&len);
		
		if(mem_cmp((unsigned char *)"AT+CGSN",atcmd,7) == 1)
		{
			if(read_cmd == 1)
			{
				//printf("%s %d\r\n",atcmd,strlen(atcmd));
			}
		}
		
		if(mem_cmp("+CREG:",atcmd,6) == 0)
		{
			
			at_creg_par(atcmd+6,len -6);
			
			if(!(is_creg == 1 || is_creg == 5))
			{
				is_connect_server = 0;
			}
			
			#if 1
			printf("CREG %d \r\n",is_creg);
			#endif
			
		}
		else if(mem_cmp("CONNECT OK",atcmd,10) == 0)
		{
			is_connect_server = 1;
		}
		else if(mem_cmp("+TCPCLOSED",atcmd,10) == 0)
		{
			is_connect_server = 0;
		}
		else if(mem_cmp("+CIPRCV",atcmd,7) == 0)
		{
			#if 0
			printf("RECE AT ON \r\n");
			#endif
			
			rece_data(atcmd+7,strToInt(atcmd+8));
		}
		else if(mem_cmp("+CIEV: READY",atcmd,12) == 0)
		{
			is_gsm_ready = 1;
			gsm_mode_config();
		}
		else if(mem_cmp("OK",atcmd,2) == 0)
		{
			
			#if 0
			printf("RECE AT ON \r\n");
			#endif
			
			if(is_net_init == 0 && is_gsm_ready == 1)
			{
				enable_net_at();
			}
		}
	}

}

void data_convert(unsigned char *src,u16 slen,unsigned char *dst)
{
  u16 i= 0 ,j= 0;
	unsigned char sh,sl;
	for(i = 0 ;i < slen;i+=2)
	{
		sh = *(src+i);
		sl = *(src+i+1);
		
		sh = toupper(sh) - 0x30;
		sl = toupper(sl) - 0x30;
		
		if(sh > 9)
			sh -= 7;
		
		if(sl > 9)
			sl -= 7;
		
		*(dst+j++) = sh*16+sl;
	}
	
	*(dst+j) = '\0';
}

void rece_data(unsigned char *data,u16 len)
{
	unsigned char data_pack[100];
	unsigned char dst_data[100];
	
	memcpy(data_pack,data+(len < 10 ? 3 : 4),len);
	
	data_convert(data_pack,len,dst_data);
	
	d(dst_data,strlen((char*)dst_data));
	
	d(data_pack,len);
}
