#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 9
#define SS_PIN 10
#define SPEAKER_PIN 8
#define trigPin 7
#define echoPin 6


#define REDLIGHT_PIN 4
#define WHITELIGHT_PIN 5
#define GREENLIGHT_PIN 3

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance.

float distance;
float current_distance;
bool alarm = false;

void setup() {
  
  Serial.begin(9600);  // Initialize serial communications.
  SPI.begin();          // Initialize SPI bus.
  mfrc522.PCD_Init();   // Initialize MFRC522 card reader.
  
  pinMode(SPEAKER_PIN, OUTPUT); // Initialize speaker pin
  pinMode(REDLIGHT_PIN, OUTPUT); // Initialize red light pin
  pinMode(WHITELIGHT_PIN, OUTPUT); // Initialize white light pin
  pinMode(GREENLIGHT_PIN, OUTPUT); // Initialize green light pin
  
  pinMode(trigPin, OUTPUT); // initialize trigger pin for distance Control
  pinMode(echoPin, INPUT); // init echo pin for distance Control
  
  Serial.println("Ready to read NFC tags!");
}

void playSound(bool a){

  if(a == true){
    digitalWrite(GREENLIGHT_PIN, HIGH);     
      // Play a sound from low to high
    for (int i = 1; i <= 13; i +=1){
      tone(SPEAKER_PIN,900);
      delay(30);
      noTone(SPEAKER_PIN);
    }
    noTone(SPEAKER_PIN);
    delay(30);
    tone(SPEAKER_PIN,600);
    delay(100);
    tone(SPEAKER_PIN,1000);
    noTone(SPEAKER_PIN);
    digitalWrite(GREENLIGHT_PIN, LOW);
  }
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
}

float getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  float duration = pulseIn(echoPin, HIGH);
  float distance = duration * 0.034 / 2;
  
  return distance;
}

float calibrateDistance() {
  Serial.println("Calibrating distance...");
  digitalWrite(WHITELIGHT_PIN, HIGH);

  // Wait for 10 seconds and check if the distance changes
  unsigned long startTime = millis();
  unsigned long lastDistanceCheckTime = 0;
  int lastDistance = 0;
  while (millis() - startTime < 10000) {
    if (millis() - lastDistanceCheckTime > 100) {
      int currentDistance = getDistance();
      if (currentDistance != lastDistance) {
        // Distance changed, reset timer
        lastDistanceCheckTime = millis();
        lastDistance = currentDistance;
      }
    }
    delay(10);
  }

  // Set distance variable as reference
  distance = lastDistance;
  return (distance);

  digitalWrite(WHITELIGHT_PIN, LOW);
}

void playAlarm(){
  for (int i = 0; i < 8; i++) {
    tone(SPEAKER_PIN, 800);
    delay(100);
    noTone(SPEAKER_PIN);
    delay(100);
  }
  for (int i = 0; i < 8; i++) {
    tone(SPEAKER_PIN, 200);
    delay(100);
    noTone(SPEAKER_PIN);
    delay(100);
  }
}

bool checkNFC(){
  // Look for new cards.
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    // Print card ID number to serial monitor.
    String cardId = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
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
    if (cardId == "5753974b" || cardId == "2a0c6180") {
      playSound(true);
      Serial.print("Welcome!");   

      return true;
    }
    // if the nfc card is wrong
    else{
      playSound(false);
      Serial.print("Intruder!");
    
      return false;
    }
    mfrc522.PICC_HaltA();  // Halt PICC
  }

}


void loop() {
  bool nfc = checkNFC();
  alarm = false;
  if (nfc) {
    distance = calibrateDistance();
    Serial.print("Calibrated distance: ");
    Serial.print(distance);
    Serial.println(" cm");
    digitalWrite(GREENLIGHT_PIN, HIGH);
    delay(1000);
    digitalWrite(GREENLIGHT_PIN, LOW);
    bool alarm = false;
    while (nfc) {
      current_distance = getDistance();
      if (abs(current_distance - distance) > 3) {  // check if distance changed by more than 3 cm
        // distance changed, set off alarm
        playAlarm();
        alarm = true;
      }
      delay(100);
      nfc = checkNFC();
    }
    while(alarm) {
      playSound(true);
      //alarm = false;
    }
  }
  delay(100);
}









