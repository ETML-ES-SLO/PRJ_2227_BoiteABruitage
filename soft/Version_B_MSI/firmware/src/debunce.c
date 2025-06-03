/*--------------------------------------------------------*/
// dac_ad5620_H.c
/*--------------------------------------------------------*/
//	Description :	Utiilisation DAC AD5620
//
//	Auteur 		: 	LDD
//	Version		:	V1.0
//	Compilateur	:	XC32 V2.50
//
/*--------------------------------------------------------*/



#include "debunce.h"
#include "app.h"

Memory Input_1;
Memory Input_2;
Memory Input_3;
Memory Input_4;
Memory Input_5;
Memory Input_6;
Memory Input_7;
Memory Input_8;
Memory Input_9;
Memory Input_10;

void Debunce_Input(void)
{
    //sauvegarde états précédents
    Memoire_Save(&Input_1);
    Memoire_Save(&Input_2);
    Memoire_Save(&Input_3);
    Memoire_Save(&Input_4);
    Memoire_Save(&Input_5);
    Memoire_Save(&Input_6);
    Memoire_Save(&Input_7);
    Memoire_Save(&Input_8);
    Memoire_Save(&Input_9);
    Memoire_Save(&Input_10);
   
    //sauvegarde état actuel
   Input_1.Value_actuel = Input_1StateGet();
   Input_2.Value_actuel = Input_2StateGet();
   Input_3.Value_actuel = Input_3StateGet();
   Input_4.Value_actuel = Input_4StateGet();
   Input_5.Value_actuel = Input_5StateGet();
   Input_6.Value_actuel = Input_6StateGet();
   Input_7.Value_actuel = Input_7StateGet();
   Input_8.Value_actuel = Input_8StateGet();
   Input_9.Value_actuel = Input_9StateGet();
   Input_10.Value_actuel = Input_10StateGet(); 
}
void Memoire_Save(Memory *Input1)
{
    Input1->memoire_2 = Input1->memoire_1;
    Input1->memoire_1 = Input1->Value_actuel;
}
bool Check_FlanMontant(uint8_t InputNumber)
{
    
    //choix de l'entrée à lire 
    switch (InputNumber)
    {
        case 1 : 
            return Result_mes(Input_1);
            break;
       
        case 2 : 
            return Result_mes(Input_2);
            break;
        case 3 : 
            return Result_mes(Input_3);
            break;
        case 4 : 
            return Result_mes(Input_4);
            break;
        case 5 : 
            return Result_mes(Input_5);
            break;
        case 6 : 
            return Result_mes(Input_6);
            break;
        case 7 : 
            return Result_mes(Input_7);
            break;
        case 8 : 
            return Result_mes(Input_8);
            break;
        case 9 : 
            return Result_mes(Input_9);
            break;
        case 10 : 
            return Result_mes(Input_10);
            break;
            default :
                return false;
                break;    
    }
    
    
}

bool Result_mes(Memory Input2)
{
    if ((Input2.Value_actuel == true)&&
            (Input2.memoire_1 == false) &&
            (Input2.memoire_2 == false))
    {
        return true;
    }
    else return false;
}
