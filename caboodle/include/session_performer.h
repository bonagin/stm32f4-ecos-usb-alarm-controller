#ifndef SRC_SESSION_PERFORMER_H_
#define SRC_SESSION_PERFORMER_H_
#include "session.h"

class cSessionPerformer
{
protected:
	cSession * mSession;

public:
	cSessionPerformer(){ mSession = 0; };
	virtual ~cSessionPerformer(){};
	virtual cyg_bool performSession(cSession * session) = 0;
};

#endif /* SRC_SESSION_PERFORMER_H_ */
