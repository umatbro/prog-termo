#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include "fun.h"

//wyświetlacz
LiquidCrystal lcdDisplay(2,3,4,5,6,7);

//Timery
Timer tempMeasureTimer;

//przyciski
Button buttonPlus(8,PULLUP);
Button buttonMinus(9,PULLUP);
Button buttonAccept(10,PULLUP);
Temperature tValue(20);
Temperature tDesired(45);
RegulacjaPID reg;
boolean isON = false;
Button *recentlyPressed; //flaga recently pressed

//RS485
const byte rxPin = 0;
const byte txPin = 1;
const byte receiveEnablePin = 11;
const byte dataEnablePin = 12;
SoftwareSerial rs485(rxPin, txPin);


//Termopara, MAX6675
const byte soPin = A5;
const byte csPin = A4;
const byte sckPin = A3;
MAX6675 thermocouple(sckPin, csPin, soPin);



void setup() {
	Serial.begin(9600);
	
	//ustawienia rs485
	rs485.begin(9600);
	pinMode(receiveEnablePin,OUTPUT);
	pinMode(dataEnablePin,OUTPUT);
	digitalWrite(receiveEnablePin,LOW);
	digitalWrite(dataEnablePin,HIGH);
	//ustawienie pinów
	
	//ustawienie ekranu
	lcdDisplay.begin(16,2);
	
/*	//oczekiwanie na termoparę
	lcdDisplay.clear();
	lcdDisplay.setCursor(0,0);
	lcdDisplay.print("Oczekiwanie na");
	lcdDisplay.setCursor(0,1);
	lcdDisplay.print("termopare");
	while(thermocouple.readCelsius() == 0 || isnan(thermocouple.readCelsius()) ) {delay(2000);}
*/	
	lcdDisplay.clear();
	lcdDisplay.setCursor(0,0);
	lcdDisplay.print("USTAW   |  START");
	  
	lcdDisplay.setCursor(0,1);
	lcdDisplay.print("-/+"+(String)(tValue.getTempValue(thermocouple)));
	displayTemp(tDesired.value(),lcdDisplay,1);
	
	
}

void loop() 
{
	//BŁĄD - nie ma podłączonej termopary, jak najszybciej wszystko wyłączyć
	while(thermocouple.readCelsius() == 0 || isnan(thermocouple.readCelsius()) ) 
	{
		lcdDisplay.clear();
		lcdDisplay.setCursor(0,0);
		lcdDisplay.print("BLAD!!! Sprawdz");
		lcdDisplay.setCursor(0,1);
		lcdDisplay.print("termopare");
		delay(2000);
	}

	boolean sTimer = tempMeasureTimer.stepTimer(500);
	if(buttonAccept.uniquePress()) 
	{
		isON = !isON;
		if(!isON)
		{
			lcdDisplay.clear();
			lcdDisplay.setCursor(0,0);
			lcdDisplay.print("USTAW   |  START");
		  
			lcdDisplay.setCursor(0,1);
			lcdDisplay.print("-/+"+(String)(tValue.getTempValue(thermocouple)));
			displayTemp(tDesired.value(),lcdDisplay,1);
		}
		else if(isON)
		{
			lcdDisplay.clear();
			lcdDisplay.setCursor(0,0);
			lcdDisplay.print("Temperatura: ");
			displayTemp(tValue.getTempValue(thermocouple),lcdDisplay,0);
			  
			lcdDisplay.setCursor(0,1);
			lcdDisplay.print("Ustaw temp.");
			displayTemp(tDesired.value(),lcdDisplay,1);
		}
	}
	
	
	if(isON==false)
	{
		if(buttonPlus.uniquePress())
		{
			displayTemp(tDesired.set( tDesired.value() + 0.5 ), lcdDisplay, 1);
			recentlyPressed = &buttonPlus;
		}
		if (buttonMinus.uniquePress())
		{
			displayTemp(tDesired.set( tDesired.value() - 0.5 ), lcdDisplay, 1);
			recentlyPressed = &buttonMinus;
		}
		unsigned long czas = recentlyPressed -> timePressed();
		if(czas>1000 && czas%200 < 10)
		{
			if(recentlyPressed == &buttonPlus) tDesired.set( tDesired.value() + 0.5 );
			if(recentlyPressed == &buttonMinus) tDesired.set( tDesired.value() - 0.5 );
			displayTemp(tDesired.value(), lcdDisplay, 1);
		}
		lcdDisplay.setCursor(0,1);
		if(sTimer) lcdDisplay.print("-/+"+(String)(tValue.getTempValue(thermocouple)));
			
		//Serial.println(recentlyPressed -> timePressed());
	}
	
	else 
	{
		displayTemp(tDesired.value(), lcdDisplay, 1);
		if(sTimer) displayTemp(tValue.getTempValue(thermocouple), lcdDisplay, 0);
		//tempSimul(reg, tValueFloat, tDesired.value());
	}
	
	
}
