
#include <string.h>

#include "rdy_run.h"
#include "netx_io_areas.h"
#include "serial_vectors.h"
#include "systime.h"
#include "uprintf.h"
#include "uart.h"



#if ASIC_TYP==10
/* NXHX10-ETM */
static const UART_CONFIGURATION_T tUartCfg =
{
	.uc_rx_mmio = 20U,
	.uc_tx_mmio = 21U,
	.uc_rts_mmio = 0xffU,
	.uc_cts_mmio = 0xffU,
	.us_baud_div = UART_BAUDRATE_DIV(UART_BAUDRATE_115200)
};
#elif ASIC_TYP==56
/* NXHX51-ETM */
static const UART_CONFIGURATION_T tUartCfg =
{
	.uc_rx_mmio = 34U,
	.uc_tx_mmio = 35U,
	.uc_rts_mmio = 0xffU,
	.uc_cts_mmio = 0xffU,
	.us_baud_div = UART_BAUDRATE_DIV(UART_BAUDRATE_115200)
};
#elif ASIC_TYP==50
/* NXHX50-ETM */
static const UART_CONFIGURATION_T tUartCfg =
{
	.uc_rx_mmio = 34U,
	.uc_tx_mmio = 35U,
	.uc_rts_mmio = 0xffU,
	.uc_cts_mmio = 0xffU,
	.us_baud_div = UART_BAUDRATE_DIV(UART_BAUDRATE_115200)
};
#elif ASIC_TYP==100 || ASIC_TYP==500
static const UART_CONFIGURATION_T tUartCfg =
{
	.us_baud_div = UART_BAUDRATE_DIV(UART_BAUDRATE_115200)
};
#endif


#define IO_UART_UNIT 0

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


	systime_init();
	uart_init(0, &tUartCfg);

	/* Set the serial vectors. */
	memcpy(&tSerialVectors, &tSerialVectors_Uart, sizeof(SERIAL_COMM_UI_FN_T));

	uprintf("*** hallo! ***\n");

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


