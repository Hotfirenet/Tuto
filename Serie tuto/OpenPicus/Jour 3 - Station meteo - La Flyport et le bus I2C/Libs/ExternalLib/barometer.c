/** \file barometer.c
 *  \brief Grove devices support library 
 */

/**
\addtogroup Grove devices
@{
*/
/* **************************************************************************																					
 *                                OpenPicus                 www.openpicus.com
 *                                                            italian concept
 * 
 *            openSource wireless Platform for sensors and Internet of Things	
 * **************************************************************************
 *  FileName:        barometer.c
 *  Dependencies:    OpenPicus libraries
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
#include "taskFlyport.h"
#include "grovelib.h"
#include "barometer.h"


struct Interface *attachSensorToI2CBus(void *,int,int);
int set_register(BYTE,BYTE, BYTE);
BYTE read_register(BYTE,BYTE);

/**
 * struct coefficents - Calibration coefficents for Barometer Grove Sensor Device
 */
typedef struct coefficents_
{
	int ac1;
	int ac2; 
	int ac3; 
	unsigned int ac4;
	unsigned int ac5;
	unsigned int ac6;
	int b1; 
	int b2;
	int mb;
	int mc;
	int md;	
}coefficents;


/**
 * struct Baro - The structure for Baro grove sensor 
 */
struct Baro
{
	const void *class;
	coefficents coeff;
	BYTE devaddress; 
	long b5;//used for calibration purpose
	struct Interface *inter;
		
};


/**
 * static int init(struct Baro* self) - Read from barometer device  registers all calibration coefficents
 * \param *_self - pointer to the Baro grove device class.
* \return - The status of the operation
 <UL>
	<LI><Breturn = 0:</B> the operation was successful.</LI> 
	<LI><B>return = -1:</B> the operation was unsuccessful.</LI> 
 </UL>
*/
static int init(struct Baro* self)
{
	//read the Baro coefficents from built-in EEPROM
	/********First coefficent***/
	self->coeff.ac1 = (int)read_register(self->devaddress,AC1_MSB)<<8;
	self->coeff.ac1 = self->coeff.ac1 + read_register(self->devaddress,AC1_LSB);
	/********second coefficent***/	
	self->coeff.ac2 = (int)read_register(self->devaddress,AC2_MSB)<<8;
	self->coeff.ac2 = self->coeff.ac2 + read_register(self->devaddress,AC2_LSB);
	/********third coefficent***/	
	self->coeff.ac3 = (int)read_register(self->devaddress,AC3_MSB)<<8;
	self->coeff.ac3 =self->coeff.ac3 + read_register(self->devaddress,AC3_LSB);
	/********forth coefficent***/	
	self->coeff.ac4 = (unsigned int)read_register(self->devaddress,AC4_MSB)<<8;
	self->coeff.ac4 = self->coeff.ac4 + read_register(self->devaddress,AC4_LSB);
	/********fifth coefficent***/	
	self->coeff.ac5 = (unsigned int)read_register(self->devaddress,AC5_MSB)<<8;
	self->coeff.ac5 = self->coeff.ac5 + read_register(self->devaddress,AC5_LSB);
	/********sixth coefficent***/	
	self->coeff.ac6 = (unsigned int)read_register(self->devaddress,AC6_MSB)<<8;
	self->coeff.ac6 = self->coeff.ac6 + read_register(self->devaddress,AC6_LSB);
	/********seventh coefficent***/	
	self->coeff.b1 = (int)read_register(self->devaddress,B1_MSB)<<8;
	self->coeff.b1 = self->coeff.b1 + read_register(self->devaddress,B1_LSB);
	/********eighth coefficent***/	
	self->coeff.b2 = (int)read_register(self->devaddress,B2_MSB)<<8;
	self->coeff.b2 = self->coeff.b2 + read_register(self->devaddress,B2_LSB);
	/********ninenth coefficent***/	
	self->coeff.mb = (int)read_register(self->devaddress,MB_MSB)<<8;
	self->coeff.mb = self->coeff.mb + read_register(self->devaddress,MB_LSB);
	/********tenth coefficent***/	
	self->coeff.mc = (int)read_register(self->devaddress,MC_MSB)<<8;
	self->coeff.mc = self->coeff.mc + read_register(self->devaddress,MC_LSB);
	/********eleventh coefficent***/	
	self->coeff.md = (int)read_register(self->devaddress,MD_MSB)<<8;
	self->coeff.md = self->coeff.md + read_register(self->devaddress,MD_LSB);

	//check if the BMP085 is connected
	I2CStart();
	I2CWrite(self->devaddress | 0);
	if(I2C1STATbits.ACKSTAT)
	{
		I2CStop();
		return -1;
	}
	else
	{
		I2CStop();
		return 0;
	}
}

/**
 * static float Baro_compensate(struct Baro* self,long d,BYTE param) - Perfoms the calibration function 
 * \param *_self - pointer to the Baro grove device class.
 * \param long uncal - uncalibrated data.
 * \param BYTE param - which data to be got
* \return - The calibrated data
*/
static float Baro_compensate(struct Baro* self,long uncal,BYTE param)
{
	long x1, x2, x3, b3, b6, p;
	unsigned long b4, b7;
	float data = 0;
	//calculate the temperature

	if(param == BMP085_TEMP)
	{

		x1 = ((long) (uncal - self->coeff.ac6) * self->coeff.ac5) >> 15;
		x2 = ((long) self->coeff.mc << 11) / (x1 + self->coeff.md);
		self->b5 = x1 + x2;
		data = (float)((self->b5 + 8) >> 4);
		data = data/10.0; 
	}
	else
	{		
		//calculate the pressure
		b6 = self->b5 - 4000;
		x1 = (self->coeff.b2 * (b6 * b6 >> 12)) >> 11; 
		x2 = self->coeff.ac2 * b6 >> 11;
		x3 = x1 + x2;
		b3 = ((((long) self->coeff.ac1 * 4 + x3) << 3)+2) >> 2; 
		x1 = self->coeff.ac3 * b6 >> 13;
		x2 = (self->coeff.b1 * (b6 * b6 >> 12)) >> 16;
		x3 = ((x1 + x2) + 2) >> 2;
		b4 = (self->coeff.ac4 * (unsigned long) (x3 + 32768)) >> 15;
		b7 = ((unsigned long) uncal - b3) * (50000 >> 3);
		p = b7 < 0x80000000 ? (b7 * 2) / b4 : (b7 / b4) * 2;
		x1 = (p >> 8) * (p >> 8);
		x1 = (x1 * 3038) >> 16;
		x2 = (-7357 * p) >> 16;
		data = (float)(p + ((x1 + x2 + 3791) >> 4));	
		data = data/100.0; 			// direct conversion to milibars
	}	
	return data;
}	


/**
 * static void *Baro_ctor (void * _self, va_list *app) -Baro grove device Constructor  
 * \param *_self - pointer to the Baro grove device class.
 * \param *app - i2c device address
* \return - Pointer to the Baro devices instantiated
*/
static void *Baro_ctor (void * _self, va_list *app)
{
	struct Baro *self = _self;
	self->devaddress =  va_arg(*app, const BYTE);
	self->inter = NULL;
	return self;
}

/**
 * static void Baro_dtor (void * _sensor)- Baro grove device Destructor  
 * \param *_sensor - pointer to the Baro grove device class.
 * \return - None
*/
static void Baro_dtor (void * _sensor)
{
}


/**
 * static void* Baro_attach (void * _board,void *_sensor,int n) - attach a Baro grove device to the GroveNest I2C port  
 * \param *_board - pointer to the GroveNest 
 * \param *_sensor - pointer to the Baro grove device class.
 * \param ic2bus -  which I2C bus the device is connected to
 * \return 
 <UL>
	<LI><Breturn = Pointer to the I2C interface created:</B> the operation was successful.</LI> 
	<LI><B>return = NULL:</B> the operation was unsuccessful.</LI> 
 </UL>
 */
static void *Baro_attach (void * _board,void *_sensor,int ic2bus)
{
	struct Baro *sensor = _sensor;
	sensor->inter = attachSensorToI2CBus(_board,ic2bus,(int)sensor->devaddress);
	return sensor->inter;
}


/**
 *  static int Baro_configure (void * _self, va_list *app) -  Configure the Baro grove device
 * \param *_self - pointer to the device 
 * \param *app - none 
 * \return:
 	<LI><Breturn = 0:</B>when the Serial LCD device is properly configured </LI> 
 	<LI><Breturn = -1:</B>when the operation was unsucceful (the Serial LCD device will not work) </LI> 
 </UL>
 */
static int Baro_config (void * _self, va_list *app)
{
	struct Baro *self = _self;
	return init(self);	
}

/**
 * static float Baro_get(void * _self,va_list *app) -  write into the Baro device.
 * \param *_self - pointer to the device 
 * \param *app - Which data to be got 
 *  -BMP085_TEMP - Temperature
 *  -BMP085_PRES - Pressure
 *\return - The calibrated data (Temperature or Pressure).
*/
static float Baro_get (void * _self,va_list *app)
{
	struct Baro *self = _self;
	BYTE param =  va_arg(*app, const BYTE);
	BYTE LSB = 0;
	BYTE MSB = 0;
	BYTE XLSB = 0;
	long data = 0;
	float value = 0;
	if((param != BMP085_TEMP) && (param != BMP085_PRES))
	{	
		flag = 1;
		return -1;
	}
	if(set_register(self->devaddress,CRT_REG,BMP085_TEMP))
	{	
		flag = 1;
		return -1;
	}
	vTaskDelay(2);
	MSB = read_register(self->devaddress,0xF6);
	LSB = read_register(self->devaddress,0xF7);
	data =(int)(MSB<<8)|LSB;
	value = Baro_compensate(self,data,param);
	if(param != BMP085_PRES)
		return value;
	if(set_register(self->devaddress,CRT_REG,BMP085_PRES))
	{	
		flag = 1;
		return -1;
	}
	vTaskDelay(7);
	MSB = read_register(self->devaddress,0xF6);
	LSB = read_register(self->devaddress,0xF7);
	XLSB= read_register(self->devaddress,0xF8);
	data =(((long)MSB<<16)|((long)LSB<<8)|((long) XLSB))>>5;
	return Baro_compensate(self,data,param);
}



static const struct SensorClass _Baro =
{	
	sizeof(struct Baro),
	Baro_ctor,
	Baro_dtor,
	Baro_attach,
	Baro_config,
	0,
	Baro_get
};

const void *Baro = &_Baro;



