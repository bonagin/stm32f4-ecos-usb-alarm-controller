#ifndef SRC_USB_DEVICE_H_
#define SRC_USB_DEVICE_H_
#include <interface_usb.h>
#include <session_factory.h>

#include <kses_term.h>

#include "definitions.h"

class cUSBdevice : public cSessionFactory, public duplexInterface
{
	static cUSBdevice * __instance;
	usbInterface * mUSB;

	cyg_uint8 mRXbuff[USBRXBUFF_SIZE];
	cyg_uint32 mRXlen;
	cyg_bool mRXwaiting;

	cyg_cond_t mUSBCond;
	cyg_mutex_t mUSBmutex;

	bool mTXflag;

	cyg_uint8 mStack[USB_STACK_SIZE];
	cyg_thread mThread;
	cyg_handle_t mThreadHandle;
	static void thread_func(cyg_addrword_t arg);
	void run();

	cUSBdevice(cyg_uint32 serial, cSessionPerformer * performer);

public:
	static void init(cyg_uint32 serial, cSessionPerformer * performer);
	virtual ~cUSBdevice();

	void createSession(cyg_uint8 * buff, cyg_uint32 buff_len, duplexInterface * iface);

	cyg_bool transmit(cyg_uint8 * buff,cyg_uint32 buff_len);
	cyg_uint32 receive(cyg_uint8 * buff,cyg_uint32 buff_len);

	void StartWrite();

	static void debug(cTerm & t,int argc,char *argv[]);

};

#endif /* SRC_USB_DEVICE_H_ */
