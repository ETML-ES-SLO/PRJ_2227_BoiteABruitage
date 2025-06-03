/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    ETML-ES

  File Name:
    SDCard_WavReader_JMO.c

  Summary:
    This file contains the source code for SDCard_WavReader_JMO.

  Description:
 * Ce fichier contien le code source pour SDCard_WavReader_JMO.  Il implémente 
 * la logique de la machine d'état de la tache et peut appeler les routines API 
 * d'autres modules MPLAB Harmony du système, tels que les pilotes, les services
 * système et les intergiciels. Cependant, il n'appelle aucune interface système
 * (telle que les fonctions « Initialiser » et « Tâches ») des modules du système
 * et ne formule aucune hypothèse quant au moment où ces fonctions sont appelées.
 * Cette responsabilité incombe aux fichiers système spécifiques à la configuration.
 *******************************************************************************/

#include "system_config/default/system_config.h"


// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2014 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/
// DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "SDCard_WavReader_JMO.h"


// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************
// *****************************************************************************
/* SDCard_WavReader Data

  Summary:
 Contient les donnees de SDCard_WavReader

  Description:
    This structure holds the application's data.

  Remarks:
    Cette structure devrait etre initalisee par la fonctions 
 * SDCard_WavReader_Initialize function

*/

SDCARD_WAVREADER_DATA sdCardWavData;
static uint32_t readCnt;
static uint32_t waitCnt;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/*----------------------------------------------------------------------------*/
/**
 * @brief   Cette fonction sort un echantillon sur une sortie analogique, ici sur
 *          sur un OC. Elle doit etre appelee par l'ISR du timer utilise a la 
 *          frequence d'echantillonnage.
 */
void SDCARD_SamplesOutputCallback()
{
//    LED2Off();
    
    SDCARD_WAVREADER_BUFFER_BLOCK *block;
    
    /* Pointer sur le bloc en cours de lecture */
    block = &sdCardWavData.bufBlocks[sdCardWavData.blockNrPlaying];
    
    /* En cas de donnees dispo, jouez l'echantillon actuel */
    if(block->blockHasData)
    {
        /* formatter le sample (+32768 pour centrer signal en monotension)
         *  pour la sortie DAC utilisee (DAC PWM -> /1024)*/
        uint16_t sampleToPlay = (block->data[block->readPos] +32768) / 1024;
        
        Dac_Write(sampleToPlay);
               //        DRV_OC0_PulseWidthSet(sampleToPlay);
        
        /* En cas de fin de bloc, indiquer qu'il n'y a plus de donnees 
         * et passer au bloc suivant */
        block->readPos++;
        if (block->readPos >= SDCARD_READ_BLOCK_SIZE)
        {
            block->blockHasData = false;
            sdCardWavData.blockNrPlaying++;
            if (sdCardWavData.blockNrPlaying >= SDCARD_NROF_BUFF_BLOCKS)
            {
                sdCardWavData.blockNrPlaying = 0;
            }
        }
    }
//    LED2On();
}   /* SDCARD_SamplesOutputCallback */

/* ************************************************************************** */
/* ************************************************************************** */
/** @name Section: Local Functions                                            */
/* ************************************************************************** */
/* ************************************************************************** */
///@{

/*----------------------------------------------------------------------------*/
/**
 * @brief   Cette fonction affiche l'etat courant sur le LCD du kit ES.
 *          S'il n'est pas necessaire d'afficher un etat en particulier, le
 *          mettre en commentaire
 * 
 * @param   state   L'etat a afficher
 */
/*static void DisplayCurrentSDState(SDCARD_STATES state) {
    lcd_gotoxy(1, 3);

    switch (state) {
        case SDCARD_STATE_MOUNT_DISK:
            printf_lcd("MOUNT_DISK          ");
            break;
        case SDCARD_STATE_UNMOUNT_DISK:
            printf_lcd("UNMOUNT_DISK        ");
            break;
        case SDCARD_STATE_SET_CURRENT_DRIVE:
            printf_lcd("SET_CURRENT_DRIVE   ");
            break;
        case SDCARD_STATE_OPEN_FILE_TO_READ:
            printf_lcd("OPEN_FILE_TO_READ   ");
            break;
        case SDCARD_STATE_READ_WAVE_HEADER:
            printf_lcd("READ_WAVE_HEADER    ");
            break;
        case SDCARD_STATE_WAIT_FREE_BUF_BLOCK:
//            printf_lcd("WAIT_FREE_BUF_BLOCK ");
            break;
        case SDCARD_STATE_READ_FILE_BLOCK:
            printf_lcd("READ_FILE_BLOCK     ");
            break;
        case SDCARD_STATE_DIRECTORY_FILE_SEARCH :
            printf_lcd("DIR_FILE_SEARCH     ");
            break;
        case SDCARD_STATE_CLOSE_FILE :
            printf_lcd("CLOSE_FILE          ");
            break;
        case SDCARD_STATE_WAIT_END_SAMPLES_OUT:
            printf_lcd("WAIT_END_SAMPLES_OUT");
            break;
        case SDCARD_STATE_WAIT_READ_REQUEST:
            printf_lcd("WAIT_READ_REQUEST   ");
            break;
        case SDCARD_STATE_IDLE :
            printf_lcd("IDLE                ");
            break;
        case SDCARD_STATE_ERROR:
            printf_lcd("ERROR               ");
            break;
        default:
            printf_lcd("UNKNOWN STATE       ");
            break;
            
    }
}*/   /* DisplayCurrentSDState */

/*----------------------------------------------------------------------------*/
/**
 * @brief   Cette fonction met a jour le timer pour la frequence en Hz
 *          d'echantillonnage de sortie du signal WAV. Formule de calcul:
 * 
 *          counterValue = [(timerFreq / timerPreScaler) / samplingFreq] - 1
 */
void SDCard_SetOutputSamplingRate_Hz(uint32_t sFreq)
{
    uint32_t counterValue = DRV_TMR2_PeriodValueGet();
    uint32_t timerPrescaler = 1;
    
    uint32_t timerFreq = DRV_TMR2_CounterFrequencyGet();
    TMR_PRESCALE prescaleValue = DRV_TMR2_PrescalerGet();
       
    switch (prescaleValue){
        case TMR_PRESCALE_VALUE_1:
            timerPrescaler = 1;
            break;
        case TMR_PRESCALE_VALUE_2:
            timerPrescaler = 2;
            break;
        case TMR_PRESCALE_VALUE_4:
            timerPrescaler = 4;
            break;
        case TMR_PRESCALE_VALUE_8:
            timerPrescaler = 8;
            break;
        case TMR_PRESCALE_VALUE_16:
            timerPrescaler = 16;
            break;
        case TMR_PRESCALE_VALUE_32:
            timerPrescaler = 32;
            break;
        case TMR_PRESCALE_VALUE_64:
            timerPrescaler =64;
            break;
        case TMR_PRESCALE_VALUE_256:
            timerPrescaler = 256;
            break;
        default:
            break;
    }
    
    counterValue = (((timerFreq / timerPrescaler) / sFreq) - 1);
    DRV_TMR2_PeriodValueSet(counterValue);
    
}   /* SDCard_SetOutputSamplingRate_Hz */

/*----------------------------------------------------------------------------*/
/**
 * @brief   Effectue une transition de la machine d'etat. Les actions d'entree
 *          dans le nouvel etat doivent etre faites ici si besoin.
 * 
 * @param newState  nouvel etat de la machine
 */
static void SDCard_SetNewState(SDCARD_STATES newState)
{
    sdCardWavData.state = newState;
//    DisplayCurrentSDState(sdCardWavData.state);
    
}   /* SDCard_SetNewState */

/*----------------------------------------------------------------------------*/
/**
 * @brief   Cette fonction decode le debut du bloc passe en param et verifie si
 *          la structure est bien celle d'un fichier .WAV. Elle met a jour la
 *          structure de sdCardWavData.
 * 
 * @return  true si la structure trouvee correspond a un fichier .WAV supporte
 * 
 * @remark  Les formats suivants sont supportes, autrement fonction retourne false:
 *          Type de format :        PCM (=1)
 *          Nombre de canaux:       1 (=Mono)
 *          Freq. echantillonnage:  1000 - 22050 Hz 
 *          Bytes par Bloc:         2 (=1 canal a 16 bits)
 *          Bits par echantillon:   16 bit
 */
static bool SDCard_DecodeWavHeader(void *header)
{
    bool hIsOk = false;
    uint32_t value32Bit = 0;
    uint16_t value16Bit = 0;
    SDCARD_WAV_HEADER *h;
    h = &sdCardWavData.wavHeader;
    
    memcpy(h, header, sizeof(SDCARD_WAV_HEADER));
    /* debut de fichier */
    hIsOk = memcmp(h->riff, "RIFF", 4) == 0;
    
    /* on saute la taille du fichier car detection EOF a la lecture */
    /* verification format WAVE */
    hIsOk = hIsOk && (memcmp(h->typeHeader, "WAVE", 4) == 0);

    /* verification du bloc decrivant le format audio (fixe 16 octets) */
    hIsOk = hIsOk && (memcmp(h->formatChunkMarker, "fmt ", 4) == 0);
    memcpy(&value32Bit, h->formatDataLength, 4);
    hIsOk = hIsOk && (value32Bit == 16);
    
    /* Type de Format (1: PCM entier, 3: PCM flottant, 65534: WAVE_FORMAT_EXTENSIBLE) */
    memcpy(&value16Bit, h->formatType, 2);
    hIsOk = hIsOk && (value16Bit == 1);

    /* Nb de canaux */
    memcpy(&value16Bit, h->numberOfChannels, 2);
    hIsOk = hIsOk && (value16Bit == 1);
    
    /* Donnees de freq d'echantillonnage et ajustement du timer associe */
    /* Ici les bytes/s ne sont pas surveilles */
    memcpy(&value32Bit, h->samplingRate, 4);
    SDCard_SetOutputSamplingRate_Hz(value32Bit);
    
    /* Controle du format des echantillons */
    memcpy(&value16Bit, h->bytesPerBloc, 2);
    hIsOk = hIsOk && (value16Bit == 2);
    memcpy(&value16Bit, h->bitsPerSample, 2);
    hIsOk = hIsOk && (value16Bit == 16);

    /* Controle balise debut des donnees (echantillons) */
    /* Rem: Taille des donnees non surveillee, car detection EOF a la lecture */
    hIsOk = hIsOk && (memcmp(h->data, "data", 4) == 0);

    return hIsOk;
    
}   /* SDCard_DecodeWavHeader */

///@}

// *****************************************************************************
// *****************************************************************************
// Section: Interface Functions
// *****************************************************************************
// *****************************************************************************

/*----------------------------------------------------------------------------*/

void SDCard_RequestReadFile (char *fileName)
{
    //sdCardWavData.readingFileNr = fileNr;
    //sdCardWavData.fileName;
    strcpy(sdCardWavData.fileName, fileName);
    sdCardWavData.readFileIsRequested = true;
    sdCardWavData.readFileIsInProgress = true;

}   /* SDCard_RequestReadFile */

/*----------------------------------------------------------------------------*/

bool SDCard_IsFileReadingInProgress ()
{
    return sdCardWavData.readFileIsInProgress;
    
}   /* SDCard_IsFileReadingInProgress */

/*******************************************************************************
  Function:
    void SDCard_WavReader_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */
void SDCard_WavReader_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    sdCardWavData.state = SDCARD_STATE_MOUNT_DISK;
    //DisplayCurrentSDState(sdCardWavData.state);
    
//    sdCardWavData.state = SDCARD_STATE_IDLE;

    /* Initialize state machine and other parameters. */
    sdCardWavData.blockNrForWriting = 0;
    sdCardWavData.blockNrPlaying = 0;
    sdCardWavData.fileHandle = SYS_FS_HANDLE_INVALID;
    sdCardWavData.readFileIsInProgress = false;
    sdCardWavData.readFileIsRequested = false;

}   /* SDCard_WavReader_Initialize */


/******************************************************************************
  Function:
    void SDCard_WavReader_Tasks ( void )

  Remarks:
    See prototype in SDCard_WavReader_JMO.h.
 */

void SDCard_WavReader_Tasks ( void )
{
    /* The SDCard Reader task state machine */
    switch(sdCardWavData.state)
    {
        case SDCARD_STATE_MOUNT_DISK:
            if(SYS_FS_Mount("/dev/mmcblka1", "/mnt/myDrive", FAT, 0, NULL) != 0)
            {
                /* The disk could not be mounted. Try
                 * mounting again until success. */
                sdCardWavData.fsError = SYS_FS_Error();
                SDCard_SetNewState(SDCARD_STATE_MOUNT_DISK);
            }
            else
            {
                /* Mount was successful. Unmount the disk, for testing. */
                SDCard_SetNewState(SDCARD_STATE_SET_CURRENT_DRIVE);
            }
            break;

        case SDCARD_STATE_UNMOUNT_DISK:
            if(SYS_FS_Unmount("/mnt/myDrive") != 0)
            {
                /* The disk could not be un mounted. Try
                 * un mounting again untill success. */
                SDCard_SetNewState(SDCARD_STATE_UNMOUNT_DISK);
            }
            else
            {
                /* UnMount was successful. Go Idle */
                SDCard_SetNewState(SDCARD_STATE_IDLE);
            }
            break;

        case SDCARD_STATE_SET_CURRENT_DRIVE:
            if(SYS_FS_CurrentDriveSet("/mnt/myDrive") == SYS_FS_RES_FAILURE)
            {
                /* Error while setting current drive */
                sdCardWavData.fsError = SYS_FS_Error();
                SDCard_SetNewState(SDCARD_STATE_ERROR);
            }
            else
            {
                /* Open a file for reading. */
                SDCard_SetNewState(SDCARD_STATE_WAIT_READ_REQUEST);
            }
            break;

        case SDCARD_STATE_OPEN_FILE_TO_READ:
            sdCardWavData.fileHandle = SYS_FS_FileOpen(
                    sdCardWavData.fileName,
                    (SYS_FS_FILE_OPEN_READ));
            if(sdCardWavData.fileHandle == SYS_FS_HANDLE_INVALID)
            {
                /* Could not open the file. Error out*/
                sdCardWavData.fsError = SYS_FS_Error();
                SDCard_SetNewState(SDCARD_STATE_ERROR);
            }
            else
            {
                /* Ready for file reading */
                readCnt = 0;
                waitCnt = 0;
                sdCardWavData.readFileIsRequested = false;
                SDCard_SetNewState(SDCARD_STATE_READ_WAVE_HEADER);
                
//                lcd_gotoxy(11, 4);
//                printf_lcd("%d", readCnt);
            }
            break;

        case SDCARD_STATE_READ_WAVE_HEADER:
            if (SYS_FS_FileRead(sdCardWavData.fileHandle,(void *) sdCardWavData.bufBlocks[sdCardWavData.blockNrForWriting].data,512)== -1) {
                /* There was an error while reading the file.
                 * Close the file and error out. */

                SYS_FS_FileClose(sdCardWavData.fileHandle);
                sdCardWavData.fsError = SYS_FS_Error();
                SDCard_SetNewState(SDCARD_STATE_ERROR);
            }
            else if (SYS_FS_FileEOF(sdCardWavData.fileHandle) == 1) /* Test for end of file */ {
                /* Continue the read process, until the end of file is reached */

                SDCard_SetNewState(SDCARD_STATE_CLOSE_FILE);
            }
            else 
            {
                /* actions avec le bloc lu et stocke */
                bool wavFileIsOk = SDCard_DecodeWavHeader((void *) sdCardWavData.bufBlocks[sdCardWavData.blockNrForWriting].data);
                
                /* Preparer le bloc pour lecture, pointeur readPos AVANT signal HasData */
                if (wavFileIsOk)
                {
                    sdCardWavData.bufBlocks[sdCardWavData.blockNrForWriting].readPos = 22;
                    sdCardWavData.bufBlocks[sdCardWavData.blockNrForWriting].blockHasData = true;
                }
                /* passer au bloc suivant pour prochaine lecture disk */
                sdCardWavData.blockNrForWriting++;
                if (sdCardWavData.blockNrForWriting >= SDCARD_NROF_BUFF_BLOCKS) {
                    sdCardWavData.blockNrForWriting = 0;
                }

                /* verifier que le bloc est libre (pas de lecture d'echantillons en cours */
                if (sdCardWavData.bufBlocks[sdCardWavData.blockNrForWriting].blockHasData) 
                {
                    SDCard_SetNewState(SDCARD_STATE_WAIT_FREE_BUF_BLOCK); /* attendre qu'il soit libre */
                }
                else
                {
                    SDCard_SetNewState(SDCARD_STATE_READ_FILE_BLOCK);
                }
                
                /* si l'entete de fichier etait incorrecte -> erreur */
                if (wavFileIsOk == false)
                {
                    SDCard_SetNewState(SDCARD_STATE_ERROR);
                }
                readCnt++;
            }
            break;

        case SDCARD_STATE_READ_FILE_BLOCK:
            if (SYS_FS_FileRead (sdCardWavData.fileHandle, (void *) sdCardWavData.bufBlocks[sdCardWavData.blockNrForWriting].data,512)== -1) {
                /* There was an error while reading the file.
                 * Close the file and error out. */

                SYS_FS_FileClose(sdCardWavData.fileHandle);
                sdCardWavData.fsError = SYS_FS_Error();
                SDCard_SetNewState(SDCARD_STATE_ERROR);
            }
            else if (SYS_FS_FileEOF(sdCardWavData.fileHandle) == 1) /* Test for end of file */ {
                /* Continue the read process, until the end of file is reached */

                SDCard_SetNewState(SDCARD_STATE_CLOSE_FILE);
            }
            else {
                /* actions avec le bloc lu et stocke */
                /* Preparer le bloc pour lecture, pointeur readPos AVANT signal HasData */
                sdCardWavData.bufBlocks[sdCardWavData.blockNrForWriting].readPos = 0;
                sdCardWavData.bufBlocks[sdCardWavData.blockNrForWriting].blockHasData = true;

                /* passer au bloc suivant pour prochaine lecture disk */
                sdCardWavData.blockNrForWriting++;
                if (sdCardWavData.blockNrForWriting >= SDCARD_NROF_BUFF_BLOCKS) {
                    sdCardWavData.blockNrForWriting = 0;
                }

                /* verifier que le bloc est libre (pas de lecture d'echantillons en cours */
                if (sdCardWavData.bufBlocks[sdCardWavData.blockNrForWriting].blockHasData) {
                    SDCard_SetNewState(SDCARD_STATE_WAIT_FREE_BUF_BLOCK); /* attendre qu'il soit libre */
                }
                readCnt++;
            }
            break;
            
        case SDCARD_STATE_WAIT_FREE_BUF_BLOCK:
            if (sdCardWavData.bufBlocks[sdCardWavData.blockNrForWriting].blockHasData)
                {
                    SDCard_SetNewState(SDCARD_STATE_WAIT_FREE_BUF_BLOCK); /* attendre qu'il soit libre */
                }
            else
            {
                waitCnt++;
                SDCard_SetNewState(SDCARD_STATE_READ_FILE_BLOCK); /* c'est libre, proceder a la lecture */
            }
            break;
            

        case SDCARD_STATE_CLOSE_FILE:
 //           LED5On();

            SYS_FS_FileClose(sdCardWavData.fileHandle);
             /* Successful. Lets idle. */
            sdCardWavData.readFileIsInProgress = false;
//            lcd_gotoxy(11, 4);
//            printf_lcd("%d, %d", readCnt, waitCnt);
            SDCard_SetNewState(SDCARD_STATE_WAIT_READ_REQUEST);
            break;

        case SDCARD_STATE_WAIT_READ_REQUEST:
            
            if (sdCardWavData.readFileIsRequested){
                SDCard_SetNewState(SDCARD_STATE_OPEN_FILE_TO_READ);
            }
            break;
            
        case SDCARD_STATE_ERROR:
            /* SDCard_WavReader has failed. Switch on the 'error LED'.*/
//            LED6Off();
            break;
        default:
            break;

    }

}   /* SDCard_WavReader_Tasks */



/*******************************************************************************
 End of File
 */

