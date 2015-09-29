/*
 * session_write_data.cc
 *
 *  Created on: 15 Jul 2015
 *      Author: root
 */
#include <crc.h>
#include <session.h>

#include "sess_write_data.h"
#include "input_port.h"

cSessWriteData::cSessWriteData(duplexInterface * interface) : cSession(interface)
{
}

bool cSessWriteData::execute()
{
	diag_printf("Executing the cSessWriteData session\n");
    bool status = false;
    cyg_uint8 buff[512];

    if(cInput::get()->getPortState(0))
  	  buff[0] = 0xFF;
    else
  	  buff[0] = 0x00;

    cyg_uint32 len = waitReply(buff,1,512);

	 if(len)
	 {
		if(buff[0] == 0xF7)
		{
			status = true;
			diag_printf("Received acknowledgment\n");
		}
	 }

	return status;
}

cSessWriteData::~cSessWriteData()
{

}



