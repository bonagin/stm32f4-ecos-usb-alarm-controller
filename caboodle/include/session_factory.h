#ifndef SESSION_FACTORY_H_
#define SESSION_FACTORY_H_
#include <cyg/kernel/kapi.h>

#include "session.h"
#include "duplexInterface.h"
#include "session_performer.h"

class cSessionFactory
{
protected:
	cSessionPerformer * mSessPerformer;

public:
	cSessionFactory(cSessionPerformer * performer) : mSessPerformer(performer){};
	virtual ~cSessionFactory(){};

	void setSessionPerformer(cSessionPerformer * performer){ mSessPerformer = performer; };

	virtual void createSession(cyg_uint8 * buff, cyg_uint32 buff_len, duplexInterface * iface) = 0;

};

#endif /* SESSION_FACTORY_H_ */
