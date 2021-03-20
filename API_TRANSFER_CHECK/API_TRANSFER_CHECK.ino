// Include Libraries
#include <Arduino.h>
#include <SD.h>
#include <Arduino_FreeRTOS.h>
#include <SPI.h>
#include <WiFi.h>


char ssid[] = "wifiusername";
char pass[] = "password";   
int keyIndex = 0;               
int status = WL_IDLE_STATUS;
WiFiServer server(80);
// define two tasks for Store & Compare
void TaskStore( void *pvParameters );
void TaskCompare( void *pvParameters );

#define SDFILE_PIN_CS  10

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; //physical address
char serverName[] = "https://fastag-internal.parkzap.com/account/mockable_test/"; // test web page server
EthernetClient client;

String readString, readString1;
int x=0; //for counting line feeds
char lf=20; //line feed character



// Global variables and defines

// object initialization
File sdFile;


// define vars for testing menu
const int timeout = 10000;       //define timeout of 10 sec
char menuOption = 0;
long time0;

// Setup the essentials for your circuit to work. It runs first every time your circuit is powered with electricity.
void setup() 
{   
    
    // Setup Serial which is useful for debugging
    // Use the Serial Monitor to view printed messages
    Serial.begin(9600);
    while (!Serial) ; // wait for serial port to connect. Needed for native USB
    Serial.println("start");
    
    // Set SPI SS pin to output otherwise the SD library functions will not work.
    // The SD is set to use SPI SS Arduino pin 10 as chip select(CS) by default.
    // To change the pin use SD.begin(SD_CS_PIN)
    pinMode(SDFILE_PIN_CS, OUTPUT);
    // Check if the card is present and can be initialized
    if (!SD.begin()) {
    Serial.println(F("Card failed, or not present"));
    while(1);
    }
    Serial.println(F("card initialized."));
    menuOption = menu();

    xTaskCreate(
    TaskStore
    ,  (const portCHAR *)"Store"   // A name just for humans
    ,  128  // Stack size
    ,  NULL
    ,  2  // priority
    ,  NULL );

  xTaskCreate(
    TaskCompare
    ,  (const portCHAR *) "Compare"
    ,  128 // This stack size can be checked & adjusted by reading Highwater
    ,  NULL
    ,  1  // priority
    ,  NULL );

  // Now the task scheduler, which takes over control of scheduling individual tasks, is automatically started.
    if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    while(true);
  }

  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    delay(10000);
  }
  server.begin();
  printWifiStatus();
}


void loop() 
{
    
    WiFiClient client = server.available();
    client.println("https://fastag-internal.parkzap.com/account/mockable_test/");
      client.println("Content-Type: text/html");
      client.println();
      delay (50);
    if(menuOption == '1') {
    // Micro SD module - Test Code
    // The SD example code creates a datalog.txt file for logging sensor data
    // open the file. 
    sdFile = SD.open("datalog.txt", FILE_WRITE);
    // if the file exists in SD card, write sensor data
    if (sdFile) {
    sdFile.println("ENTER SENSOR DATA HERE");
    // close the file
    sdFile.close();
    } 
    else {
    // if the file didn't open, print an error
    Serial.println(F("error opening file."));
    }
    }
    
    if (millis() - time0 > timeout)
    {
        menuOption = menu();
    }
    
}



// Menu function for selecting the components to be tested
// Follow serial monitor for instrcutions
char menu()
{

    Serial.println(F("\nWhich component would you like to test?"));
    Serial.println(F("(1) Micro SD module"));
    Serial.println(F("(menu) send anything else or press on board reset button\n"));
    while (!Serial.available());

    // Read data from serial monitor if received
    while (Serial.available()) 
    {
        char c = Serial.read();
        if (isAlphaNumeric(c)) 
        {   
            
            if(c == '1') 
          Serial.println(F("Now Testing Micro SD module"));
            else
            {
                Serial.println(F("illegal input!"));
                return 0;
            }
            time0 = millis();
            return c;
        }
    }
void TaskStore(void *pvParameters)  // This is a task.
{
  (void) pvParameters;

  static char cmdBuffer[CMDBUFFER_SIZE] = "";
 char d;
 while(Serial.available())
 if (Serial.available())  {
    char d = Serial.read();  //gets one byte from serial buffer
    myFile.println(Serial.read);//puts data in sd card
  }
}

void TaskCompare(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
    if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();
    for (int i=0;i<=100;i++))
      if (Serial.available())  {
        char d = myFile.read();
      if(incomingByte=d){
        Serial.println(corresponding number: Serial.read)
      }
  }
void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
