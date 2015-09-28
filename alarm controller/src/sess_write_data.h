/*
 * session_write_data.h
 *
 *  Created on: 15 Jul 2015
 *      Author: root
 */

#ifndef SESSION_WRITE_DATA_H_
#define SESSION_WRITE_DATA_H_

#include <session.h>

class cSessWriteData : public cSession
{
public:

	cSessWriteData(duplexInterface * iface);
	virtual ~cSessWriteData();

	bool execute();

};


#endif /* SESSION_WRITE_DATA_H_ */
