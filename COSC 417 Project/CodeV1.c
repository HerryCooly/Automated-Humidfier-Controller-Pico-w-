#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include <AM2320.h>
AM2320 th(&Wire1);
// WIFI credentials
const char *ssid = "SHAW-817B";
const char *password = "copied1343elect";

// Your Domain name with URL path or IP address with path
const char *serverName = "https://use1-wap.tplinkcloud.com/?token=bae77b5f-BTLWn6Ph5buQP7kcKD0WJKP";

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 5000;

void setup()
{
  Serial.begin(9600);
  // Start IC2 bus 1
  Wire1.begin();
  // Setting up the leds
  pinMode(LED_BUILTIN, OUTPUT); // ON&OFF status of the pico
  digitalWrite(LED_BUILTIN, HIGH);
  // Setting up the leds
  pinMode(21, OUTPUT); // ON&OFF status of the humidfier
  // digitalWrite(21,HIGH);
  pinMode(20, OUTPUT); // First state of the humidity level
  // digitalWrite(20,HIGH);
  pinMode(19, OUTPUT); // Second state of the humidity level
  // digitalWrite(19,HIGH);
  pinMode(18, OUTPUT); // Third state of the humidity level
  // digitalWrite(18,HIGH);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop()
{
  
  // Getting the reading from the sensor
  Serial.println(F("Chip = AM2320"));
  switch(th.Read()) {
    case 2:
      Serial.println(F("  CRC failed"));
      break;
    case 1:
      Serial.println(F("  Sensor offline"));
      break;
    case 0:
      Serial.print(F("  Humidity = "));
      Serial.print(th.Humidity);
      Serial.println(F("%"));
      Serial.print(F("  Temperature = "));
      Serial.print(th.cTemp);
      Serial.println(F("*C"));
      Serial.println();
      break;
  }
  delay(2000);
  
  
  // Send an HTTP POST request every 10 minutes
  // Check WiFi connection status
  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.print("Connected to WiFi network with IP Address: ");
    Serial.println(WiFi.localIP());
    HTTPClient http;
    http.setInsecure();
    // Your Domain name with URL path or IP address with path
    http.begin(serverName);
    // If you need an HTTP request with a content type: application/json, use the following:
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode;
    Serial.print(th.Humidity);
    if (th.Humidity < 20.0)
    {
      httpResponseCode = http.POST("{\"method\":\"passthrough\",\"params\":{\"deviceId\":\"8006EEC6ACA6C9A134FF30415ED330182186A0F5\",\"requestData\":{\"system\":{\"set_relay_state\":{\"state\":1}}}}}");
      digitalWrite(21, HIGH);
      digitalWrite(20, LOW);
      digitalWrite(19, LOW);
      digitalWrite(18, LOW);
    }
    else if (th.Humidity >= 20.0 && th.Humidity < 40.0)
    {
      httpResponseCode = http.POST("{\"method\":\"passthrough\",\"params\":{\"deviceId\":\"8006EEC6ACA6C9A134FF30415ED330182186A0F5\",\"requestData\":{\"system\":{\"set_relay_state\":{\"state\":1}}}}}");
      digitalWrite(21, HIGH);
      digitalWrite(20, HIGH);
      digitalWrite(19, LOW);
      digitalWrite(18, LOW);
    }

    else if (th.Humidity >= 40.0 && th.Humidity < 60.0)
    {
      httpResponseCode = http.POST("{\"method\":\"passthrough\",\"params\":{\"deviceId\":\"8006EEC6ACA6C9A134FF30415ED330182186A0F5\",\"requestData\":{\"system\":{\"set_relay_state\":{\"state\":1}}}}}");
      digitalWrite(21, HIGH);
      digitalWrite(20, HIGH);
      digitalWrite(19, HIGH);
      digitalWrite(18, LOW);
    }
    else
    {
      httpResponseCode = http.POST("{\"method\":\"passthrough\",\"params\":{\"deviceId\":\"8006EEC6ACA6C9A134FF30415ED330182186A0F5\",\"requestData\":{\"system\":{\"set_relay_state\":{\"state\":0}}}}}");
      digitalWrite(21, LOW);
      digitalWrite(20, HIGH);
      digitalWrite(19, HIGH);
      digitalWrite(18, HIGH);
    }

    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String payload = http.getString();
    Serial.print("HTTP String: ");
    Serial.println(payload);
    // Free resources
    http.end();
  }
  else
  {
    Serial.println("WiFi Disconnected");
  }
  // delay 10 seconds avoding sending too many POST request
  delay(10000);
}