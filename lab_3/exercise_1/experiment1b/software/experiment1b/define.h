// Copyright by Adam Kinsman, Henry Ko and Nicola Nicolici
// Developed for the Embedded Systems course (COE4DS4)
// Department of Electrical and Computer Engineering
// McMaster University
// Ontario, Canada

#ifndef	  __define_H__
#define	  __define_H__

#include <io.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "sys/alt_alarm.h"
#include "alt_types.h"
#include "system.h"
#include <sys/alt_irq.h>
#include "PB_button.h"
#include "custom_counter.h"
#include "switch.h"

#endif

#define ARRAY_SIZE_MAX 12

typedef struct {
	int door_open;
	int elevator_move;
	int hold;
} counter;

typedef struct {
	int des_floor;
	int cur_floor;
	int direction;
	int initial;
	int state;
	int hold_1;
	int led;
} floor_P;
