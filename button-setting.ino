
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include "fun.h"
//#include "Button/Button.h"


LiquidCrystal lcdDisplay(2,3,4,5,6,7);

Button buttonPlus(8,PULLUP);
Button buttonMinus(9,PULLUP);
Button buttonAccept(10,PULLUP);
Temperature tValue(20);
Temperature tDesired(45);
RegulacjaPID reg;
boolean isON = false;
Button *recentlyPressed; //flaga recently pressed

float tValueFloat;
void setup() {
	 tValueFloat = tValue.getTempValue();
	Serial.begin(9600);
	//ustawienie pinów
	
	//ustawienie ekranu
	lcdDisplay.begin(16,2);
	lcdDisplay.setCursor(0,0);
	lcdDisplay.print("USTAW   |  START");
	  
	lcdDisplay.setCursor(0,1);
	lcdDisplay.print("-/+");
	displayTemp(tDesired.value(),lcdDisplay,1);
}



void loop() 
{
	if(buttonAccept.uniquePress()) 
	{
		isON = !isON;
		if(!isON)
		{
			lcdDisplay.clear();
			lcdDisplay.setCursor(0,0);
			lcdDisplay.print("USTAW   |  START");
		  
			lcdDisplay.setCursor(0,1);
			lcdDisplay.print("-/+");
			displayTemp(tDesired.value(),lcdDisplay,1);
		}
		else if(isON)
		{
			lcdDisplay.clear();
			lcdDisplay.setCursor(0,0);
			lcdDisplay.print("Temperatura: ");
			displayTemp(tValue.getTempValue(),lcdDisplay,0);
			  
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
		

		//Serial.println(recentlyPressed -> timePressed());
	}
	
	else 
	{
		displayTemp(tDesired.value(), lcdDisplay, 1);
		displayTemp(tValueFloat, lcdDisplay, 0);
		tempSimul(reg, tValueFloat, tDesired.value());
	}
}


