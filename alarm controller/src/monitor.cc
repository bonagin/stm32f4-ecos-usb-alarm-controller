/*
 * sess_perfomer.cc
 *
 *  Created on: 13 Jul 2015
 *      Author: root
 */

#include <cyg/kernel/diag.h>
#include <stdio.h>
#include <stdlib.h>

#include "utils.h"
#include "version.h"
#include "led.h"
#include "session.h"
#include "monitor.h"

//#define USB_SESS_GETVERSION 0x01
//#define USB_SESS_TRANSFER_BIN 0x02

//__externC void hal_sam4s_watchdog_restart();

cMonitor * cMonitor::__instance = 0;

cyg_uint32 serial_ = 0x87654321;

void cMonitor::init()
{
	if(!__instance)
	{
		__instance = new cMonitor();
	}
}

cMonitor::cMonitor()
{
	mSession = 0;

	cyg_thread_create(MONITOR_PRIORITY,
			cMonitor::Mon_thread,
			(cyg_addrword_t)this,
			(char *)"Alarm Monitor",
			mMonStack,
			MONITOR_STACK_SIZE,
			&mMonThreadHandle,
			&mMonThread);
	cyg_thread_resume(mMonThreadHandle);
}

void cMonitor::Mon_thread(cyg_addrword_t args)
{
	cMonitor* _this = (cMonitor*)args;

	for(;;)
	{
		_this->run();
	}
}

cyg_bool cMonitor::run()
{
	if(mSession)
	{
		if(mSession->execute())
			diag_printf(GREEN("cMonitor: Session success\n"));
		else
			diag_printf(RED("cMonitor: Session failed\n"));


		delete mSession;
		mSession = 0;
	}

	cyg_thread_delay(10);

	return true;
}

cyg_bool cMonitor::performSession(cSession * session)
{
	if(!__instance)
		return false;

	if(__instance->mSession)
		return false;

	__instance->mSession = session;

	return true;
}

void cMonitor::debug(cTerm & t,int argc,char *argv[])
{
  if(argc > 1)
  {

  }
  else
  {

  }
}


cMonitor::~cMonitor()
{

}


