#include <Arduino.h>
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <ESP_Mail_Client.h>

#define WIFI_SSID "MBKd"
#define WIFI_PASSWORD "00000000"

/** The smtp host name e.g. smtp.gmail.com for GMail or smtp.office365.com for Outlook or smtp.mail.yahoo.com */
#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465

/* The sign in credentials */
#define AUTHOR_EMAIL "mohamedbilelkhadhraoui@gmail.com"
#define AUTHOR_PASSWORD "cpde xrxz jjwh lqtw"

/* Recipient's email*/
#define RECIPIENT_EMAIL "bkhadhraoui870@gmail.com"

/* Declare the global used SMTPSession object for SMTP transport */
SMTPSession smtp;

/* Callback function to get the Email sending status */
void smtpCallback(SMTP_Status status);

#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>

#define RST_PIN     4
#define SS_PIN      5

MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key; 

// Course line 
int Prof =0;
int Stud =0;
// Init array that will store new NUID 

byte ProfessorRfid[4] = {134, 117, 126, 31};
byte nuidPICC[4] = {0, 0, 0, 0};
// set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 1;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  

/* Declare the message class */
SMTP_Message message;
/* Declare the Session_Config for user defined session credentials */
  Session_Config config;

void setup() 
{
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522 
  lcd.print("Starting System");
  for (byte i = 0; i < 6; i++) 
  {
    key.keyByte[i] = 0xFF;
  }
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(300);
  }
  MailClient.networkReconnect(true);
  smtp.debug(1);

  /* Set the callback function to get the sending results */
  //smtp.callback(smtpCallback);

  

  /* Set the session config */
  config.server.host_name = SMTP_HOST;
  config.server.port = SMTP_PORT;
  config.login.email = AUTHOR_EMAIL;
  config.login.password = AUTHOR_PASSWORD;
  config.login.user_domain = "";

  /*
  Set the NTP config time
  For times east of the Prime Meridian use 0-12
  For times west of the Prime Meridian add 12 to the offset.
  Ex. American/Denver GMT would be -6. 6 + 12 = 18
  See https://en.wikipedia.org/wiki/Time_zone for a list of the GMT/UTC timezone offsets
  */
  config.time.ntp_server = F("pool.ntp.org,time.nist.gov");
  config.time.gmt_offset = 3;
  config.time.day_light_offset = 0;

  

  /* Set the message headers */
  message.sender.name = F("Professor Louay");
  message.sender.email = AUTHOR_EMAIL;
  message.subject = F("Gamification Session");
  message.addRecipient(F("Sara"), RECIPIENT_EMAIL);
   
  //Send raw text message
  String textMsg = "Dear Bilel, Your current Gamification Course takes place in Classroom 007. Enjoy.";
  message.text.content = textMsg.c_str();
  message.text.charSet = "us-ascii";
  message.text.transfer_encoding = Content_Transfer_Encoding::enc_7bit;
  
  message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_low;
  message.response.notify = esp_mail_smtp_notify_success | esp_mail_smtp_notify_failure | esp_mail_smtp_notify_delay;


  /* Connect to the server */
  if (!smtp.connect(&config)){
    return;
  }

  if (!smtp.isLoggedIn()){
  }
  else{
    if (smtp.isAuthenticated())
    {

    }
  }
  lcd.clear();
}

void loop() 
{
    smtp.callback(smtpCallback);

  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if ( ! rfid.PICC_IsNewCardPresent())
    return;

  // Verify if the NUID has been readed
  if ( ! rfid.PICC_ReadCardSerial())
    return;

  for (byte i = 0; i < 4; i++) 
  {
    nuidPICC[i] = rfid.uid.uidByte[i];
  }
  if (ProfessorRfid[0] == nuidPICC[0] && ProfessorRfid[1] == nuidPICC[1] && 
        ProfessorRfid[2] == nuidPICC[2] && ProfessorRfid[3] == nuidPICC[3] )
  {
    if (Prof == 0 )
    {
      Prof = 1;
      lcd.setCursor(0, 0);
      lcd.print("WelcomeProfessor");
      delay(2500);
      lcd.clear(); 
      if (Prof == 1)
      {
        Serial.println("PPPP");
        if (!MailClient.sendMail(&smtp, &message))
        {

        }

      }
      

    }
    else
    {
      lcd.setCursor(0, 0);
      lcd.print("Access Denied");
      delay(1500);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("ProfessorInClass");
      delay(1500);
      lcd.clear(); 
    }
  }
  else
  {
    if ( Stud == 0 )
    {
      if ( Prof == 1 )
      {
        lcd.setCursor(0, 0);
        lcd.print("Welcome Bilel");
        delay(2500);
        lcd.clear();
        Stud = 1;
      }
      else 
      {
        lcd.setCursor(0, 0);
        lcd.print("Access Denied");
        delay(1500);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("WairForProfessor");
        delay(1500);
        lcd.clear();
      }
    } 
    else
    {
      lcd.setCursor(0, 0);
      lcd.print("Access Denied");
      delay(1500);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("BilelInClass");
      delay(1500);
      lcd.clear();
    }
  }

  // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
}


/* Callback function to get the Email sending status */
void smtpCallback(SMTP_Status status){
  /* Print the current status */
  Serial.println(status.info());

  /* Print the sending result */
  if (status.success()){
    // ESP_MAIL_PRINTF used in the examples is for format printing via debug Serial port
    // that works for all supported Arduino platform SDKs e.g. AVR, SAMD, ESP32 and ESP8266.
    // In ESP8266 and ESP32, you can use Serial.printf directly.

    Serial.println("----------------");
    ESP_MAIL_PRINTF("Message sent success: %d\n", status.completedCount());
    ESP_MAIL_PRINTF("Message sent failed: %d\n", status.failedCount());
    Serial.println("----------------\n");

    for (size_t i = 0; i < smtp.sendingResult.size(); i++)
    {
      /* Get the result item */
      SMTP_Result result = smtp.sendingResult.getItem(i);

      // In case, ESP32, ESP8266 and SAMD device, the timestamp get from result.timestamp should be valid if
      // your device time was synched with NTP server.
      // Other devices may show invalid timestamp as the device time was not set i.e. it will show Jan 1, 1970.
      // You can call smtp.setSystemTime(xxx) to set device time manually. Where xxx is timestamp (seconds since Jan 1, 1970)
      
      ESP_MAIL_PRINTF("Message No: %d\n", i + 1);
      ESP_MAIL_PRINTF("Status: %s\n", result.completed ? "success" : "failed");
      ESP_MAIL_PRINTF("Date/Time: %s\n", MailClient.Time.getDateTimeString(result.timestamp, "%B %d, %Y %H:%M:%S").c_str());
      ESP_MAIL_PRINTF("Recipient: %s\n", result.recipients.c_str());
      ESP_MAIL_PRINTF("Subject: %s\n", result.subject.c_str());
    }
    Serial.println("----------------\n");

    // You need to clear sending result as the memory usage will grow up.
    smtp.sendingResult.clear();
  }
}



