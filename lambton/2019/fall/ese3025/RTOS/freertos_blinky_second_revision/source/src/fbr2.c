
/*
 * FreeRTOS Blinky Revision 2
 *
 * author-modifier: Takis Zourntos
 * Copyright (C) 2019 emad studio inc.
 *
 * code based on FreeRTOS Blinky NXP starter code (see copyright below)
 *
 */
#include "board.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdint.h>

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/
typedef enum
{
	Red = 0U,
	Green = 1U,
	Blue = 2U
} palette_t;

struct colourStruct
{
	palette_t	 colour;
	portTickType delayTicks;
};
typedef struct colourStruct colour_t;

colour_t	LEDparams[3];

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/* Sets up system hardware */
static void prvSetupHardware(void)
{
	SystemCoreClockUpdate();
	Board_Init();

	/* Initial state for each LED is off */
	Board_LED_Set(Red, false);
	Board_LED_Set(Green, false);
	Board_LED_Set(Blue, false);

}

/* LED Red task/thread */
static void vLEDTaskRed(void *pvParameters)
{
	colour_t *colourCS;
	colourCS = (colour_t *) pvParameters;
	palette_t		col = colourCS[Red].colour;
	portTickType	numticks = colourCS[Red].delayTicks;
	portTickType	xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();

	while (1)
	{
		Board_LED_Set(col, true);
		vTaskDelayUntil(&xLastWakeTime, numticks);
		Board_LED_Set(col, false);
		vTaskDelayUntil(&xLastWakeTime, (numticks << 1));
	}
}

/* LED Green task/thread */
static void vLEDTaskGreen(void *pvParameters)
{
	colour_t *colourCS;
	colourCS = (colour_t *) pvParameters;
	palette_t		col = colourCS[Green].colour;
	portTickType	numticks = colourCS[Green].delayTicks;
	portTickType	xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();

	while (1)
	{
		vTaskDelayUntil(&xLastWakeTime, numticks);
		Board_LED_Set(col, true);
		vTaskDelayUntil(&xLastWakeTime, numticks);
		Board_LED_Set(col, false);
		vTaskDelayUntil(&xLastWakeTime, numticks);
	}
}

/* LED Green task/thread */
static void vLEDTaskBlue(void *pvParameters)
{
	colour_t *colourCS;
	colourCS = (colour_t *) pvParameters;
	palette_t		col = colourCS[Blue].colour;
	portTickType	numticks = colourCS[Blue].delayTicks;
	portTickType	xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();

	while (1)
	{
		vTaskDelayUntil(&xLastWakeTime, (numticks << 1));
		Board_LED_Set(col, true);
		vTaskDelayUntil(&xLastWakeTime, numticks);
		Board_LED_Set(col, false);
	}
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/

int main(void)
{
	/* initialize LED parameters */
	LEDparams[Red].colour = Red;
	LEDparams[Red].delayTicks = configTICK_RATE_HZ;
	LEDparams[Green].colour = Green;
	LEDparams[Green].delayTicks = configTICK_RATE_HZ;
	LEDparams[Blue].colour = Blue;
	LEDparams[Blue].delayTicks = configTICK_RATE_HZ;

	/* initialize hardware */
	prvSetupHardware();

	/* create Red LED toggle thread */
	xTaskCreate(vLEDTaskRed, (signed char *) "vTaskLedRed",
				configMINIMAL_STACK_SIZE, LEDparams, (tskIDLE_PRIORITY + 3UL),
				(xTaskHandle *) NULL);

	/* create Green LED toggle thread */
	xTaskCreate(vLEDTaskGreen, (signed char *) "vTaskLedGreen",
				configMINIMAL_STACK_SIZE, LEDparams, (tskIDLE_PRIORITY + 2UL),
				(xTaskHandle *) NULL);

	/* create Blue LED toggle thread */
	xTaskCreate(vLEDTaskBlue, (signed char *) "vTaskLedBlue",
				configMINIMAL_STACK_SIZE, LEDparams, (tskIDLE_PRIORITY + 1UL),
				(xTaskHandle *) NULL);

	/* Start the scheduler */
	vTaskStartScheduler();

	/* Should never arrive here */
	return 1;
}

/**
 * @}
 */
/*
 * @brief FreeRTOS Blinky example
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2014
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */
