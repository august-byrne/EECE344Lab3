/*******************************************************************************
* EECE344 Lab 3 Code
*	This program .....
*
*
*
*
* August Byrne, 10/30/2020
*******************************************************************************/
#include "MCUType.h"               /* Include header files                    */
#include "MemTest.h"
#include "BasicIO.h"
#include "K65TWR_ClkCfg.h"
#include "MK65F18.h"
#include "K65TWR_GPIO.c"

#define STRGLEN 2

void PORTA_IRQHandler(void);

typedef enum {SW_CNT, HW_CNT, CO_CNT} COUNT_STATES;
COUNT_STATES CountingState;

INT8U PulseCnt;
int CountChanged;

void main(void){

	INT8C in_strg[STRGLEN];			//a character array representing a string
	INT32U hex_word;				//a hexadecimal word
    INT8C char_in;					//Received character
    INT8U in_err;					//Input error
    INT8U is_valid;					//must be 1 in order to move onto the next step in the program
    INT32U low_addr = 0x00000000;				//Received low address
    INT32U high_addr = 0x001FFFFF;				//Received high address

    K65TWR_BootClock();             /* Initialize MCU clocks                  */
    BIOOpen(BIO_BIT_RATE_115200);   /* Initialize Serial Port                 */

    BIOPutStrg("Program has started running\n\r");		//Program startup temp message

	BIOPutStrg("CS: ");				//program start initial checksum
	BIOOutHexWord(low_addr);
	BIOPutStrg("-");
	BIOOutHexWord(high_addr);
	BIOPutStrg("  ");
	INT16U math_val = CalcChkSum((INT8U *)low_addr,(INT8U *)high_addr);
	BIOOutHexHWord(math_val);
	BIOPutStrg("\n\r");

    while(1){
        BIOPutStrg("Program start of loop\n\r");	//Program start of loop temp message
        is_valid = 0;
    	while (is_valid == 0){
    		BIOPutStrg("\n\rEnter the type of PulseCnt you want to implement\n\rEnter s for software PulseCnt, h for hardware PulseCnt, and c for combination PulseCnt: ");	//prompt message
    		in_err = BIOGetStrg(STRGLEN,in_strg);		//receiving the PulseCnt type
    		if(in_err == 0){
    			switch(in_strg){
    			case 's':
    				is_valid = 1;
    				CountingState = SW_CNT;
    				break;
    			case 'h':
    				is_valid = 1;
    				CountingState = HW_CNT;
    				break;
    			case 'c':
    				is_valid = 1;
    				CountingState = SW_CNT;
    				break;
    			}
    			default:
    				BIOPutStrg("\n\rThat input is not an option. Try Again\n\r");	//prompt message
    		}else{
    			BIOPutStrg("\n\rThat input is not an option. Try Again\n\r");	//prompt message
    		}
    	}

    	char_in = 'a';
        switch(CountingState){
        case SW_CNT:

        	PulseCnt = 0;
    		GpioSw2Init(PORT_IRQ_FE); // init SW2 to detect a falling edge
    		SW2_CLR_ISF(); // clear interrupt flag

        	while('q' != char_in){
        		char_in = BIOGetChar();

        		//poll for button state


        		//if button pressed{
        			//if the button flag set to yes{ do nothing}
        			//else {
        				//add one to count

        				//BIOPut
        		BIOPutStrg('\r\r\r');
        		BIODecWord(PulseCnt,3,BIO_OD_MODE_LZ);
        				//set button pressed flag to yes
        		//}else{ set button flag to no}



        	}
        	break;
        case HW_CNT:
        	PulseCnt = 0;
        	CountChanged = 0;
        	NVIC_ClearPendingIRQ(PORTA_IRQn);
        	NVIC_EnableIRQ(PORTA_IRQn);
    		GpioSw2Init(PORT_IRQ_FE); // init SW2 to detect a falling edge
    		SW2_CLR_ISF(); // clear interrupt flag
        	while('q' != char_in){
        		char_in = BIOGetChar();
        		if(CountChanged == 1){
        			CountChanged = 0;
            		BIOPutStrg('\r\r\r');
            		BIODecWord(PulseCnt,3,BIO_OD_MODE_LZ);
        		}
        	}
        	GpioSw2Init(PORT_IRQ_OFF);
        	break;
        case CO_CNT:
        	PulseCnt = 0;
    		GpioSw2Init(PORT_IRQ_FE); // init SW2 to detect a falling edge
    		SW2_CLR_ISF(); // clear interrupt flag
        	while('q' != char_in){
        		char_in = BIOGetChar();
        		if(SW2_ISF != 0){ //check flag
        			SW2_CLR_ISF(); // Clear flag
        			PulseCnt++;
            		BIOPutStrg('\r\r\r');
            		BIODecWord(PulseCnt,3,BIO_OD_MODE_LZ);
        		}
        	}
        	break;
        default:
        	BIOPutStrg("\n\This input broke the program!\n\r");	//prompt message
        }

    }

}


void PORTA_IRQHandler(void){
if(SW2_ISF != 0x00){
SW2_CLR_ISF();
PulseCnt++;
CountChanged = 1;
}else{
//some other PORTA bit was set
}
}





