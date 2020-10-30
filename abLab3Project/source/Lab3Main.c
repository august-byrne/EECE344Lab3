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

#define STRGLEN 9

void main(void){

	INT8C in_strg[STRGLEN];			//a character array representing a string
	INT32U hex_word;				//a hexadecimal word
    INT8C char_in;					//Received character
    INT8U in_err;					//Input error
    INT8U is_valid;					//must be 1 in order to move onto the next step in the program
    INT32U low_addr;				//Received low address
    INT32U high_addr;				//Received high address

    K65TWR_BootClock();             /* Initialize MCU clocks                  */
    BIOOpen(BIO_BIT_RATE_115200);   /* Initialize Serial Port                 */

    BIOPutStrg("Program has started running\n\r");		//Program startup message
    while(1){
        BIOPutStrg("Hash checking program\n\r");	//Program start of loop message
        is_valid = 0;
    	while (is_valid == 0){
    		BIOPutStrg("\n\rEnter the Low Address (32-bit hex address): ");	//prompt message
    		in_err = BIOGetStrg(STRGLEN,in_strg);		//receiving the low_address
    		if(in_err == 0){
    			in_err = BIOHexStrgtoWord(in_strg, &hex_word);       /*Convert string to hex word */
    			if(in_err == 0){
    				low_addr = hex_word;
    				is_valid = 1;
    			}else{
    				BIOPutStrg("\n\rLow Address is incorrect (null or has non-hex characters). Try Again\n\r");	//prompt message
    			}
    		}else{
    			BIOPutStrg("\n\rLow Address exceeded allowed length. Try Again\n\r");	//prompt message
    		}
    	}

    	is_valid = 0;
    	while (is_valid == 0){
    		BIOPutStrg("\n\rEnter the High Address (32-bit hex address): ");	//prompt message
    		in_err = BIOGetStrg(STRGLEN,in_strg);		//receiving the high_address
    		if(in_err == 0){
				in_err = BIOHexStrgtoWord(in_strg, &hex_word);       /*Convert string to hex word */
				if(in_err == 0){
					high_addr = hex_word;
					is_valid = 1;
				}else{
    				BIOPutStrg("\n\rHigh Address is incorrect (null or has non-hex characters). Try Again\n\r");	//prompt message
    			}
    		}else{
    		    BIOPutStrg("\n\rHigh Address exceeded allowed length. Try Again\n\r");	//prompt message
    		}
    	}

    	if (low_addr <= high_addr){
    		BIOPutStrg("\n\rCS: ");	//prompt message
    		BIOOutHexWord(low_addr);
    		BIOPutStrg("-");
    		BIOOutHexWord(high_addr);
    		BIOPutStrg("  ");
    		INT16U math_val = CalcChkSum((INT8U *)low_addr,(INT8U *)high_addr);
    		BIOOutHexHWord(math_val);

			BIOPutStrg("\n\rPress Enter to check a new hash\n\r");	//prompt message
			is_valid = 0;
			while (is_valid == 0){
				char_in = BIOGetChar();		//receiving a character
				if(char_in == '\r'){
					is_valid = 1;
				}
			}
    	}else{
    		BIOPutStrg("Your Low Address points to a higher memory address than your High Address\n\rPlease try again\n\r");	//prompt message
    	}

    }
}
