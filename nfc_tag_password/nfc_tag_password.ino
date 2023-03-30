#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 9
#define SS_PIN 10
#define SPEAKER_PIN 9
#define REDLIGHT_PIN 7
#define WHITELIGHT_PIN 4
#define GREENLIGHT_PIN 2

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance.

void setup() {
  Serial.begin(9600);  // Initialize serial communications.
  SPI.begin();          // Initialize SPI bus.
  mfrc522.PCD_Init();   // Initialize MFRC522 card reader.
  pinMode(SPEAKER_PIN, OUTPUT); // Initialize speaker pin
  pinMode(REDLIGHT_PIN, OUTPUT); // Initialize red light pin
  pinMode(WHITELIGHT_PIN, OUTPUT); // Initialize white light pin
  pinMode(GREENLIGHT_PIN, OUTPUT); // Initialize green light pin
  Serial.println("Ready to read NFC tags!");
}

void loop() {
  // Look for new cards.
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    // Print card ID number to serial monitor.
    Serial.print("Card ID: ");
    String cardId = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
      Serial.print(mfrc522.uid.uidByte[i], HEX);
      cardId.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : ""));
      cardId.concat(String(mfrc522.uid.uidByte[i], HEX));
    }

    Serial.println();
    Serial.println(cardId);
    
    for (int i = 31; i <= 1568; i += 62) {
      digitalWrite(WHITELIGHT_PIN, HIGH);
      tone(SPEAKER_PIN, i);
      delay(20);
      digitalWrite(WHITELIGHT_PIN, LOW);
    } 
    noTone(SPEAKER_PIN);
    delay(1000);
    //if the card is right
    if (cardId == "5753974b") {
      digitalWrite(GREENLIGHT_PIN, HIGH);      
      Serial.println("Welcome!");
      // Play a sound from low to high
      for (int i = 1; i <= 13; i +=1){
        tone(SPEAKER_PIN,900);
        delay(30);
        noTone(SPEAKER_PIN);
      }
      noTone(SPEAKER_PIN);
      delay(30);
      tone(SPEAKER_PIN,600);
      delay(30);
      tone(SPEAKER_PIN,1000);
      noTone(SPEAKER_PIN);
      digitalWrite(GREENLIGHT_PIN, LOW);
    }
    // if the nfc card is wrong
    else{
      digitalWrite(REDLIGHT_PIN, HIGH);
      tone(SPEAKER_PIN,500);
      delay(150);
      noTone(SPEAKER_PIN);
      tone(SPEAKER_PIN,500);
      delay(150);
      noTone(SPEAKER_PIN);
      tone(SPEAKER_PIN,500);
      delay(150);
      noTone(SPEAKER_PIN);
      tone(SPEAKER_PIN,500);
      noTone(SPEAKER_PIN);
      digitalWrite(REDLIGHT_PIN, LOW);


    }
    
    
    mfrc522.PICC_HaltA();  // Halt PICC
  }
}
