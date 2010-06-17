/*---------------------------------------------------------------------------
  Author : Christoph Thelen

           Hilscher GmbH, Copyright (c) 2006, All Rights Reserved

           Redistribution or unauthorized use without expressed written 
           agreement from the Hilscher GmbH is forbidden
---------------------------------------------------------------------------*/


#ifndef __I2C_HIFSTA_H__
#define __I2C_HIFSTA_H__


#if ASIC_TYP==500 || ASIC_TYP==100 || ASIC_TYP==50
//-------------------------------------
// some defines for the mysterious hif regs
// taken from the "netX Program Reference Guide Rev0.3", page 16

#define MSK_sta_netx_rdy			0x00000001U
#define SRT_sta_netx_rdy			0
#define MSK_sta_netx_run			0x00000002U
#define SRT_sta_netx_run			1
#define MSK_sta_netx_netx_state			0x0000000cU
#define SRT_sta_netx_netx_state			2
#define MSK_sta_netx_host_state			0x000000f0U
#define SRT_sta_netx_host_state			4
#define MSK_sta_netx_netx_sta_code		0x0000ff00U
#define SRT_sta_netx_netx_sta_code		8
#define MSK_sta_netx_rdy_in			0x00010000U
#define SRT_sta_netx_rdy_in			16
#define MSK_sta_netx_run_in			0x00020000U
#define SRT_sta_netx_run_in			17
#define MSK_sta_netx_rdy_pol			0x00040000U
#define SRT_sta_netx_rdy_pol			18
#define MSK_sta_netx_run_pol			0x00080000U
#define SRT_sta_netx_run_pol			19
#define MSK_sta_netx_rdy_drv			0x01000000U
#define SRT_sta_netx_rdy_drv			24
#define MSK_sta_netx_run_drv			0x02000000U
#define SRT_sta_netx_run_drv			25

#endif

/*-----------------------------------*/

typedef struct
{
	unsigned long ulMask;
	unsigned long ulState;
	unsigned long ulTimer;
	unsigned int uiPosition;
} BLINKI_HANDLE_T;

/*-----------------------------------*/

void i2c_hifsta_init(void);

/* led routines */
void i2c_hifsta_rdy_on_run_off(void);
void i2c_hifsta_rdy_off_run_on(void);
void i2c_hifsta_rdy_off_run_off(void);

/* blinki routines */
void blinki(BLINKI_HANDLE_T *ptHandle);

/*-----------------------------------*/

#endif	/* __I2C_HIFSTA_H__ */

