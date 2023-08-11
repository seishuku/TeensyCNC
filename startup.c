#include <stdint.h>
#include "MIMXRT1062.h"
#include "CMSIS/core_cm7.h"

extern unsigned long _stextload;
extern unsigned long _stext;
extern unsigned long _etext;
extern unsigned long _sdataload;
extern unsigned long _sdata;
extern unsigned long _edata;
extern unsigned long _sbss;
extern unsigned long _ebss;
extern unsigned long _flexram_bank_config;
extern unsigned long _flashimagelen;
extern void _estack(void);

void ResetISR(void);
__attribute__ ((weak)) void NMI_Handler(void);
__attribute__ ((weak)) void HardFault_Handler(void);
__attribute__ ((weak)) void MemManage_Handler(void);
__attribute__ ((weak)) void BusFault_Handler(void);
__attribute__ ((weak)) void UsageFault_Handler(void);
__attribute__ ((weak)) void SVC_Handler(void);
__attribute__ ((weak)) void DebugMon_Handler(void);
__attribute__ ((weak)) void PendSV_Handler(void);
__attribute__ ((weak)) void SysTick_Handler(void);
__attribute__ ((weak)) void IntDefaultHandler(void);

__attribute__ ((weak)) void DMA0_DMA16_IRQHandler(void);
__attribute__ ((weak)) void DMA1_DMA17_IRQHandler(void);
__attribute__ ((weak)) void DMA2_DMA18_IRQHandler(void);
__attribute__ ((weak)) void DMA3_DMA19_IRQHandler(void);
__attribute__ ((weak)) void DMA4_DMA20_IRQHandler(void);
__attribute__ ((weak)) void DMA5_DMA21_IRQHandler(void);
__attribute__ ((weak)) void DMA6_DMA22_IRQHandler(void);
__attribute__ ((weak)) void DMA7_DMA23_IRQHandler(void);
__attribute__ ((weak)) void DMA8_DMA24_IRQHandler(void);
__attribute__ ((weak)) void DMA9_DMA25_IRQHandler(void);
__attribute__ ((weak)) void DMA10_DMA26_IRQHandler(void);
__attribute__ ((weak)) void DMA11_DMA27_IRQHandler(void);
__attribute__ ((weak)) void DMA12_DMA28_IRQHandler(void);
__attribute__ ((weak)) void DMA13_DMA29_IRQHandler(void);
__attribute__ ((weak)) void DMA14_DMA30_IRQHandler(void);
__attribute__ ((weak)) void DMA15_DMA31_IRQHandler(void);
__attribute__ ((weak)) void DMA_ERROR_IRQHandler(void);
__attribute__ ((weak)) void CTI0_ERROR_IRQHandler(void);
__attribute__ ((weak)) void CTI1_ERROR_IRQHandler(void);
__attribute__ ((weak)) void CORE_IRQHandler(void);
__attribute__ ((weak)) void LPUART1_IRQHandler(void);
__attribute__ ((weak)) void LPUART2_IRQHandler(void);
__attribute__ ((weak)) void LPUART3_IRQHandler(void);
__attribute__ ((weak)) void LPUART4_IRQHandler(void);
__attribute__ ((weak)) void LPUART5_IRQHandler(void);
__attribute__ ((weak)) void LPUART6_IRQHandler(void);
__attribute__ ((weak)) void LPUART7_IRQHandler(void);
__attribute__ ((weak)) void LPUART8_IRQHandler(void);
__attribute__ ((weak)) void LPI2C1_IRQHandler(void);
__attribute__ ((weak)) void LPI2C2_IRQHandler(void);
__attribute__ ((weak)) void LPI2C3_IRQHandler(void);
__attribute__ ((weak)) void LPI2C4_IRQHandler(void);
__attribute__ ((weak)) void LPSPI1_IRQHandler(void);
__attribute__ ((weak)) void LPSPI2_IRQHandler(void);
__attribute__ ((weak)) void LPSPI3_IRQHandler(void);
__attribute__ ((weak)) void LPSPI4_IRQHandler(void);
__attribute__ ((weak)) void CAN1_IRQHandler(void);
__attribute__ ((weak)) void CAN2_IRQHandler(void);
__attribute__ ((weak)) void FLEXRAM_IRQHandler(void);
__attribute__ ((weak)) void KPP_IRQHandler(void);
__attribute__ ((weak)) void TSC_DIG_IRQHandler(void);
__attribute__ ((weak)) void GPR_IRQ_IRQHandler(void);
__attribute__ ((weak)) void LCDIF_IRQHandler(void);
__attribute__ ((weak)) void CSI_IRQHandler(void);
__attribute__ ((weak)) void PXP_IRQHandler(void);
__attribute__ ((weak)) void WDOG2_IRQHandler(void);
__attribute__ ((weak)) void SNVS_HP_WRAPPER_IRQHandler(void);
__attribute__ ((weak)) void SNVS_HP_WRAPPER_TZ_IRQHandler(void);
__attribute__ ((weak)) void SNVS_LP_WRAPPER_IRQHandler(void);
__attribute__ ((weak)) void CSU_IRQHandler(void);
__attribute__ ((weak)) void DCP_IRQHandler(void);
__attribute__ ((weak)) void DCP_VMI_IRQHandler(void);
__attribute__ ((weak)) void Reserved68_IRQHandler(void);
__attribute__ ((weak)) void TRNG_IRQHandler(void);
__attribute__ ((weak)) void SJC_IRQHandler(void);
__attribute__ ((weak)) void BEE_IRQHandler(void);
__attribute__ ((weak)) void SAI1_IRQHandler(void);
__attribute__ ((weak)) void SAI2_IRQHandler(void);
__attribute__ ((weak)) void SAI3_RX_IRQHandler(void);
__attribute__ ((weak)) void SAI3_TX_IRQHandler(void);
__attribute__ ((weak)) void SPDIF_IRQHandler(void);
__attribute__ ((weak)) void PMU_EVENT_IRQHandler(void);
__attribute__ ((weak)) void Reserved78_IRQHandler(void);
__attribute__ ((weak)) void TEMP_LOW_HIGH_IRQHandler(void);
__attribute__ ((weak)) void TEMP_PANIC_IRQHandler(void);
__attribute__ ((weak)) void USB_PHY1_IRQHandler(void);
__attribute__ ((weak)) void USB_PHY2_IRQHandler(void);
__attribute__ ((weak)) void ADC1_IRQHandler(void);
__attribute__ ((weak)) void ADC2_IRQHandler(void);
__attribute__ ((weak)) void DCDC_IRQHandler(void);
__attribute__ ((weak)) void Reserved86_IRQHandler(void);
__attribute__ ((weak)) void GPIO10_Combined_0_31_IRQHandler(void);
__attribute__ ((weak)) void GPIO1_INT0_IRQHandler(void);
__attribute__ ((weak)) void GPIO1_INT1_IRQHandler(void);
__attribute__ ((weak)) void GPIO1_INT2_IRQHandler(void);
__attribute__ ((weak)) void GPIO1_INT3_IRQHandler(void);
__attribute__ ((weak)) void GPIO1_INT4_IRQHandler(void);
__attribute__ ((weak)) void GPIO1_INT5_IRQHandler(void);
__attribute__ ((weak)) void GPIO1_INT6_IRQHandler(void);
__attribute__ ((weak)) void GPIO1_INT7_IRQHandler(void);
__attribute__ ((weak)) void GPIO1_Combined_0_15_IRQHandler(void);
__attribute__ ((weak)) void GPIO1_Combined_16_31_IRQHandler(void);
__attribute__ ((weak)) void GPIO2_Combined_0_15_IRQHandler(void);
__attribute__ ((weak)) void GPIO2_Combined_16_31_IRQHandler(void);
__attribute__ ((weak)) void GPIO3_Combined_0_15_IRQHandler(void);
__attribute__ ((weak)) void GPIO3_Combined_16_31_IRQHandler(void);
__attribute__ ((weak)) void GPIO4_Combined_0_15_IRQHandler(void);
__attribute__ ((weak)) void GPIO4_Combined_16_31_IRQHandler(void);
__attribute__ ((weak)) void GPIO5_Combined_0_15_IRQHandler(void);
__attribute__ ((weak)) void GPIO5_Combined_16_31_IRQHandler(void);
__attribute__ ((weak)) void FLEXIO1_IRQHandler(void);
__attribute__ ((weak)) void FLEXIO2_IRQHandler(void);
__attribute__ ((weak)) void WDOG1_IRQHandler(void);
__attribute__ ((weak)) void RTWDOG_IRQHandler(void);
__attribute__ ((weak)) void EWM_IRQHandler(void);
__attribute__ ((weak)) void CCM_1_IRQHandler(void);
__attribute__ ((weak)) void CCM_2_IRQHandler(void);
__attribute__ ((weak)) void GPC_IRQHandler(void);
__attribute__ ((weak)) void SRC_IRQHandler(void);
__attribute__ ((weak)) void Reserved115_IRQHandler(void);
__attribute__ ((weak)) void GPT1_IRQHandler(void);
__attribute__ ((weak)) void GPT2_IRQHandler(void);
__attribute__ ((weak)) void PWM1_0_IRQHandler(void);
__attribute__ ((weak)) void PWM1_1_IRQHandler(void);
__attribute__ ((weak)) void PWM1_2_IRQHandler(void);
__attribute__ ((weak)) void PWM1_3_IRQHandler(void);
__attribute__ ((weak)) void PWM1_FAULT_IRQHandler(void);
__attribute__ ((weak)) void FLEXSPI2_IRQHandler(void);
__attribute__ ((weak)) void FLEXSPI_IRQHandler(void);
__attribute__ ((weak)) void SEMC_IRQHandler(void);
__attribute__ ((weak)) void USDHC1_IRQHandler(void);
__attribute__ ((weak)) void USDHC2_IRQHandler(void);
__attribute__ ((weak)) void USB_OTG2_IRQHandler(void);
__attribute__ ((weak)) void USB_OTG1_IRQHandler(void);
__attribute__ ((weak)) void ENET_IRQHandler(void);
__attribute__ ((weak)) void ENET_1588_Timer_IRQHandler(void);
__attribute__ ((weak)) void XBAR1_IRQ_0_1_IRQHandler(void);
__attribute__ ((weak)) void XBAR1_IRQ_2_3_IRQHandler(void);
__attribute__ ((weak)) void ADC_ETC_IRQ0_IRQHandler(void);
__attribute__ ((weak)) void ADC_ETC_IRQ1_IRQHandler(void);
__attribute__ ((weak)) void ADC_ETC_IRQ2_IRQHandler(void);
__attribute__ ((weak)) void ADC_ETC_ERROR_IRQ_IRQHandler(void);
__attribute__ ((weak)) void PIT_IRQHandler(void);
__attribute__ ((weak)) void ACMP1_IRQHandler(void);
__attribute__ ((weak)) void ACMP2_IRQHandler(void);
__attribute__ ((weak)) void ACMP3_IRQHandler(void);
__attribute__ ((weak)) void ACMP4_IRQHandler(void);
__attribute__ ((weak)) void Reserved143_IRQHandler(void);
__attribute__ ((weak)) void Reserved144_IRQHandler(void);
__attribute__ ((weak)) void ENC1_IRQHandler(void);
__attribute__ ((weak)) void ENC2_IRQHandler(void);
__attribute__ ((weak)) void ENC3_IRQHandler(void);
__attribute__ ((weak)) void ENC4_IRQHandler(void);
__attribute__ ((weak)) void TMR1_IRQHandler(void);
__attribute__ ((weak)) void TMR2_IRQHandler(void);
__attribute__ ((weak)) void TMR3_IRQHandler(void);
__attribute__ ((weak)) void TMR4_IRQHandler(void);
__attribute__ ((weak)) void PWM2_0_IRQHandler(void);
__attribute__ ((weak)) void PWM2_1_IRQHandler(void);
__attribute__ ((weak)) void PWM2_2_IRQHandler(void);
__attribute__ ((weak)) void PWM2_3_IRQHandler(void);
__attribute__ ((weak)) void PWM2_FAULT_IRQHandler(void);
__attribute__ ((weak)) void PWM3_0_IRQHandler(void);
__attribute__ ((weak)) void PWM3_1_IRQHandler(void);
__attribute__ ((weak)) void PWM3_2_IRQHandler(void);
__attribute__ ((weak)) void PWM3_3_IRQHandler(void);
__attribute__ ((weak)) void PWM3_FAULT_IRQHandler(void);
__attribute__ ((weak)) void PWM4_0_IRQHandler(void);
__attribute__ ((weak)) void PWM4_1_IRQHandler(void);
__attribute__ ((weak)) void PWM4_2_IRQHandler(void);
__attribute__ ((weak)) void PWM4_3_IRQHandler(void);
__attribute__ ((weak)) void PWM4_FAULT_IRQHandler(void);
__attribute__ ((weak)) void ENET2_IRQHandler(void);
__attribute__ ((weak)) void ENET2_1588_Timer_IRQHandler(void);
__attribute__ ((weak)) void CAN3_IRQHandler(void);
__attribute__ ((weak)) void Reserved171_IRQHandler(void);
__attribute__ ((weak)) void FLEXIO3_IRQHandler(void);
__attribute__ ((weak)) void GPIO6_7_8_9_IRQHandler(void);

__attribute__((naked))
void unused_interrupt_vector(void)
{
	__disable_irq();
	while(1);
}

void DMA0_DMA16_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void DMA1_DMA17_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void DMA2_DMA18_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void DMA3_DMA19_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void DMA4_DMA20_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void DMA5_DMA21_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void DMA6_DMA22_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void DMA7_DMA23_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void DMA8_DMA24_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void DMA9_DMA25_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void DMA10_DMA26_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void DMA11_DMA27_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void DMA12_DMA28_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void DMA13_DMA29_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void DMA14_DMA30_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void DMA15_DMA31_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void DMA_ERROR_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void CTI0_ERROR_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void CTI1_ERROR_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void CORE_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void LPUART1_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void LPUART2_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void LPUART3_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void LPUART4_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void LPUART5_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void LPUART6_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void LPUART7_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void LPUART8_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void LPI2C1_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void LPI2C2_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void LPI2C3_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void LPI2C4_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void LPSPI1_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void LPSPI2_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void LPSPI3_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void LPSPI4_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void CAN1_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void CAN2_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void FLEXRAM_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void KPP_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void TSC_DIG_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void GPR_IRQ_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void LCDIF_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void CSI_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void PXP_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void WDOG2_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void SNVS_HP_WRAPPER_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void SNVS_HP_WRAPPER_TZ_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void SNVS_LP_WRAPPER_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void CSU_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void DCP_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void DCP_VMI_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void Reserved68_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void TRNG_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void SJC_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void BEE_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void SAI1_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void SAI2_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void SAI3_RX_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void SAI3_TX_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void SPDIF_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void PMU_EVENT_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void Reserved78_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void TEMP_LOW_HIGH_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void TEMP_PANIC_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void USB_PHY1_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void USB_PHY2_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void ADC1_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void ADC2_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void DCDC_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void Reserved86_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void GPIO10_Combined_0_31_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void GPIO1_INT0_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void GPIO1_INT1_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void GPIO1_INT2_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void GPIO1_INT3_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void GPIO1_INT4_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void GPIO1_INT5_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void GPIO1_INT6_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void GPIO1_INT7_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void GPIO1_Combined_0_15_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void GPIO1_Combined_16_31_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void GPIO2_Combined_0_15_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void GPIO2_Combined_16_31_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void GPIO3_Combined_0_15_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void GPIO3_Combined_16_31_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void GPIO4_Combined_0_15_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void GPIO4_Combined_16_31_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void GPIO5_Combined_0_15_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void GPIO5_Combined_16_31_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void FLEXIO1_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void FLEXIO2_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void WDOG1_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void RTWDOG_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void EWM_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void CCM_1_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void CCM_2_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void GPC_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void SRC_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void Reserved115_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void GPT1_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void GPT2_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void PWM1_0_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void PWM1_1_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void PWM1_2_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void PWM1_3_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void PWM1_FAULT_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void FLEXSPI2_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void FLEXSPI_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void SEMC_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void USDHC1_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void USDHC2_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void USB_OTG2_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void USB_OTG1_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void ENET_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void ENET_1588_Timer_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void XBAR1_IRQ_0_1_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void XBAR1_IRQ_2_3_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void ADC_ETC_IRQ0_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void ADC_ETC_IRQ1_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void ADC_ETC_IRQ2_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void ADC_ETC_ERROR_IRQ_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void PIT_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void ACMP1_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void ACMP2_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void ACMP3_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void ACMP4_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void Reserved143_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void Reserved144_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void ENC1_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void ENC2_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void ENC3_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void ENC4_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void TMR1_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void TMR2_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void TMR3_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void TMR4_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void PWM2_0_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void PWM2_1_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void PWM2_2_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void PWM2_3_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void PWM2_FAULT_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void PWM3_0_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void PWM3_1_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void PWM3_2_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void PWM3_3_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void PWM3_FAULT_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void PWM4_0_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void PWM4_1_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void PWM4_2_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void PWM4_3_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void PWM4_FAULT_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void ENET2_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void ENET2_1588_Timer_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void CAN3_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void Reserved171_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void FLEXIO3_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));
void GPIO6_7_8_9_DriverIRQHandler(void) __attribute__ ((weak, alias("unused_interrupt_vector")));

__attribute__ ((used, aligned(1024), section(".vectors")))
void (* const _VectorTable[])(void)=
{
    &_estack,                       // The initial stack pointer
    ResetISR,                          // The reset handler
    NMI_Handler,                       // NMI Handler
    HardFault_Handler,                 // Hard Fault Handler
    MemManage_Handler,                 // MPU Fault Handler
    BusFault_Handler,                  // Bus Fault Handler
    UsageFault_Handler,                // Usage Fault Handler
    0,                                 // Reserved
    0,                                 // Reserved
    0,                                 // Reserved
    0,                                 // Reserved
    SVC_Handler,                       // SVCall Handler
    DebugMon_Handler,                  // Debug Monitor Handler
    0,                                 // Reserved
    PendSV_Handler,                    // PendSV Handler
    SysTick_Handler,                   // SysTick Handler

    // Chip Level - MIMXRT1062
    DMA0_DMA16_IRQHandler,            // 16 : DMA channel 0/16 transfer complete
    DMA1_DMA17_IRQHandler,            // 17 : DMA channel 1/17 transfer complete
    DMA2_DMA18_IRQHandler,            // 18 : DMA channel 2/18 transfer complete
    DMA3_DMA19_IRQHandler,            // 19 : DMA channel 3/19 transfer complete
    DMA4_DMA20_IRQHandler,            // 20 : DMA channel 4/20 transfer complete
    DMA5_DMA21_IRQHandler,            // 21 : DMA channel 5/21 transfer complete
    DMA6_DMA22_IRQHandler,            // 22 : DMA channel 6/22 transfer complete
    DMA7_DMA23_IRQHandler,            // 23 : DMA channel 7/23 transfer complete
    DMA8_DMA24_IRQHandler,            // 24 : DMA channel 8/24 transfer complete
    DMA9_DMA25_IRQHandler,            // 25 : DMA channel 9/25 transfer complete
    DMA10_DMA26_IRQHandler,           // 26 : DMA channel 10/26 transfer complete
    DMA11_DMA27_IRQHandler,           // 27 : DMA channel 11/27 transfer complete
    DMA12_DMA28_IRQHandler,           // 28 : DMA channel 12/28 transfer complete
    DMA13_DMA29_IRQHandler,           // 29 : DMA channel 13/29 transfer complete
    DMA14_DMA30_IRQHandler,           // 30 : DMA channel 14/30 transfer complete
    DMA15_DMA31_IRQHandler,           // 31 : DMA channel 15/31 transfer complete
    DMA_ERROR_IRQHandler,             // 32 : DMA error interrupt channels 0-15 / 16-31
    CTI0_ERROR_IRQHandler,            // 33 : CTI0_Error
    CTI1_ERROR_IRQHandler,            // 34 : CTI1_Error
    CORE_IRQHandler,                  // 35 : CorePlatform exception IRQ
    LPUART1_IRQHandler,               // 36 : LPUART1 TX interrupt and RX interrupt
    LPUART2_IRQHandler,               // 37 : LPUART2 TX interrupt and RX interrupt
    LPUART3_IRQHandler,               // 38 : LPUART3 TX interrupt and RX interrupt
    LPUART4_IRQHandler,               // 39 : LPUART4 TX interrupt and RX interrupt
    LPUART5_IRQHandler,               // 40 : LPUART5 TX interrupt and RX interrupt
    LPUART6_IRQHandler,               // 41 : LPUART6 TX interrupt and RX interrupt
    LPUART7_IRQHandler,               // 42 : LPUART7 TX interrupt and RX interrupt
    LPUART8_IRQHandler,               // 43 : LPUART8 TX interrupt and RX interrupt
    LPI2C1_IRQHandler,                // 44 : LPI2C1 interrupt
    LPI2C2_IRQHandler,                // 45 : LPI2C2 interrupt
    LPI2C3_IRQHandler,                // 46 : LPI2C3 interrupt
    LPI2C4_IRQHandler,                // 47 : LPI2C4 interrupt
    LPSPI1_IRQHandler,                // 48 : LPSPI1 single interrupt vector for all sources
    LPSPI2_IRQHandler,                // 49 : LPSPI2 single interrupt vector for all sources
    LPSPI3_IRQHandler,                // 50 : LPSPI3 single interrupt vector for all sources
    LPSPI4_IRQHandler,                // 51 : LPSPI4  single interrupt vector for all sources
    CAN1_IRQHandler,                  // 52 : CAN1 interrupt
    CAN2_IRQHandler,                  // 53 : CAN2 interrupt
    FLEXRAM_IRQHandler,               // 54 : FlexRAM address out of range Or access hit IRQ
    KPP_IRQHandler,                   // 55 : Keypad nterrupt
    TSC_DIG_IRQHandler,               // 56 : TSC interrupt
    GPR_IRQ_IRQHandler,               // 57 : GPR interrupt
    LCDIF_IRQHandler,                 // 58 : LCDIF interrupt
    CSI_IRQHandler,                   // 59 : CSI interrupt
    PXP_IRQHandler,                   // 60 : PXP interrupt
    WDOG2_IRQHandler,                 // 61 : WDOG2 interrupt
    SNVS_HP_WRAPPER_IRQHandler,       // 62 : SRTC Consolidated Interrupt. Non TZ
    SNVS_HP_WRAPPER_TZ_IRQHandler,    // 63 : SRTC Security Interrupt. TZ
    SNVS_LP_WRAPPER_IRQHandler,       // 64 : ON-OFF button press shorter than 5 secs (pulse event)
    CSU_IRQHandler,                   // 65 : CSU interrupt
    DCP_IRQHandler,                   // 66 : DCP_IRQ interrupt
    DCP_VMI_IRQHandler,               // 67 : DCP_VMI_IRQ interrupt
    Reserved68_IRQHandler,            // 68 : Reserved interrupt
    TRNG_IRQHandler,                  // 69 : TRNG interrupt
    SJC_IRQHandler,                   // 70 : SJC interrupt
    BEE_IRQHandler,                   // 71 : BEE interrupt
    SAI1_IRQHandler,                  // 72 : SAI1 interrupt
    SAI2_IRQHandler,                  // 73 : SAI1 interrupt
    SAI3_RX_IRQHandler,               // 74 : SAI3 interrupt
    SAI3_TX_IRQHandler,               // 75 : SAI3 interrupt
    SPDIF_IRQHandler,                 // 76 : SPDIF interrupt
    PMU_EVENT_IRQHandler,             // 77 : Brown-out event interrupt
    Reserved78_IRQHandler,            // 78 : Reserved interrupt
    TEMP_LOW_HIGH_IRQHandler,         // 79 : TempSensor low/high interrupt
    TEMP_PANIC_IRQHandler,            // 80 : TempSensor panic interrupt
    USB_PHY1_IRQHandler,              // 81 : USBPHY (UTMI0), Interrupt
    USB_PHY2_IRQHandler,              // 82 : USBPHY (UTMI1), Interrupt
    ADC1_IRQHandler,                  // 83 : ADC1 interrupt
    ADC2_IRQHandler,                  // 84 : ADC2 interrupt
    DCDC_IRQHandler,                  // 85 : DCDC interrupt
    Reserved86_IRQHandler,            // 86 : Reserved interrupt
    GPIO10_Combined_0_31_IRQHandler,  // 87 : Combined interrupt indication for GPIO10 signal 0 throughout 31
    GPIO1_INT0_IRQHandler,            // 88 : Active HIGH Interrupt from INT0 from GPIO
    GPIO1_INT1_IRQHandler,            // 89 : Active HIGH Interrupt from INT1 from GPIO
    GPIO1_INT2_IRQHandler,            // 90 : Active HIGH Interrupt from INT2 from GPIO
    GPIO1_INT3_IRQHandler,            // 91 : Active HIGH Interrupt from INT3 from GPIO
    GPIO1_INT4_IRQHandler,            // 92 : Active HIGH Interrupt from INT4 from GPIO
    GPIO1_INT5_IRQHandler,            // 93 : Active HIGH Interrupt from INT5 from GPIO
    GPIO1_INT6_IRQHandler,            // 94 : Active HIGH Interrupt from INT6 from GPIO
    GPIO1_INT7_IRQHandler,            // 95 : Active HIGH Interrupt from INT7 from GPIO
    GPIO1_Combined_0_15_IRQHandler,   // 96 : Combined interrupt indication for GPIO1 signal 0 throughout 15
    GPIO1_Combined_16_31_IRQHandler,  // 97 : Combined interrupt indication for GPIO1 signal 16 throughout 31
    GPIO2_Combined_0_15_IRQHandler,   // 98 : Combined interrupt indication for GPIO2 signal 0 throughout 15
    GPIO2_Combined_16_31_IRQHandler,  // 99 : Combined interrupt indication for GPIO2 signal 16 throughout 31
    GPIO3_Combined_0_15_IRQHandler,   // 100: Combined interrupt indication for GPIO3 signal 0 throughout 15
    GPIO3_Combined_16_31_IRQHandler,  // 101: Combined interrupt indication for GPIO3 signal 16 throughout 31
    GPIO4_Combined_0_15_IRQHandler,   // 102: Combined interrupt indication for GPIO4 signal 0 throughout 15
    GPIO4_Combined_16_31_IRQHandler,  // 103: Combined interrupt indication for GPIO4 signal 16 throughout 31
    GPIO5_Combined_0_15_IRQHandler,   // 104: Combined interrupt indication for GPIO5 signal 0 throughout 15
    GPIO5_Combined_16_31_IRQHandler,  // 105: Combined interrupt indication for GPIO5 signal 16 throughout 31
    FLEXIO1_IRQHandler,               // 106: FLEXIO1 interrupt
    FLEXIO2_IRQHandler,               // 107: FLEXIO2 interrupt
    WDOG1_IRQHandler,                 // 108: WDOG1 interrupt
    RTWDOG_IRQHandler,                // 109: RTWDOG interrupt
    EWM_IRQHandler,                   // 110: EWM interrupt
    CCM_1_IRQHandler,                 // 111: CCM IRQ1 interrupt
    CCM_2_IRQHandler,                 // 112: CCM IRQ2 interrupt
    GPC_IRQHandler,                   // 113: GPC interrupt
    SRC_IRQHandler,                   // 114: SRC interrupt
    Reserved115_IRQHandler,           // 115: Reserved interrupt
    GPT1_IRQHandler,                  // 116: GPT1 interrupt
    GPT2_IRQHandler,                  // 117: GPT2 interrupt
    PWM1_0_IRQHandler,                // 118: PWM1 capture 0, compare 0, or reload 0 interrupt
    PWM1_1_IRQHandler,                // 119: PWM1 capture 1, compare 1, or reload 0 interrupt
    PWM1_2_IRQHandler,                // 120: PWM1 capture 2, compare 2, or reload 0 interrupt
    PWM1_3_IRQHandler,                // 121: PWM1 capture 3, compare 3, or reload 0 interrupt
    PWM1_FAULT_IRQHandler,            // 122: PWM1 fault or reload error interrupt
    FLEXSPI2_IRQHandler,              // 123: FlexSPI2 interrupt
    FLEXSPI_IRQHandler,               // 124: FlexSPI0 interrupt
    SEMC_IRQHandler,                  // 125: SEMC interrupt
    USDHC1_IRQHandler,                // 126: USDHC1 interrupt
    USDHC2_IRQHandler,                // 127: USDHC2 interrupt
    USB_OTG2_IRQHandler,              // 128: USBO2 USB OTG2
    USB_OTG1_IRQHandler,              // 129: USBO2 USB OTG1
    ENET_IRQHandler,                  // 130: ENET interrupt
    ENET_1588_Timer_IRQHandler,       // 131: ENET_1588_Timer interrupt
    XBAR1_IRQ_0_1_IRQHandler,         // 132: XBARA1 output signal 0, 1 interrupt
    XBAR1_IRQ_2_3_IRQHandler,         // 133: XBARA1 output signal 2, 3 interrupt
    ADC_ETC_IRQ0_IRQHandler,          // 134: ADCETC IRQ0 interrupt
    ADC_ETC_IRQ1_IRQHandler,          // 135: ADCETC IRQ1 interrupt
    ADC_ETC_IRQ2_IRQHandler,          // 136: ADCETC IRQ2 interrupt
    ADC_ETC_ERROR_IRQ_IRQHandler,     // 137: ADCETC Error IRQ interrupt
    PIT_IRQHandler,                   // 138: PIT interrupt
    ACMP1_IRQHandler,                 // 139: ACMP interrupt
    ACMP2_IRQHandler,                 // 140: ACMP interrupt
    ACMP3_IRQHandler,                 // 141: ACMP interrupt
    ACMP4_IRQHandler,                 // 142: ACMP interrupt
    Reserved143_IRQHandler,           // 143: Reserved interrupt
    Reserved144_IRQHandler,           // 144: Reserved interrupt
    ENC1_IRQHandler,                  // 145: ENC1 interrupt
    ENC2_IRQHandler,                  // 146: ENC2 interrupt
    ENC3_IRQHandler,                  // 147: ENC3 interrupt
    ENC4_IRQHandler,                  // 148: ENC4 interrupt
    TMR1_IRQHandler,                  // 149: TMR1 interrupt
    TMR2_IRQHandler,                  // 150: TMR2 interrupt
    TMR3_IRQHandler,                  // 151: TMR3 interrupt
    TMR4_IRQHandler,                  // 152: TMR4 interrupt
    PWM2_0_IRQHandler,                // 153: PWM2 capture 0, compare 0, or reload 0 interrupt
    PWM2_1_IRQHandler,                // 154: PWM2 capture 1, compare 1, or reload 0 interrupt
    PWM2_2_IRQHandler,                // 155: PWM2 capture 2, compare 2, or reload 0 interrupt
    PWM2_3_IRQHandler,                // 156: PWM2 capture 3, compare 3, or reload 0 interrupt
    PWM2_FAULT_IRQHandler,            // 157: PWM2 fault or reload error interrupt
    PWM3_0_IRQHandler,                // 158: PWM3 capture 0, compare 0, or reload 0 interrupt
    PWM3_1_IRQHandler,                // 159: PWM3 capture 1, compare 1, or reload 0 interrupt
    PWM3_2_IRQHandler,                // 160: PWM3 capture 2, compare 2, or reload 0 interrupt
    PWM3_3_IRQHandler,                // 161: PWM3 capture 3, compare 3, or reload 0 interrupt
    PWM3_FAULT_IRQHandler,            // 162: PWM3 fault or reload error interrupt
    PWM4_0_IRQHandler,                // 163: PWM4 capture 0, compare 0, or reload 0 interrupt
    PWM4_1_IRQHandler,                // 164: PWM4 capture 1, compare 1, or reload 0 interrupt
    PWM4_2_IRQHandler,                // 165: PWM4 capture 2, compare 2, or reload 0 interrupt
    PWM4_3_IRQHandler,                // 166: PWM4 capture 3, compare 3, or reload 0 interrupt
    PWM4_FAULT_IRQHandler,            // 167: PWM4 fault or reload error interrupt
    ENET2_IRQHandler,                 // 168: ENET2 interrupt
    ENET2_1588_Timer_IRQHandler,      // 169: ENET2_1588_Timer interrupt
    CAN3_IRQHandler,                  // 170: CAN3 interrupt
    Reserved171_IRQHandler,           // 171: Reserved interrupt
    FLEXIO3_IRQHandler,               // 172: FLEXIO3 interrupt
    GPIO6_7_8_9_IRQHandler,           // 173: GPIO6, GPIO7, GPIO8, GPIO9 interrupt
};

volatile uint32_t Tick;

volatile uint32_t systick_millis_count=0;
volatile uint32_t systick_cycle_count=0;

void SysTick_Handler(void)
{
	Tick++;
	systick_cycle_count=DWT->CYCCNT;
	systick_millis_count++;
}

volatile uint32_t F_CPU_ACTUAL=396000000;
volatile uint32_t F_BUS_ACTUAL=132000000;
uint32_t scale_cpu_cycles_to_microseconds=0;

uint32_t set_arm_clock(uint32_t frequency)
{
	uint32_t cbcdr=CCM->CBCDR; // pg 1021
	uint32_t cbcmr=CCM->CBCMR; // pg 1023
	uint32_t dcdc=DCDC->REG3;

	// compute required voltage
	uint32_t voltage=1150; // default = 1.15V

	if(frequency>528000000)
		voltage=1250; // 1.25V
	else if(frequency<=24000000)
		voltage=950; // 0.95

	// if voltage needs to increase, do it before switch clock speed
	CCM->CCGR6|=CCM_CCGR6_CG3_MASK;

	if((dcdc&DCDC_REG3_TRG_MASK)<DCDC_REG3_TRG((voltage-800)/25))
	{
		dcdc&=~DCDC_REG3_TRG_MASK;
		dcdc|=DCDC_REG3_TRG((voltage-800)/25);
		DCDC->REG3=dcdc;

		while(!(DCDC->REG0&DCDC_REG0_STS_DC_OK_MASK)); // wait voltage settling
	}

	if(!(cbcdr&CCM_CBCDR_PERIPH_CLK_SEL_MASK))
	{
		const uint32_t need1s=CCM_ANALOG_PLL_USB1_TOG_ENABLE_MASK|CCM_ANALOG_PLL_USB1_POWER_MASK|CCM_ANALOG_PLL_USB1_LOCK_MASK|CCM_ANALOG_PLL_USB1_EN_USB_CLKS_MASK;
		uint32_t sel, div;

		if((CCM_ANALOG->PLL_USB1&need1s)==need1s)
		{
			sel=0;
			div=3; // divide down to 120 MHz, so IPG is ok even if IPG_PODF=0
		}
		else
		{
			sel=1;
			div=0;
		}

		if((cbcdr&CCM_CBCDR_PERIPH_CLK2_PODF_MASK)!=CCM_CBCDR_PERIPH_CLK2_PODF(div))
		{
			// PERIPH_CLK2 divider needs to be changed
			cbcdr&=~CCM_CBCDR_PERIPH_CLK2_PODF_MASK;
			cbcdr|=CCM_CBCDR_PERIPH_CLK2_PODF(div);
			CCM->CBCDR=cbcdr;
		}

		if((cbcmr&CCM_CBCMR_PERIPH_CLK2_SEL_MASK)!=CCM_CBCMR_PERIPH_CLK2_SEL(sel))
		{
			// PERIPH_CLK2 source select needs to be changed
			cbcmr&=~CCM_CBCMR_PERIPH_CLK2_SEL_MASK;
			cbcmr|=CCM_CBCMR_PERIPH_CLK2_SEL(sel);
			CCM->CBCMR=cbcmr;

			while(CCM->CDHIPR&CCM_CDHIPR_PERIPH2_CLK_SEL_BUSY_MASK); // wait
		}

		// switch over to PERIPH_CLK2
		cbcdr|=CCM_CBCDR_PERIPH_CLK_SEL_MASK;
		CCM->CBCDR=cbcdr;

		while(CCM->CDHIPR&CCM_CDHIPR_PERIPH_CLK_SEL_BUSY_MASK); // wait
	}

	// DIV_SELECT: 54-108 = official range 648 to 1296 in 12 MHz steps
	uint32_t div_arm=1, div_ahb=1;

	while(frequency*div_arm*div_ahb<648000000)
	{
		if(div_arm<8)
			div_arm++;
		else
		{
			if(div_ahb<5)
			{
				div_ahb++;
				div_arm=1;
			}
			else
				break;
		}
	}

	uint32_t mult=(frequency*div_arm*div_ahb+6000000)/12000000;

	if(mult>108)
		mult=108;

	if(mult<54)
		mult=54;

	frequency=mult*12000000/div_arm/div_ahb;

	const uint32_t arm_pll_mask=CCM_ANALOG_PLL_ARM_LOCK_MASK|CCM_ANALOG_PLL_ARM_BYPASS_MASK|CCM_ANALOG_PLL_ARM_ENABLE_MASK|CCM_ANALOG_PLL_ARM_POWERDOWN_MASK|CCM_ANALOG_PLL_ARM_DIV_SELECT_MASK;

	if((CCM_ANALOG->PLL_ARM&arm_pll_mask)!=(CCM_ANALOG_PLL_ARM_LOCK_MASK|CCM_ANALOG_PLL_ARM_ENABLE_MASK|CCM_ANALOG_PLL_ARM_DIV_SELECT(mult)))
	{
		CCM_ANALOG->PLL_ARM=CCM_ANALOG_PLL_ARM_POWERDOWN_MASK;
		CCM_ANALOG->PLL_ARM=CCM_ANALOG_PLL_ARM_ENABLE_MASK|CCM_ANALOG_PLL_ARM_DIV_SELECT(mult);

		while(!(CCM_ANALOG->PLL_ARM&CCM_ANALOG_PLL_ARM_LOCK_MASK)); // wait for lock
	}

	if((CCM->CACRR&CCM_CACRR_ARM_PODF_MASK)!=(div_arm-1))
	{
		CCM->CACRR=CCM_CACRR_ARM_PODF(div_arm-1);

		while(CCM->CDHIPR&CCM_CDHIPR_ARM_PODF_BUSY_MASK); // wait
	}

	if((cbcdr&CCM_CBCDR_AHB_PODF_MASK)!=CCM_CBCDR_AHB_PODF(div_ahb-1))
	{
		cbcdr&=~CCM_CBCDR_AHB_PODF_MASK;
		cbcdr|=CCM_CBCDR_AHB_PODF(div_ahb-1);
		CCM->CBCDR=cbcdr;
		while(CCM->CDHIPR&CCM_CDHIPR_AHB_PODF_BUSY_MASK); // wait
	}

	uint32_t div_ipg=(frequency+149999999)/150000000;

	if(div_ipg>4)
		div_ipg=4;

	if((cbcdr&CCM_CBCDR_IPG_PODF_MASK)!=(CCM_CBCDR_IPG_PODF(div_ipg-1)))
	{
		cbcdr&=~CCM_CBCDR_IPG_PODF_MASK;
		cbcdr|=CCM_CBCDR_IPG_PODF(div_ipg-1);
		CCM->CBCDR=cbcdr;
	}

	CCM->CBCDR&=~CCM_CBCDR_PERIPH_CLK_SEL_MASK;

	while(CCM->CDHIPR&CCM_CDHIPR_PERIPH_CLK_SEL_BUSY_MASK); // wait

	F_CPU_ACTUAL=frequency;
	F_BUS_ACTUAL=frequency/div_ipg;
	scale_cpu_cycles_to_microseconds=0xFFFFFFFFu/(uint32_t)(frequency/1000000u);

	if((dcdc&DCDC_REG3_TRG_MASK)>DCDC_REG3_TRG((voltage-800)/25))
	{
		dcdc&=~DCDC_REG3_TRG_MASK;
		dcdc|=DCDC_REG3_TRG((voltage-800)/25);
		DCDC->REG3=dcdc;

		while(!(DCDC->REG0&DCDC_REG0_STS_DC_OK_MASK)); // wait voltage settling
	}

	return frequency;
}

__attribute__((section(".startup"), optimize("no-tree-loop-distribute-patterns")))
static void memory_copy(uint32_t *dest, const uint32_t *src, uint32_t *dest_end)
{
    if (dest == src)
        return;
    while (dest < dest_end)
    {
        *dest++ = *src++;
    }
}

__attribute__((section(".startup"), optimize("no-tree-loop-distribute-patterns")))
static void memory_clear(uint32_t *dest, uint32_t *dest_end)
{
    while (dest < dest_end)
    {
        *dest++ = 0;
    }
}

// concise defines for SCB_MPU_RASR and SCB_MPU_RBAR, ARM DDI0403E, pg 696
#define MPU_RASR_AP(n)		((uint32_t)(((n)&7)<<24))
#define MPU_RASR_TEX(n)		((uint32_t)(((n)&7)<<19))
#define MPU_RASR_SIZE(n)	((uint32_t)(((n)&31)<<1))
#define MPU_RBAR_REGION(n)	((uint32_t)((n)&15))

#define NOEXEC			MPU_RASR_XN_Msk
#define READONLY		MPU_RASR_AP(7)
#define READWRITE		MPU_RASR_AP(3)
#define NOACCESS		MPU_RASR_AP(0)
#define MEM_CACHE_WT	MPU_RASR_TEX(0)|MPU_RASR_C_Msk
#define MEM_CACHE_WB	MPU_RASR_TEX(0)|MPU_RASR_C_Msk|MPU_RASR_B_Msk
#define MEM_CACHE_WBWA	MPU_RASR_TEX(1)|MPU_RASR_C_Msk|MPU_RASR_B_Msk
#define MEM_NOCACHE		MPU_RASR_TEX(1)
#define DEV_NOCACHE		MPU_RASR_TEX(2)
#define SIZE_32B		(MPU_RASR_SIZE(4)|MPU_RASR_ENABLE_Msk)
#define SIZE_64B		(MPU_RASR_SIZE(5)|MPU_RASR_ENABLE_Msk)
#define SIZE_128B		(MPU_RASR_SIZE(6)|MPU_RASR_ENABLE_Msk)
#define SIZE_256B		(MPU_RASR_SIZE(7)|MPU_RASR_ENABLE_Msk)
#define SIZE_512B		(MPU_RASR_SIZE(8)|MPU_RASR_ENABLE_Msk)
#define SIZE_1K			(MPU_RASR_SIZE(9)|MPU_RASR_ENABLE_Msk)
#define SIZE_2K			(MPU_RASR_SIZE(10)|MPU_RASR_ENABLE_Msk)
#define SIZE_4K			(MPU_RASR_SIZE(11)|MPU_RASR_ENABLE_Msk)
#define SIZE_8K			(MPU_RASR_SIZE(12)|MPU_RASR_ENABLE_Msk)
#define SIZE_16K		(MPU_RASR_SIZE(13)|MPU_RASR_ENABLE_Msk)
#define SIZE_32K		(MPU_RASR_SIZE(14)|MPU_RASR_ENABLE_Msk)
#define SIZE_64K		(MPU_RASR_SIZE(15)|MPU_RASR_ENABLE_Msk)
#define SIZE_128K		(MPU_RASR_SIZE(16)|MPU_RASR_ENABLE_Msk)
#define SIZE_256K		(MPU_RASR_SIZE(17)|MPU_RASR_ENABLE_Msk)
#define SIZE_512K		(MPU_RASR_SIZE(18)|MPU_RASR_ENABLE_Msk)
#define SIZE_1M			(MPU_RASR_SIZE(19)|MPU_RASR_ENABLE_Msk)
#define SIZE_2M			(MPU_RASR_SIZE(20)|MPU_RASR_ENABLE_Msk)
#define SIZE_4M			(MPU_RASR_SIZE(21)|MPU_RASR_ENABLE_Msk)
#define SIZE_8M			(MPU_RASR_SIZE(22)|MPU_RASR_ENABLE_Msk)
#define SIZE_16M		(MPU_RASR_SIZE(23)|MPU_RASR_ENABLE_Msk)
#define SIZE_32M		(MPU_RASR_SIZE(24)|MPU_RASR_ENABLE_Msk)
#define SIZE_64M		(MPU_RASR_SIZE(25)|MPU_RASR_ENABLE_Msk)
#define SIZE_128M		(MPU_RASR_SIZE(26)|MPU_RASR_ENABLE_Msk)
#define SIZE_256M		(MPU_RASR_SIZE(27)|MPU_RASR_ENABLE_Msk)
#define SIZE_512M		(MPU_RASR_SIZE(28)|MPU_RASR_ENABLE_Msk)
#define SIZE_1G			(MPU_RASR_SIZE(29)|MPU_RASR_ENABLE_Msk)
#define SIZE_2G			(MPU_RASR_SIZE(30)|MPU_RASR_ENABLE_Msk)
#define SIZE_4G			(MPU_RASR_SIZE(31)|MPU_RASR_ENABLE_Msk)
#define REGION(n)		(MPU_RBAR_REGION(n)|MPU_RBAR_VALID_Msk)

__attribute__((section(".flashmem")))
void configure_cache(void)
{
	MPU->CTRL=0; // turn off MPU

	uint32_t i=0;

	MPU->RBAR=0x00000000|REGION(i++);
	MPU->RASR=MPU_RASR_TEX(0)|NOACCESS|NOEXEC|SIZE_4G;
	MPU->RBAR=0x00000000|REGION(i++); // ITCM
	MPU->RASR=MEM_NOCACHE|READWRITE|SIZE_512K;
	MPU->RBAR=0x00000000|REGION(i++); // trap NULL pointer deref
	MPU->RASR=DEV_NOCACHE|NOACCESS|SIZE_32B;
	MPU->RBAR=0x00200000|REGION(i++); // Boot ROM
	MPU->RASR=MEM_CACHE_WT|READONLY | SIZE_128K;
	MPU->RBAR=0x20000000|REGION(i++); // DTCM
	MPU->RASR=MEM_NOCACHE|READWRITE|NOEXEC|SIZE_512K;
	MPU->RBAR=((uint32_t)&_ebss)|REGION(i++); // trap stack overflow
	MPU->RASR=MPU_RASR_TEX(0)|NOACCESS|NOEXEC|SIZE_32B;
	MPU->RBAR=0x20200000|REGION(i++); // RAM (AXI bus)
	MPU->RASR=MEM_CACHE_WBWA|READWRITE|NOEXEC|SIZE_1M;
	MPU->RBAR=0x40000000|REGION(i++); // Peripherals
	MPU->RASR=DEV_NOCACHE|READWRITE|NOEXEC|SIZE_64M;
	MPU->RBAR=0x60000000 | REGION(i++); // QSPI Flash
	MPU->RASR=MEM_CACHE_WBWA|READONLY|SIZE_16M;
	MPU->RBAR=0x70000000|REGION(i++); // FlexSPI2
	MPU->RASR=MEM_CACHE_WBWA|READWRITE|NOEXEC|SIZE_16M;
	MPU->CTRL=MPU_CTRL_ENABLE_Msk;

	asm("dsb");
	asm("isb");
	SCB->ICIALLU=0;

	asm("dsb");
	asm("isb");
	SCB->CCR|=(SCB_CCR_IC_Msk|SCB_CCR_DC_Msk);
}

void main();

__attribute__((section(".startup"), optimize("no-tree-loop-distribute-patterns"), naked))
void ResetISR(void)
{
    // FlexRAM bank configuration
    IOMUXC_GPR->GPR17=(uint32_t)&_flexram_bank_config;
    IOMUXC_GPR->GPR16=0x00000007;
    IOMUXC_GPR->GPR14=0x00AA0000;
    __asm__ volatile ("MSR MSP, %0" : : "r" (&_estack) : );

    // Initialize memory
    memory_copy(&_stext, &_stextload, &_etext);
    memory_copy(&_sdata, &_sdataload, &_edata);
    memory_clear(&_sbss, &_ebss);

    // enable FPU
	SCB->CPACR=0x00F00000;

	// Set vector table
	SCB->VTOR=(uint32_t)_VectorTable;

	// Configure core clocks
	set_arm_clock(396000000);

	// Configure and enable cache
	configure_cache();

	// Set up SysTick
	CoreDebug->DEMCR|=CoreDebug_DEMCR_TRCENA_Msk;
	DWT->CTRL|=DWT_CTRL_CYCCNTENA_Msk; // turn on cycle counter
	Tick=DWT->CYCCNT; // compiled 0, corrected w/1st systick

	NVIC_SetPriority(SysTick_IRQn, (1<<__NVIC_PRIO_BITS)-1);

	SysTick->LOAD=(100000/1000)-1;
	SysTick->VAL=0;
	SysTick->CTRL=SysTick_CTRL_TICKINT_Msk|SysTick_CTRL_ENABLE_Msk;

	// Enable interrupts
	__asm__ volatile ("CPSIE i");

	// Call the `main()` function defined in `main.c`.
    main();

	// main should never return...
	while(1);
}

__attribute__ ((section(".bootdata"), used))
const uint32_t BootData[3]=
{
	0x60000000,
	(uint32_t)&_flashimagelen,
	0
};


__attribute__ ((section(".csf"), used))
const uint32_t hab_csf[768];	// placeholder for HAB signature


__attribute__ ((section(".ivt"), used))
const uint32_t ImageVectorTable[8]=
{
	0x432000D1,		// header
	(uint32_t)&ResetISR,// program entry
	0,			// reserved
	0,			// dcd
	(uint32_t)BootData,	// abs address of boot data
	(uint32_t)ImageVectorTable, // self
	(uint32_t)hab_csf,	// command sequence file
	0			// reserved
};

__attribute__ ((section(".flashconfig"), used))
uint32_t FlexSPI_NOR_Config[128]=
{
	// 448 byte common FlexSPI configuration block, 8.6.3.1 page 223 (RT1060 rev 0)
	// MCU_Flashloader_Reference_Manual.pdf, 8.2.1, Table 8-2, page 72-75
	0x42464346,		// Tag				0x00
	0x56010000,		// Version
	0,			// reserved
	0x00030301,		// columnAdressWidth,dataSetupTime,dataHoldTime,readSampleClkSrc

	0x00000000,		// waitTimeCfgCommands,-,deviceModeCfgEnable
	0,			// deviceModeSeq
	0, 			// deviceModeArg
	0x00000000,		// -,-,-,configCmdEnable

	0,			// configCmdSeqs		0x20
	0,
	0,
	0,

	0,			// cfgCmdArgs			0x30
	0,
	0,
	0,

	0x00000000,		// controllerMiscOption		0x40
	0x00080401,		// lutCustomSeqEnable,serialClkFreq,sflashPadType,deviceType
	0,			// reserved
	0,			// reserved

	0x00200000,		// sflashA1Size			0x50
	0,			// sflashA2Size
	0,			// sflashB1Size
	0,			// sflashB2Size

	0,			// csPadSettingOverride		0x60
	0,			// sclkPadSettingOverride
	0,			// dataPadSettingOverride
	0,			// dqsPadSettingOverride

	0,			// timeoutInMs			0x70
	0,			// commandInterval
	0,			// dataValidTime
	0x00000000,		// busyBitPolarity,busyOffset

	0x0A1804EB,		// lookupTable[0]		0x80
	0x26043206,		// lookupTable[1]
	0,			// lookupTable[2]
	0,			// lookupTable[3]

	0x24040405,		// lookupTable[4]		0x90
	0,			// lookupTable[5]
	0,			// lookupTable[6]
	0,			// lookupTable[7]

	0,			// lookupTable[8]		0xA0
	0,			// lookupTable[9]
	0,			// lookupTable[10]
	0,			// lookupTable[11]

	0x00000406,		// lookupTable[12]		0xB0
	0,			// lookupTable[13]
	0,			// lookupTable[14]
	0,			// lookupTable[15]

	0,			// lookupTable[16]		0xC0
	0,			// lookupTable[17]
	0,			// lookupTable[18]
	0,			// lookupTable[19]

	0x08180420,		// lookupTable[20]		0xD0
	0,			// lookupTable[21]
	0,			// lookupTable[22]
	0,			// lookupTable[23]

	0,			// lookupTable[24]		0xE0
	0,			// lookupTable[25]
	0,			// lookupTable[26]
	0,			// lookupTable[27]

	0,			// lookupTable[28]		0xF0
	0,			// lookupTable[29]
	0,			// lookupTable[30]
	0,			// lookupTable[31]

	0x081804D8,		// lookupTable[32]		0x100
	0,			// lookupTable[33]
	0,			// lookupTable[34]
	0,			// lookupTable[35]

	0x08180402,		// lookupTable[36]		0x110
	0x00002004,		// lookupTable[37]
	0,			// lookupTable[38]
	0,			// lookupTable[39]

	0,			// lookupTable[40]		0x120
	0,			// lookupTable[41]
	0,			// lookupTable[42]
	0,			// lookupTable[43]

	0x00000460,		// lookupTable[44]		0x130
	0,			// lookupTable[45]
	0,			// lookupTable[46]
	0,			// lookupTable[47]

	0,			// lookupTable[48]		0x140
	0,			// lookupTable[49]
	0,			// lookupTable[50]
	0,			// lookupTable[51]

	0,			// lookupTable[52]		0x150
	0,			// lookupTable[53]
	0,			// lookupTable[54]
	0,			// lookupTable[55]

	0,			// lookupTable[56]		0x160
	0,			// lookupTable[57]
	0,			// lookupTable[58]
	0,			// lookupTable[59]

	0,			// lookupTable[60]		0x170
	0,			// lookupTable[61]
	0,			// lookupTable[62]
	0,			// lookupTable[63]

	0,			// LUT 0: Read			0x180
	0,			// LUT 1: ReadStatus
	0,			// LUT 3: WriteEnable
	0,			// LUT 5: EraseSector

	0,			// LUT 9: PageProgram		0x190
	0,			// LUT 11: ChipErase
	0,			// LUT 15: Dummy
	0,			// LUT unused?

	0,			// LUT unused?			0x1A0
	0,			// LUT unused?
	0,			// LUT unused?
	0,			// LUT unused?

	0,			// reserved			0x1B0
	0,			// reserved
	0,			// reserved
	0,			// reserved

	// 64 byte Serial NOR configuration block, 8.6.3.2, page 346

	256,			// pageSize			0x1C0
	4096,			// sectorSize
	1,			// ipCmdSerialClkFreq
	0,			// reserved

	0x00010000,		// block size			0x1D0
	0,			// reserved
	0,			// reserved
	0,			// reserved

	0,			// reserved			0x1E0
	0,			// reserved
	0,			// reserved
	0,			// reserved

	0,			// reserved			0x1F0
	0,			// reserved
	0,			// reserved
	0			// reserved
};
