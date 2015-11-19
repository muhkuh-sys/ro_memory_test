
#define CFG_USE_UART 1


#include <string.h>

#include "rdy_run.h"
#include "netx_io_areas.h"
#include "serial_vectors.h"
#include "systime.h"
#include "uprintf.h"
#include "version.h"


#if CFG_USE_UART!=0
#       include "uart.h"
#else
#       include "usb_cdc.h"
#endif


typedef enum ENUM_CHIP_SUBTYP
{
	CHIP_SUBTYP_NETX50              = 0,
	CHIP_SUBTYP_NETX51              = 1,
	CHIP_SUBTYP_NETX52              = 2
} CHIP_SUBTYP_T;


#if CFG_USE_UART!=0
#       if ASIC_TYP==10
		/* NXHX10-ETM */
		static const UART_CONFIGURATION_T tUartCfg =
		{
			.uc_rx_mmio = 20U,
			.uc_tx_mmio = 21U,
			.uc_rts_mmio = 0xffU,
			.uc_cts_mmio = 0xffU,
			.us_baud_div = UART_BAUDRATE_DIV(UART_BAUDRATE_115200)
		};
#       elif ASIC_TYP==56
		/* NXHX51-ETM */
		static const UART_CONFIGURATION_T tUartCfg_netx51 =
		{
			.uc_rx_mmio = 34U,
			.uc_tx_mmio = 35U,
			.uc_rts_mmio = 0xffU,
			.uc_cts_mmio = 0xffU,
			.us_baud_div = UART_BAUDRATE_DIV(UART_BAUDRATE_115200)
		};

		static const UART_CONFIGURATION_T tUartCfg_netx52 =
		{
			.uc_rx_mmio = 20U,
			.uc_tx_mmio = 21U,
			.uc_rts_mmio = 0xffU,
			.uc_cts_mmio = 0xffU,
			.us_baud_div = UART_BAUDRATE_DIV(UART_BAUDRATE_115200)
		};
#       elif ASIC_TYP==50
		/* NXHX50-ETM */
		static const UART_CONFIGURATION_T tUartCfg =
		{
			.uc_rx_mmio = 34U,
			.uc_tx_mmio = 35U,
			.uc_rts_mmio = 0xffU,
			.uc_cts_mmio = 0xffU,
			.us_baud_div = UART_BAUDRATE_DIV(UART_BAUDRATE_115200)
		};
#       elif ASIC_TYP==100 || ASIC_TYP==500
		static const UART_CONFIGURATION_T tUartCfg =
		{
			.us_baud_div = UART_BAUDRATE_DIV(UART_BAUDRATE_115200)
		};
#       endif



#       define IO_UART_UNIT 0

	static unsigned char io_uart_get(void)
	{
		return (unsigned char)uart_get(IO_UART_UNIT);
	}


	static void io_uart_put(unsigned int uiChar)
	{
		uart_put(IO_UART_UNIT, (unsigned char)uiChar);
	}


	static unsigned int io_uart_peek(void)
	{
		return uart_peek(IO_UART_UNIT);
	}


	static void io_uart_flush(void)
	{
		uart_flush(IO_UART_UNIT);
	}


	static const SERIAL_COMM_UI_FN_T tSerialVectors_Uart =
	{
		.fn =
		{
			.fnGet = io_uart_get,
			.fnPut = io_uart_put,
			.fnPeek = io_uart_peek,
			.fnFlush = io_uart_flush
		}
	};
#else  /* CFG_USE_UART!=0 */
	static const SERIAL_COMM_UI_FN_T tSerialVectors_Usb =
	{
		.fn =
		{
			.fnGet = usbcdc_get,
			.fnPut = usbcdc_put,
			.fnPeek = usbcdc_peek,
			.fnFlush = usbcdc_flush
		}
	};
#endif

SERIAL_COMM_UI_FN_T tSerialVectors;


extern unsigned long _binary_test_bin_end[];
extern unsigned long _binary_test_bin_start[];

void test_main(void) __attribute__ ((noreturn));
void test_main(void)
{
	BLINKI_HANDLE_T tBlinkiHandle;
	unsigned long ulPrn_inc;
	unsigned long ulPrn_reg;
	unsigned long *pulCnt;
	unsigned long *pulEnd;
	unsigned long ulRead;
	unsigned int uiTestCnt;
#if ASIC_TYP==56
	HOSTDEF(ptAsicCtrlArea);
	HOSTDEF(ptMmioCtrlArea);
	unsigned long ulValue;
	unsigned long ulChipSubType;
	CHIP_SUBTYP_T tChipSubTyp;
#endif


	systime_init();
#if CFG_USE_UART!=0
#       if ASIC_TYP==56
		/* Get the chip subtype from mem_a18 and mem_a19:
		 *  18 19
		 *   0  0  netX50
		 *   1  0  netX51
		 *   0  1  netX52
		 *   1  1  reserved
		 */
		ulValue = ptAsicCtrlArea->ulSample_at_nres;
		ulChipSubType  = (ulValue&HOSTMSK(sample_at_nres_sar_mem_a18))>> HOSTSRT(sample_at_nres_sar_mem_a18);
		ulChipSubType |= (ulValue&HOSTMSK(sample_at_nres_sar_mem_a19))>>(HOSTSRT(sample_at_nres_sar_mem_a19)-1);
		tChipSubTyp = (CHIP_SUBTYP_T)ulChipSubType;
		
		/* netX51 and netX52 have different default UART pins. */
		if( tChipSubTyp==CHIP_SUBTYP_NETX52 )
		{
			uart_init(IO_UART_UNIT, &tUartCfg_netx52);
		}
		else
		{
			uart_init(IO_UART_UNIT, &tUartCfg_netx51);
		}
#       else
		uart_init(0, &tUartCfg);
#       endif
	
	/* Set the serial vectors. */
	memcpy(&tSerialVectors, &tSerialVectors_Uart, sizeof(SERIAL_COMM_UI_FN_T));
#else
	usbcdc_init();

	/* Set the serial vectors. */
	memcpy(&tSerialVectors, &tSerialVectors_Usb, sizeof(SERIAL_COMM_UI_FN_T));
#endif


	uprintf("*** Memory RO test by cthelen@hilscher.com ***\n");
	uprintf("V" VERSION_ALL " " VERSION_VCS "\n\n");

	uprintf("This test checks a pre-defined pattern in the following area:\n");
	uprintf(" 0x%08x - 0x%08x\n", (unsigned long)_binary_test_bin_start, (unsigned long)_binary_test_bin_end);

	uiTestCnt = 0;
	while(1)
	{
		uprintf("test run %d\n", uiTestCnt);
		if( (uiTestCnt&1)==0 )
		{
			/* Switch the leds off. */
			rdy_run_setLEDs(RDYRUN_OFF);
		}
		else
		{
			/* Switch the green led on. */
			rdy_run_setLEDs(RDYRUN_GREEN);
		}

		/* Loop over the complete test data. */
		pulCnt = _binary_test_bin_start;
		pulEnd = _binary_test_bin_end;
		ulPrn_reg = *(pulCnt++);
		ulPrn_inc = *(pulCnt++);
		while( pulCnt<pulEnd )
		{
			ulPrn_reg += ulPrn_inc;
			ulRead = *(pulCnt++);
			if( ulPrn_reg!=ulRead )
			{
				break;
			}
		}

		if( pulCnt<pulEnd )
		{
			uprintf("!!! Error !!!\n");
			uprintf("Expected: 0x%08x\n", ulPrn_reg);
			uprintf("Read:     0x%08x\n", ulRead);
			break;
		}

		++uiTestCnt;
	}


	uprintf("Stop!\n");

	rdy_run_blinki_init(&tBlinkiHandle, 0x00000055, 0x00000150);
	while(1)
	{
		rdy_run_blinki(&tBlinkiHandle);
	}
}


