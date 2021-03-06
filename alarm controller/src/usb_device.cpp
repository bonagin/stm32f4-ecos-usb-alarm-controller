#include <cyg/kernel/diag.h>
#include <string.h>
#include <stdlib.h>

#include <utils.h>

#include "usb_device.h"

#include "sess_write_data.h"
#include "sess_read_data.h"
#include "input_port.h"

#define READ_DATA			0xA1
#define WRITE_DATA			0xA8

cUSBdevice * cUSBdevice::__instance = 0;

void cUSBdevice::init(cyg_uint32 serial, cSessionPerformer * performer)
{
	if(!__instance)
		__instance = new cUSBdevice(serial, performer);
}

cUSBdevice::cUSBdevice(cyg_uint32 serial, cSessionPerformer * performer) : cSessionFactory(performer)
{
	usbInterface::init(serial);
	mUSB = usbInterface::get();
	mRXwaiting = false;
	mRXlen = 0;
	mTXflag = false;

	cyg_mutex_init(&mUSBmutex);
	cyg_cond_init(&mUSBCond, &mUSBmutex);

	cyg_thread_create(USB_PRIORITY,
			thread_func,
			(cyg_addrword_t)this,
			(char *)"USB Thread",
			mStack,
			USB_STACK_SIZE,
			&mThreadHandle,
			&mThread);
	cyg_thread_resume(mThreadHandle);
}

void cUSBdevice::thread_func(cyg_addrword_t arg)
{
	diag_printf("USB initialized\n");

	cUSBdevice * _this = (cUSBdevice*)arg;

	while(1)
	{
		_this->run();
	}
}

void cUSBdevice::run()
{
	if(!mUSB)
	{
	  cyg_thread_delay(100);
	  return;
	}

	cyg_uint8 buff[USBRXBUFF_SIZE];
	cyg_uint32 len = mUSB->receive(buff, USBRXBUFF_SIZE);

	if(len)
	{
		//diag_printf("cUSBdevice: RX %d\n", len);
		//diag_dump_buf(buff, len);

		cyg_mutex_lock(&mUSBmutex);
		if(!mRXwaiting)
		{
			//diag_printf("cUSBdevice: create\n");
			createSession(buff, len, this);
		}
		else
		{
			//diag_printf("cUSBdevice: waiting\n");
			memcpy(mRXbuff, buff, len);
			mRXlen = len;
			cyg_cond_signal(&mUSBCond);
		}
		cyg_mutex_unlock(&mUSBmutex);
	}

}

void cUSBdevice::createSession(cyg_uint8 * buff, cyg_uint32 buff_len, duplexInterface * interface)
{
	cyg_uint8 command_byte = buff[0];
	diag_printf("\ncUSBdevice: Session type byte = 0x%02X\n",command_byte);
	diag_printf("rx len = %d\n",buff_len);
	bool status = false;
	cSession * s = 0;

    if(buff_len > 0)
    {
     switch(command_byte)
     {
     case READ_DATA:
	  s = new cSessReadData(interface);
	  status = mSessPerformer->performSession(s);
	  break;
     case WRITE_DATA:
   	  s = new cSessWriteData(interface);
   	  status = mSessPerformer->performSession(s);
   	  break;
     }

		if(!status)
		{
			diag_printf(RED("cUSBdevice: failed to perform session\n"));

			if(s)
				delete s;
		}
    }
}

void cUSBdevice::StartWrite()
{
     mTXflag = true;
}

cyg_uint32 cUSBdevice::receive(cyg_uint8 * buff,cyg_uint32 buff_len)
{
	cyg_uint32 rxLen = 0;

	cyg_mutex_lock(&mUSBmutex);
	mRXwaiting = true;

	if(cyg_cond_timed_wait(&mUSBCond, cyg_current_time() + 500))
	{
		if(mRXlen < buff_len)
		{
			//diag_printf("cUSBdevice rx %d\n", mRXlen);
			memcpy(buff, mRXbuff, mRXlen);
			rxLen = mRXlen;
		}
	}

	mRXwaiting = false;
	cyg_mutex_unlock(&(__instance->mUSBmutex));

	return rxLen;
}

cyg_bool cUSBdevice::transmit(cyg_uint8 * buff,cyg_uint32 buff_len)
{
	if(mUSB)
		return mUSB->transmit(buff, buff_len);

	return false;
}

void cUSBdevice::debug(cTerm & t,int argc,char *argv[])
{
	if(!__instance)
		return;

	static cyg_uint8 buff[1024] __attribute__((section(".ccm")));
	cyg_uint32 len = 8;

	memset(buff, 0xEE, 1024);

	if(argc > 1)
	{
		len = atoi(argv[1]);
	}


	diag_printf("USB Transmit %d\n", len);
	//diag_dump_buf(buff, len);

	__instance->transmit(buff, len);

	len = __instance->receive(buff, 1024);

	if(len)
	{
		diag_printf("RX %d\n", len);
		diag_dump_buf(buff, len);
	}
	else
		t << "USB Transmit timed out\n";
}

cUSBdevice::~cUSBdevice()
{
}

