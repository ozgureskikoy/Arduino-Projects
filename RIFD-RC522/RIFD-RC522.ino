#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         22          
#define SS_PIN          21          

MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(9600);        
  SPI.begin();               
  mfrc522.PCD_Init();
pinMode(26,OUTPUT);
}
void loop() {
  
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }

  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
     
  } 
      content.toUpperCase();
  if ((content.substring(1) == "49 8F 9B 99")  ) //|| (content.substring(1) == "46 08 74 F9")
       { Serial.println("   Authorized Access");
        digitalWrite(26,HIGH);
        
        delay(3000);
        digitalWrite(26,LOW);
        }
        
  else
  {
    Serial.println("  Access denied");
        digitalWrite(26,LOW);}
}
