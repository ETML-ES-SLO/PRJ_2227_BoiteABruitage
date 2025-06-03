#ifndef __DRIVERADC_H
#define __DRIVERADC_H
/*--------------------------------------------------------*/
//	DriverAdc.h
/*--------------------------------------------------------*/
//	Description :	Gestion ADC10 en mode scan
//
//
//	Auteur 		: 	C. Huber
//
//	Version		:	V1.0
//	Compilateur	:	XC32 V1.33 & Harmony V1.00
//
/*--------------------------------------------------------*/

// #include "bsp_config.h"
#include <stdint.h>

// Structure � adapter selon le nombre de canaux
// Limite � 8 avec les buffers altern�

typedef struct {
   uint16_t Chan0;
} S_ADCResults ;

/*--------------------------------------------------------*/
// Fonction BSP_InitADC10
/*--------------------------------------------------------*/
void InitADC10(void);

/*--------------------------------------------------------*/
// Fonction BSP_ReadAllADC()
/*--------------------------------------------------------*/
S_ADCResults BSP_ReadAllADC();

#endif
