#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 53
#define RST_PIN 52
#define IRQ_PIN 11

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

void setup() {
  Serial.begin(9600);  // Initialize serial communication
  SPI.begin();         // Initialize SPI bus
  mfrc522.PCD_Init();  // Initialize MFRC522 module
  attachInterrupt(digitalPinToInterrupt(IRQ_PIN), irqHandler, FALLING); // Attach interrupt handler
}

void loop() {
  // Main program loop
}

void irqHandler() {
  // Interrupt service routine
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  
  Serial.print("UID tag :");
  String uidString = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    uidString.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : ""));
    uidString.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println(uidString);
  
  // Dump the data stored in the card
 
}
