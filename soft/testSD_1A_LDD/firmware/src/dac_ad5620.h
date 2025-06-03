#ifndef dac_ad5620_H
#define dac_ad5620_H

/*--------------------------------------------------------*/
// dac_ad5620_H.h
/*--------------------------------------------------------*/
//	Description :	Utiilisation DAC AD5620
//
//	Auteur 		: 	LDD
//	Version		:	V1.0
//	Compilateur	:	XC32 V2.50
//
/*--------------------------------------------------------*/



#include <stdint.h>

// prototypes des fonctions
void Dac_Init(void);
void Dac_Write( uint16_t ech);

#endif // dac_ad5620_H
