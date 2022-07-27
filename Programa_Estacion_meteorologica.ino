//#include <virtuabotixRTC.h>
#include <SFE_BMP180.h>
#include <SD.h>
#include "DHT.h"
#include <Wire.h>
#include  <LiquidCrystal.h>
#include <Wire.h>
#include "RTClib.h"


//RTC_DS1307 rtc;
RTC_DS3231 rtc;
//Wiring SCLK -> 6, I/O -> 7, CE -> 4
//virtuabotixRTC myRTC (6, 7, 4); // Si cambia el cableado, cambie los pines aquí también
SFE_BMP180 bmp180;
DHT dht(2, DHT11);
File myFile;
double PresionNivelMar=1013.25; //presion sobre el nibel del mar en mbar
LiquidCrystal lcd(8, 10, 24, 25, 26, 27);
int segundo,minuto,hora,dia,mes, residuo;
long anio; //variable año
DateTime HoraFecha;
//const int sensorPin = A8;    const int sensorVel = A8;   
int sensorValue, velocidad; 
float sensorValuex, aux,resul;
void setup()
{
  Serial.begin(9600);
  rtc.begin(); //Inicializamos el RTC
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("CESMAG_TG");
  lcd.setCursor(6, 1);
  lcd.print("2021");
  delay(1000);
  dht.begin();
  if (bmp180.begin())
    Serial.println("BMP180 iniciado correctamenten");
  else
  {
    Serial.println("Error al iniciar el BMP180");
    //while(1); // bucle infinito
  }
  Serial.print("Iniciando SD ...");
  if (!SD.begin(4)) {
    Serial.println("No se pudo inicializar");
    return;
  }
  Serial.println("inicializacion exitosa");
 
}

void loop()
{
  HoraFecha = rtc.now(); //obtenemos la hora y fecha actual
  segundo=HoraFecha.second();
  minuto=HoraFecha.minute();
  hora=HoraFecha.hour();
  dia=HoraFecha.day();
  mes=HoraFecha.month();
  anio=HoraFecha.year();
  residuo = minuto % 4;
  delay(2000);
  char status;
  double T,P,A;
  lcd.clear();
  
  lcd.setCursor(2, 0);
  lcd.print("7/11/21 1:00PM  ");
  lcd.setCursor(1, 1);
  lcd.print("H:75 T:14 L:980");
  delay(2000);
  float h = dht.readHumidity();
  float t = dht.readTemperature();
   //sensorValue = analogRead(sensorPin); 
   resul =analogRead(A0); 
  aux = resul*5;
  aux= aux/890;
  velocidad = aux*6;
    
   
   //Serial.print(sensorValue);
  //Serial.println(velocidad);
  status = bmp180.startTemperature();
         if (status != 0)
        {   
           delay(status); //Pausa para que finalice la lectura
           status = bmp180.getTemperature(T); //Obtener la temperatura
           if (status != 0)
             {
               status = bmp180.startPressure(3);//Inicio lectura de presión
             if (status != 0)
             {        
                  delay(status);//Pausa para que finalice la lectura        
                  status = bmp180.getPressure(P,T);//Obtenemos la presión
             if (status != 0)
              {                  
                    
                 //-------Calculamos la altitud--------
                A= bmp180.altitude(P,PresionNivelMar);
              
              }      
            }      
         }   
       } 
  
  residuo = minuto % 5;

  if (residuo == 0){
    myFile = SD.open("datalog.txt", FILE_WRITE);//abrimos  el archivo
  
      if (myFile) { 
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Guardando Datos");
        lcd.setCursor(0, 1);
        //lcd.print("h=");
        lcd.print(hora,1);
        lcd.print(":");
        lcd.print(minuto,1);
        lcd.print(":");
        lcd.print(segundo,1);
        lcd.print("  ");
        lcd.print(dia,1);
        lcd.print("/");
        lcd.print(mes,1);
        lcd.print("/");
        lcd.print(anio,1);
        Serial.println("Guardando Datos Estacion: ");  
        Serial.print("hora = ");
        Serial.print(hora);
        Serial.print(":");
        Serial.print(minuto);
        Serial.print(":");
        Serial.print(segundo);
        Serial.print("  Fecha = ");
        Serial.print(dia);
        Serial.print("/");
        Serial.print(mes);
        Serial.print("/");
        Serial.println(anio);
       
        myFile.print("hora = ");
        myFile.print(hora);
        myFile.print(":");
        myFile.print(minuto);
        myFile.print(":");
        myFile.print(segundo);
        myFile.print("  Fecha = ");
        myFile.print(dia);
        myFile.print("/");
        myFile.print(mes);
        myFile.print("/");
        myFile.print(anio);
        myFile.print(" ");

        myFile.print(", Velocidad=");
        myFile.print(velocidad);
        myFile.print(", Humedad=");
        myFile.print(h);
        myFile.print(" % ");
        myFile.print(", Temperatura=");
        myFile.print(t);
        myFile.print(" *C ");
        myFile.print(", Presion=");
        myFile.print(P);
        myFile.print(" mb ");
        myFile.print(", Altitud=");
        myFile.print(A);
        myFile.println(" m s.n.m.");  
                
        myFile.close(); //cerramos el archivo
 
        } else {
          Serial.println("Error al abrir el archivo");
     }
    
  }else {
     

         Serial.print("hora = ");
         Serial.print(hora);
         Serial.print(":");
         Serial.print(minuto);
         Serial.print(":");
         Serial.print(segundo);
         Serial.print("  Fecha = ");
         Serial.print(dia);
         Serial.print("/");
         Serial.print(mes);
         Serial.print("/");
         Serial.print(anio);
        
        Serial.print("  ");
        Serial.print(", Velocidad=");
        Serial.print(velocidad);
        Serial.print(" m/s ");
        Serial.print(", Humedad=");
        Serial.print(h);
        Serial.print(" % ");
        Serial.print(", Temperatura=");
        Serial.print(t);
        Serial.print(" *C");
        Serial.print(", Presion=");
        Serial.print(P);
        Serial.print(" mb ");
        Serial.print(", Altitud=");
        Serial.print(A);
        Serial.println(" m s.n.m."); 
       
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(" Fecha y Hora");
        lcd.setCursor(0, 1);
        //lcd.print("h=");
        lcd.print(hora,1);
        lcd.print(":");
        lcd.print(minuto,1);
        lcd.print(":");
        lcd.print(segundo,1);
        lcd.print("  ");
        lcd.print(dia,1);
        lcd.print("/");
        lcd.print(mes,1);
        lcd.print("/");
        lcd.print(anio,1);
        delay(2000);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("H=");
        lcd.print(h,1);
        lcd.print("%");
        lcd.print(" ");
        lcd.setCursor(8, 0);
        lcd.print("T=");
        lcd.print(t,1);
        lcd.print("C");
        lcd.print(" ");
        delay(2000);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("P=");
        lcd.print(P,1);
        lcd.print("mb");
        lcd.print(" ");
        lcd.setCursor(0, 1);
        lcd.print("A=");
        lcd.print(A,1);
        lcd.print("m s.n.m.");
        lcd.print(" ");
        delay(2000);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("V=");
        lcd.print(velocidad,1);
        lcd.print("m/s");
        lcd.print(" ");
       
  }
 
  
 
  delay(1000);
}
