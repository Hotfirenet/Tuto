/** \file barometer.h
 *  \brief header file for the Serial LCD Grove device library
 */

/**
\addtogroup 
@{
*/

/* **************************************************************************																					
 *                                OpenPicus                 www.openpicus.com
 *                                                            italian concept
 * 
 *            openSource wireless Platform for sensors and Internet of Things	
 * **************************************************************************
 *  FileName:        barometer.h
 *  Dependencies:    Microchip configs files
 *  Module:          FlyPort WI-FI
 *  Compiler:        Microchip C30 v3.12 or higher
 *
 *  Author               Rev.    Date              Comment
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Davide Vicca   1.0     01/07/2013		   First release  (core team)
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

extern const void *Baro;


#define BAROM_ADDR 	0xEE 
#define CRT_REG 	0xF4 
#define	BMP085_TEMP	0x2E 
#define BMP085_PRES	0xF4

/*coefficent registers in EEPROM space*/

#define AC1_LSB 	0xAB 
#define AC1_MSB 	0xAA 
#define AC2_LSB 	0xAD 
#define AC2_MSB 	0xAC 
#define AC3_LSB 	0xAF 
#define AC3_MSB 	0xAE 
#define AC4_LSB 	0xB1 
#define AC4_MSB 	0xB0 
#define AC5_LSB 	0xB3 
#define AC5_MSB 	0xB2 
#define AC6_LSB 	0xB5 
#define AC6_MSB 	0xB4 
#define B1_LSB	 	0xB7 
#define B1_MSB 		0xB6 
#define B2_LSB	 	0xB9 
#define B2_MSB 		0xB8 
#define MB_LSB	 	0xBB 
#define MB_MSB 		0xBA 
#define MC_LSB	 	0xBD 
#define MC_MSB 		0xBC 
#define MD_LSB	 	0xBF 
#define MD_MSB 		0xBE 



