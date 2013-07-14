#include "taskFlyport.h"
#include "grovelib.h"

#include "analog_temp.h"
#include "rht03.h"

#include "barometer.h"
#include "oled.h"
 
void FlyportTask()
{	
	//on declare des variables 
	char msg[100];
	float G_TempA = 0.0, G_lightA = 0.0, G_gazA = 0.0, G_HumiD = 0.0, G_TempD = 0.0, G_BaroI2C = 0.0, G_TempI2C = 0.0;
	extern char VTempA[5], VLightA[5], VGazA[5], VHumiD[5], VTempD[5], VBaroI2C[5], VTempI2C[5];
	
 
	vTaskDelay(50);
	UARTWrite(1,"Bienvenue sur ma station!\r\n");
 
	// GROVE board
	void *board = new(GroveNest);
 
	// GROVE devices	
	// Analog Temperature Sensor
	void *tempAn = new(An_Temp);
	void *light_sensor = new(An_i);
	void *gas_sensor = new (An_i);
	
	//Digital Sensor
	void *rht03 = new(RHT03);
	
	//IC2 sensor
	void *baro = new(Baro,BAROM_ADDR);
 
	//I2C Oled
	void *oled = new(Oled, OLED_ADDR0);
	
	// Attach devices
	attachToBoard(board, tempAn, AN1);
	attachToBoard(board, light_sensor, AN2);
	attachToBoard(board, gas_sensor, AN3);
	attachToBoard(board,rht03,DIG1);
	attachToBoard(board, baro, I2C);
	attachToBoard(board, oled, I2C);
	
	//Configuration
	configure(rht03, 3);
	configure(baro);
	configure(oled);
	
	// Connection to Network
	#if defined (FLYPORT)
	WFConnect(WF_DEFAULT);
	while (WFStatus != CONNECTED);
	#endif
	#if defined (FLYPORTETH)
	while(!MACLinked);
	#endif
	UARTWrite(1,"Flyport connected... hello world!\r\n");
	vTaskDelay(200);	
 
	while(1)
	{
		// recupere les infos
		G_TempA = get(tempAn);
		sprintf(VTempA, "%.1f", (double)G_TempA);
		vTaskDelay(10);
		
		G_lightA = get(light_sensor);
		sprintf(VLightA, "%.0f", (double)((1023-G_lightA)*10/G_lightA));
		vTaskDelay(10);		
		
		G_gazA = get(gas_sensor);
		sprintf(VGazA, "%3.2f", (double)G_gazA);
		vTaskDelay(10);		
		
		// Reading humidity
		G_HumiD = get(rht03, HUMD);
		if(!readError())
		{
			sprintf(VHumiD,"%.1f", (double)G_HumiD);
		}
                // Reading temperature
		vTaskDelay(10);
		G_TempD = get(rht03, TEMP);
		if(!readError())
		{
			sprintf(VTempD,"%.1f", (double)G_TempD);
		}	
		vTaskDelay(10);
		G_BaroI2C = get(baro,BMP085_PRES);
		if(!readError())
		{
			sprintf(VBaroI2C,"%.1f",(double)G_BaroI2C);
		}
		vTaskDelay(10);
		if(!readError())
		{
			G_TempI2C = get(baro,BMP085_TEMP);
			sprintf(VTempI2C,"%.1f",(double)G_TempI2C);
		}		
		
		
		//On affiche les infos
		
		sprintf(msg, "Ma station meteo");
		set(oled, msg, STRING, BLACK, 0, 0);
		
		
		sprintf(msg, "Temp %s *C\r\n", VTempA);
		UARTWrite(1, msg);
		set(oled, msg, STRING, BLACK, 2, 0);
		vTaskDelay(10);	
		
		sprintf(msg, "Luminosite %s %\r\n", VLightA);
		UARTWrite(1, msg);
		set(oled, msg, STRING, BLACK, 3, 0);
		vTaskDelay(10);		
		
		sprintf(msg, "teneur en gaz %s ppm\r\n", VGazA);
		UARTWrite(1, msg);
		set(oled, msg, STRING, BLACK, 4, 0);
		vTaskDelay(10);			

		sprintf(msg, "Humidite %s %\r\n", VHumiD);
		UARTWrite(1, msg);
		set(oled, msg, STRING, BLACK, 5, 0);
		vTaskDelay(10);		
		
		sprintf(msg, "Temp %s *C\r\n", VTempD);
		UARTWrite(1, msg);
		vTaskDelay(10);	
		
		sprintf(msg, "Pression %s mPa\r\n", VBaroI2C);
		UARTWrite(1, msg);
		set(oled, msg, STRING, BLACK, 6, 0);
		vTaskDelay(10);		
		
		sprintf(msg, "Temp %s *C\r\n", VTempI2C);
		UARTWrite(1, msg);
		vTaskDelay(10);			
		
		vTaskDelay(1000);
		
	}
}
