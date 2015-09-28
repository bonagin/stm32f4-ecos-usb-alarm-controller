#include <cyg/kernel/diag.h>

#include "session.h"

cSession::cSession(duplexInterface * iface) : mIf(iface)
{
}

cyg_uint32 cSession::waitReply(cyg_uint8 * buff, cyg_uint32 data_len, cyg_uint32 buff_size)
{
	if(mIf->transmit(buff, data_len))
	{
		return mIf->receive(buff, buff_size);
	}

	return 0;
}
