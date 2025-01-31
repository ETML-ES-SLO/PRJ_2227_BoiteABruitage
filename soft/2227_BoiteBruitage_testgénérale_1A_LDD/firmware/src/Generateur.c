
// Canevas manipulation GenSig avec menu
// C. HUBER  09/02/2015
// Fichier Generateur.C
// Gestion  du générateur

// Prévu pour signal de 40 echantillons

// Migration sur PIC32 30.04.2014 C. Huber

//version final le 07.03.2023 - CME 


#include "Generateur.h"
#include "Mc32gestSpiDac.h"
#include "dac_ad5620.h"
#include <stdint.h>
#include <math.h>

// T.P. 2016 100 echantillons
#define MAX_ECH 100
#define VAL_TIC_MAX 1000
//déclaration des variable
S_Amplitude Ampli;
int32_t tb_Amplitude[MAX_ECH];

// Mise à jour du signal (forme, amplitude, offset)
void  GENSIG_UpdateSignal(uint16_t Vallue )
{ 
    //intialisation de la variable statique offset
    int16_t Offset = VAL_TIC_MAX/2;
    
    //initialisation de l'increment
    int i;
    
    //gestion de l'amplitude 
    Ampli.Nb_Tic = (Offset*Vallue)/100;
    
    //gestion de l'offest
            
    
    //---Gestion de la seclection des formes des signaux---/ 
     
    for( i = 0; i < 100; i++)
     {
    //calcul pour obtenir un sinus
        tb_Amplitude[i] = (Ampli.Nb_Tic/1.28)*(4/M_PI * sin( M_PI *(3.6*i)/180))+ Offset;
    }
}


// Execution du générateur
// Fonction appelée dans Int timer3 (cycle variable variable)

// Version provisoire pour test du DAC à modifier
void  GENSIG_Execute(void)
{
    //Initaliser EchNb à 0 en static
   static uint16_t EchNb = 0;
   
   //incrire la valeur de notre tableau dans le DAC sur le channel 0
   Dac_Write(tb_Amplitude[EchNb]);
   //incrémenter EchNb 
   EchNb++;
   //si EchNB est supperieur à 100  
   EchNb = EchNb % MAX_ECH;
  
   
}



