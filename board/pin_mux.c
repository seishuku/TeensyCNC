/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Pins v13.1
processor: MIMXRT1062xxxxB
package_id: MIMXRT1062DVL6B
mcu_data: ksdk2_0
processor_version: 13.0.2
external_user_signals: {}
pin_labels:
- {pin_num: J13, pin_signal: GPIO_AD_B1_11, label: motorXa, identifier: motorXa}
- {pin_num: L13, pin_signal: GPIO_AD_B1_10, label: motorXb, identifier: motorXb}
- {pin_num: M13, pin_signal: GPIO_AD_B1_09, label: motorYa, identifier: motorYa}
- {pin_num: H13, pin_signal: GPIO_AD_B1_08, label: motorYb, identifier: motorYb}
- {pin_num: D8, pin_signal: GPIO_B0_03, label: headSolenoid, identifier: headSolenoid}
- {pin_num: L11, pin_signal: GPIO_AD_B1_02, label: loadButton, identifier: loadButton}
- {pin_num: E7, pin_signal: GPIO_B0_01, label: encoderXb, identifier: encoderXb}
- {pin_num: E8, pin_signal: GPIO_B0_02, label: encoderXa, identifier: encoderXa}
- {pin_num: K10, pin_signal: GPIO_AD_B1_07, label: encoderYb, identifier: encoderYb}
- {pin_num: J12, pin_signal: GPIO_AD_B1_06, label: encoderYa, identifier: encoderYa}
- {pin_num: M11, pin_signal: GPIO_AD_B0_02, label: motorXa, identifier: motorXa}
- {pin_num: G11, pin_signal: GPIO_AD_B0_03, label: motorXb, identifier: motorXb}
- {pin_num: H5, pin_signal: GPIO_EMC_06, label: motorXb, identifier: motorXb}
- {pin_num: H3, pin_signal: GPIO_EMC_08, label: motorXa, identifier: motorXa}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "fsl_gpio.h"
#include "pin_mux.h"

/* FUNCTION ************************************************************************************************************
 * 
 * Function Name : BOARD_InitBootPins
 * Description   : Calls initialization functions.
 * 
 * END ****************************************************************************************************************/
void BOARD_InitBootPins(void) {
    BOARD_InitPins();
}

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPins:
- options: {callFromInitBoot: 'true', coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: H3, peripheral: PWM2, signal: 'A, 1', pin_signal: GPIO_EMC_08, direction: OUTPUT}
  - {pin_num: H5, peripheral: PWM2, signal: 'A, 0', pin_signal: GPIO_EMC_06, direction: OUTPUT}
  - {pin_num: M13, peripheral: PWM4, signal: 'A, 1', pin_signal: GPIO_AD_B1_09, direction: OUTPUT}
  - {pin_num: H13, peripheral: PWM4, signal: 'A, 0', pin_signal: GPIO_AD_B1_08, direction: OUTPUT}
  - {pin_num: E8, peripheral: GPIO7, signal: 'gpio_io, 02', pin_signal: GPIO_B0_02, direction: INPUT, gpio_interrupt: kGPIO_IntRisingEdge}
  - {pin_num: E7, peripheral: GPIO7, signal: 'gpio_io, 01', pin_signal: GPIO_B0_01, direction: INPUT, gpio_interrupt: kGPIO_IntRisingEdge}
  - {pin_num: J12, peripheral: GPIO6, signal: 'gpio_io, 22', pin_signal: GPIO_AD_B1_06, direction: INPUT, gpio_interrupt: kGPIO_IntRisingEdge}
  - {pin_num: K10, peripheral: GPIO6, signal: 'gpio_io, 23', pin_signal: GPIO_AD_B1_07, direction: INPUT, gpio_interrupt: kGPIO_IntRisingEdge}
  - {pin_num: L11, peripheral: GPIO6, signal: 'gpio_io, 18', pin_signal: GPIO_AD_B1_02, direction: INPUT, gpio_interrupt: kGPIO_NoIntmode}
  - {pin_num: D8, peripheral: GPIO7, signal: 'gpio_io, 03', pin_signal: GPIO_B0_03, direction: OUTPUT}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitPins(void) {
  CLOCK_EnableClock(kCLOCK_Iomuxc);           

  /* GPIO configuration of loadButton on GPIO_AD_B1_02 (pin L11) */
  gpio_pin_config_t loadButton_config = {
      .direction = kGPIO_DigitalInput,
      .outputLogic = 0U,
      .interruptMode = kGPIO_NoIntmode
  };
  /* Initialize GPIO functionality on GPIO_AD_B1_02 (pin L11) */
  GPIO_PinInit(GPIO6, 18U, &loadButton_config);

  /* GPIO configuration of encoderYa on GPIO_AD_B1_06 (pin J12) */
  gpio_pin_config_t encoderYa_config = {
      .direction = kGPIO_DigitalInput,
      .outputLogic = 0U,
      .interruptMode = kGPIO_IntRisingEdge
  };
  /* Initialize GPIO functionality on GPIO_AD_B1_06 (pin J12) */
  GPIO_PinInit(GPIO6, 22U, &encoderYa_config);
  /* Enable GPIO pin interrupt on GPIO_AD_B1_06 (pin J12) */
  GPIO_PortEnableInterrupts(GPIO6, 1U << 22U);

  /* GPIO configuration of encoderYb on GPIO_AD_B1_07 (pin K10) */
  gpio_pin_config_t encoderYb_config = {
      .direction = kGPIO_DigitalInput,
      .outputLogic = 0U,
      .interruptMode = kGPIO_IntRisingEdge
  };
  /* Initialize GPIO functionality on GPIO_AD_B1_07 (pin K10) */
  GPIO_PinInit(GPIO6, 23U, &encoderYb_config);
  /* Enable GPIO pin interrupt on GPIO_AD_B1_07 (pin K10) */
  GPIO_PortEnableInterrupts(GPIO6, 1U << 23U);

  /* GPIO configuration of encoderXb on GPIO_B0_01 (pin E7) */
  gpio_pin_config_t encoderXb_config = {
      .direction = kGPIO_DigitalInput,
      .outputLogic = 0U,
      .interruptMode = kGPIO_IntRisingEdge
  };
  /* Initialize GPIO functionality on GPIO_B0_01 (pin E7) */
  GPIO_PinInit(GPIO7, 1U, &encoderXb_config);
  /* Enable GPIO pin interrupt on GPIO_B0_01 (pin E7) */
  GPIO_PortEnableInterrupts(GPIO7, 1U << 1U);

  /* GPIO configuration of encoderXa on GPIO_B0_02 (pin E8) */
  gpio_pin_config_t encoderXa_config = {
      .direction = kGPIO_DigitalInput,
      .outputLogic = 0U,
      .interruptMode = kGPIO_IntRisingEdge
  };
  /* Initialize GPIO functionality on GPIO_B0_02 (pin E8) */
  GPIO_PinInit(GPIO7, 2U, &encoderXa_config);
  /* Enable GPIO pin interrupt on GPIO_B0_02 (pin E8) */
  GPIO_PortEnableInterrupts(GPIO7, 1U << 2U);

  /* GPIO configuration of headSolenoid on GPIO_B0_03 (pin D8) */
  gpio_pin_config_t headSolenoid_config = {
      .direction = kGPIO_DigitalOutput,
      .outputLogic = 0U,
      .interruptMode = kGPIO_NoIntmode
  };
  /* Initialize GPIO functionality on GPIO_B0_03 (pin D8) */
  GPIO_PinInit(GPIO7, 3U, &headSolenoid_config);

  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_02_GPIO1_IO18, 0U); 
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_06_GPIO1_IO22, 0U); 
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_07_GPIO1_IO23, 0U); 
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_08_FLEXPWM4_PWMA00, 0U); 
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_09_FLEXPWM4_PWMA01, 0U); 
  IOMUXC_SetPinMux(IOMUXC_GPIO_B0_01_GPIO2_IO01, 0U); 
  IOMUXC_SetPinMux(IOMUXC_GPIO_B0_02_GPIO2_IO02, 0U); 
  IOMUXC_SetPinMux(IOMUXC_GPIO_B0_03_GPIO2_IO03, 0U); 
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_06_FLEXPWM2_PWMA00, 0U); 
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_08_FLEXPWM2_PWMA01, 0U); 
  IOMUXC_GPR->GPR26 = ((IOMUXC_GPR->GPR26 &
    (~(BOARD_INITPINS_IOMUXC_GPR_GPR26_GPIO_MUX1_GPIO_SEL_MASK))) 
      | IOMUXC_GPR_GPR26_GPIO_MUX1_GPIO_SEL(0xC40000U) 
    );
  IOMUXC_GPR->GPR27 = ((IOMUXC_GPR->GPR27 &
    (~(BOARD_INITPINS_IOMUXC_GPR_GPR27_GPIO_MUX2_GPIO_SEL_MASK))) 
      | IOMUXC_GPR_GPR27_GPIO_MUX2_GPIO_SEL(0x0EU) 
    );
}

/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
