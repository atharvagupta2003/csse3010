/**
***************************************************************
* @file mylib/ s4819560_sysmon.h
* @author s48195609
* @date 13/04/2024
* @brief system monitor
* REFERENCE: templates/mylib, sourcelib/os/fr_sysmon
***************************************************************
* EXTERNAL FUNCTIONS
***************************************************************
s4819560_sysmon_init() - initialise system monitor
***************************************************************
*/

#ifndef s4819560_SYSMON_H
#define s4819560_SYSMON_H

void s4819560_sysmon_init();
//System monitor pins
#define SYSMON_CHAN0_PIN		10
#define SYSMON_CHAN0_GPIO		GPIOD

#define SYSMON_CHAN1_PIN		7
#define SYSMON_CHAN1_GPIO		GPIOG

#define SYSMON_CHAN2_PIN		4
#define SYSMON_CHAN2_GPIO		GPIOG

#define S4819560_REG_SYSMON_CHAN0_CLR()  SYSMON_CHAN0_GPIO->BSRR = (1 << (SYSMON_CHAN0_PIN + 16))
#define S4819560_REG_SYSMON_CHAN0_SET()  SYSMON_CHAN0_GPIO->BSRR = (1 << SYSMON_CHAN0_PIN)
#define S4819560_REG_SYSMON_CHAN1_CLR()  SYSMON_CHAN1_GPIO->BSRR = (1 << (SYSMON_CHAN1_PIN + 16))
#define S4819560_REG_SYSMON_CHAN1_SET()  SYSMON_CHAN1_GPIO->BSRR = (1 << SYSMON_CHAN1_PIN)
#define S4819560_REG_SYSMON_CHAN2_CLR()  SYSMON_CHAN2_GPIO->BSRR = (1 << (SYSMON_CHAN2_PIN + 16))
#define S4819560_REG_SYSMON_CHAN2_SET()  SYSMON_CHAN2_GPIO->BSRR = (1 << SYSMON_CHAN2_PIN)

#endif