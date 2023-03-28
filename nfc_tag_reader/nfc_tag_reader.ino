#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 9
#define SS_PIN 10
#define BUTTON_PIN 2 // Assuming button connected to digital pin 2
#define BUTTON_HOLD_TIME 3000 // Button hold time in milliseconds

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance.
byte cardID[4]; // Array to store the card ID number
bool storeID = false; // Flag to indicate whether to store card ID number
unsigned long buttonPressTime = 0; // Time when the button was pressed

void setup() {
  Serial.begin(9600);  // Initialize serial communications.
  SPI.begin();          // Initialize SPI bus.
  mfrc522.PCD_Init();   // Initialize MFRC522 card reader.
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Set button pin as input with internal pull-up resistor
  Serial.println("Ready to read NFC tags!");
}

void loop() {
  if (digitalRead(BUTTON_PIN) == LOW) { // Check if button is pressed
    buttonPressTime = millis(); // Record button press time
    while (digitalRead(BUTTON_PIN) == LOW && millis() - buttonPressTime < BUTTON_HOLD_TIME) {
      // Wait for button to be released or hold time to be reached
    }
    if (millis() - buttonPressTime >= BUTTON_HOLD_TIME) { // Check if hold time is reached
      Serial.println("Button held for 3 seconds! Place card near reader...");
      storeID = true; // Set flag to store card ID number
      delay(500); // Debounce delay
    }
  }
  
  // Look for new cards if flag is set.
  if (storeID && mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    // Store card ID number in byte array.
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      cardID[i] = mfrc522.uid.uidByte[i];
    }
    Serial.println("Card ID stored!");
    storeID = false; // Reset flag
    mfrc522.PICC_HaltA();  // Halt PICC
    Serial.print("Stored card ID: ");
    for (byte i = 0; i < 4; i++) {
      Serial.print(cardID[i] < 0x10 ? " 0" : " ");
      Serial.print(cardID[i], HEX);
    }
    Serial.println();
  }
}
