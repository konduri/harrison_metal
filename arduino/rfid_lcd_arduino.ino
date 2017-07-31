//For Keypad
#include <Keypad.h>

//For Servo
#include <Servo.h>

//For LCD
#include <LiquidCrystal.h>

// MFRC522 - Version: Latest // FOR RFID
#include <MFRC522.h>
#include <MFRC522Extended.h>
#include <deprecated.h>
#include <require_cpp11.h>
#include <SPI.h>


//RFID initializations
#define RST_PIN   5     // Configurable, see typical pin layout above
#define SS_PIN    53   // Configurable, see typical pin layout above
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance
/* Set your new UID here! */
#define NEW_UID {0xDE, 0xAD, 0xBE, 0xEF}
MFRC522::MIFARE_Key key;

//For Servo initialization
Servo myservo;  // create servo object to control a servo
int pos = 0; // Initial postion for servo

//For Buzzer initialization
int buzzer = 3;//the pin of the active buzzer

//LCD initialization
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

//Keypad initialization
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = 
{
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {A0, A1, A2, A3}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {A4, A5, A6, A7}; //connect to the column pinouts of the keypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 


void setup() 
{
  //Baud rate
  Serial.begin(9600);
  
  while (!Serial);     // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  SPI.begin();         // Init SPI bus
  mfrc522.PCD_Init();  // Init MFRC522 card
  //Serial.println(F("Warning: this example overwrites the UID of your UID changeable card, use with care!"));
  
  //Servo setup
  myservo.attach(2); // Setup the servo on this pin
  //Buzzer setup
  pinMode(buzzer,OUTPUT);//initialize the buzzer pin as an output
  //LCD setup
  lcd.begin(16, 2);

  // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
}


void loop() 
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Reading");

    // Look for new cards, and select one if present
  if ( ! mfrc522.PICC_IsNewCardPresent() || ! mfrc522.PICC_ReadCardSerial() ) {
    delay(50);
    return;
  }
  // Now a card is selected. The UID and SAK is in mfrc522.uid.
  // Dump UID
  Serial.print(F("Card UID:"));

  String content= "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    //Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    //Serial.print( mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  } 
  Serial.println(content);
  Serial.println(">");
  content.toUpperCase();  
  
  //Scanned, now check if its correct RFID
  if (content.substring(1) == "31 74 84 1B") //
  {
    bool enable_servo = false;
    lcd.clear();
    lcd.print("Enter PIN");
    char stringOne[] = "xxxx";
    int char_count=0;
    int timer_count=0;
    
    while (timer_count <10000) //10 seconds to enter key
    {
      timer_count++;
      delay(1);
      char customKey = customKeypad.getKey();
      if (customKey)
      {
        stringOne[char_count] =customKey;
        char_count++;
        lcd.clear();
        lcd.print(stringOne);
        if (char_count==4)  //No of characters in PIN
        {
          delay(500); //For user to see password on display
          if (String(stringOne)=="4321") //Brutally hardcoded password
          {
            lcd.clear();
            lcd.print("Access Grandted!");
            
            lcd.setCursor(0, 1);
            lcd.print("Opening Door");
            enable_servo=true;
            break;
          }
          else
          {
            lcd.clear();
            lcd.print("Incorrect");
            delay(1500);
            break;
          };
          
        };
        
      };
    };
    
    if (enable_servo)
    {
      //Move imaginary lock
      for (pos = 0; pos <= 180; pos += 1) 
      { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
        myservo.write(pos);              // tell servo to go to position in variable 'pos'
        delay(15);                       // waits 15ms for the servo to reach the position
      }
      for (pos = 180; pos >= 0; pos -= 1) 
      { // goes from 180 degrees to 0 degrees
        myservo.write(pos);              // tell servo to go to position in variable 'pos'
        delay(15);                       // waits 15ms for the servo to reach the position
      }
      delay(500);
    };
  }
  else //Incorrect RFID swiped
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Denied Access");
    for(int i=0;i<2000;i++)
    {
    digitalWrite(buzzer,HIGH);
    delay(1);//wait for 1ms
    digitalWrite(buzzer,LOW);
    delay(1);//wait for 1ms
    }
  };
  
  //debounce for RFID
  delay(1000);
}
