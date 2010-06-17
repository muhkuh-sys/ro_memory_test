/*---------------------------------------------------------------------------
  Author : Christoph Thelen

           Hilscher GmbH, Copyright (c) 2006, All Rights Reserved

           Redistribution or unauthorized use without expressed written 
           agreement from the Hilscher GmbH is forbidden
---------------------------------------------------------------------------*/

#include "i2c_hifsta.h"

#include "netx_io_areas.h"
#include "systime.h"


/*-------------------------------------*/

/* defines for the pio pin values passed to set pins
 * the logic is inverted as only the low level drives the output
 */
typedef enum
{
	PIO_I2C_LO = 1,
	PIO_I2C_HI = 0
} tI2cOut;

/*
 * static vars
 */

/* data must be changed after the clock, this var saves the last value */
static unsigned long ulLastDataPart;

/*-------------------------------------*/

#if ASIC_TYP==10 || ASIC_TYP==50
static void i2c_hifsta_clock(void)
{
}
#elif ASIC_TYP==100 || ASIC_TYP==500
static void i2c_hifsta_clock(void)
{
}
#endif


#if ASIC_TYP==10
void i2c_hifsta_rdy_on_run_off(void)
{
	unsigned long ulValue;


	ulValue = ptAsicCtrlArea->ulRdy_run_cfg;

	/* sclk (rdy) lo */
	ulValue |= MSK_NX10_rdy_run_cfg_RDY_DRV;
	ptAsicCtrlArea->ulRdy_run_cfg = ulValue;
	/* sdata (run) lo */
	ulValue |= MSK_NX10_rdy_run_cfg_RUN_DRV;
	ptAsicCtrlArea->ulRdy_run_cfg = ulValue;
	i2c_hifsta_clock();

	/* sdata (run) hi */
	ulValue &= ~MSK_NX10_rdy_run_cfg_RUN_DRV;
	ptAsicCtrlArea->ulRdy_run_cfg = ulValue;
	i2c_hifsta_clock();
}


void i2c_hifsta_rdy_off_run_on(void)
{
	unsigned long ulValue;


	ulValue = ptAsicCtrlArea->ulRdy_run_cfg;

	/* sclk (rdy) lo */
	ulValue |= MSK_NX10_rdy_run_cfg_RDY_DRV;
	ptAsicCtrlArea->ulRdy_run_cfg = ulValue;
	/* sdata (run) lo */
	ulValue |= MSK_NX10_rdy_run_cfg_RUN_DRV;
	ptAsicCtrlArea->ulRdy_run_cfg = ulValue;
	i2c_hifsta_clock();

	/* sclk (rdy) hi */
	ulValue &= ~MSK_NX10_rdy_run_cfg_RDY_DRV;
	ptAsicCtrlArea->ulRdy_run_cfg = ulValue;
	i2c_hifsta_clock();
}


void i2c_hifsta_rdy_off_run_off(void)
{
	unsigned long ulValue;


	ulValue = ptAsicCtrlArea->ulRdy_run_cfg;

	/* sclk (rdy) hi */
	ulValue &= ~MSK_NX10_rdy_run_cfg_RDY_DRV;
	ptAsicCtrlArea->ulRdy_run_cfg = ulValue;
	/* sdata (run) lo */
	ulValue &= ~MSK_NX10_rdy_run_cfg_RUN_DRV;
	ptAsicCtrlArea->ulRdy_run_cfg = ulValue;
	i2c_hifsta_clock();
}
#elif ASIC_TYP==50
void i2c_hifsta_rdy_on_run_off(void)
{
	unsigned long ulValue;


	ulValue = ptNetxControlledGlobalRegisterBlock1Area->ulSta_netx;

	// sclk (rdy) lo
	ulValue &= ~MSK_sta_netx_rdy;
	ulValue |= MSK_sta_netx_run_drv | MSK_sta_netx_rdy_drv;
	ptNetxControlledGlobalRegisterBlock1Area->ulSta_netx = ulValue;
	i2c_hifsta_clock();
	i2c_hifsta_clock();

	// sdata (run) lo
	ulValue |= MSK_sta_netx_run;
	ptNetxControlledGlobalRegisterBlock1Area->ulSta_netx = ulValue;
	i2c_hifsta_clock();
}


void i2c_hifsta_rdy_off_run_on(void)
{
	unsigned long ulValue;


	ulValue = ptNetxControlledGlobalRegisterBlock1Area->ulSta_netx;

	// sclk (rdy) lo
	ulValue &= ~MSK_sta_netx_rdy;
	ulValue |= MSK_sta_netx_run_drv | MSK_sta_netx_rdy_drv;
	ptNetxControlledGlobalRegisterBlock1Area->ulSta_netx = ulValue;
	i2c_hifsta_clock();

	// sdata (run) lo
	ulValue &= ~MSK_sta_netx_run;
	ptNetxControlledGlobalRegisterBlock1Area->ulSta_netx = ulValue;
	i2c_hifsta_clock();
	
	ulValue |= MSK_sta_netx_rdy;
	ptNetxControlledGlobalRegisterBlock1Area->ulSta_netx = ulValue;
	i2c_hifsta_clock();
}


void i2c_hifsta_rdy_off_run_off(void)
{
	unsigned long ulValue;


	ulValue = ptNetxControlledGlobalRegisterBlock1Area->ulSta_netx;

	// sclk (rdy) hi
	ulValue &= ~MSK_sta_netx_rdy;
	ptNetxControlledGlobalRegisterBlock1Area->ulSta_netx = ulValue;
	i2c_hifsta_clock();
	i2c_hifsta_clock();

	// sdata (run) lo
	ulValue &= ~MSK_sta_netx_run;
	ptNetxControlledGlobalRegisterBlock1Area->ulSta_netx = ulValue;
	i2c_hifsta_clock();
}
#elif ASIC_TYP==100 || ASIC_TYP==500
void i2c_hifsta_rdy_on_run_off(void)
{
	unsigned long ulValue;


	ulValue = ptNetxControlledGlobalRegisterBlock1Area->ulSta_netx;

	// sclk (rdy) lo
	ulValue &= ~MSK_sta_netx_rdy;
	ulValue |= MSK_sta_netx_run_drv | MSK_sta_netx_rdy_drv;
	ptNetxControlledGlobalRegisterBlock1Area->ulSta_netx = ulValue;
	i2c_hifsta_clock();
	i2c_hifsta_clock();

	// sdata (run) lo
	ulValue |= MSK_sta_netx_run;
	ptNetxControlledGlobalRegisterBlock1Area->ulSta_netx = ulValue;
	i2c_hifsta_clock();
}


void i2c_hifsta_rdy_off_run_on(void)
{
	unsigned long ulValue;


	ulValue = ptNetxControlledGlobalRegisterBlock1Area->ulSta_netx;

	// sclk (rdy) lo
	ulValue &= ~MSK_sta_netx_rdy;
	ulValue |= MSK_sta_netx_run_drv | MSK_sta_netx_rdy_drv;
	ptNetxControlledGlobalRegisterBlock1Area->ulSta_netx = ulValue;
	i2c_hifsta_clock();

	// sdata (run) lo
	ulValue &= ~MSK_sta_netx_run;
	ptNetxControlledGlobalRegisterBlock1Area->ulSta_netx = ulValue;
	i2c_hifsta_clock();
	
	ulValue |= MSK_sta_netx_rdy;
	ptNetxControlledGlobalRegisterBlock1Area->ulSta_netx = ulValue;
	i2c_hifsta_clock();
}


void i2c_hifsta_rdy_off_run_off(void)
{
	unsigned long ulValue;


	ulValue = ptNetxControlledGlobalRegisterBlock1Area->ulSta_netx;

	// sclk (rdy) hi
	ulValue &= ~MSK_sta_netx_rdy;
	ptNetxControlledGlobalRegisterBlock1Area->ulSta_netx = ulValue;
	i2c_hifsta_clock();
	i2c_hifsta_clock();

	// sdata (run) lo
	ulValue &= ~MSK_sta_netx_run;
	ptNetxControlledGlobalRegisterBlock1Area->ulSta_netx = ulValue;
	i2c_hifsta_clock();
}
#endif

/*-------------------------------------*/


typedef void (*pfn_led)(void);

static const pfn_led apfn_led_state[4] =
{
	i2c_hifsta_rdy_off_run_off,		/* 0: off */
	i2c_hifsta_rdy_off_run_off,		/* 1: off */
	i2c_hifsta_rdy_on_run_off,		/* 2: green */
	i2c_hifsta_rdy_off_run_on		/* 3: yellow */
};

void blinki(BLINKI_HANDLE_T *ptHandle)
{
	unsigned int uiPosition;
	unsigned int uiMaskBit;
	unsigned int uiStateBit;
	unsigned int uiBlinkiBit;
	int f_doRestart;


	if( systime_elapsed(ptHandle->ulTimer, 125)!=0 )
	{
		do
		{
			/* the timer is not running -> show the next blinki state */
			uiPosition = ptHandle->uiPosition;
			uiMaskBit = (ptHandle->ulMask>>uiPosition)&1U;
			uiStateBit = (ptHandle->ulState>>uiPosition)&1U;

			/* reset the counter if the blinki state is a masked 1 */
			/* NOTE: do not do this for the first bit (uiHandle==0), this would be an endless loop! */
			f_doRestart = ( uiPosition!=0U && uiMaskBit==0U && uiStateBit!=0U );
			if( f_doRestart==0 )
			{
				/* next position */
				++uiPosition;
				uiPosition &= 0x1fU;
			}
			else
			{
				/* restart blinki sequence */
				uiPosition = 0U;
			}
			ptHandle->uiPosition = uiPosition;
		} while( f_doRestart!=0 );

		/* show the led state */
		uiBlinkiBit = (uiMaskBit<<1) | uiStateBit;
		apfn_led_state[uiBlinkiBit]();

		/* update handle */
		ptHandle->ulTimer = systime_get_ms();
	}
}


void i2c_hifsta_init(void)
{
	/* initial mode for data is 'input' */
	ulLastDataPart = 0;
}

