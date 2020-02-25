// Copyright by Adam Kinsman, Henry Ko and Nicola Nicolici
// Developed for the Embedded Systems course (COE4DS4)
// Department of Electrical and Computer Engineering
// McMaster University
// Ontario, Canada

// This is the embedded software for the
// LCD / Camera design

#include <stdio.h>
#include "io.h"
#include "system.h"
#include "sys/alt_irq.h"
#include "sys/alt_stdio.h"
#include "priv/alt_busy_sleep.h"

int combine_key3(int arr[6][10], int base, int config);

int combine_key3(int arr[6][10], int base, int config){
	int i;
	int var = 0, total_var = 0;
	for (i = base; i < base + 4; i++){
		var = arr[config][i] & 0xFF;
		total_var = (total_var << 8) | var;
	}
	return total_var;
}


void TouchPanel_int(void) {
    static int exposure = 0x0400, run = 1;
    int coeff[6][10] = {{-1, -1, -1, 0, 0, 0, 1, 1, 1, 16},
    		{-1, 0, 1, -1, 0, 1, -1, 0, 1, 32},
    		{-2, -2, -2, 0, 0, 0, 2, 2, 2, 48},
    		{-2, 0, 2, -2, 0, 2, -2, 0, 2, 64},
    		{-1, -3, -1, 0, 0, 0, 1, 3, 1, 80},
    		{-1, 0, 1, -3, 0, 3, -1, 0, 1, 96}};

    static int config = 0;
	static int key3_config = 0;
    int TP_val, x_val, y_val, key = 6;
    int key3_p1, key3_p2, key3_p3;

    TP_val = IORD(NIOS_LCD_CAMERA_COMPONENT_0_TOUCHPANEL_BASE, 0);
    x_val = (TP_val >> 20) & 0xFF; y_val = (TP_val >> 4) & 0xFF;

    if (((TP_val >> 31) & 0x1) && (x_val >= 0xC9) && (x_val <= 0xF1)) {
        if ((y_val >= 0x17) && (y_val <= 0x33)) { // Key 0
            key = 0;
            IOWR(NIOS_LCD_CAMERA_COMPONENT_0_CONSOLE_BASE, 0, 0x1);
        }
        if ((y_val >= 0x3D) && (y_val <= 0x58)) { // Key 1
            key = 1;
            IOWR(NIOS_LCD_CAMERA_COMPONENT_0_CONSOLE_BASE, 0, 0x2);
        }
        if ((y_val >= 0x62) && (y_val <= 0x7E)) { // Key 2
            key = 2;
            IOWR(NIOS_LCD_CAMERA_COMPONENT_0_CONSOLE_BASE, 0, 0x4);
        }
        if ((y_val >= 0x88) && (y_val <= 0xA4)) { // Key 3
            key = 3;
            IOWR(NIOS_LCD_CAMERA_COMPONENT_0_CONSOLE_BASE, 0, 0x8);
        }
        if ((y_val >= 0xAE) && (y_val <= 0xC9)) { // Key 4
            key = 4;
            IOWR(NIOS_LCD_CAMERA_COMPONENT_0_CONSOLE_BASE, 0, 0x10);
        }
        if ((y_val >= 0xD3) && (y_val <= 0xEF)) { // Key 5
            key = 5;
            IOWR(NIOS_LCD_CAMERA_COMPONENT_0_CONSOLE_BASE, 0, 0x20);
        }
    } else IOWR(NIOS_LCD_CAMERA_COMPONENT_0_CONSOLE_BASE, 0, 0x0);
    
    if (IORD(NIOS_LCD_CAMERA_COMPONENT_0_TOUCHPANEL_BASE, 2) & 0x2) { // posedge
        switch (key) {
            case 0 : 
                if (run == 1) {
                    IOWR(NIOS_LCD_CAMERA_COMPONENT_0_CAMERA_BASE, 1, 0x8);
                    run = 0;
                } else {
                    IOWR(NIOS_LCD_CAMERA_COMPONENT_0_CAMERA_BASE, 1, 0x4);
                    run = 1;
                }
                break;
            case 1 : 
                config++; if (config == 4) config = 0;
                IOWR(NIOS_LCD_CAMERA_COMPONENT_0_IMAGELINE_BASE, 4, config);
                break;
            case 2 : 
                IOWR(NIOS_LCD_CAMERA_COMPONENT_0_IMAGELINE_BASE, 4, 4);
                break;
            case 3 :
            	key3_config++; if (key3_config == 6) key3_config = 0;
            	IOWR(NIOS_LCD_CAMERA_COMPONENT_0_IMAGELINE_BASE, 4, 5);

            	///combine key3_p1
            	key3_p1 = combine_key3(coeff, 0, key3_config);
            	key3_p2 = combine_key3(coeff, 4, key3_config);
            	key3_p3 = (coeff[key3_config][8]&0xFF)<<8 | (coeff[key3_config][9]&0xFF);
            	printf("configure = %d\n",key3_config);
            	printf("1 part = %x, 2 part = %x, 3 part = %x\n",key3_p1, key3_p2, key3_p3);

            	switch (key3_config) {
            	case 0 :
            		IOWR(NIOS_LCD_CAMERA_COMPONENT_0_IMAGELINE_BASE, 5, key3_p1);
            		IOWR(NIOS_LCD_CAMERA_COMPONENT_0_IMAGELINE_BASE, 6, key3_p2);
            		IOWR(NIOS_LCD_CAMERA_COMPONENT_0_IMAGELINE_BASE, 7, key3_p3);
            		break;
            	case 1 :
            		IOWR(NIOS_LCD_CAMERA_COMPONENT_0_IMAGELINE_BASE, 5, key3_p1);
            		IOWR(NIOS_LCD_CAMERA_COMPONENT_0_IMAGELINE_BASE, 6, key3_p2);
            		IOWR(NIOS_LCD_CAMERA_COMPONENT_0_IMAGELINE_BASE, 7, key3_p3);
            		break;
            	case 2 :
            		IOWR(NIOS_LCD_CAMERA_COMPONENT_0_IMAGELINE_BASE, 5, key3_p1);
            		IOWR(NIOS_LCD_CAMERA_COMPONENT_0_IMAGELINE_BASE, 6, key3_p2);
            		IOWR(NIOS_LCD_CAMERA_COMPONENT_0_IMAGELINE_BASE, 7, key3_p3);
            		break;
            	case 3 :
            		IOWR(NIOS_LCD_CAMERA_COMPONENT_0_IMAGELINE_BASE, 5, key3_p1);
            		IOWR(NIOS_LCD_CAMERA_COMPONENT_0_IMAGELINE_BASE, 6, key3_p2);
            		IOWR(NIOS_LCD_CAMERA_COMPONENT_0_IMAGELINE_BASE, 7, key3_p3);
            		break;
            	case 4 :
            		IOWR(NIOS_LCD_CAMERA_COMPONENT_0_IMAGELINE_BASE, 5, key3_p1);
            		IOWR(NIOS_LCD_CAMERA_COMPONENT_0_IMAGELINE_BASE, 6, key3_p2);
            		IOWR(NIOS_LCD_CAMERA_COMPONENT_0_IMAGELINE_BASE, 7, key3_p3);
            		break;
            	case 5 :
            		IOWR(NIOS_LCD_CAMERA_COMPONENT_0_IMAGELINE_BASE, 5, key3_p1);
            		IOWR(NIOS_LCD_CAMERA_COMPONENT_0_IMAGELINE_BASE, 6, key3_p2);
            		IOWR(NIOS_LCD_CAMERA_COMPONENT_0_IMAGELINE_BASE, 7, key3_p3);
            		break;
            	}
                break;
            case 4 : 
                if (exposure <= 0xFEFF) exposure += 0x0100;
                IOWR(NIOS_LCD_CAMERA_COMPONENT_0_CAMERA_BASE, 0, exposure);
                IOWR(NIOS_LCD_CAMERA_COMPONENT_0_CAMERA_BASE, 1, 0x2);
                break;                              
            case 5 : 
                if (exposure >= 0x0100) exposure -= 0x0100;
                IOWR(NIOS_LCD_CAMERA_COMPONENT_0_CAMERA_BASE, 0, exposure);
                IOWR(NIOS_LCD_CAMERA_COMPONENT_0_CAMERA_BASE, 1, 0x2);
                break;
        }
    }
    
    TP_val = IORD(NIOS_LCD_CAMERA_COMPONENT_0_TOUCHPANEL_BASE, 2);
    IOWR(NIOS_LCD_CAMERA_COMPONENT_0_TOUCHPANEL_BASE, 2, TP_val & 0x30);
}

int main() 
{
	alt_irq_register(NIOS_LCD_CAMERA_COMPONENT_0_TOUCHPANEL_IRQ, NULL, (void *)TouchPanel_int);

    printf("Experiment 3!\n");

    // initialize the touch panel
    IOWR(NIOS_LCD_CAMERA_COMPONENT_0_TOUCHPANEL_BASE, 2, 0x0);
    IOWR(NIOS_LCD_CAMERA_COMPONENT_0_TOUCHPANEL_BASE, 1, 0x400000);

    // initialize the camera
    IOWR(NIOS_LCD_CAMERA_COMPONENT_0_CAMERA_BASE, 0, 0x0400);
    IOWR(NIOS_LCD_CAMERA_COMPONENT_0_CAMERA_BASE, 1, 0x2);
    while ((IORD(NIOS_LCD_CAMERA_COMPONENT_0_CAMERA_BASE, 1) & 0x1) == 0);
    IOWR(NIOS_LCD_CAMERA_COMPONENT_0_CAMERA_BASE, 1, 0x4);

    // initialize the buttons
    IOWR(NIOS_LCD_CAMERA_COMPONENT_0_CONSOLE_BASE, 1, 0x0);
    
    // initialize the filter pipe
    IOWR(NIOS_LCD_CAMERA_COMPONENT_0_IMAGELINE_BASE, 4, 0);

    while (1);
    
    return 0;
}
