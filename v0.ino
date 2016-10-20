#include <LiquidCrystal.h>
#include "fun.h"


const int LED = 9;
Button buttonPlus(10);
Button buttonMinus(11);
Button buttonAccept(12);
Temperature t1(20,15);
bool ON = false;

LiquidCrystal lcdDisplay(2,3,4,5,6,7);


//funkcje
void lcd16RollString (LiquidCrystal lcd, String napis, int rzad);
void displayTemp(float value, LiquidCrystal lcd, int row); //wyświetl wartość value temperatury w wierszu row wyświetlacza lcd
void buttonPlus_OnClick();
void buttonMinus_OnClick();
bool buttonAccept_OnClick();


void setup() {
	//ustawienie pinów
	pinMode(LED,OUTPUT);
  digitalWrite(LED,LOW);
	
	//ustawienie ekranu
  lcdDisplay.begin(16,2);
  lcdDisplay.setCursor(0,0);
  lcdDisplay.print("Temperatura: ");
  displayTemp(t1.getTempValue(),lcdDisplay,0);
  
  lcdDisplay.setCursor(0,1);
  lcdDisplay.print("Ustaw temp.");
  displayTemp(t1.desired,lcdDisplay,1);
}

void loop() 
{
	buttonPlus_OnClick();
	buttonMinus_OnClick();
	displayTemp(t1.desired, lcdDisplay, 1);
	displayTemp(t1.getTempValue(), lcdDisplay, 0);
	while( buttonAccept_OnClick())
	{
		t1.tempSimulation(t1.isHeatingReq());
		displayTemp(t1.desired, lcdDisplay, 1);
		displayTemp(t1.getTempValue(), lcdDisplay, 0);
	}		
}


void buttonPlus_OnClick()
{
  if(buttonPlus.isPushed()) 
  {
    t1.increaseDesired(0.5);
    delay(150);
  }
}

void buttonMinus_OnClick()
{
	if(buttonMinus.isPushed())
	{
		t1.decreaseDesired(0.5);
		delay(150);
	}
}

bool buttonAccept_OnClick()
{
	if(buttonAccept.isPushed()) return ON = !ON;
	else return ON;
}
