/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_ftm.h"


/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define FTM_MOTOR FTM0
#define FTM_CHANNEL_DC_MOTOR kFTM_Chnl_0
#define FTM_CHANNEL_SERVO_MOTOR kFTM_Chnl_3

void setupPWM_Motor(){
	ftm_config_t ftmInfo;
	ftm_chnl_pwm_signal_param_t ftmParam;
	ftm_pwm_level_select_t pwmLevel = kFTM_HighTrue;
	ftmParam.chnlNumber = FTM_CHANNEL_DC_MOTOR;
	ftmParam.level = pwmLevel;
	ftmParam.dutyCyclePercent = 7;
	ftmParam.firstEdgeDelayPercent = 0U;
	ftmParam.enableComplementary = false;
	ftmParam.enableDeadtime = false;
	FTM_GetDefaultConfig(&ftmInfo);
	ftmInfo.prescale = kFTM_Prescale_Divide_128;
	FTM_Init(FTM_MOTOR, &ftmInfo);
	FTM_SetupPwm(FTM_MOTOR, &ftmParam, 1U, kFTM_EdgeAlignedPwm, 50U, CLOCK_GetFreq(
	kCLOCK_BusClk));
	FTM_StartTimer(FTM_MOTOR, kFTM_SystemClock);
}
void setupPWM_Servo(){
	ftm_config_t ftmInfo;
	ftm_chnl_pwm_signal_param_t ftmParam;
	ftm_pwm_level_select_t pwmLevel = kFTM_HighTrue;
	ftmParam.chnlNumber = FTM_CHANNEL_SERVO_MOTOR;
	ftmParam.level = pwmLevel;
	ftmParam.dutyCyclePercent = 7;
	ftmParam.firstEdgeDelayPercent = 0U;
	ftmParam.enableComplementary = false;
	ftmParam.enableDeadtime = false;
	FTM_GetDefaultConfig(&ftmInfo);
	ftmInfo.prescale = kFTM_Prescale_Divide_128;
	FTM_Init(FTM_MOTOR, &ftmInfo);
	FTM_SetupPwm(FTM_MOTOR, &ftmParam, 1U, kFTM_EdgeAlignedPwm, 50U, CLOCK_GetFreq(
	kCLOCK_BusClk));
	FTM_StartTimer(FTM_MOTOR, kFTM_SystemClock);
}
void updatePWM_dutyCycle(ftm_chnl_t channel, float dutyCycle){
	uint32_t cnv, cnvFirstEdge = 0, mod;
	/* The CHANNEL_COUNT macro returns -1 if it cannot match the FTM instance */
	assert(-1 != FSL_FEATURE_FTM_CHANNEL_COUNTn(FTM_MOTOR));
	mod = FTM_MOTOR->MOD;
	if (dutyCycle == 0U){
		/* Signal stays low */
		cnv = 0;
	}
	else{
		cnv = mod * dutyCycle;
		/* For 100% duty cycle */
		if (cnv >= mod){
			cnv = mod + 1U;
			}
	}
	FTM_MOTOR->CONTROLS[channel].CnV = cnv;
}

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
	uint8_t ch;
	    int servoinput;
	    int dcinput;
	    float dutyCycle_DC, dutyCycle_Servo;

	    BOARD_InitBootPins();
	    BOARD_InitBootClocks();

	    setupPWM_Motor();
	    setupPWM_Servo();
	    /******* Delay *******/
	    for(volatile int i = 0U; i < 1000000; i++)
	    __asm("NOP");

	    updatePWM_dutyCycle(FTM_CHANNEL_DC_MOTOR, 0.0615);
	    FTM_SetSoftwareTrigger(FTM_MOTOR, true);

	    printf("DC Input (-100 to 100)");
	    scanf("%d", &dcinput);
	    dutyCycle_DC = dcinput * 0.025f/100.0f + 0.0615;

	    printf("Servo Input (-90 to 90)");
	    scanf("%d", &servoinput);
	    dutyCycle_Servo = (((servoinput + 90)/180.0f) * 0.05f) + 0.05;

	    updatePWM_dutyCycle(FTM_CHANNEL_DC_MOTOR, dutyCycle_DC);
	    updatePWM_dutyCycle(FTM_CHANNEL_SERVO_MOTOR, dutyCycle_Servo);

	    FTM_SetSoftwareTrigger(FTM_MOTOR, true);

	    while (1)
	    {}
}
