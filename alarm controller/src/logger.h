
#ifndef LOGGER_H_
#define LOGGER_H_

#include <cyg/kernel/kapi.h>
#include <kses_term.h>
#include "definitions.h"
#include "usb_device.h"

#define USB_BUFF_SIZE	1024

class cLOG
{
	static cLOG* __instance;
    cUSBdevice * mUSBdev;

	cyg_uint8 mTXbuff[USB_BUFF_SIZE];

	cyg_uint8 mLOGStack[LOG_STACK_SIZE];
	cyg_thread mLOGThread;
	cyg_handle_t mLOGThreadHandle;
	static void log_thread(cyg_addrword_t args);

	cLOG();

public:
	static void read_logs(cyg_uint32 start_address_, cyg_uint32 last_address_);
	static void init();
	static cLOG* get();
	void write_log(cyg_uint8 dir);
};



#endif /* LOGGER_H_ */
