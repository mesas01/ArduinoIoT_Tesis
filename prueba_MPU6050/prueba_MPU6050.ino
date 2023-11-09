#include "WiFi.h"
#include <Adafruit_MPU6050.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <ThingSpeak.h>
//--------------------------------------------------------------------------
Adafruit_MPU6050 mpu;
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &Wire);
const char* ssid = "SLB";
const char* password =  "12345678";
//const char* ssid = "Apto1103";
//const char* password =  "Santiagobarrera";
//--------------------------------------------------------------------------
//Definimos las credenciales para la conexión a la plataforma
unsigned long channelID = 2288436;
const char* WriteAPIKey ="6ZY7N8I3NT479Y7R";
//Definimos el cliente WiFi que usaremos
WiFiClient cliente;
//--------------------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  // while (!Serial);
  Serial.println("MPU6050 OLED ");

  if (!mpu.begin()) {
    Serial.println("Sensor init failed");
    while (1)
      yield();
  }
  Serial.println("Found a MPU-6050 sensor");

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }
  display.display();
  delay(500); // Pause for 2 seconds
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setRotation(0);

//-------------------------------wifi-------------------------------------------

  WiFi.begin(ssid, password);
 
    while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
//--------------------------------------------------------------------------
//Una vez conectado, se iniciará la conexión a la Plataforma usando el cliente definido anteriormente*/
  ThingSpeak.begin(cliente);
  delay(5000);
}
//------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------
void loop() {
//Hacemos la conexión y envío de datos a la plataforma, utilizando las credenciales definidas anteriormente/
ThingSpeak.writeFields(channelID,WriteAPIKey);
//--------------------------------------------------------------------------
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  display.clearDisplay();
  display.setCursor(0, 0);

  Serial.print("Accelerometer ");
  Serial.print("X: ");
  Serial.print(a.acceleration.x, 2);
  Serial.print(" m/s^2, ");
  Serial.print("Y: ");
  Serial.print(a.acceleration.y, 2);
  Serial.print(" m/s^2, ");
  Serial.print("Z: ");
  Serial.print(a.acceleration.z, 2);
  Serial.println(" m/s^2");

  display.println("Accelerometer - m/s^2");
  display.print(a.acceleration.x, 2);
  display.print(", ");
  display.print(a.acceleration.y, 2);
  display.print(", ");
  display.print(a.acceleration.z, 2);
  display.println("");

  Serial.print("Gyroscope ");
  Serial.print("X: ");
  Serial.print(g.gyro.x, 2);
  Serial.print(" rps, ");
  Serial.print("Y: ");
  Serial.print(g.gyro.y, 2);
  Serial.print(" rps, ");
  Serial.print("Z: ");
  Serial.print(g.gyro.z, 2);
  Serial.println(" rps");

  display.println("Gyroscope - rps");
  display.print(g.gyro.x, 2);
  display.print(", ");
  display.print(g.gyro.y, 2);
  display.print(", ");
  display.print(g.gyro.z, 2);
  display.println("");
  display.display();
  delay(100);
//--------------------------------------------------------------------------

//Indicamos el orden de envío por campos o Field, en el orden definido de la plataforma, junto a los valores del sensor giroscopio*/
ThingSpeak.setField(1,a.acceleration.x);
ThingSpeak.setField(2,a.acceleration.y);
ThingSpeak.setField(3,a.acceleration.z);
//Indicamos el orden de envío por campos o Field, en el orden definido de la plataforma, junto a los valores del sensor Acelerometro*/
ThingSpeak.setField(4,g.gyro.x);
ThingSpeak.setField(5,g.gyro.y);
ThingSpeak.setField(6,g.gyro.z);
//Imprimimos una frase indicando el envío/
Serial.println("Datos enviados a ThingSpeak!");
}
