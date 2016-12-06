
#define CFG_USE_UART 1


#include <string.h>

#include "rdy_run.h"
#include "netx_io_areas.h"
#include "serial_vectors.h"
#include "systime.h"
#include "uprintf.h"
#include "version.h"

#if ASIC_TYP==ASIC_TYP_NETX90_MPW
#       include "netx90/comled.h"
#endif


#if CFG_USE_UART!=0
#       include "uart_standalone.h"
#else
#       include "usb_cdc.h"
#endif


#if CFG_USE_UART!=0
UART_STANDALONE_DEFINE_GLOBALS
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
#if ASIC_TYP==ASIC_TYP_NETX90_MPW
	COMLED_BLINKI_HANDLE_T tBlinkiHandle;
#else
	BLINKI_HANDLE_T tBlinkiHandle;
#endif
	unsigned long ulPrn_inc;
	unsigned long ulPrn_reg;
	unsigned long *pulCnt;
	unsigned long *pulEnd;
	unsigned long ulRead;
	unsigned int uiTestCnt;


	systime_init();
#if CFG_USE_UART!=0
	uart_standalone_initialize();
#else
	usbcdc_init();

	/* Set the serial vectors. */
	memcpy(&tSerialVectors, &tSerialVectors_Usb, sizeof(SERIAL_COMM_UI_FN_T));
#endif

#if ASIC_TYP==ASIC_TYP_NETX90_MPW
	comled_initialize(0);
	comled_initialize(1);
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
			/* Switch the LEDs off. */
#if ASIC_TYP==ASIC_TYP_NETX90_MPW
			comled_setLEDs(0, COMLED_OFF);
#else
			rdy_run_setLEDs(RDYRUN_OFF);
#endif
		}
		else
		{
			/* Switch the green led on. */
#if ASIC_TYP==ASIC_TYP_NETX90_MPW
			comled_setLEDs(0, COMLED_GREEN);
#else
			rdy_run_setLEDs(RDYRUN_GREEN);
#endif
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

#if ASIC_TYP==ASIC_TYP_NETX90_MPW
	comled_blinki_init(&tBlinkiHandle, 0, 0x00000001, 0x00000004);
	while(1)
	{
		comled_blinki(&tBlinkiHandle);
	};
#else
	rdy_run_blinki_init(&tBlinkiHandle, 0x00000001, 0x00000004);
	while(1)
	{
		rdy_run_blinki(&tBlinkiHandle);
	}
#endif
}


