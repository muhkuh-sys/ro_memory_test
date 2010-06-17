
#include <string.h>

#include "i2c_hifsta.h"
#include "netx_io_areas.h"
#include "systime.h"
#include "uprintf.h"
#include "uart.h"


static const UART_CONFIGURATION_T tUartCfg_nxhx10_etm =
{
	.uc_rx_mmio = 20U,
	.uc_tx_mmio = 21U,
	.uc_rts_mmio = 0xffU,
	.uc_cts_mmio = 0xffU,
	.us_baud_div = UART_BAUDRATE_DIV(UART_BAUDRATE_115200)
};

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
	i2c_hifsta_init();
	uart_init(0, &tUartCfg_nxhx10_etm);

	uprintf("*** hallo! ***\n");

	uiTestCnt = 0;
	while(1)
	{
		uprintf("test run %d\n", uiTestCnt);
		if( (uiTestCnt&1)==0 )
		{
			/* Switch the leds off. */
			i2c_hifsta_rdy_off_run_off();
		}
		else
		{
			/* Switch the green led on. */
			i2c_hifsta_rdy_off_run_on();
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

	tBlinkiHandle.ulMask     = 0x00000055;
	tBlinkiHandle.ulState    = 0x00000150;
	tBlinkiHandle.ulTimer    = 0;
	tBlinkiHandle.uiPosition = 0;
	while(1)
	{
		blinki(&tBlinkiHandle);
	}
}


