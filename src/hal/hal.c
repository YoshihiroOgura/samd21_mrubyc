/*! @file
  @brief
  Hardware abstraction layer
        for SAMD21
  <pre>
  Copyright (C) 2016-2023 Kyushu Institute of Technology.
  Copyright (C) 2016-2023 Shimane IT Open-Innovation Center.
  This file is distributed under BSD 3-Clause License.
  </pre>
*/

/***** Feature test switches ************************************************/
/***** System headers *******************************************************/
/***** Local headers ********************************************************/
#include "hal.h"

/***** Constat values *******************************************************/
/***** Macros ***************************************************************/
/***** Typedefs *************************************************************/
/***** Function prototypes **************************************************/
/***** Local variables ******************************************************/
/***** Global variables *****************************************************/
/***** Signal catching functions ********************************************/
/***** Local functions ******************************************************/
/***** Global functions *****************************************************/

//================================================================
/*!@brief
  initialize
*/

void TC3_Handler(void){
  mrbc_tick();
  TC3_REGS->COUNT16.TC_INTFLAG |= TC_INTFLAG_MC0(1);
}

void hal_init(void){
  // Configure asynchronous clock source
  GCLK_REGS->GCLK_CLKCTRL = GCLK_CLKCTRL_ID_TCC2_TC3;
  GCLK_REGS->GCLK_CLKCTRL |= GCLK_CLKCTRL_GEN_GCLK0;
  GCLK_REGS->GCLK_CLKCTRL |= GCLK_CLKCTRL_CLKEN(1);
  
  // Configure synchronous bus clock
  PM_REGS->PM_APBCSEL |= PM_APBCSEL_APBCDIV_DIV1;
  PM_REGS->PM_APBCMASK |= PM_APBCMASK_TC3(1);
  
  // Configure Count Mode (16-bit)
  TC3_REGS->COUNT16.TC_CTRLA |= TC_CTRLA_MODE_COUNT16;
  
  // Configure Stadgy Mode
  TC3_REGS->COUNT16.TC_CTRLA |= TC_CTRLA_RUNSTDBY(1);
  
  // Configure Prescaler for divide by 2 (500kHz clock to COUNT)
  TC3_REGS->COUNT16.TC_CTRLA |= TC_CTRLA_PRESCALER_DIV1;
  //TC3_REGS->COUNT16.TC_CTRLA |= TC_CTRLA_PRESCALER_DIV16_Val;
  
  // Configure TC3 Compare Mode for compare channel 0
  TC3_REGS->COUNT16.TC_CTRLA |= TC_CTRLA_WAVEGEN_MFRQ;
  
  // Initialize compare value for 100mS @ 500kHz
  TC3_REGS->COUNT16.TC_CC[0] |= 800;
  
  // Enable TC3 compare mode interrupt generation
  TC3_REGS->COUNT16.TC_INTENSET |= TC_INTENSET_MC0(1);
  
  // Enable TC3
  TC3_REGS->COUNT16.TC_CTRLA |= TC_CTRLA_ENABLE(1);
  
  // Wait until TC3 is enabled
  while(TC3_REGS->COUNT16.TC_STATUS == TC_STATUS_SYNCBUSY(1));

  /* Set TC3 Interrupt Priority to Level 1 */
  NVIC_SetPriority(TC3_IRQn, 1);
  
  /* Enable TC3 NVIC Interrupt Line */
  NVIC_EnableIRQ(TC3_IRQn);
}