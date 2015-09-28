#ifndef USBTHREAD_H_
#define USBTHREAD_H_
#include <cyg/kernel/kapi.h>

#include <hdlc_framer.h>
#include <duplexInterface.h>

#define USBRXBUFF_SIZE 1024

class usbInterface : public duplexInterface
{
public:
	enum eUSBstate
	{
		unknown,
		reset,
		configured,
		suspended,
		resumed
	};

private:
	static usbInterface * __instance;

	bool mBannerFlag;

	eUSBstate mUSBstatus;

	cyg_cond_t mUSBrxCond;
	cyg_mutex_t mUSBmutex;

	static cyg_uint8 mUSBRXbuff[USBRXBUFF_SIZE];
	cyg_uint32 mUSBRXlen;


	usbInterface(cyg_uint32 serial);

public:
	static void init(cyg_uint32 serial);
	static usbInterface* get(){ return __instance; };

	//USB device function call backs
	static void     USBD_USR_Init(void);
	static void     USBD_USR_DeviceReset (cyg_uint8 speed);
	static void     USBD_USR_DeviceConfigured (void);
	static void     USBD_USR_DeviceSuspended(void);
	static void     USBD_USR_DeviceResumed(void);

	static void     USBD_USR_DeviceConnected(void);
	static void     USBD_USR_DeviceDisconnected(void);


	//Communication device class functions
	static cyg_uint16 cdc_Init();
	static cyg_uint16 cdc_DeInit(void);
	static cyg_uint16 cdc_Ctrl(cyg_uint32 Cmd, cyg_uint8* Buf, cyg_uint32 Len);
	static cyg_uint16 cdc_DataTx (cyg_uint8* Buf, cyg_uint32 Len);
	static cyg_uint16 cdc_rxData(cyg_uint8* Buf, cyg_uint32 Len);

	void setUSBstate(eUSBstate state);
	eUSBstate getUSBstate(){ return mUSBstatus; };

	cyg_bool transmit(cyg_uint8 * buff,cyg_uint32 buff_len);
	cyg_uint32 receive(cyg_uint8 * buff,cyg_uint32 buff_len);


	virtual ~usbInterface();
};

#endif /* USBTHREAD_H_ */
