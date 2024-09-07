#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
#define pinledvermelho 4
#define pinledverde 5

MFRC522 rfid(SS_PIN, RST_PIN);  // Instance of the class
MFRC522::MIFARE_Key key;

int code[] = { 224, 155, 245, 25 };  //This is the stored UID (Unlock Card)
int techfarm_card[] = { 83, 00, 148, 49 };  //This is the stored UID (Unlock Card)

int codeRead = 0;
int pinbuzzer = 2;
int tempo = 400;
String uidString;

void setup() {
  Serial.begin(9600);
  SPI.begin();      // Init SPI bus
  rfid.PCD_Init();  // Init MFRC522
  Serial.println(F("Arduino RFID Gustavo e Virgilio"));
  pinMode(pinledvermelho, OUTPUT);
  pinMode(pinledverde, OUTPUT);
  pinMode(pinbuzzer, OUTPUT);
}

void loop() {

  if (rfid.PICC_IsNewCardPresent()) {
    readRFID();
  }
  delay(100);
}



void readRFID() {
  rfid.PICC_ReadCardSerial();
  Serial.print(F("\n Tipo de PICC: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  Serial.println(rfid.PICC_GetTypeName(piccType));

  // Check is the PICC of Classic MIFARE type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI && piccType != MFRC522::PICC_TYPE_MIFARE_1K && piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("Your tag is not of type MIFARE Classic."));
    return;
  }

  Serial.println("UID da PICC escaneado:");
  printDec(rfid.uid.uidByte, rfid.uid.size);
  uidString = String(rfid.uid.uidByte[0]) + " " + String(rfid.uid.uidByte[1]) + " " + String(rfid.uid.uidByte[2]) + " " + String(rfid.uid.uidByte[3]);
  int i = 0;
  boolean match = true;
  boolean tech_farm = false;
  while (i < rfid.uid.size) {
    if (!(int(rfid.uid.uidByte[i]) == int(code[i]))) {
      match = false;
    }
    if ((int(rfid.uid.uidByte[i]) == int(techfarm_card[i]))) {
      match = false;
      tech_farm = true;
    }    
    i++;
  }

  if (tech_farm == true) {

    digitalWrite(pinledverde, HIGH);
    delay(150);
    Serial.println("\n*** Bem Vindo, Tech Farm! ***");
    delay(150);
    tone(pinbuzzer, 294, tempo);
    delay(tempo);
    digitalWrite(pinledverde, LOW);
    delay(150); 

  }
  if (match) {
    digitalWrite(pinledverde, HIGH);
    delay(150);
    Serial.println("\n*** Acesso Liberado! ***");
    delay(150);
    tone(pinbuzzer, 294, tempo);
    delay(tempo);
    digitalWrite(pinledverde, LOW);
    delay(150);    
  } else {
    digitalWrite(pinledvermelho, HIGH);
    delay(150);
    Serial.println("\nCartao Desconhecido!");
    delay(150);
    tone(pinbuzzer, 440, tempo);
    delay(tempo);
    digitalWrite(pinledvermelho, LOW);
    delay(150);     
  }
  Serial.println("============================");

  rfid.PICC_HaltA();

  // Stop encryption on PCD

  rfid.PCD_StopCrypto1();
}



void printDec(byte *buffer, byte bufferSize) {

  for (byte i = 0; i < bufferSize; i++) {

    Serial.print(buffer[i] < 0x10 ? " 0" : " ");

    Serial.print(buffer[i], DEC);
  }
}