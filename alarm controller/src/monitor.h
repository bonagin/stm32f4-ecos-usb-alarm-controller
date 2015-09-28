/*
 * sess_perfomer.h
 *
 *  Created on: 13 Jul 2015
 *      Author: root
 */

#ifndef SESS_PERFOMER_H_
#define SESS_PERFOMER_H_

#include <cyg/kernel/kapi.h>
#include <kses_term.h>
#include <session.h>
#include <session_performer.h>
#include <debug.h>
#include <time.h>

#include "definitions.h"

class cMonitor : public cSessionPerformer, public cDebug
{
	static cMonitor * __instance;

	cyg_uint8 mMonStack[MONITOR_STACK_SIZE];
	cyg_thread mMonThread;
	cyg_handle_t mMonThreadHandle;
	static void Mon_thread(cyg_addrword_t args);

	cMonitor();

public:
	static void init();
	static cMonitor * get(){ return __instance; };
	virtual ~cMonitor();

	cyg_bool run();
	cyg_bool performSession(cSession * session);
	static void debug(cTerm & t,int argc,char *argv[]);

};



#endif /* SESS_PERFOMER_H_ */
