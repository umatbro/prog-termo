#include <LiquidCrystal.h>
#include "fun.h"


const int LED = 11;
/*
nieużywane przyciski
Button buttonPlus(10);
Button buttonMinus(11);
Button buttonAccept(12);*/
Temperature t1(20,15);
RemoteButton pilot;
IRrecv irrecv(10);
bool ON = false;

LiquidCrystal lcdDisplay(2,3,4,5,6,7);


//funkcje
void lcd16RollString (LiquidCrystal lcd, String napis, int rzad);
void displayTemp(float value, LiquidCrystal lcd, int row); //wyświetl wartość value temperatury w wierszu row wyświetlacza lcd


void setup() {
	//ustawienie pinów
	pinMode(LED,OUTPUT);
	digitalWrite(LED,LOW);
	irrecv.enableIRIn();
	Serial.begin(9600);
	
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
	displayTemp(t1.desired, lcdDisplay, 1);
	displayTemp(t1.getTempValue(), lcdDisplay, 0);
	test2();
	//t1.tempSimulation();
}

/*
void test()
{
	switch(pilot.readIRSignal(irrecv))
	{
	//gora
		case pilot.buttonUp:
			t1.increaseDesired(0.5); break;
		//case 0x13058: t1.increaseDesired(0.5); break;
		case pilot.buttonDown:
			t1.decreaseDesired(0.5); break;
		//case 0x13059: t1.decreaseDesired(0.5); break;
		case (pilot.buttonStart):
			while(pilot.readIRSignal(irrecv) != pilot.buttonStop)
			{
				t1.tempSimulation();
			}
		default: break;
	}
	
}*/

void test2()
{
	if (pilot.readIRSignal(irrecv) == pilot.buttonUp) t1.increaseDesired(0.5);
	if (pilot.readIRSignal(irrecv) == pilot.buttonDown) t1.decreaseDesired(0.5);
	else if (pilot.readIRSignal(irrecv) == pilot.buttonStart)
	{
		bool stopSignal=true;
		while(stopSignal == true)
			{
				Serial.println("hejka");
				t1.tempSimulation();
				displayTemp(t1.getTempValue(), lcdDisplay, 0);
				if(pilot.readIRSignal(irrecv) == pilot.buttonStop) stopSignal = false;
			}
	}
}
