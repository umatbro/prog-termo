#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include "fun.h"

//wyświetlacz
LiquidCrystal lcdDisplay(2,3,4,5,6,7);

//Timery
Timer tempMeasureTimer;
Timer timer2;

//przyciski
Button buttonPlus(8,PULLUP);
Button buttonMinus(9,PULLUP);
Button buttonAccept(10,PULLUP);
Temperature tValue(20);
Temperature tDesired(45);
RegulacjaPID reg;

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

//Termopara, MAX6675
const byte soPin = A5;
const byte csPin = A4;
const byte sckPin = A3;
MAX6675 thermocouple(sckPin, csPin, soPin);


//Zasilacz Manson2405
Manson2405 powerSupply;
int voltage = 0, current = 0;
int maxVoltage = 200; //wartości napięcia w woltach = voltage/10; wartości prądu w amperach = current/100;


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
  lcdDisplay.begin(16,2);
  
  lcdDisplay.clear();
  lcdDisplay.setCursor(0,0);
  lcdDisplay.print("USTAW   |  START");
    
  lcdDisplay.setCursor(0,1);
  lcdDisplay.print("-/+"+(String)(tValue.getTempValue(thermocouple)));
  displayTemp(tDesired.value(),lcdDisplay,1);
  
  
}

void loop() 
{
	boolean sTimer = tempMeasureTimer.stepTimer(500);
	if(sTimer)
	{
		//BŁĄD - nie ma podłączonej termopary, jak najszybciej wszystko wyłączyć
		//WAŻNE dodać wyłączenie zasilacza
		while(thermocouple.readCelsius() == 0 || isnan(thermocouple.readCelsius()) ) 
		{
			lcdDisplay.setCursor(0,0);
			lcdDisplay.print("BLAD!!! Sprawdz");
			lcdDisplay.setCursor(0,1);
			lcdDisplay.print("termopare");
			delay(2000);
			lcdDisplay.clear();
		}	
	}

	if(buttonAccept.uniquePress()) 
	{
		//zamienić na switcha
		//if(buttonAccept.timePressed() > 1000 && buttonAccept.timePressed()%1000 < 5)
		//{
		menuOption++;
		if(menuOption > 3) menuOption = 1;
		lcdDisplay.clear();
	}
	//rs485.println(buttonAccept.timePressed());
  
  
	switch(menuOption)
	{
		case 1: //ustawienia temperatury
		{
			lcdDisplay.setCursor(0,0);
			lcdDisplay.print("USTAW   |  START");
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
			if(sTimer) 
			{
				lcdDisplay.print((String)(tValue.getTempValue(thermocouple))+"    -/+");
				displayTemp(tDesired.value(),lcdDisplay,1);
			}
			//break case
			break;
		}
		
		case 2: //ustawienie zasilacza
		{
			if(timer2.stepTimer(1000))
			{
				lcdDisplay.clear();
				lcdDisplay.setCursor(0,0);
				lcdDisplay.print("Opcje");
				lcdDisplay.setCursor(0,1);
				lcdDisplay.print("zasilacza (+)"); 
			}
			if(buttonPlus.uniquePress()) //submenu
			{
				int endLoop = 137;
				lcdDisplay.clear();
				lcdDisplay.setCursor(0,0);
				lcdDisplay.print("SUBMENU");
				int submenuOption = 0;
				while(submenuOption != endLoop)
				{
					//przełączanie między submenu
					if(buttonAccept.isPressed())
					{
						submenuOption++;
						if(submenuOption>3) submenuOption = 1;
						lcdDisplay.clear();
						switch(submenuOption)
						{
							case 1: //rozpoczęcie sesji
							lcdDisplay.clear();
							lcdPrint(lcdDisplay,"Start sesji",0);
							lcdPrint(lcdDisplay,"(+)", 1);
							break;
							
							case 2:
							lcdDisplay.clear();
							lcdPrint(lcdDisplay,"SUB2",0);
							lcdPrint(lcdDisplay,"(+)", 1);
							break;
							
							case 3:
							lcdDisplay.setCursor(0,0);
							lcdDisplay.print("Koniec sesji");
							lcdPrint(lcdDisplay,"(+)",1);
							break;
							
							default: break;
						}
						delay(500);
					}
					//dzialanie submenu
					switch(submenuOption)
					{
						case 1:  //rozpoczęcie sesji
						{
						if(buttonPlus.isPressed()) //submenu wybor adresu
						{				
							lcdDisplay.clear();
							lcdPrint(lcdDisplay,"Adres: ",0);									
							while(!buttonAccept.uniquePress())
							{
								//konwersja na string
								String psAdressString = (String)psAdress;
								if(psAdress<10) psAdressString = "0" + psAdressString;
								//wyswietlanie adresu
								//if(timer2.stepTimer(700)) 
									lcdPrint(lcdDisplay,psAdressString,1);

								if(buttonPlus.uniquePress())
								{
									psAdress++;
									if(psAdress>31) psAdress = 31;
								}
								if(buttonMinus.uniquePress())
								{
									psAdress--;
									if(psAdress<0) psAdress = 0;
								}
							}
							//konwersja na string
							String psAdressString = (String)psAdress;
							if(psAdress<10) psAdressString = "0" + psAdressString;
							//wysłanie komendy
							sendCommand(rs485,"SESS"+psAdressString);
						}
						break;
						}
						
						case 2: break;
						
						case 3: //koniec sesji
						{
							String psAdressString = (String)psAdress;
							if(psAdress<10) psAdressString = "0" + psAdressString;
							if(buttonPlus.uniquePress())
								sendCommand(rs485,"ENDS"+psAdressString);
						}
						
						
						default: break;
					}
					
					if(buttonAccept.timePressed() > 1000) submenuOption = endLoop;
						/*switch(submenuOption)
						{
						case 1:
						lcdDisplay.setCursor(0,0);
						lcdDisplay.print("SUB 1");
						break;
						
						case 2:
						lcdDisplay.setCursor(0,0);
						lcdDisplay.print("SUB 2");
						break;
						
						default: break;
						}*/
					if(menuOption == endLoop) break;
					
					
				}
				/*String napis = "dobrze";
				rs485.print(napis);
				while(rs485.read() != -1);
				digitalWrite(receiveEnablePin,HIGH);
				digitalWrite(dataEnablePin,LOW);
				int czas =0;
				while(!rs485.available()) {}
				
				napis = rs485.readStringUntil(0x0D);
				rs485.print(napis+" ");
				digitalWrite(receiveEnablePin,LOW);
				digitalWrite(dataEnablePin,HIGH);*/
			}
			//break case
			break;
		}
		
		case 3: //regulacja temperatury
		{
			String psAdressString = (String)psAdress;
			int endLoop = 137;
			if(psAdress<10) psAdressString = "0" + psAdressString;
			//wyświetlenie komunikatu
			if(timer2.stepTimer(1000)) lcdDisplay.clear();
			lcdPrint(lcdDisplay,"START/STOP",0);
			lcdPrint(lcdDisplay,"(+)",1);
			
			if(buttonPlus.uniquePress()) 
			{
				lcdDisplay.clear();
				submenuOption++;
				if(submenuOption > 2) submenuOption = 1;
				switch(submenuOption)
				{
					case 1: //grzanie wyłączone
					{
						lcdDisplay.clear();
						lcdPrint(lcdDisplay,"START/STOP",0);
						lcdPrint(lcdDisplay,"(+)",1);
					}
					break;
					
					case 2: //włączone
					{
						lcdDisplay.setCursor(0,0);
						lcdDisplay.print("V:");
						lcdDisplay.setCursor(10,0);
						lcdDisplay.print("U:"+(String)(tDesired.value()));
						lcdDisplay.setCursor(0,1);
						lcdDisplay.print("C:");
						lcdDisplay.setCursor(10,1);
						lcdDisplay.print("T:");
						current = 20;
						sendCommand(rs485,"CURR"+psAdressString+"020");
						Timer timerReg;
						while(!buttonAccept.uniquePress())
						{
							if(timerReg.stepTimer(500))
							{
								float temperature = tValue.getTempValue(thermocouple);
								displayTemp(temperature, lcdDisplay, 1);
								lcdDisplay.setCursor(3,1);
								lcdDisplay.print((String)(current/100.00));
								//potrzebny voltage
								voltage = maxVoltage*(reg.regulator(tDesired.value(), temperature))/100.00;
								lcdDisplay.setCursor(2,0);
								lcdDisplay.print("000");
								String voltageString;
								if(voltage < 10) 
								{ 
									lcdDisplay.setCursor(4,0); 
									voltageString = "00"+(String)voltage;
								}
								else if(voltage < 100) {
									lcdDisplay.setCursor(3,0);
									voltageString = "0"+(String)voltage;
								}
								else {
									lcdDisplay.setCursor(2,0);
									voltageString = (String)voltage;
								}
								lcdDisplay.print(voltage);
								sendCommand(rs485,"VOLT"+psAdressString+voltageString);
								
								//warunek bezpieczeństwa
								if(temperature > tDesired.value() + 5) break;
							}
						}
						sendCommand(rs485,"VOLT"+psAdressString+"000");
						delay(10);
						sendCommand(rs485,"CURR"+psAdressString+"000");
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
