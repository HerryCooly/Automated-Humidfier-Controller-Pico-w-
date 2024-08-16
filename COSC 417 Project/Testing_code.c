#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>


const char* ssid = "SHAW-817B";
const char* password = "copied1343elect";

//Your Domain name with URL path or IP address with path
const char* serverName = "https://use1-wap.tplinkcloud.com/?token=bae77b5f-BTLWn6Ph5buQP7kcKD0WJKP";

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 5000;

void setup() {
  //Setting up the leds
  pinMode(LED_BUILTIN, OUTPUT);//ON&OFF status of the pico
  digitalWrite(LED_BUILTIN, HIGH);
  //Setting up the leds
  pinMode(21, OUTPUT);//ON&OFF status of the humidfier
  //digitalWrite(21,HIGH);
  pinMode(20,OUTPUT);// First state of the humidity level
  //digitalWrite(20,HIGH);
  pinMode(19,OUTPUT);// Second state of the humidity level
  //digitalWrite(19,HIGH);
  pinMode(18,OUTPUT);// Third state of the humidity level
  //digitalWrite(18,HIGH);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");
}

void loop() {



  //Send an HTTP POST request every 10 minutes
  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      Serial.print("Connected to WiFi network with IP Address: ");
      Serial.println(WiFi.localIP());
      HTTPClient http;
      http.setInsecure();
          // Your Domain name with URL path or IP address with path
      http.begin(serverName);
          // If you need an HTTP request with a content type: application/json, use the following:
      http.addHeader("Content-Type", "application/json");
      int httpResponseCode = http.POST("{\"method\":\"passthrough\",\"params\":{\"deviceId\":\"8006EEC6ACA6C9A134FF30415ED330182186A0F5\",\"requestData\":{\"system\":{\"set_relay_state\":{\"state\":1}}}}}");
         
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);

      String payload = http.getString();
      Serial.print("HTTP String: ");
      Serial.println(payload);
            
      // Free resources
      http.end();
      
    } else {
      Serial.println("WiFi Disconnected");
    }
    
    lastTime = millis();
  }
}