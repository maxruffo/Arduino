#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 9
#define SS_PIN 10

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance.

void setup() {
  Serial.begin(9600);  // Initialize serial communications.
  SPI.begin();          // Initialize SPI bus.
  mfrc522.PCD_Init();   // Initialize MFRC522 card reader.
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
    
    if (cardId == "5753974b") {
      Serial.println("Welcome!");
    }
    
    mfrc522.PICC_HaltA();  // Halt PICC
  }
}
