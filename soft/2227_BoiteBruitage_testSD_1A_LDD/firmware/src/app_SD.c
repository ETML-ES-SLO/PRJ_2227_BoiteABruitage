/*******************************************************************************
  MPLAB Harmony Application Source File
  
  Company:
    Microchip Technology Inc.
  
  File Name:
    app_sd.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It 
    implements the logic of the application's state machine and it may call 
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

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

#include "app_sd.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.
    
    Application strings and buffers are be defined outside this structure.
*/

 APP_SD_DATA COHERENT_ALIGNED appDatasd;
 uint8_t Chose_file_toread = 0;


// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback functions.
*/


// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary local functions.
*/


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_SD_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
     appDatasd.state = APP_MOUNT_DISK;
     //DRV_TMR0_Initialize();

    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
}


/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_SD_Tasks ( void )
{
    /* The application task state machine */
    switch(appDatasd.state)
    {
        case APP_MOUNT_DISK:
            if(SYS_FS_Mount("/dev/mmcblka1", "/mnt/myDrive", FAT, 0, NULL) != 0)
            {
                /* The disk could not be mounted. Try
                 * mounting again untill success. */

                appDatasd.state = APP_MOUNT_DISK;
            }
            else
            {
                /* Mount was successful. Unmount the disk, for testing. */

                appDatasd.state = APP_SET_CURRENT_DRIVE;
            }
            break;
        case APP_SET_CURRENT_DRIVE:
            if(SYS_FS_CurrentDriveSet("/mnt/myDrive") == SYS_FS_RES_FAILURE)
            {
                /* Error while setting current drive */
                appDatasd.state = APP_ERROR;
            }
            else
            {
                /* Open a file for reading. */
                appDatasd.state = APP_IDLE;
            }

        case APP_OPEN_FILE:
            switch (Chose_file_toread)
            {
                case 0:
                appDatasd.fileHandle = SYS_FS_FileOpen("01.wav",
                (SYS_FS_FILE_OPEN_READ));
                    break;
                    case 1:
                appDatasd.fileHandle = SYS_FS_FileOpen("02.wav",
                (SYS_FS_FILE_OPEN_READ));
                    break;
                    case 2:
                appDatasd.fileHandle = SYS_FS_FileOpen("03.wav",
                (SYS_FS_FILE_OPEN_READ));
                    break;
                    case 3:
                appDatasd.fileHandle = SYS_FS_FileOpen("04.wav",
                (SYS_FS_FILE_OPEN_READ));
                    break;
                    case 4:
                appDatasd.fileHandle = SYS_FS_FileOpen("05.wav",
                (SYS_FS_FILE_OPEN_READ));
                    break;
                    case 5:
                appDatasd.fileHandle = SYS_FS_FileOpen("06.wav",
                (SYS_FS_FILE_OPEN_READ));
                    break;
                    case 6:
                appDatasd.fileHandle = SYS_FS_FileOpen("07.wav",
                (SYS_FS_FILE_OPEN_READ));
                    break;
                    case 7:
                appDatasd.fileHandle = SYS_FS_FileOpen("08.wav",
                (SYS_FS_FILE_OPEN_READ));
                    break;
                    case 8:
                appDatasd.fileHandle = SYS_FS_FileOpen("09.wav",
                (SYS_FS_FILE_OPEN_READ));
                    break;
                    case 9:
                appDatasd.fileHandle = SYS_FS_FileOpen("10.wav",
                (SYS_FS_FILE_OPEN_READ));
                    break;
                    default:

                    break;
                    
            }
            if(appDatasd.fileHandle == SYS_FS_HANDLE_INVALID)
            {
                /* Could not open the file. Error out*/
                appDatasd.state = APP_ERROR;
            }
            else
            {
                /* Create a directory. */
                appDatasd.state = APP_READ_FILE;
            }
            break;

        case APP_READ_FILE:

            if(SYS_FS_FileRead(appDatasd.fileHandle, (void *)appDatasd.data, 512) == -1)
            {
                /* There was an error while reading the file.
                 * Close the file and error out. */

                SYS_FS_FileClose(appDatasd.fileHandle);
                appDatasd.state = APP_ERROR;
            }
            else
            {
                /* If read was success, try writing to the new file */
                if(SYS_FS_FileWrite(appDatasd.fileHandle1, (const void *)appDatasd.data, 512) == -1)
                {
                    /* Write was not successful. Close the file
                     * and error out.*/
                    SYS_FS_FileClose(appDatasd.fileHandle1);
                    appDatasd.state = APP_ERROR;
                }
                else if(SYS_FS_FileEOF(appDatasd.fileHandle) == 1)    /* Test for end of file */
                {
                    /* Continue the read and write process, untill the end of file is reached */

                    appDatasd.state = APP_CLOSE_FILE;
                }
            }
            break;

        case APP_CLOSE_FILE:
            /* Close both files */
            SYS_FS_FileClose(appDatasd.fileHandle);
            SYS_FS_FileClose(appDatasd.fileHandle1);
             /* The test was successful. Lets idle. */
            appDatasd.state = APP_IDLE;
            break;

        case APP_IDLE:
            /* The appliction comes here when the demo
             * has completed successfully. Switch on
             * green LED. */
           // BSP_LEDOn(APP_SUCCESS_LED);
            break;
        case APP_ERROR:
            /* The appliction comes here when the demo
             * has failed. Switch on the red LED.*/
            //BSP_LEDOn(APP_FAILURE_LED);
            break;
        default:
            break;

    }

} //End of APP_Tasks
void APP_SD_State_Piste (APP_SD_STATES NewState,uint8_t chose_file)
{
 
    appDatasd.state = NewState;
    Chose_file_toread = chose_file;
}
 

/*******************************************************************************
 End of File
 */
