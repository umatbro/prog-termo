#include "proj.h"


/*
	------------------------------------------------------------------
							METODY
	------------------------------------------------------------------
*/


/* ------------------------TEMPERATURE----------------------------- */


Temperature::Temperature(float tv) {
	val = tv;
}

float Temperature::getTempValue(MAX6675 thermocouple) {
	val = 0.95*(thermocouple.readCelsius()) - 11.58;
	return val;
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
	if( (millisecNow - millisecStart > milliseconds) && (millisecNow - millisecStart)%milliseconds < threshold ) {
		millisecStart = millis();
		return true;
	}
	else return false;
}


/*----------------------------MANSON 2405---------------------------------*/

String Manson2405::getResponse(SoftwareSerial rs485) {
	rs485.begin(9600);
	String response;
	rs485.listen();
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

String Manson2405::sendCommand(SoftwareSerial rs, String caption) {
	rs.print(caption);
	rs.write(0x0D);
	return caption;
}

String Manson2405::startSession(SoftwareSerial rs, int adress){
	String adressString = (String)adress;
	if(adress<10) adressString = "0" + adressString;
	
	sendCommand(rs, "SESS"+adressString);
	return adressString;
}

String Manson2405::endSession(SoftwareSerial rs, int adress){
	String adressString = (String)adress;
	if(adress<10) adressString = "0" + adressString;
	
	sendCommand(rs,"ENDS"+adressString);
	return adressString;
}

/*--------------------------REGULATOR PID--------------------------------*/
void RegPID::setK(float value){
	k = value;
}
void RegPID::setKI(float value){
	k_i = value;
}
void RegPID::setKD(float value){
	k_d = value;
}

RegPID::RegPID(float k_, float k_i_, float k_d_){
	setK(k_);
	setKI(k_i_);
	setKD(k_d_);
}

float RegPID::regulator(float desired, float tval)  {

    //zmienne pomocnicze
    float p,i,d,r;
    float u; //uchyb regulacji
    static float u_p = 0; //uchyb regulacji w poprzednim wywo�aniu
    static float su = 0; //suma minionych uchyb�w regulacji
    u = desired - tval; // aktualny uchyb regulacji

	// wyznaczenie skladnika proporcjonalnego
    p = k * u;

	// wyznaczenie składnika całkowego
	su = su + u; //najpierw trzeba wyliczyć sumę wszystkich uchybów;
	i = k_i * su;

	// wyznaczenie składnika D
	d = k_d * (u-u_p);
	u_p = u; //zapisanie chwilowej wartości uchybu

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
		for(int i =0;i<=(napis.length()-16);i++) {
		  //lcd.clear();
		  lcd.setCursor(0,rzad);
		  for(int j=i;j<=i+15;j++) {
			if(j<napis.length()){lcd.print(napis[j]);}
		  }
		  delay(1000);
		}
	} else {
		lcd.setCursor(0,rzad);
		for(int i=0;i<napis.length();i++){
			lcd.print(napis[i]);
		}
    }
}
  
  
//wyświetlanie wartości liczbowej, 4 ostatnie znaki na wyświetlaczu lcd
extern void displayTemp(float value, LiquidCrystal lcd, int row) {
	if (value < 10) { //liczby jednocyfrowe
		lcd.setCursor(12,row);
		lcd.print(" ");
		lcd.setCursor(13,row);
		lcd.print((String)value);
	}
	else if (value >= 10 && value <100 ) { //liczby dwucyfrowe
		lcd.setCursor(12,row);
		lcd.print((String)value);
	} else{
		lcd.setCursor(13,row);
		lcd.print("err");
	}
}