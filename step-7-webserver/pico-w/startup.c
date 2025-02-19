// Copyright (c) 2022 Cesanta Software Limited
// All rights reserved

#include "mcu.h"

// Startup code
__attribute__((naked, section(".boot"))) void _reset(void) {
  // Initialise flash
  XIP_SSI->SSIENR = 0;
  XIP_SSI->BAUDR = 2;
  XIP_SSI->CTRLR0 =
      (XIP_SSI_CTRLR0_SPI_FRF_STD << XIP_SSI_CTRLR0_SPI_FRF_Pos) |
      (XIP_SSI_CTRLR0_TMOD_EEPROM_READ << XIP_SSI_CTRLR0_TMOD_Pos) |
      ((32 - 1) << XIP_SSI_CTRLR0_DFS_32_Pos);
  XIP_SSI->CTRLR1 = (0 << XIP_SSI_CTRLR1_NDF_Pos);
  XIP_SSI->SPI_CTRLR0 =
      (0x03 << XIP_SSI_SPI_CTRLR0_XIP_CMD_Pos) |
      ((24 / 4) << XIP_SSI_SPI_CTRLR0_ADDR_L_Pos) |
      (XIP_SSI_SPI_CTRLR0_INST_L_8B << XIP_SSI_SPI_CTRLR0_INST_L_Pos) |
      (XIP_SSI_SPI_CTRLR0_TRANS_TYPE_1C1A << XIP_SSI_SPI_CTRLR0_TRANS_TYPE_Pos);
  XIP_SSI->SSIENR = XIP_SSI_SSIENR_SSI_EN_Msk;

  // Initialise memory
  extern long _sbss, _ebss, _edata, _stext, _sflash;
  for (long *src = &_sflash, *dst = &_stext; dst < &_edata;) *dst++ = *src++;
  for (long *src = &_sbss; src < &_ebss;) *src++ = 0;

  extern void main(void);
  main();             // Call main()
  for (;;) (void) 0;  // Loop forever if main() returns
}

void DefaultIRQHandler(void) {
  for (;;) (void) 0;
}
#define WEAK_ALIAS __attribute__((weak, alias("DefaultIRQHandler")))

WEAK_ALIAS void NMI_Handler(void);
WEAK_ALIAS void HardFault_Handler(void);
WEAK_ALIAS void MemManage_Handler(void);
WEAK_ALIAS void BusFault_Handler(void);
WEAK_ALIAS void UsageFault_Handler(void);
WEAK_ALIAS void SVC_Handler(void);
WEAK_ALIAS void DebugMon_Handler(void);
WEAK_ALIAS void PendSV_Handler(void);
WEAK_ALIAS void SysTick_Handler(void);

WEAK_ALIAS void TIMER0_IRQHandler(void);
WEAK_ALIAS void TIMER1_IRQHandler(void);
WEAK_ALIAS void TIMER2_IRQHandler(void);
WEAK_ALIAS void TIMER3_IRQHandler(void);
WEAK_ALIAS void PWM_IRQHandler(void);
WEAK_ALIAS void USBCTRL_IRQHandler(void);
WEAK_ALIAS void XIP_IRQHandler(void);
WEAK_ALIAS void PIO00_IRQHandler(void);
WEAK_ALIAS void PIO01_IRQHandler(void);
WEAK_ALIAS void PIO10_IRQHandler(void);
WEAK_ALIAS void PIO11_IRQHandler(void);
WEAK_ALIAS void DMA0_IRQHandler(void);
WEAK_ALIAS void DMA1_IRQHandler(void);
WEAK_ALIAS void IO_BANK0_IRQHandler(void);
WEAK_ALIAS void IO_QSPI_IRQHandler(void);
WEAK_ALIAS void SIO_PROC0_IRQHandler(void);
WEAK_ALIAS void SIO_PROC1_IRQHandler(void);
WEAK_ALIAS void CLOCKS_IRQHandler(void);
WEAK_ALIAS void SPI0_IRQHandler(void);
WEAK_ALIAS void SPI1_IRQHandler(void);
WEAK_ALIAS void UART0_IRQHandler(void);
WEAK_ALIAS void UART1_IRQHandler(void);
WEAK_ALIAS void ADC_FIFO_IRQHandler(void);
WEAK_ALIAS void I2C0_IRQHandler(void);
WEAK_ALIAS void I2C1_IRQHandler(void);
WEAK_ALIAS void RTC_IRQHandler(void);

extern void _estack(void);

// IRQ table
__attribute__((section(".vectors"))) void (*vectors[])(void) = {
    // Cortex interrupts
    _estack, _reset, NMI_Handler, HardFault_Handler, MemManage_Handler,
    BusFault_Handler, UsageFault_Handler, 0, 0, 0, 0, SVC_Handler,
    DebugMon_Handler, 0, PendSV_Handler, SysTick_Handler,

    // Peripheral interrupts
    TIMER0_IRQHandler, TIMER1_IRQHandler, TIMER2_IRQHandler, TIMER3_IRQHandler,
    PWM_IRQHandler, USBCTRL_IRQHandler, XIP_IRQHandler, PIO00_IRQHandler,
    PIO01_IRQHandler, PIO10_IRQHandler, PIO11_IRQHandler, DMA0_IRQHandler,
    DMA1_IRQHandler, IO_BANK0_IRQHandler, IO_QSPI_IRQHandler,
    SIO_PROC0_IRQHandler, SIO_PROC1_IRQHandler, CLOCKS_IRQHandler,
    SPI0_IRQHandler, SPI1_IRQHandler, UART0_IRQHandler, UART1_IRQHandler,
    ADC_FIFO_IRQHandler, I2C0_IRQHandler, I2C1_IRQHandler, RTC_IRQHandler};
