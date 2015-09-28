#ifndef DUPLEXINTERFACE_H_
#define DUPLEXINTERFACE_H_
#include <cyg/kernel/kapi.h>

class duplexInterface
{
public:
	duplexInterface(){};

	virtual cyg_bool transmit(cyg_uint8 * buff,cyg_uint32 buff_len) = 0;
	virtual cyg_uint32 receive(cyg_uint8 * buff,cyg_uint32 buff_len) = 0;

	virtual ~duplexInterface(){};
};

#endif /* DUPLEXINTERFACE_H_ */
