/*
 * File:   main.c
 * Author: YoshihiroOgura
 *
 * Created on 2022/12/21, 14:29
 */
// USER_WORD_0
#pragma config NVMCTRL_BOOTPROT = SIZE_0BYTES
#pragma config NVMCTRL_EEPROM_SIZE = SIZE_0BYTES
#pragma config BOD33USERLEVEL = 0x7 // Enter Hexadecimal value
#pragma config BOD33_EN = DISABLED
#pragma config BOD33_ACTION = RESET
#pragma config WDT_ENABLE = DISABLED
#pragma config WDT_ALWAYSON = DISABLED
#pragma config WDT_PER = CYC16384
#pragma config WDT_WINDOW_0 = SET

// USER_WORD_1
#pragma config WDT_WINDOW_1 = 0x5 // Enter Hexadecimal value
#pragma config WDT_EWOFFSET = CYC16384
#pragma config WDT_WEN = DISABLED
#pragma config BOD33_HYST = DISABLED
#pragma config NVMCTRL_REGION_LOCKS = 0xFFFF // Enter Hexadecimal value

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include "mrubyc.h"

#define MEMORY_SIZE (1024*10)
uint8_t memory_pool[MEMORY_SIZE];

const uint8_t test[] = {
0x52,0x49,0x54,0x45,0x30,0x33,0x30,0x30,0x00,0x00,0x00,0x83,0x4d,0x41,0x54,0x5a,
0x30,0x30,0x30,0x30,0x49,0x52,0x45,0x50,0x00,0x00,0x00,0x67,0x30,0x33,0x30,0x30,
0x00,0x00,0x00,0x5b,0x00,0x01,0x00,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x24,
0x2d,0x01,0x00,0x00,0x07,0x02,0x2d,0x01,0x01,0x01,0x07,0x02,0x2d,0x01,0x02,0x01,
0x06,0x02,0x2d,0x01,0x03,0x01,0x07,0x02,0x2d,0x01,0x02,0x01,0x25,0xff,0xe5,0x11,
0x01,0x38,0x01,0x69,0x00,0x00,0x00,0x04,0x00,0x07,0x73,0x65,0x74,0x6d,0x6f,0x64,
0x65,0x00,0x00,0x05,0x77,0x72,0x69,0x74,0x65,0x00,0x00,0x05,0x73,0x6c,0x65,0x65,
0x70,0x00,0x00,0x06,0x77,0x72,0x69,0x74,0x65,0x32,0x00,0x45,0x4e,0x44,0x00,0x00,
0x00,0x00,0x08,
};

int hal_write(int fd, const void *buf, int nbytes) {
    return 0;
}

int hal_flush(int fd) {
    return 0;
}

static void c_gpio_setmode(mrb_vm *vm, mrb_value *v, int argc) {
  PORT_REGS->GROUP[0].PORT_DIRSET = (1<<17);
  PORT_REGS->GROUP[0].PORT_PINCFG[17] = PORT_PINCFG_PMUXEN(0)
                            | PORT_PINCFG_INEN(0)
                            | PORT_PINCFG_PULLEN(0)
                            | PORT_PINCFG_DRVSTR(0);
  PORT_REGS->GROUP[0].PORT_CTRL = (1<<17);
}

static void c_gpio_write(mrb_vm *vm, mrb_value *v, int argc) {
    int mode = GET_INT_ARG(1);
    PORT_REGS->GROUP[0].PORT_DIRSET = (1<<17);
}

static void c_gpio_write2(mrb_vm *vm, mrb_value *v, int argc) {
    PORT_REGS->GROUP[0].PORT_DIRCLR = (1<<17);
}

void main(void) {
  SYSCTRL_REGS->SYSCTRL_OSC8M |= SYSCTRL_OSC8M_PRESC_0
                                | SYSCTRL_OSC8M_RUNSTDBY(1)
                                | SYSCTRL_OSC8M_ENABLE(1);
  GCLK_REGS->GCLK_GENCTRL = GCLK_GENCTRL_RUNSTDBY(1)
                           | GCLK_GENCTRL_GENEN(1)
                           | GCLK_GENCTRL_SRC_OSC8M
                           | GCLK_GENCTRL_ID(0);
  while(GCLK_REGS->GCLK_STATUS == GCLK_STATUS_SYNCBUSY(1));
  mrbc_init(memory_pool, MEMORY_SIZE);
  mrbc_define_method(0, mrbc_class_object, "write2", c_gpio_write2);
  mrbc_define_method(0, mrbc_class_object, "write", c_gpio_write);
  mrbc_define_method(0, mrbc_class_object, "setmode", c_gpio_setmode);
  mrbc_create_task(test, 0);
  mrbc_run();
}
