//Librerias
#include <ThingSpeak.h>
#include <SPI.h>
#include <MFRC522.h>  
#include "WiFi.h"
//--------------------------------------------------------------------------
//Constants
#define SS_PIN 21
#define RST_PIN 4
//--------------------------------------------------------------------------
const char* ssid = "SLB";
const char* password =  "12345678";
//const char* ssid = "Apto1103";
//const char* password =  "Santiagobarrera";
//--------------------------------------------------------------------------
//Parameters
const int ipaddress[4] = {103, 97, 67, 25};
byte globalValue0;  // Variable global para almacenar el valor
byte globalValue1;  // Variable global para almacenar el valor

//--------------------------------------------------------------------------
//Variables
byte nuidPICC[4] = {0, 0, 0, 0};
MFRC522::MIFARE_Key key;
MFRC522 rfid = MFRC522(SS_PIN, RST_PIN);
//--------------------------------------------------------------------------
//Definimos las credenciales para la conexión a la plataforma
unsigned long channelID = 2288436;
const char* WriteAPIKey ="6ZY7N8I3NT479Y7R";
//Definimos el cliente WiFi que usaremos
WiFiClient cliente;
//--------------------------------------------------------------------------

void setup() {
  
   //Init Serial USB
  Serial.begin(115200);
  delay(1000);
  Serial.println(F("Initialize System"));
  //init rfid D8,D5,D6,D7
  SPI.begin();
  rfid.PCD_Init();

  Serial.print(F("Reader :"));
  rfid.PCD_DumpVersionToSerial();

//--------------------------------------------------------------------------

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
//--------------------------------------------------------------------------
void loop() {
/*Hacemos la conexión y envío de datos a la plataforma, utilizando las credenciales definidas anteriormente*/
  ThingSpeak.writeFields(channelID,WriteAPIKey);
  readRFID();
//Indicamos el orden de envío por campos o Field, en el orden definido de la plataforma, junto a los valores del sensor*/
  ThingSpeak.setField(7,globalValue0);
  ThingSpeak.setField(8,globalValue1);
  //*Imprimimos una frase indicando el envío*/
Serial.println("Datos enviados a ThingSpeak!");
}
//--------------------------------------------------------------------------
void readRFID(void ) { /* function readRFID */
  ////Read RFID card

  for (byte i = 0; i < 6; i++) {
      key.keyByte[i] = 0xFF;
  }
  // Look for new 1 cards
  if ( ! rfid.PICC_IsNewCardPresent())
      return;

  // Verify if the NUID has been readed
  if (  !rfid.PICC_ReadCardSerial())
      return;

  // Store NUID into nuidPICC array
  for (byte i = 0; i < 4; i++) {
      nuidPICC[i] = rfid.uid.uidByte[i];
  }

  Serial.print(F("RFID In dec: "));
  printDec(rfid.uid.uidByte, rfid.uid.size);
  Serial.println();

  // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
//--------------------------------------------------------------------------

}

//Helper routine to dump a byte array as dec values to Serial.
void printDec(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
     globalValue0 = buffer[0];  // Asignar el valor a la variable global
     globalValue1 = buffer[1];  // Asignar el valor a la variable global

      Serial.print(buffer[i] < 0x10 ? " 0" : " ");
      Serial.print(buffer[i], DEC);

  }
}

