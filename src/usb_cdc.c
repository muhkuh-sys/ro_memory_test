

#include "netx_io_areas.h"

#include "usb_cdc.h"


typedef union
{
	unsigned char auc[64];
	unsigned long aul[16];
} USB_CONFIGURATION_T;

static const USB_CONFIGURATION_T tUsbConfig =
{
	.auc =
	{
		/* device descriptor */
		0x39, 0x19,             /* vendor id */
		0x0c, 0x00,             /* product id */
		0x02, 0x00,             /* device release number */
		/* config descriptor */
		0x00, 0x00,             /* conf. characteristics, max. power consumption */

		/* manufacturer string  */
		34, 0x03,
		'H', 'i', 'l', 's', 'c', 'h', 'e', 'r',
		' ', 'G', 'm', 'b', 'H', ' ', ' ', ' ',

		/* device string */
		34, 0x03,
		'H', 'B', 'o', 'o', 't', ' ', 'm', 'o',
		'n', 'i', 't', 'o', 'r', ' ', ' ', ' ',

		/* S/N string */
		34, 0x03,
		'0', '0', '0', '0', '0', '0', '0', '0',
		'0', '0', '0', '0', '0', '0', '0', '2'
	}
};


static void enum_write_config_data(void)
{
	const unsigned long *pulCnt;
	const unsigned long *pulEnd;
	unsigned long *pulDst;


	/* Get a pointer to the start and end of the enumeration data. */
	pulCnt = tUsbConfig.aul;
	pulEnd = tUsbConfig.aul + (sizeof(USB_CONFIGURATION_T)/sizeof(unsigned long));

	/* Get a pointer to the enumeration ram. */
	pulDst = (unsigned long*)HOSTADDR(usb_dev_enum_ram);

	/* Copy the data. */
	while( pulCnt<pulEnd )
	{
		*(pulDst++) = *(pulCnt++);
	}
}



int usbcdc_init(void)
{
	HOSTDEF(ptUsbDevCtrlArea);
	HOSTDEF(ptUsbDevFifoCtrlArea);
	unsigned long ulValue;


	/* Disable the core before changing the configuration. */
	ptUsbDevCtrlArea->ulUsb_dev_cfg = HOSTMSK(usb_dev_cfg_usb_dev_reset);
	ptUsbDevCtrlArea->ulUsb_dev_cfg = 0;

	/* Set the UART FIFOs to streaming mode. */
	ulValue  = 0 << HOSTSRT(usb_dev_fifo_ctrl_conf_mode_interrupt);
	ulValue |= 0 << HOSTSRT(usb_dev_fifo_ctrl_conf_mode_uart_rx);
	ulValue |= 0 << HOSTSRT(usb_dev_fifo_ctrl_conf_mode_uart_tx);
	ulValue |= 0 << HOSTSRT(usb_dev_fifo_ctrl_conf_mode_jtag_rx);
	ulValue |= 0 << HOSTSRT(usb_dev_fifo_ctrl_conf_mode_jtag_tx);
	ptUsbDevFifoCtrlArea->ulUsb_dev_fifo_ctrl_conf = ulValue;

	enum_write_config_data();

	ulValue  = HOSTMSK(usb_dev_cfg_usb_core_enable);
	ptUsbDevCtrlArea->ulUsb_dev_cfg = ulValue;

	/* Clear all IRQs. */
	ptUsbDevCtrlArea->ulUsb_dev_irq_raw = 0xffffffffU;

	return 0;
}



void usbcdc_put(unsigned int uiData)
{
	HOSTDEF(ptUsbDevFifoCtrlArea);
	HOSTDEF(ptUsbDevFifoArea);
	unsigned long ulValue;


	/* Wait for space in the TX FIFO. */
	do
	{
		ulValue  = ptUsbDevFifoCtrlArea->ulUsb_dev_fifo_ctrl_uart_ep_tx_stat;
		ulValue &= HOSTMSK(usb_dev_fifo_ctrl_uart_ep_tx_stat_full);
	} while( ulValue!=0 );

	/* Write the char to the FIFO. */
	ptUsbDevFifoArea->ulUsb_dev_uart_tx_data = uiData;
}



void usbcdc_flush(void)
{
	HOSTDEF(ptUsbDevFifoCtrlArea);
	unsigned long ulValue;


	/* Wait until the TX FIFO is empty. */
	do
	{
		ulValue  = ptUsbDevFifoCtrlArea->ulUsb_dev_fifo_ctrl_uart_ep_tx_stat;
		ulValue &= HOSTMSK(usb_dev_fifo_ctrl_uart_ep_tx_stat_empty);
	} while( ulValue==0 );
}



unsigned char usbcdc_get(void)
{
	HOSTDEF(ptUsbDevFifoCtrlArea);
	HOSTDEF(ptUsbDevFifoArea);
	unsigned long ulValue;


	/* Wait for one char. */
	do
	{
		ulValue  = ptUsbDevFifoCtrlArea->ulUsb_dev_fifo_ctrl_uart_ep_rx_stat;
		ulValue &= HOSTMSK(usb_dev_fifo_ctrl_uart_ep_rx_stat_empty);
	} while( ulValue!=0 );

	/* Get the char. */
	ulValue  = ptUsbDevFifoArea->ulUsb_dev_uart_rx_data;
	ulValue &= 0xff;
	return (unsigned char)ulValue;
}



unsigned int usbcdc_peek(void)
{
	HOSTDEF(ptUsbDevFifoCtrlArea);
	unsigned long ulValue;


	ulValue  = ptUsbDevFifoCtrlArea->ulUsb_dev_fifo_ctrl_uart_ep_rx_stat;
	ulValue &= HOSTMSK(usb_dev_fifo_ctrl_uart_ep_rx_stat_fill_level);

	return (unsigned int)ulValue;
}


void usbcdc_close(void)
{
	HOSTDEF(ptUsbDevCtrlArea);


	/* Disable the core. */
	ptUsbDevCtrlArea->ulUsb_dev_cfg = HOSTMSK(usb_dev_cfg_usb_dev_reset);
	ptUsbDevCtrlArea->ulUsb_dev_cfg = 0;

	/* Clear all IRQs. */
	ptUsbDevCtrlArea->ulUsb_dev_irq_raw = 0xffffffffU;
}


