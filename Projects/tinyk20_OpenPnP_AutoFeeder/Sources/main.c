/* ###################################################################
**     Filename    : main.c
**     Project     : tinyk20_PNP_V1
**     Processor   : MK20DN128VFT5
**     Version     : Driver 01.01
**     Compiler    : GNU C Compiler
**     Date/Time   : 2018-04-16, 10:39, # CodeGen: 0
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file main.c
** @version 01.01
** @brief
**         Main module.
**         This module contains user's application code.
*/         
/*!
**  @addtogroup main_module main module documentation
**  @{
*/         
/* MODULE main */


/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "LED1.h"
#include "LEDpin1.h"
#include "BitIoLdd1.h"
#include "MCUC1.h"
#include "SW_FWD.h"
#include "BitIoLdd2.h"
#include "SW_REV.h"
#include "BitIoLdd3.h"
#include "SW_PEELER.h"
#include "BitIoLdd4.h"
#include "ENC1.h"
#include "BitIoLdd5.h"
#include "PWMA.h"
#include "PwmLdd1.h"
#include "TU1.h"
#include "DIRA.h"
#include "BitIoLdd6.h"
#include "DIRB.h"
#include "BitIoLdd7.h"
#include "PWMB.h"
#include "PwmLdd2.h"
#include "HMODE.h"
#include "BitIoLdd8.h"
#include "FRTOS1.h"
#include "RTOSCNTRLDD1.h"
#include "UTIL1.h"
#include "WAIT1.h"
#include "CLS1.h"
#include "XF1.h"
#include "CS1.h"
#include "LED2.h"
#include "LEDpin2.h"
#include "BitIoLdd9.h"
#include "AS1.h"
#include "ASerialLdd1.h"
#include "HF1.h"
#include "TU2.h"
#include "KIN1.h"
#include "RTT1.h"
#include "AS2.h"
#include "ASerialLdd2.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
/* User includes (#include below this line is not maintained by Processor Expert) */
#include "application.h"
#include "platform.h"


/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
  /* Write your local variable definition here */

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/

  /* Write your code here */
  PL_Init();	// Comm and Enc Tasks
  APP_Init();
  APP_Run();	// APP Task

  /*** Don't write any code past this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.5 [05.21]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
