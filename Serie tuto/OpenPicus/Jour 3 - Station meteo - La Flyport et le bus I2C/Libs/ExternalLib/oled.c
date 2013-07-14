/* **************************************************************************																					
 *                                OpenPicus                 www.openpicus.com
 *                                                            italian concept
 * 
 *            openSource wireless Platform for sensors and Internet of Things	
 * **************************************************************************
 *  FileName:        oled.c
 *  Module:          FlyPort WI-FI - FlyPort ETH
 *  Compiler:        Microchip C30 v3.12 or higher
 *
 *  Author               Rev.    Date              Comment
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Vicca Davide	     1.0     2/09/2012		   First release  
 *  
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 *  Software License Agreement
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  This is free software; you can redistribute it and/or modify it under
 *  the terms of the GNU General Public License (version 2) as published by 
 *  the Free Software Foundation AND MODIFIED BY OpenPicus team.
 *  
 *  ***NOTE*** The exception to the GPL is included to allow you to distribute
 *  a combined work that includes OpenPicus code without being obliged to 
 *  provide the source code for proprietary components outside of the OpenPicus
 *  code. 
 *  OpenPicus software is distributed in the hope that it will be useful, but 
 *  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 *  more details. 
 * 
 * 
 * Warranty
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * THE SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT
 * WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
 * LIMITATION, ANY WARRANTY OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * WE ARE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF
 * PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY OR SERVICES, ANY CLAIMS
 * BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE
 * THEREOF), ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION, OR OTHER
 * SIMILAR COSTS, WHETHER ASSERTED ON THE BASIS OF CONTRACT, TORT
 * (INCLUDING NEGLIGENCE), BREACH OF WARRANTY, OR OTHERWISE.
 *
 **************************************************************************/

#define __OLED_C
 
#include "taskFlyport.h"
#include "grovelib.h"
#include "oled.h"
#include <stdlib.h>

struct Interface *attachSensorToI2CBus(void *,int,int);
int set_register(BYTE,BYTE, BYTE);
BYTE read_register(BYTE,BYTE);

struct Oled
{
	const void *class;
	struct Interface *inter;
	BYTE devaddress; //already shifted;
};

static int setXY(int address,unsigned char row,unsigned char col)
{
	if(set_register(address,COMMAND_MODE,0xb0+row))
		return -1;
	if(set_register(address,COMMAND_MODE,0x00+(8*col&0x0f)))
		return -1;
	return set_register(address,COMMAND_MODE,0x10+((8*col>>4)&0x0f));
}

static int SendChar(int address,unsigned char data) // begin transmitting
{
	return set_register(address,DATA_MODE,data);
}
static int reset(int address)
{
	int k,i;
	for(k=0;k<8;k++)
	{		
		if(setXY(address,k,0))
			return -1;
		for(i=0;i<128;i++)//clear all COL
		{
			if(SendChar(address,0))
				return -1;
			//Delay10us(5);
		}
	}
	return 0;
}

static int init(struct Oled *self)
{
	//Set display OFF
	if(set_register(self->devaddress,COMMAND_MODE,DISPLAY_OFF))//Reset Mode register
		return -1;
	//Delay10us(1);
	//Set the default colour display
	if(set_register(self->devaddress,COMMAND_MODE,DEFAULT_COLOUR))//Reset Mode register
		return -1;
	//Delay10us(1);
	//Set display ON
	if(set_register(self->devaddress,COMMAND_MODE,DISPLAY_ON))//Reset Mode register
		return -1;
	//Delay10us(1);
	//set memory addressing
	if(set_register(self->devaddress,COMMAND_MODE,SET_MEMORY))//Reset memory addressing
		return -1;
	//Delay10us(1);

	return reset(self->devaddress);
}


static void *Oled_ctor (void * _self, va_list *app)
{
	struct Oled *self = _self;
	self->devaddress =  va_arg(*app, const BYTE);
	self->inter = NULL;
	return self;
}

static void Oled_dtor (void * _sensor)
{
}


static void *Oled_attach (void * _board,void *_sensor,int ic2bus)
{
	struct Oled *sensor = _sensor;
	int add = sensor->devaddress;
	sensor->inter = attachSensorToI2CBus(_board,ic2bus,add);
	return sensor->inter;
}


static int Oled_config (void * _self, va_list *app)
{
	struct Oled *self = _self;
	return init(self);	
}


static int Oled_set (void * _self, va_list *app)
{
	struct Oled *self = _self;
	char *text =  va_arg(*app, char *);
	BYTE type =  va_arg(*app, const BYTE);	
	BYTE bkground_colour =  va_arg(*app, const BYTE);	
	BYTE x =  va_arg(*app, const BYTE);
	BYTE y =  va_arg(*app, const BYTE);
	if(set_register(self->devaddress,COMMAND_MODE,bkground_colour))//Reset Mode register
		return -1;
	
	if(type != IMAGE)
	{
		setXY(self->devaddress,x,y);
		int i;
		while(*text)
		{
			for(i=0;i<8;i++)
			{
				SendChar(self->devaddress,myFont[(*text-0x20)][i]);
			}			
			text++;
		}
	}
	else
	{
		int dim =  va_arg(*app, const int);
		int i = 0;
		//set memory addressing
		if(set_register(self->devaddress,COMMAND_MODE,0x00))//Reset memory addressing
			return -1;
		if(set_register(self->devaddress,COMMAND_MODE,x))//Reset memory addressing
			return -1;
		if(set_register(self->devaddress,COMMAND_MODE,y))//Reset memory addressing
			return -1;
		for(i=0;i<dim;i++)
		{
			SendChar(self->devaddress,*text);
			text++;

		}
	}	
			
	
	return 0;
}

static const struct SensorClass _Oled =
{	
	sizeof(struct Oled),
	Oled_ctor,
	Oled_dtor,
	Oled_attach,
	Oled_config,
	Oled_set,
	0,
};

const void *Oled = &_Oled;



