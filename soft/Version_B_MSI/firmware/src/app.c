/*******************************************************************************
  MPLAB Harmony Application Source File
  
  Company:
    Microchip Technology Inc.
  
  File Name:
    app.c

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

#include "app.h"

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

APP_DATAS appData;
uint8_t testValue = 0;
uint32_t ADC_Value = 0;
uint16_t Value_Calcul = 0;
uint16_t Value_ampl_max = 0;
uint16_t Value_ampl_min = 0;
bool change_sound = false;
S_ADCResults ADC_Result;
uint16_t Purcent_value;

bool chose_square_sin = false;
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

void APP_Initialize(void) {
    /* Place the App state machine in its initial state. */
    appData.state = APP_STATE_INIT;

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

void APP_Tasks(void) {

    int activeInput = GET_ACTIVE_INPUT();

    /* Check the application's current state. */
    switch (appData.state) {

            /* Application's initial state. */
        case APP_STATE_INIT:
        {
            /*first time in ?*/

            appData.state = APP_STATE_WAIT;

            /*Drivers init*/
            /*timers*/
            DRV_TMR0_Start();
            DRV_TMR1_Start();
            DRV_TMR2_Start();

            /*ADC*/
            InitADC10();

            /*SPI1*/
            CS_DACOn();
            Dac_Init();

            break;
        }

        case APP_STATE_WAIT:
        {
            //nothing to do
            break;
        }

        case APP_STATE_SERVICE_TASKS:
        {
            /*****   Appel de la machine d'état pour lecture 
                                    de fichiers .wav sur SD_Card   *****/
            SDCard_WavReader_Tasks();

            //*****   WAIT FOR CONTACT   *****//
            if (SDCard_IsFileReadingInProgress() == false) {
                switch (activeInput) {
                        switch (activeInput) {
                            case INPUT_1:
                                // Code pour Input 1
                                SDCard_RequestReadFile("myfile1.wav");
                                break;

                            case INPUT_2:
                                // Code pour Input 2
                                SDCard_RequestReadFile("myfile2.wav");
                                break;

                            case INPUT_3:
                                // Code pour Input 3
                                SDCard_RequestReadFile("myfile3.wav");
                                break;

                            case INPUT_4:
                                // Code pour Input 4
                                SDCard_RequestReadFile("myfile4.wav");
                                break;

                            case INPUT_5:
                                // Code pour Input 5
                                SDCard_RequestReadFile("myfile5.wav");
                                break;

                            case INPUT_6:
                                // Code pour Input 6
                                SDCard_RequestReadFile("myfile6.wav");
                                break;

                            case INPUT_7:
                                // Code pour Input 7
                                SDCard_RequestReadFile("myfile7.wav");
                                break;

                            case INPUT_8:
                                // Code pour Input 8
                                SDCard_RequestReadFile("myfile8.wav");
                                break;

                            case INPUT_9:
                                // Code pour Input 9
                                SDCard_RequestReadFile("myfile9.wav");
                                break;

                            case INPUT_10:
                                // Code pour Input 9
                                SDCard_RequestReadFile("myfile10.wav");
                                break;
                                
                            case SAMPLING:
                                SDCard_SetOutputSamplingRate_Hz(SAMPLING_RATE_MINIMUM);
                            default:
                                // Aucun input actif
                                break;
                        }
                }

                //appData.state = APP_STATE_WAIT;

                break;
            }
            /* TODO: implement your application state machine.*/


            /* The default state should never be executed. */
            default:
            {
                /* TODO: Handle error in application's state machine. */
                break;
            }
        }
    }
}


    /********************************************/
    /* Fonction UpdateState                     */
    /* Permet de changer l'état de la machine   */
    /* d'état                                   */

    /********************************************/
    void Update_AppTask(APP_STATE New_State) {
        appData.state = New_State;
    }


    /********************************************/
    /* Fonction GET_ACTIVE_INPUT                */
    /* Récupère l'état des entrées              */
    /*                                          */

    /********************************************/
    int GET_ACTIVE_INPUT(void) {

        if (Input_1StateGet()) {
            return INPUT_1;
        }
        if (Input_2StateGet()) {
            return INPUT_2;
        }
        if (Input_3StateGet()) {
            return INPUT_3;
        }
        if (Input_4StateGet()) {
            return INPUT_4;
        }
        if (Input_5StateGet()) {
            return INPUT_5;
        }
        if (Input_6StateGet()) {
            return INPUT_6;
        }
        if (Input_7StateGet()) {
            return INPUT_7;
        }
        if (Input_8StateGet()) {
            return INPUT_8;
        }
        if (Input_9StateGet()) {
            return INPUT_9;
        }
        if (Input_10StateGet()) {
            return INPUT_10;
        }

        return 0; // Aucun actif
    }





    /*******************************************************************************
     End of File
     */
