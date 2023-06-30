#include <ESP8266WiFi.h>
#include <ThingESP.h>
#include <DHT.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// librerias y objeto del sensor temp y hum
#define DHTTYPE DHT11 // Tipo de sensor DHT (DHT11)
#define DHTPIN 0 // Pin de datos del sensor DHT11
DHT dht(DHTPIN, DHTTYPE);
const int mq7_pin = A0;
LiquidCrystal_I2C lcd(0x27,16,2);

// objeto de conexion con web server ThingESP para alertas whassp
ThingESP8266 thing("juanpasq", "puertaDepto13", "30281182");

//int LED = D1;
int zumbador = 13; //D7 ESP8266
unsigned long previousMillis = 0;
const long INTERVAL = 5000;  

void setup()
{
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  dht.begin();
  //pinMode(LED, OUTPUT);
  thing.SetWiFi("JMP", "juan[1255]");
  thing.initDevice();
  pinMode(zumbador, OUTPUT);


}

// funcion para peticiones WhatsApp
String HandleResponse(String query){

  Serial.println("Procesando peticion");

  // bloque de toma de valores de sensores
  int mq7Value = analogRead(mq7_pin);
  // Convertimos la lectura en un valor de voltaje
  float voltaje = mq7Value * (5.0 / 1023.0);

  // Lee la temperatura en grados Celsius y humedad
  float tem = dht.readTemperature();
  float hum = dht.readHumidity();

  /*
    Serial.print("adc:");
    Serial.print(mq7Value);
    Serial.print("    voltaje:");
    Serial.println(voltaje);
  */


  // por consola para test
  // Verifica si la lectura fue exitosa
  /*if (isnan(tem || hum)) {
      Serial.println("Error al leer la temperatura o humedad!");
  } else{

    //valores por consola
    Serial.print("Nivel CO: ");
    Serial.print(mq7Value);
    Serial.println("%");
    Serial.print("Temperatura: ");
    Serial.print(tem);
    Serial.println("°C");
    Serial.print("Humedad: ");
    Serial.print(hum);
    Serial.println("%");
    
  }

 */

  // me llega "tem" y retorno valor
  if (query == "tem") {
    //digitalWrite(LED, 1);
    return "Temperatura: "+String(tem)+ " ºC";
  }
  // me llega "hum" y retorno valor
  else if (query == "hum") {
    //digitalWrite(LED, 0);
    return "Humedad: "+String(hum)+" %";
  }
  // me llega "co" y retorno valor
  else if (query == "co")
    //return digitalRead(LED) ? "LED is OFF" : "LED is ON";
    return "Nivel CO: "+String(mq7Value)+" %";
  
  else if (query == "todo")
    //return digitalRead(LED) ? "LED is OFF" : "LED is ON";
    return "Temperatura: "+String(tem)+" ºC"+" - Humedad: "+String(hum)+" %"+ " - Nivel CO: "+String(mq7Value)+" %";
  
  // cualquier otro mensaje consulta que le llega
  else return "Consulta invalida. Ingrese palabra tem, hum, co o todo";

}

void loop(){
  
  Serial.println("//////////////////////ini//////////////////////////////");
  
  //Serial.println("Comienzo loop");
  thing.Handle();
  // tiempo de ventana donde espera respuesta
  delay(2000);

  // creamos nueva variable para leer datos
  float tem1 = dht.readTemperature();
  float hum1 = dht.readHumidity();

  // bloque de toma de valores de sensores
  int mq7Value1 = analogRead(mq7_pin);
  // Convertimos la lectura en un valor de voltaje
  float voltaje1 = mq7Value1 * (5.0 / 1023.0);
  
  //valores por consola
    Serial.print("Nivel CO: ");
    Serial.print(mq7Value1);
    Serial.println("%");
    Serial.print("Temperatura: ");
    Serial.print(tem1);
    Serial.println("°C");
    Serial.print("Humedad: ");
    Serial.print(hum1);
    Serial.println("%");

  // alarma
  if(hum1 > 70){
    digitalWrite(zumbador, HIGH);
  }
  else {
    digitalWrite(zumbador, LOW);
  }
  
  char buffer1[10];
  dtostrf(tem1, 6, 2, buffer1); 
  
  char buffer2[10];
  dtostrf(hum1, 6, 2, buffer2); 

  char buffer3[10];
  dtostrf(mq7Value1, 6, 2, buffer3); 
  //limpiamos lcd
  lcd.clear();
  // posicionamos el mensaje 
  lcd.setCursor(0,0);
  lcd.print("T:");
  lcd.print(buffer1);
  //lcd.print("ºC - ");
  lcd.print(" H:");
  lcd.print(buffer2);
  lcd.print(" %");
  // posicionamos el mensaje 
  lcd.setCursor(0,1);
  lcd.print("Nivel CO: ");
  lcd.print(buffer3);
  //lcd.print(" %");

  delay(2000);

  Serial.println("/////////////////////end///////////////////////////////");
}
