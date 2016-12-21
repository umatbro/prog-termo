#include "fun.h"


/*
	------------------------------------------------------------------
							METODY
	------------------------------------------------------------------
*/


/* ------------------------TEMPERATURE----------------------------- */


Temperature::Temperature(float tv) {
	val = tv;
}

float Temperature::increaseDesired(float howMuch) {
	if (desired>= 50) return 50;
	else return desired += howMuch;
}

float Temperature::decreaseDesired(float howMuch) {
	if (desired<=0) return 0;
	else return desired -= howMuch;
}

float Temperature::getTempValue(MAX6675 thermocouple) {
	return 0.95*(thermocouple.readCelsius()) - 11.58;
}
	
float Temperature::value() {
	return val;
}


float Temperature::set(float setting) {
	val = setting;
	return val;
}


/*----------------------------TIMER----------------------------------------*/
Timer::Timer() {
	millisecNow = millis();
	millisecStart = millis();
	threshold = 20;
}

boolean Timer::stepTimer(unsigned long milliseconds) {
	millisecNow = millis();
	if( (millisecNow - millisecStart > milliseconds) && (millisecNow - millisecStart)%milliseconds < threshold )
	{
		millisecStart = millis();
		return true;
	}
	else return false;
}


/*----------------------------MANSON 2405---------------------------------*/

String Manson2405::getResponse(SoftwareSerial rs485) {
	rs485.begin(9600);
	String response;
	while(rs485.read() != -1);
	//zmiana na odczyt
	digitalWrite(11,HIGH); //receiveEnablePin
	digitalWrite(12,LOW); //dataEnablePin
	while(!rs485.available()) {}
	response = rs485.readStringUntil(0x0D);
	//ustawienie na wysyłanie
	digitalWrite(11,LOW);
	digitalWrite(12,HIGH);
	rs485.end();
	return response;
}


/*--------------------------REGULATOR PID--------------------------------*/
float RegulacjaPID::regulator(float w_zad, float wy_o)  {
    float k=200;
    float k_i = 0.002;
    float k_d = 20;

    //zmienne pomocnicze
    float p,i,d,r;
    float u; //uchyb regulacji
    static float u_p = 0; //uchyb regulacji w poprzednim wywo�aniu
    static float su = 0; //suma minionych uchyb�w regulacji
    u = w_zad - wy_o; // aktualny uchyb regulacji

	// wyznaczenie skladnika proporcjonalnego
    p = k * u;

	// wyznaczenie składnika ca?kowego
	su = su + u; //najpierw trzeba wyliczyć sumę wszystkich uchybów;
	i = k_i * su;

	// wyznaczenie składnika D
	d = k_d * (u-u_p);
	u_p = u; //zapamitaj chwilową wartość uchybu dla przyszłych obliczeń

	r = p + i + d; //sygnał wyjściowy regulatora

	//ograniczenia regulowanej wartości
	if (r < 0) r = 0; //czyli moc nie może być ujemna
	if (r > 100) r = 100;

    return r;
}


/*
	INNE
*/
extern void lcdPrint(LiquidCrystal lcd, String napis, int rzad) {
	lcd.setCursor(0,rzad);
	lcd.print(napis);
}

//przewijanie napisu po ekranie gdy ilo�� znak�w wi�ksza od 16 
extern void lcd16RollString(LiquidCrystal lcd, String napis, int rzad) {
    if(napis.length()>16){
    for(int i =0;i<=(napis.length()-16);i++)
    {
      //lcd.clear();
      lcd.setCursor(0,rzad);
      for(int j=i;j<=i+15;j++)
      {
        if(j<napis.length()){lcd.print(napis[j]);}
      }
      delay(1000);
    }}
    else{lcd.setCursor(0,rzad);
    for(int i=0;i<napis.length();i++){
    lcd.print(napis[i]);}
    }
}
  
  
//wyświetlanie wartości liczbowej, 4 ostatnie znaki na wyświetlaczu lcd
extern void displayTemp(float value, LiquidCrystal lcd, int row) {
	if (value < 10) //liczby jednocyfrowe
	{
		lcd.setCursor(12,row);
		lcd.print(" ");
		lcd.setCursor(13,row);
		lcd.print((String)value);
	}
	else if (value >= 10 && value <100 ) //liczby dwucyfrowe
	{
		lcd.setCursor(12,row);
		lcd.print((String)value);
	}
	else
	{
		lcd.setCursor(13,row);
		lcd.print("err");
	}
}

extern void tempSimul(RegulacjaPID regulacja, float& tValue, float tDesired) {
	int wspolczynnik;
	wspolczynnik =(int)(regulacja.regulator(tDesired,tValue));
	tValue += (wspolczynnik/10)*0.5;
	
	Serial.print(tDesired);
	Serial.print("\t");
	Serial.println(tValue);
	randomSeed(analogRead(A5));
	float randTemp = (random(0,700))/100;
	tValue -= randTemp;
	analogWrite(11,map(wspolczynnik,0,100,0,255));
	delay(200);
}

//WYŚLIJ KOMENDĘ
extern String sendCommand(SoftwareSerial rs, String caption) {
	rs.print(caption);
	rs.write(0x0D);
	return caption;
}