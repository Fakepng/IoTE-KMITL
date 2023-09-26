#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include <Arduino_JSON.h>

String httpGETRequest(const char* serverName);
String numberFormat(double number);

const char* SSID = "Fakepng-IoT";
const char* PASSWORD = "iotengineering";

const char* END_POINT = "https://catalog.excise.go.th/api/3/action/datastore_search?limit=5&resource_id=39283cc3-8973-41e3-83b4-37d69c22417b";

unsigned long lastTime = 0;
unsigned long timerDelay = 10000;

String jsonBuffer;

void setup() {
  Serial.begin(9600);

  WiFi.begin(SSID, PASSWORD);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.print("Connected IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if ((millis() - lastTime) > timerDelay) {
    if (WiFi.status() == WL_CONNECTED) {
      String serverPath = END_POINT;

      jsonBuffer = httpGETRequest(serverPath.c_str());
      JSONVar myObject = JSON.parse(jsonBuffer);

      if (JSON.typeof(myObject) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
      }

      JSONVar result = myObject["result"]["records"];

      for (int i = 0; i < result.length(); i++) {
        JSONVar record = result[i];
        
        String caseAmt = numberFormat((double)record["CASE_AMT"]);
        String treasuryAmt = numberFormat((double)record["TREASURY_AMT"]);


        Serial.print("Year: ");
        Serial.println(record["BUDGET_YEAR"]);
        Serial.print("  - Case amount: ");
        Serial.print(caseAmt);
        Serial.println(" baht");
        Serial.print("  - Treasury amount: ");
        Serial.print(treasuryAmt);
        Serial.println(" baht");
        Serial.println();
      }
    
    } else {
      Serial.println("WiFi Disconnected");
    }

    lastTime = millis();
  }
}

String httpGETRequest(const char* serverName) {
  std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
  client->setInsecure();

  HTTPClient http;

  http.begin(*client, serverName);
  int httpResponseCode = http.GET();

  String payload = "{}";

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();

  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }

  http.end();

  return payload;
}

String numberFormat(double number) {
  String result = "";
  
  String num = String(number);
  int count = 0;

  for (int i = num.indexOf(".") - 1; i >= 0; i--) {
    count++;
    result = num[i] + result;

    if (count == 3 && i > 0) {
      count = 0;
      result = "," + result;
    }
  }

  result = result + num.substring(num.indexOf("."), num.length());

  
  return result;
}