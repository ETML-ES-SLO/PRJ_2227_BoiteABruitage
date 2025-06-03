/*******************************************************************************
  MPLAB Harmony Application Header File

  Company:
    Microchip Technology Inc.

  File Name:
    SDCard_WavReader_JMO.h

  Summary:
    This header file provides prototypes and definitions for SDCard_WavReader_JMO.

  Description:
 * Ce header file fournit le function prototypes et data type definitions pour
 * acceder, lire des fichiers .WAV sur une carte SD.  Certaines sont necessaires
 * au systeme (comme "SDCard_WavReader_Initialize" et "SDCard_WavReader_Tasks")
 * et d'autres  sont utilisees en interne ou par l'app cliente).

*******************************************************************************/

//DOM-IGNORE-BEGIN
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
//DOM-IGNORE-END

#ifndef _SDCARD_WAVREADER_JMO_H
#define _SDCARD_WAVREADER_JMO_H


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "system_config.h"
#include "system_definitions.h"

// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************

//#ifdef DRV_SDHC_USE_DMA
//#define DATA_BUFFER_ALIGN             __attribute__((coherent, aligned(32)))
//#else
//#define DATA_BUFFER_ALIGN             __attribute__((aligned(32)))
//#endif

#define SDCARD_NROF_BUFF_BLOCKS         2
#define SDCARD_READ_BLOCK_SIZE          256
#define SDCARD_FILENAME_MAX_LENGTH      31

// *****************************************************************************
/* SDCard_WavReader States

  Summary:
    SDCard_WavReader states enumeration

  Description:
    This enumeration defines the valid SDCard_WavReader states.  These states
    determine the behavior of SDCard_WavReader.
*/

typedef enum
{
	/* SDCard_WavReader's state machine definitions. */
	
    SDCARD_STATE_MOUNT_DISK = 0,                /* mount the disk */
            SDCARD_STATE_UNMOUNT_DISK,          /* unmount the disk */
            SDCARD_STATE_SET_CURRENT_DRIVE,     /* Set the current drive */
            SDCARD_STATE_OPEN_FILE_TO_READ,     /* open the file to read */
            SDCARD_STATE_READ_WAVE_HEADER,      /* decode .WAV header */
            SDCARD_STATE_WAIT_FREE_BUF_BLOCK,   /* wait for free block before reading next from SD */
            SDCARD_STATE_READ_FILE_BLOCK,       /* block file read */
            SDCARD_STATE_DIRECTORY_FILE_SEARCH, /* Search File in directory */
            SDCARD_STATE_CLOSE_FILE,            /* close the file*/
            SDCARD_STATE_WAIT_END_SAMPLES_OUT,  /* wait for end of samples sent out */
            SDCARD_STATE_WAIT_READ_REQUEST,     /* wait request for starting a new reading */
            SDCARD_STATE_IDLE,                  /* close the file and idles */
            SDCARD_STATE_ERROR                  /* An app error has occurred */

} SDCARD_STATES;


// *****************************************************************************
/* SDCard_WavReader Data definitions
 */
typedef struct
{
    uint8_t riff[4];
    uint8_t completeFileSize[4];
    uint8_t typeHeader[4];
    uint8_t formatChunkMarker[4];
    uint8_t formatDataLength[4];
    uint8_t formatType[2];
    uint8_t numberOfChannels[2];
    uint8_t samplingRate[4];
    uint8_t bytesPerSecond[4];      // = samplingRate * bytesPerBloc
    uint8_t bytesPerBloc[2];        // = numberOfChannels * bitsPerSample/8
    uint8_t bitsPerSample[2];
    uint8_t data[4];
    uint8_t fileSizeData[4];    
} SDCARD_WAV_HEADER;

typedef struct
{
    int16_t         data[SDCARD_READ_BLOCK_SIZE];
    bool            blockHasData;
    uint16_t        readPos;
    
}SDCARD_WAVREADER_BUFFER_BLOCK;

typedef struct
{
    SDCARD_STATES   state;          /* SD Card's current state */
    SYS_FS_HANDLE   fileHandle;     /* SYS_FS File handle */
    SYS_FS_ERROR    fsError;        ///< pour l'identification d'erreurs du File System
    SDCARD_WAV_HEADER wavHeader;    ///< en-tete fichier .WAV
    
    /* read data buffer */
    uint16_t        data[256];
    SDCARD_WAVREADER_BUFFER_BLOCK   bufBlocks[SDCARD_NROF_BUFF_BLOCKS];
    uint32_t        blockNrForWriting;
    uint16_t        blockNrPlaying;
    
    bool            readFileIsRequested;
    bool            readFileIsInProgress;
    char            fileName[SDCARD_FILENAME_MAX_LENGTH];
    
} SDCARD_WAVREADER_DATA;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

void SDCARD_SamplesOutputCallback();

// *****************************************************************************
// *****************************************************************************
// Section: Interface Functions
// *****************************************************************************
// *****************************************************************************


void SDCard_RequestReadFile (char *fileName);
bool SDCard_IsFileReadingInProgress ();
void SDCard_SetOutputSamplingRate_Hz(uint32_t sFreq);
	
// *****************************************************************************
// *****************************************************************************
// Section: Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void SDCard_WavReader_Initialize ( void )

  Summary:
     Initialization routine

  Description:
    This routine initializes SDCard_WavReader. This function opens
    the necessary data and drivers.

  Precondition:
    All other system initialization routines should be called before calling
    this routine (in "SYS_Initialize").

  Parameters:
    None.

  Returns:
    None.
*/

void SDCard_WavReader_Initialize ( void );


/*******************************************************************************
  Function:
    void SDCard_WavReader_Tasks ( void )

  Summary:
    SDCard_WavReader tasks function

  Description:
    This routine is the SDCard_WavReader tasks function.  It
    defines the SDCard_WavReader's state machine and core logic.

  Remarks:
    This routine must be called from application service tasks state.
 */

void SDCard_WavReader_Tasks ( void );


#endif /* _SDCARD_WAVREADER_JMO_H */
/*******************************************************************************
 End of File
 */

