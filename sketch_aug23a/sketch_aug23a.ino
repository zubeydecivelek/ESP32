#include <WiFi.h>
#include <Arduino.h>

const char *ssid = "CERVOS_STATION";
const char *password = "zervos22";

  IPAddress local_IP(192, 168, 1, 184);
  // Set your Gateway IP address
  IPAddress gateway(192, 168, 1, 10);
  IPAddress subnet(255, 255, 255, 0);
  IPAddress primaryDNS(8, 8, 8, 8);  

  WiFiServer Server(23);
  WiFiClient Client;

void handleTelnet(){

    if (Server.hasClient()) { //check if there are any new clients
Client = Server.available();
if (!Client) Serial.println("available broken");
Serial.print("New client: ");
Serial.println(Client.remoteIP());
}

if (Client && Client.connected()) { //check clients for data
  if (Client.available()) {
    while (Client.available())
      Serial.println(Client.read()); //get data from the telnet client and push it to the UART
  }
}
else if (Client) Client.stop();
   /* if(!Telnet || !Telnet.connected()){
      if(Telnet) Telnet.stop();
      Telnet = TelnetServer.available();
    }else{
      char c = Telnet.read();
      Serial.print(c);
      TelnetServer.available().stop();
    }*/
  
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.config(local_IP,gateway,subnet);
  WiFi.begin(ssid,password);
  Serial.print("\n Connecting to ");Serial.println(ssid);

  uint8_t i=0;
  while(WiFi.status()!= WL_CONNECTED && i++<20)delay(500);
  if(i==21){
    Serial.println("bağlanamadı... bağlanmaya çalışıyor");
    ESP.restart();
  
  }
  Serial.begin(115200);
  Server.begin();
  Server.setNoDelay(true);
  Serial.println("Ready! Use port 23 to connect.");

}

void loop() {
  // put your main code here, to run repeatedly:

  handleTelnet();
  Client.println("meraba");
  delay(1000);

}
