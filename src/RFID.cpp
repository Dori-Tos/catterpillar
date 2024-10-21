#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         9          // Configurable, see typical pin layout above
#define SS_1_PIN        20          // Configurable, take an unused pin, must be different from other SS pins

MFRC522 mfrc522 (SS_1_PIN, RST_PIN);   // Create MFRC522 instance.

void setup() {
  Serial.begin(9600); // Initialize serial communications with the PC
  while (!Serial);    // Do nothing if no serial port is opened 

  SPI.begin();        // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522
  mfrc522.PCD_DumpVersionToSerial(); // Show details of the MFRC522 module
  Serial.println(F("Scan an RFID card to see its UID"));
}

void loop() {
  // Look for new RFID cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return; // If no new card is present, exit the loop
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return; // If no card could be read, exit the loop
  }

  // Dump UID (Unique Identifier) of the card
  Serial.print(F("Card UID:"));
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.println();

  // Dump other details of the card
  mfrc522.PICC_DumpToSerial(&(mfrc522.uid));

  // Halt PICC to stop communication
  mfrc522.PICC_HaltA();
  // Stop encryption on PCD
  mfrc522.PCD_StopCrypto1();
}
