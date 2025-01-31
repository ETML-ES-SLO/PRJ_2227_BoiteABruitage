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
#include "app_SD.h"
#include "dac_ad5620.h"

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

void APP_Initialize ( void )
{
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

void APP_Tasks ( void )
{

    /* Check the application's current state. */
    switch ( appData.state )
    {
        
        /* Application's initial state. */
        case APP_STATE_INIT:      
        {
            Dac_Init();
//            bool appInitialized = true;
//            
//            DRV_SPI1_Initialize();
            
//            CS_DACOn();
//            
//            if (appInitialized)
//            {
//            
            DRV_TMR0_Start();
            DRV_ADC_Initialize();
            appData.state = APP_STATE_WAIT;
            DRV_ADC_Open();
            Value_ampl_max = 2500;
            Value_ampl_min = 0;
//            }
            break;
        }

        case APP_STATE_SERVICE_TASKS:
        {
            
            
            
//            if(Input_1StateGet() != 0)
//            {
//                DRV_TMR1_PeriodValueSet(DO);
//                change_sound = true;
//            }
//            
//            if(Input_2StateGet() != 0)
//            {
//                DRV_TMR1_PeriodValueSet(RE);
//                change_sound = true;
//            }
//            
//            if(Input_3StateGet() != 0)
//            {
//                DRV_TMR1_PeriodValueSet(MI);
//                change_sound = true;
//            }
//            if(Input_4StateGet() != 0)
//            {
//                DRV_TMR1_PeriodValueSet(FA);
//                change_sound = true;
//            }
//            if(Input_5StateGet() != 0)
//            {
//                DRV_TMR1_PeriodValueSet(SOL);
//                change_sound = true;
//            }
//            if(Input_6StateGet() != 0)
//            {
//                DRV_TMR1_PeriodValueSet(LA);
//                change_sound = true;
//                
//            }
//            if(Input_7StateGet() != 0)
//            {
//                DRV_TMR1_PeriodValueSet(SI);
//                change_sound = true;
//            }
//            
//            if(Input_8StateGet() != 0)
//            {
//                DRV_TMR1_PeriodValueSet(DO2);
//                change_sound = true;
//            }
//            
//            if(Input_9StateGet() != 0)
//            {
//                DRV_TMR1_PeriodValueSet(RE2);
//                change_sound = true;
//            }
//            if(Input_10StateGet() != 0)
//            {
//                DRV_TMR1_PeriodValueSet(MI2);
//                change_sound = true;
//            }
//            
            
            if( change_sound == true)
            {
//                DRV_ADC_Start();
//                
//                ADC_Value = DRV_ADC_SamplesRead(0);
//                Value_Calcul = ADC_Value;
//
//
//                ADC_Value = (ADC_Value / 1023) * 100;
//
//                Value_ampl_max =1250 + ((ADC_Value * 25)/2);
//                Value_ampl_min =1250 - ((ADC_Value * 25)/2);
//                if (DRV_ADC_SamplesAvailable() == false)
//                {
//                    change_sound = false;
//                }
            }
            
            appData.state = APP_STATE_WAIT;
            break; 
        }   
        
        case APP_STATE_WAIT:
        {

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

void Update_AppTask (APP_STATE New_State)
{
    appData.state = New_State;
}


 

/*******************************************************************************
 End of File
 */
