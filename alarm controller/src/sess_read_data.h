/*
 * session_read_data.h
 *
 *  Created on: 14 Jul 2015
 *      Author: root
 */

#ifndef SESSION_READ_DATA_H_
#define SESSION_READ_DATA_H_

#include <session.h>

class cSessReadData : public cSession
{
public:

	cSessReadData(duplexInterface * iface);
	virtual ~cSessReadData();

	bool execute();
	bool handle_response(cyg_uint8 * buff, cyg_uint32 buff_len);

};


#endif /* SESSION_READ_DATA_H_ */
