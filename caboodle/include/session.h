#ifndef SESSION_H_
#define SESSION_H_
#include <cyg/kernel/kapi.h>
#include <cyg/kernel/diag.h>

#include "duplexInterface.h"

class cSession
{
protected:
	duplexInterface * mIf;

	cyg_uint32 waitReply(cyg_uint8 * buff, cyg_uint32 data_len, cyg_uint32 buff_size);

public:
	cSession(duplexInterface * iface);
	virtual ~cSession(){};

	virtual bool execute() = 0;
};

#endif /* SESSION_H_ */
