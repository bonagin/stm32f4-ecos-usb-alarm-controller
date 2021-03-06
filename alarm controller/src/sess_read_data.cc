/*
 * session_read_data.cc
 *
 *  Created on: 14 Jul 2015
 *      Author: root
 */
#include <crc.h>
#include <session.h>

#include "sess_read_data.h"
#include "F4_RTC.h"

cSessReadData::cSessReadData(duplexInterface * interface) : cSession(interface)
{
}

bool cSessReadData::execute()
{
	bool status = false;

    diag_printf("Executing the cSessReadData session\n");
    cyg_uint8 buff[512];

    buff[0] = 0xb7;
    cyg_uint32 len = waitReply(buff,1,sizeof(buff));//send the device serial as acknowledgment

    if(len)
    {
    	if(handle_response(buff,len))
    	{
    	    diag_printf("Valid data received len : %d, sending acknowledgment\n",len);
    	    status = true;
    		cyg_uint8 ACK[1] = {0xF7};
            mIf->transmit(ACK,sizeof(ACK));
    	}
    }

	return status;
}

bool cSessReadData::handle_response(cyg_uint8 * buff, cyg_uint32 buff_len)
{
	cyg_uint16 yy,mm,dd,h,m,s;

		yy = (cyg_uint16(buff[0]) << 8) + cyg_uint16(buff[1]);
		if(yy < 1900)
			return false;

		mm = (cyg_uint16) buff[2];
		if(mm > 12)
			return false;

		dd = (cyg_uint16) buff[3];
		if(dd > 31)
			return false;

		h = (cyg_uint16) buff[4];
		if(h > 24)
			return false;

		m = (cyg_uint16) buff[5];
		if(m > 60)
			return false;

		s = (cyg_uint16) buff[6];
		if(s > 60)
			return false;

		time_t timeVal;
		struct tm info;
		info.tm_year = yy - 1900;
		info.tm_mon =  mm - 1;
		info.tm_mday = dd;
		info.tm_hour = h;
		info.tm_min = m;
		info.tm_sec = s;
		timeVal = mktime(&info);

		time_t now = time(NULL);
		if(!cyg_libc_time_settime(timeVal))
			diag_printf("Updated time\n");
		else
			diag_printf("Could not update time\n");

		cyg_thread_delay(10);
		diag_printf("\t- %s\n", asctime(localtime(&now)));

	return true;
}

cSessReadData::~cSessReadData()
{

}


