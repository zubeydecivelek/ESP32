
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include "FS.h"
#include "SD.h"
#include <SPI.h>

#define SD_CS 5

const char *ssid = "ESP32";
const char *password = "123456789";

WiFiServer server(80);

const long videoSize = 16745;
long SDsize = 0;
byte btArray[100]={-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2};
int indexx=0;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  Serial.println();
  Serial.println("Configuring access point...");

  // You can remove the password parameter if you want the AP to be open.
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();

  Serial.println("Server started");

  SD.begin(SD_CS);


}
void loop() {
  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    File file = SD.open("/video.mp4");
    Serial.println("Creating file...");
    writeFile(SD, "/video.mp4");
    file.close();

    for (int i =0;i<100;i++){
      Serial.println(btArray[i]);
    }

   
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        //char c = client.read();             // read a byte, then
        byte b = client.read();
        if(indexx<100){
          btArray[indexx]=b;
          indexx++;
        }if (indexx==100){
          appendFile(SD,"/video.mp4",btArray);
          indexx=0;
        }
        Serial.println(b);
        //appendFile(SD, "/video.mp4", b);
      }

    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
void writeFile(fs::FS &fs, const char * path) {
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  file.close();
}

void appendFile(fs::FS &fs, const char * path, const byte * append) {
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if (!file) {
    Serial.println("Failed to open file for appending");
    return;
  }
  for (int i=0;i<100;i++){
    if(append[i]!= -2){
        if (file.write(append[i])) {
         
      } else {
        Serial.println("Append failed");
      }
    }
  }
 
  file.close();
}
