#include <cyg/kernel/diag.h>
#include <stdio.h>

#include "../usb_dev/inc/usb_conf.h"
#include "../usb_dev/inc/usbd_desc.h"
#include "../usb_otg/inc/usbd_cdc_core.h"

#include <TermCMD.h>
#include <utils.h>

#include "interface_usb.h"

usbInterface * usbInterface::__instance = 0;
cyg_uint8 usbInterface::mUSBRXbuff[USBRXBUFF_SIZE];

__ALIGN_BEGIN USB_OTG_CORE_HANDLE    USB_OTG_dev __ALIGN_END ;
extern  USBD_DEVICE USR_desc;

extern cyg_uint8  APP_Rx_Buffer []; /* Write CDC received data in this buffer.
                                     These data will be sent over USB IN endpoint
                                     in the CDC core functions. */
extern cyg_uint32 APP_Rx_ptr_in;    /* Increment this pointer or roll it back to
                                     start address when writing received data
                                    in the buffer APP_Rx_Buffer. */

CDC_IF_Prop_TypeDef cdc_fops =
{
		usbInterface::cdc_Init,
		usbInterface::cdc_DeInit,
		usbInterface::cdc_Ctrl,
		usbInterface::cdc_DataTx,
		usbInterface::cdc_rxData
};

USBD_Usr_cb_TypeDef USR_cb =
{
		usbInterface::USBD_USR_Init,
		usbInterface::USBD_USR_DeviceReset,
		usbInterface::USBD_USR_DeviceConfigured,
		usbInterface::USBD_USR_DeviceSuspended,
		usbInterface::USBD_USR_DeviceResumed,

		usbInterface::USBD_USR_DeviceConnected,
		usbInterface::USBD_USR_DeviceDisconnected,

};


void usbInterface::init(cyg_uint32 serial)
{
	if(!__instance)
	{
		__instance = new usbInterface(serial);
	}
}

usbInterface::usbInterface(cyg_uint32 serial)
{
	mBannerFlag = false;
	mUSBRXlen = 0;
	mUSBstatus = unknown;

	cyg_mutex_init(&mUSBmutex);
	cyg_cond_init(&mUSBrxCond, &mUSBmutex);

	USBD_Init(serial,
			&USB_OTG_dev,
			USB_OTG_FS_CORE_ID,
			&USR_desc,
			&USBD_CDC_cb,
			&USR_cb);

}

/**
 * @brief  USBD_USR_Init
 *         Displays the message on LCD for host lib initialization
 * @param  None
 * @retval None
 */
void  usbInterface::USBD_USR_Init(void)
{
	diag_printf("USB: USR init\n");
}

/**
 * @brief  USBD_USR_DeviceReset
 *         Displays the message on LCD on device Reset Event
 * @param  speed : device speed
 * @retval None
 */
void  usbInterface::USBD_USR_DeviceReset(cyg_uint8 speed )
{
	//	switch (speed)
	//	{
	//	case USB_OTG_SPEED_HIGH:
	//		diag_printf ("     USB Device Library v1.0.0 [HS]\n" );
	//		break;
	//
	//	case USB_OTG_SPEED_FULL:
	//		diag_printf("     USB Device Library v1.0.0 [FS]\n" );
	//		break;
	//	default:
	//		diag_printf("     USB Device Library v1.0.0 [??]\n" );
	//	}

	if(__instance)
		__instance->setUSBstate(reset);
}


/**
 * @brief  USBD_USR_DeviceConfigured
 *         Displays the message on LCD on device configuration Event
 * @param  None
 * @retval Staus
 */
void  usbInterface::USBD_USR_DeviceConfigured (void)
{
	if(__instance)
		__instance->setUSBstate(configured);
}

/**
 * @brief  USBD_USR_DeviceSuspended
 *         Displays the message on LCD on device suspend Event
 * @param  None
 * @retval None
 */
void  usbInterface::USBD_USR_DeviceSuspended(void)
{
	if(__instance)
		__instance->setUSBstate(suspended);
}


/**
 * @brief  USBD_USR_DeviceResumed
 *         Displays the message on LCD on device resume Event
 * @param  None
 * @retval None
 */
void  usbInterface::USBD_USR_DeviceResumed(void)
{
	if(__instance)
		__instance->setUSBstate(resumed);
}

/**
 * @brief  USBD_USR_DeviceConnected
 *         Displays the message on LCD on device connection Event
 * @param  None
 * @retval Staus
 */
void  usbInterface::USBD_USR_DeviceConnected (void)
{
	diag_printf("USB: Device Connected.\n");
}


/**
 * @brief  USBD_USR_DeviceDisonnected
 *         Displays the message on LCD on device disconnection Event
 * @param  None
 * @retval Staus
 */
void  usbInterface::USBD_USR_DeviceDisconnected (void)
{
	diag_printf("USB: Device Disconnected.\n");
}

/**
 * @brief  cdc_Init
 *         Initializes the Media on the STM32
 * @param  None
 * @retval Result of the opeartion (USBD_OK in all cases)
 */
cyg_uint16 usbInterface::cdc_Init()
{
	diag_printf("USB: CDC initialized\n");

	return USBD_OK;
}


cyg_uint16 usbInterface::cdc_DeInit(void)
{
	diag_printf("CDC De- initialized\n");

	return USBD_OK;
}

typedef struct
{
  cyg_uint32 bitrate;
  cyg_uint8  format;
  cyg_uint8  paritytype;
  cyg_uint8  datatype;
}LINE_CODING;

LINE_CODING linecoding =
  {
    115200, /* baud rate*/
    0x00,   /* stop bits-1*/
    0x00,   /* parity - none*/
    0x08    /* nb. of bits 8*/
  };

/**
 * @brief  cdc_Ctrl
 *         Manage the CDC class requests
 * @param  Cmd: Command code
 * @param  Buf: Buffer containing command data (request parameters)
 * @param  Len: Number of data to be sent (in bytes)
 * @retval Result of the opeartion (USBD_OK in all cases
 */
//not really necessary for this example
cyg_uint16 usbInterface::cdc_Ctrl (cyg_uint32 Cmd, cyg_uint8* Buf, cyg_uint32 Len)
{
	diag_printf("cdc_Ctrl %02X, len %d\n", Cmd, Len);

	switch (Cmd)
	{
	case SEND_ENCAPSULATED_COMMAND:
		/* Not  needed for this driver */
		break;

	case GET_ENCAPSULATED_RESPONSE:
		/* Not  needed for this driver */
		break;

	case SET_COMM_FEATURE:
		/* Not  needed for this driver */
		break;

	case GET_COMM_FEATURE:
		/* Not  needed for this driver */
		break;

	case CLEAR_COMM_FEATURE:
		/* Not  needed for this driver */
		break;

	case SET_LINE_CODING:
		diag_printf("set line coding\n");
		linecoding.bitrate = (cyg_uint32)(Buf[0] | (Buf[1] << 8) | (Buf[2] << 16) | (Buf[3] << 24));
		linecoding.format = Buf[4];
		linecoding.paritytype = Buf[5];
		linecoding.datatype = Buf[6];
		/* Set the new configuration */

		if(__instance && !(__instance->mBannerFlag))
		{
			__instance->mBannerFlag = true;
		}

		break;

	case GET_LINE_CODING:
		diag_printf("get line coding\n");
		Buf[0] = (cyg_uint8)(linecoding.bitrate);
		Buf[1] = (cyg_uint8)(linecoding.bitrate >> 8);
		Buf[2] = (cyg_uint8)(linecoding.bitrate >> 16);
		Buf[3] = (cyg_uint8)(linecoding.bitrate >> 24);
		Buf[4] = linecoding.format;
		Buf[5] = linecoding.paritytype;
		Buf[6] = linecoding.datatype;

		break;

	case SET_CONTROL_LINE_STATE:
		/* Not  needed for this driver */
		break;

	case SEND_BREAK:
		/* Not  needed for this driver */
		break;

	default:
		break;
	}

	return USBD_OK;
}


/**
 * @brief  cdc_DataTx
 *         CDC received data to be send over USB IN endpoint are managed in
 *         this function.
 * @param  Buf: Buffer of data to be sent
 * @param  Len: Number of data to be sent (in bytes)
 * @retval Result of the opeartion: USBD_OK
 */
cyg_uint16 usbInterface::cdc_DataTx (cyg_uint8* Buf, cyg_uint32 Len)
{
	cyg_uint32 i;
	//loop through buffer
	for( i = 0; i < Len; i++ )
	{
		//push data into transfer buffer
		APP_Rx_Buffer[APP_Rx_ptr_in] = Buf[i] ;
		//increase pointer value
		APP_Rx_ptr_in++;
		/* To avoid buffer overflow */
		if(APP_Rx_ptr_in == APP_RX_DATA_SIZE)
		{
			APP_Rx_ptr_in = 0;
		}
	}

	return USBD_OK;
}

cyg_uint32 usbInterface::receive(cyg_uint8 * buff,cyg_uint32 buff_len)
{
	cyg_uint32 rxLen = 0;

	cyg_mutex_lock(&(__instance->mUSBmutex));
	if(cyg_cond_wait(&(__instance->mUSBrxCond)))
	{
		if(__instance->mUSBRXlen)
		{
			memcpy(buff, __instance->mUSBRXbuff, __instance->mUSBRXlen);
			rxLen = __instance->mUSBRXlen;
		}
	}
	cyg_mutex_unlock(&(__instance->mUSBmutex));

	return rxLen;
}

cyg_bool usbInterface::transmit(cyg_uint8 * buff,cyg_uint32 buff_len)
{
	if(mUSBstatus != configured)
		return 0;

	cyg_uint32 frameLen = USBRXBUFF_SIZE;
	cyg_uint8 transmitBuffer[USBRXBUFF_SIZE];

	cHDLCframer::frame(buff, buff_len, transmitBuffer, &frameLen);

	//diag_printf("USB TX: %d\n", frameLen);
	//diag_dump_buf(transmitBuffer, frameLen);

	cyg_uint32 transmitLen = 0;
	cyg_uint32 txLen = 0;
	while(frameLen)
	{
		//transmit packets of 63 bytes;
		if(frameLen > CDC_DATA_MAX_PACKET_SIZE)
			txLen = CDC_DATA_MAX_PACKET_SIZE;
		else
			txLen = frameLen;

		//diag_printf("Send CDC %d\n", txLen);
		if(cdc_DataTx(&transmitBuffer[transmitLen], txLen) != USBD_OK)
			break;

		transmitLen += txLen;
		frameLen -= txLen;
	}


	return transmitLen;
}

cyg_uint16  usbInterface::cdc_rxData(cyg_uint8* buff, cyg_uint32 len)
{
	if(!__instance)
		return USBD_FAIL;

	if(!(len && (len < USBRXBUFF_SIZE)))
		return USBD_FAIL;

	static cHDLCframer frame(__instance->mUSBRXbuff, USBRXBUFF_SIZE);
	//diag_printf("cdc rx %d\n", len);

	cyg_mutex_lock(&(__instance->mUSBmutex));
	__instance->mUSBRXlen = 0;

	for(cyg_uint32 idx = 0; idx < len; idx++)
	{
		cyg_uint32 rxLen = frame.pack(buff[idx]);
		if(rxLen)
		{
			__instance->mUSBRXlen = rxLen;
			cyg_cond_signal(&(__instance->mUSBrxCond));
		}
	}
	cyg_mutex_unlock(&(__instance->mUSBmutex));

	return USBD_OK;
}

void usbInterface::setUSBstate(usbInterface::eUSBstate state)
{
	//diag_printf("Setting state %d\n", state);
	switch(state)
	{

	case reset:
	{
		diag_printf("USB: Reset\n");
		mBannerFlag = false;
	}
	break;
	case configured:
	{
		diag_printf("USB: Configured.\n");
	}
	break;
	case suspended:
	{
		diag_printf("USB: Suspend Mode.\n");
		mBannerFlag = false;
	}
	break;
	case resumed:
	{
		diag_printf("USB: Resumed\n");
	}
	break;

	default:
		break;
	}
	mUSBstatus = state;
}


usbInterface::~usbInterface()
{
}

