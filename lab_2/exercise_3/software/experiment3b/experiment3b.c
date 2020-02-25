// Copyright by Adam Kinsman, Henry Ko and Nicola Nicolici
// Developed for the Embedded Systems course (COE4DS4)
// Department of Electrical and Computer Engineering
// McMaster University
// Ontario, Canada

/* 
 * "Small Hello World" example. 
 * 
 * This example prints 'Hello from Nios II' to the STDOUT stream. It runs on
 * the Nios II 'standard', 'full_featured', 'fast', and 'low_cost' example 
 * designs. It requires a STDOUT  device in your system's hardware. 
 *
 * The purpose of this example is to demonstrate the smallest possible Hello 
 * World application, using the Nios II HAL library.  The memory footprint
 * of this hosted application is ~332 bytes by default using the standard 
 * reference design.  For a more fully featured Hello World application
 * example, see the example titled "Hello World".
 *
 * The memory footprint of this example has been reduced by making the
 * following changes to the normal "Hello World" example.
 * Check in the Nios II Software Developers Manual for a more complete 
 * description.
 * 
 * In the SW Application project (small_hello_world):
 *
 *  - In the C/C++ Build page
 * 
 *    - Set the Optimization Level to -Os
 * 
 * In System Library project (small_hello_world_syslib):
 *  - In the C/C++ Build page
 * 
 *    - Set the Optimization Level to -Os
 * 
 *    - Define the preprocessor option ALT_NO_INSTRUCTION_EMULATION 
 *      This removes software exception handling, which means that you cannot 
 *      run code compiled for Nios II cpu with a hardware multiplier on a core 
 *      without a the multiply unit. Check the Nios II Software Developers 
 *      Manual for more details.
 *
 *  - In the System Library page:
 *    - Set Periodic system timer and Timestamp timer to none
 *      This prevents the automatic inclusion of the timer driver.
 *
 *    - Set Max file descriptors to 4
 *      This reduces the size of the file handle pool.
 *
 *    - Check Main function does not exit
 *    - Uncheck Clean exit (flush buffers)
 *      This removes the unneeded call to exit when main returns, since it
 *      won't.
 *
 *    - Check Don't use C++
 *      This builds without the C++ support code.
 *
 *    - Check Small C library
 *      This uses a reduced functionality C library, which lacks  
 *      support for buffering, file IO, floating point and getch(), etc. 
 *      Check the Nios II Software Developers Manual for a complete list.
 *
 *    - Check Reduced device drivers
 *      This uses reduced functionality drivers if they're available. For the
 *      standard design this means you get polled UART and JTAG UART drivers,
 *      no support for the LCD driver and you lose the ability to program 
 *      CFI compliant flash devices.
 *
 *    - Check Access device drivers directly
 *      This bypasses the device file system to access device drivers directly.
 *      This eliminates the space required for the device file system services.
 *      It also provides a HAL version of libc services that access the drivers
 *      directly, further reducing space. Only a limited number of libc
 *      functions are available in this configuration.
 *
 *    - Use ALT versions of stdio routines:
 *
 *           Function                  Description
 *        ===============  =====================================
 *        alt_printf       Only supports %s, %x, and %c ( < 1 Kbyte)
 *        alt_putstr       Smaller overhead than puts with direct drivers
 *                         Note this function doesn't add a newline.
 *        alt_putchar      Smaller overhead than putchar with direct drivers
 *        alt_getchar      Smaller overhead than getchar with direct drivers
 *
 */

#include "io.h"
#include "system.h"
#include <unistd.h>
#include "alt_types.h"
#include "sys/alt_stdio.h"
#include "sys/alt_irq.h"
#include "altera_up_avalon_character_lcd.h"

alt_u16 disp_seven_seg(alt_u8 val) {
	switch (val) {
    	case  0 : return 0x40;
    	case  1 : return 0x79;
    	case  2 : return 0x24;
    	case  3 : return 0x30;
    	case  4 : return 0x19;
    	case  5 : return 0x12;
    	case  6 : return 0x02;
    	case  7 : return 0x78;
    	case  8 : return 0x00;
    	case  9 : return 0x18;
    	case 10 : return 0x08;
    	case 11 : return 0x03;
    	case 12 : return 0x46;
    	case 13 : return 0x21;
    	case 14 : return 0x06;
    	case 15 : return 0x0e;
    	default : return 0x7f;
	}
}

// Group A interrupt handling function definition
void SW_GRPA_interrupt(int *amount) {
	alt_u32 sw_grp_a;
	IOWR(LED_GREEN_O_BASE,0,0x1);
	sw_grp_a = IORD(SWITCH_GRPA_I_BASE, 3);

	// Pass the value by dereferencing amount pointer
	if((sw_grp_a & 0x1) == 0x1) {
    	*amount = *amount + 0x1;
	}
	if((sw_grp_a & 0x2) == 0x2) {
    	*amount = *amount + 0x2;
	}
	if((sw_grp_a & 0x4) == 0x4) {
    	*amount = *amount + 0x5;
	}
	if((sw_grp_a & 0x8) == 0x8) {
    	*amount = *amount + 0xa;
	}
	alt_printf("%x \n",*amount);
	IOWR(SWITCH_GRPA_I_BASE, 3, 0x0);
}

// Group B interrupt handling function definition
void SW_GRPB_interrupt(int *amount) {
	alt_u32 sw_grp_b;
	sw_grp_b = IORD(SWITCH_GRPB_I_BASE, 3);
	IOWR(LED_GREEN_O_BASE,0,0x0);
	if((sw_grp_b & 0x1) == 0x1) {
    	if (*amount >= 0x29){
    		// If amount left is enough to go to Montreal
        	*amount = *amount - 0x29;
			IOWR(LED_GREEN_O_BASE,0,0x4);
			IOWR(SEVEN_SEGMENT_N_O_0_BASE, 0, disp_seven_seg((*amount)%10));
			IOWR(SEVEN_SEGMENT_N_O_1_BASE, 0, disp_seven_seg((*amount)/10));
			usleep(2000000);
			IOWR(LED_GREEN_O_BASE,0,0x0);
			IOWR(SEVEN_SEGMENT_N_O_0_BASE, 0, 0x7f);
			IOWR(SEVEN_SEGMENT_N_O_1_BASE, 0, 0x7f);
			alt_printf("%x \n",*amount);
			*amount = 0x0;
		}
    	else {
    		//LED3 FOR 1.5 SECONDS
			IOWR(LED_GREEN_O_BASE,0,0x8);
    		alt_putstr("YOU ARE POOR, PLEASE BE RICH THEN COME. \n");
    		usleep(1500000);
			IOWR(LED_GREEN_O_BASE,0,0x0);
			
    	}
	}
    if((sw_grp_b & 0x2) == 0x2) {
		if (*amount >= 0x20){
			*amount = *amount - 0x20;
			IOWR(LED_GREEN_O_BASE,0,0x4);
			IOWR(SEVEN_SEGMENT_N_O_0_BASE, 0, disp_seven_seg((*amount)%10));
			IOWR(SEVEN_SEGMENT_N_O_1_BASE, 0, disp_seven_seg((*amount)/10));
			usleep(2000000);
			IOWR(LED_GREEN_O_BASE,0,0x0);
			IOWR(SEVEN_SEGMENT_N_O_0_BASE, 0, 0x7f);
			IOWR(SEVEN_SEGMENT_N_O_1_BASE, 0, 0x7f);
			alt_printf("%x \n",*amount);
			*amount = 0x0;
		}	
		else {//LED3 FOR 1.5 SECONDS
			IOWR(LED_GREEN_O_BASE,0,0x8);
			alt_putstr("YOU ARE POOR, PLEASE BE RICH THEN COME. \n");
			usleep(1500000);
			IOWR(LED_GREEN_O_BASE,0,0x0);
		}	
	}	
    if((sw_grp_b & 0x4) == 0x4) {
		if (*amount >= 0xf){
        	*amount = *amount - 0xf;
			IOWR(LED_GREEN_O_BASE,0,0x4);
			IOWR(SEVEN_SEGMENT_N_O_0_BASE, 0, disp_seven_seg((*amount)%10));
			IOWR(SEVEN_SEGMENT_N_O_1_BASE, 0, disp_seven_seg((*amount)/10));
			usleep(2000000);
			IOWR(LED_GREEN_O_BASE,0,0x0);
			IOWR(SEVEN_SEGMENT_N_O_0_BASE, 0, 0x7f);
			IOWR(SEVEN_SEGMENT_N_O_1_BASE, 0, 0x7f);
			alt_printf("%x \n",*amount);
			*amount = 0x0;
		}
        else {//LED3 FOR 1.5 SECONDS
        	IOWR(LED_GREEN_O_BASE,0,0x8);
        	alt_putstr("YOU ARE POOR, PLEASE BE RICH THEN COME. \n");
			usleep(1500000);
			IOWR(LED_GREEN_O_BASE,0,0x0);
		}
	}
	if((sw_grp_b & 0x8) == 0x8) {
    *amount = 0x0;
		IOWR(LED_GREEN_O_BASE,0,0x2);
		usleep(2500000);
		IOWR(LED_GREEN_O_BASE,0,0x0);
		alt_printf("%x \n",*amount);
	}

	//alt_printf("Switches GRPB changed\n");
	IOWR(SWITCH_GRPB_I_BASE, 3, 0x0);
}

int main()
{ 
	alt_up_character_lcd_dev *lcd_0;
	volatile int amount = 0;
	IOWR(SEVEN_SEGMENT_N_O_0_BASE, 0, 0x7f);
	IOWR(SEVEN_SEGMENT_N_O_1_BASE, 0, 0x7f);
	alt_printf("exercise 3:\n");
    
	lcd_0 = alt_up_character_lcd_open_dev(CHARACTER_LCD_0_NAME);
    
	if (lcd_0 == NULL) alt_printf("Error opening LCD device\n");
	else alt_printf("LCD device opened.\n");

	alt_up_character_lcd_init(lcd_0);
	
	alt_up_character_lcd_string(lcd_0, "Exercise 3");
	
	alt_up_character_lcd_set_cursor_pos(lcd_0, 0, 1);
	
	alt_up_character_lcd_string(lcd_0, "Welcome");
    
	// Group A interrupt
	IOWR(SWITCH_GRPA_I_BASE, 3, 0x0); // edge capture register
	IOWR(SWITCH_GRPA_I_BASE, 2, 0xF); // IRQ mask
	alt_irq_register(SWITCH_GRPA_I_IRQ, &amount, (void*)SW_GRPA_interrupt);

	// Group B interrupt
	IOWR(SWITCH_GRPB_I_BASE, 3, 0x0); // edge capture register
	IOWR(SWITCH_GRPB_I_BASE, 2, 0xF); // IRQ mask
	alt_irq_register(SWITCH_GRPB_I_IRQ, &amount, (void*)SW_GRPB_interrupt);

	/* Event loop never exits. */
	while (1) {
	}

	return 0;
}
