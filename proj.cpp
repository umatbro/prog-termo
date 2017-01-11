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
    float p, i, d, r;
    float e; //uchyb regulacji
    static float e_p = 0; //uchyb regulacji w poprzednim wywo�aniu
    static float s_e = 0; //suma minionych uchyb�w regulacji
    e = desired - tval; // aktualny uchyb regulacji

	// wyznaczenie skladnika proporcjonalnego
    p = k * e;

	// wyznaczenie składnika całkowego
	s_e += e; //najpierw trzeba wyliczyć sumę wszystkich uchybów;
	i = k_i * s_e;

	// wyznaczenie składnika D
	d = k_d * ( e - e_p);
	e_p = e; //zapisanie chwilowej wartości uchybu

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

  
//wyświetlanie wartości liczbowej value, 4 ostatnie znaki w wierszu row na wyświetlaczu lcd
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
	} else{ //liczby powyżej 100 niewyświetlane
		lcd.setCursor(13,row);
		lcd.print("err");
	}
}