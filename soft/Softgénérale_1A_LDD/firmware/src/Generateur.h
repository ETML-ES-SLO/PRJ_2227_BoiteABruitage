#ifndef Generateur_h
#define Generateur_h

// TP3 MenuGen 2016
// C. HUBER  03.02.2016
// Fichier Generateur.h
// Prototypes des fonctions du générateur  de signal


#include <stdint.h>



typedef struct {
            uint16_t Nb_Tic;
            uint16_t Min;             // Amplitude min
            uint16_t Max;             // Amplitude max  
            
} S_Amplitude;


// Mise à jour du signal (forme, amplitude, offset)
void  GENSIG_UpdateSignal(uint16_t Vallue);

// A appeler dans int Timer3
void  GENSIG_Execute(void);


#endif