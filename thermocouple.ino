#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include "proj.h"

//wyświetlacz
LiquidCrystal lcd(2,3,4,5,6,7);

//Timery
Timer tempMeasureTimer;
Timer timer2;

//przyciski
Button buttonPlus(8,PULLUP);
Button buttonMinus(9,PULLUP);
Button buttonAccept(10,PULLUP);
Temperature tValue;
Temperature tDesired(25);
RegPID reg;

//inne
boolean psConnected = false; //Power Supply connected
byte menuOption = 1;
byte submenuOption = 1;
Button *recentlyPressed; //flaga recently pressed

//RS485
const byte rxPin = 0;
const byte txPin = 1;
const byte receiveEnablePin = 11;
const byte dataEnablePin = 12;
SoftwareSerial rs485(rxPin, txPin);
int psAdress = 0; //adres zasilacza
String psAdressString = "000";

//Termopara, MAX6675
const byte soPin = A5;
const byte csPin = A4;
const byte sckPin = A3;
MAX6675 thermocouple(sckPin, csPin, soPin);


//Zasilacz Manson2405
Manson2405 powerSupply;
int voltage = 0, current = 40;
int maxVoltage = 150; //wartości napięcia w woltach = voltage/10; wartości prądu w amperach = current/100;


void setup() {
	timer2.threshold = 100;
	//Serial.begin(9600);

	//ustawienia rs485
	rs485.begin(9600);
	pinMode(receiveEnablePin,OUTPUT);
	pinMode(dataEnablePin,OUTPUT);
	digitalWrite(receiveEnablePin,LOW);
	digitalWrite(dataEnablePin,HIGH);

	//ustawienie pinów

	//ustawienie ekranu
	lcd.begin(16,2);

	lcd.clear();
	lcd.print("USTAW");

	lcd.setCursor(0,1);
	lcd.print((String)(tValue.getTempValue(thermocouple)));
	displayTemp(tDesired.value(),lcd,1);
}

void loop() 
{
	boolean sTimer = tempMeasureTimer.stepTimer(500);
	if(sTimer) {
		//BŁĄD - nie ma podłączonej termopary, jak najszybciej wszystko wyłączyć
		//WAŻNE dodać wyłączenie zasilacza
		while(thermocouple.readCelsius() == 0 || isnan(thermocouple.readCelsius()) ) {
			lcd.setCursor(0,0);
			lcd.print("BLAD!!! Sprawdz");
			lcd.setCursor(0,1);
			lcd.print("termopare");
			powerSupply.sendCommand(rs485,"VOLT"+psAdressString+"000");
			delay(10);
			powerSupply.sendCommand(rs485,"CURR"+psAdressString+"000");
			delay(2000);
			lcd.clear();
		}
	}

	if(buttonAccept.uniquePress()) {
		menuOption++;
		if(menuOption > 3) menuOption = 1;
		lcd.clear();
	}
  
	switch(menuOption) {
		case 1: {//ustawienia temperatury
			lcd.setCursor(0,0);
			lcd.print("USTAW");
			if(buttonPlus.uniquePress()) {
				displayTemp(tDesired.set( tDesired.value() + 0.5 ), lcd, 1);
				recentlyPressed = &buttonPlus;
			}
			
			if (buttonMinus.uniquePress()) {
				displayTemp(tDesired.set( tDesired.value() - 0.5 ), lcd, 1);
				recentlyPressed = &buttonMinus;
			}
			unsigned long czas = recentlyPressed -> timePressed();
			
			if(czas>1000 && czas%200 < 10) {
				if(recentlyPressed == &buttonPlus) tDesired.set( tDesired.value() + 0.5 );
				if(recentlyPressed == &buttonMinus) tDesired.set( tDesired.value() - 0.5 );
				displayTemp(tDesired.value(), lcd, 1);
			}
			lcd.setCursor(0,1);
			if(sTimer) {
				lcd.print((String)(tValue.getTempValue(thermocouple))+"    -/+");
				displayTemp(tDesired.value(),lcd,1);
			}
			//break case
			break;
		}
		
		case 2: {//ustawienie zasilacza
			if(timer2.stepTimer(1000)) {
				lcd.clear();
				lcd.print("Opcje");
				lcd.setCursor(0,1);
				lcd.print("zasilacza (+)"); 
			}
			if(buttonPlus.uniquePress()) { //submenu
				int endLoop = 137;
				lcd.clear();
				lcd.print("SUBMENU");
				int submenuOption = 0;
				while(submenuOption != endLoop) {
					//przełączanie między submenu
					if(buttonAccept.isPressed()) {
						submenuOption++;
						if(submenuOption>2) submenuOption = 1;
						lcd.clear();
						switch(submenuOption) {
							case 1: //rozpoczęcie sesji
							lcd.clear();
							lcdPrint(lcd,"Start sesji",0);
							lcdPrint(lcd,"(+)", 1);
							break;
							
							case 2:
							lcd.setCursor(0,0);
							lcd.print("Koniec sesji");
							lcdPrint(lcd,"(+)",1);
							break;
							
							default: break;
						}
						delay(500);
					}
					
					//dzialanie submenu
					switch(submenuOption) {
						case 1: { //rozpoczęcie sesji
							if(buttonPlus.isPressed()){ //submenu wybor adresu				
								lcd.clear();
								lcdPrint(lcd,"Adres: ",0);									
								while(!buttonAccept.uniquePress()) {
									//konwersja na string
									String psAdressString = (String)psAdress;
									if(psAdress<10) psAdressString = "0" + psAdressString;
									//wyswietlanie adresu
									lcdPrint(lcd,psAdressString,1);

									if(buttonPlus.uniquePress()) {
										psAdress++;
										if(psAdress>31) psAdress = 31;
									}
									if(buttonMinus.uniquePress()) {
										psAdress--;
										if(psAdress<0) psAdress = 0;
									}
								}
								
								powerSupply.startSession(rs485, psAdress);
							}
							break;
						}
						
						case 2: { //koniec sesji
							if(buttonPlus.uniquePress())
								powerSupply.endSession(rs485, psAdress);
						}

						default: break;
					}
					
					if(buttonAccept.timePressed() > 1000) submenuOption = endLoop;
					if(menuOption == endLoop) break;		
				}
			}
			//break case
			break;
		}
		
		case 3: {//regulacja temperatury
			psAdressString = (String)psAdress;
			int endLoop = 137;
			if(psAdress<10) psAdressString = "0" + psAdressString;
			//wyświetlenie komunikatu
			if(timer2.stepTimer(1000)) lcd.clear();
			lcdPrint(lcd,"START/STOP",0);
			lcdPrint(lcd,"(+)",1);
			
			if(buttonPlus.uniquePress()) { //przycisk "plus" - rozpoczęcie pracy
				lcd.clear();
				submenuOption++;
				if(submenuOption > 2) submenuOption = 1;
				switch(submenuOption) {
					case 1: { //grzanie wyłączone
						lcd.clear();
						lcdPrint(lcd,"START/STOP",0);
						lcdPrint(lcd,"(+)",1);
					}
					break;
					
					case 2:{ //włączone
						boolean safetyCondition = false; //warunek bezpieczeństwa
						lcd.setCursor(0,0);
						lcd.print("V:");
						lcd.setCursor(10,0);
						lcd.print("U:"+(String)(tDesired.value()));
						// lcd.setCursor(0,1);
						// lcd.print("C:");
						lcd.setCursor(10,1);
						lcd.print("T:");
						//current = 40;
						//przekształcenie wartości prądu na String
						String currString;
						if(current < 10) currString = "00" + (String)current;
						else if(current < 100) currString = "0"+(String)current;
						else if(current < 1000) currString = (String)current;
						else currString = "0";
						
						//wysłanie komendy
						powerSupply.sendCommand(rs485,"CURR"+psAdressString+currString);
						Timer timerReg;
						while(!buttonAccept.isPressed()) {
							if(timerReg.stepTimer(500)) {
							//if(millis()%500<10)
								float temperature = tValue.getTempValue(thermocouple);
								displayTemp(temperature, lcd, 1);
								// lcd.setCursor(3,1);
								// lcd.print((String)(current/100.00));
								//obliczone napięcie
								voltage = maxVoltage*(reg.regulator(tDesired.value(), temperature))/100.00;
								lcd.setCursor(2,0);
								lcd.print("000");
								String voltageString;
								if(voltage < 10) { 
									lcd.setCursor(4,0); 
									voltageString = "00"+(String)voltage;
								}
								else if(voltage < 100) {
									lcd.setCursor(3,0);
									voltageString = "0"+(String)voltage;
								} else {
									lcd.setCursor(2,0);
									voltageString = (String)voltage;
								}
								lcd.print(voltage);
								powerSupply.sendCommand(rs485,"VOLT"+psAdressString+voltageString);
								
								//warunek bezpieczeństwa
								if(temperature > tDesired.value() + 20) safetyCondition = true;
								
								//zabezpieczenie
								while( isnan(thermocouple.readCelsius()) || safetyCondition) {
									lcd.setCursor(0,0);
									lcd.print("BLAD!!! Sprawdz");
									lcd.setCursor(0,1);
									if(!safetyCondition)
										lcd.print("termopare");
									else lcd.print("temperature");
									delay(2000);
									lcd.clear();
									powerSupply.sendCommand(rs485,"VOLT"+psAdressString+"000");
									delay(10);
									powerSupply.sendCommand(rs485,"CURR"+psAdressString+"000");
									if(tValue.getTempValue(thermocouple) < tDesired.value() + 5 ) safetyCondition = false;
								}
							}
						}
						powerSupply.sendCommand(rs485,"VOLT"+psAdressString+"000");
						delay(10);
						powerSupply.sendCommand(rs485,"CURR"+psAdressString+"000");
					}
					break;
					
					default: break;
				}
			}
			
			//case break
			break;
		}
		
		default: break;
	}
}