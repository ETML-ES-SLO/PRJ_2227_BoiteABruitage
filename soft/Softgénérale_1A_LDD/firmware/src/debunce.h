#ifndef debunce_H
#define debunce_H

/*--------------------------------------------------------*/
// debunce.h
/*--------------------------------------------------------*/
//	Description :	anti rebond entrée optocouplée
//
//	Auteur 		: 	LDD
//	Version		:	V1.0
//	Compilateur	:	XC32 V2.50
//
/*--------------------------------------------------------*/



#include <stdint.h>
#include <stdbool.h>
// prototypes des fonctions
void Debunce_Input(void); // fonction vérification anti-rebond

typedef struct
{
    bool Value_actuel;
    bool memoire_1;
    bool memoire_2;
    
} Memory;

void Memoire_Save(Memory *Input1);
//routne si il y a un flanc montant 
bool Check_FlanMontant(uint8_t InputNumber);
//fait la vérigfication direct du flan
bool Result_mes(Memory Input2);


#endif // debunce_H
