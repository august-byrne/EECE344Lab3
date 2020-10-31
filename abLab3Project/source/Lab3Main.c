/*******************************************************************************
* EECE344 Lab 2 Code
*	This program takes a high and a low address as inputs
*	and calculates the hash/checksum of the data between the
*	two addresses. It has error checking for the addresses, and
*	will hold after calculating the hash until the user presses enter.
* August Byrne, 10/19/2020
*******************************************************************************/
#include "MCUType.h"               /* Include header files                    */
#include "MemTest.h"
#include "BasicIO.h"
#include "K65TWR_ClkCfg.h"

#define STRGLEN 2
typedef enum {SW_CNT, HW_CNT, CO_CNT} COUNT_STATES;
COUNT_STATES CountingState;

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
        BIOPutStrg("Program entered loop\n\r");	//Program start of loop temp message
        is_valid = 0;
    	while (is_valid == 0){
    		BIOPutStrg("\n\rEnter the type of counter you want to implement\n\rEnter s for software counter, h for hardware counter, and c for combination counter: ");	//prompt message
    		in_err = BIOGetStrg(STRGLEN,in_strg);		//receiving the counter type
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

        	while('q' != char_in){
        		char_in = BIOGetChar();


        	}

        case HW_CNT:

        	while('q' != char_in){
        		char_in = BIOGetChar();


        	}

        case CO_CNT:

        	while('q' != char_in){
        		char_in = BIOGetChar();


        	}

        }


    }
}
