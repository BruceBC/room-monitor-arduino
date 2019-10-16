#include <ArduinoHttpClient.h>
#include <ArduinoJson.h>
#include <SharpIR.h>
#include <WiFiNINA.h>
#include "arduino_secrets.h"

/**
 * *************************
 * Wifi Settings
 * *************************
 */
static char ssid[] = SECRET_SSID;
static char pass[] = SECRET_PASS;

/**
 * *************************
 * Websocket Settings
 * *************************
 */
static char serverAddress[] = SERVER_ADDRESS;  // server address
const int port = PORT;
static char path[] = PATH;

/**
 * *************************
 * State
 * *************************
 */
const short TERMINATE = 0;
const short READY     = 1;
const short CREATED   = 2;
const short UPDATED   = 3;

/**
 * *************************
 * Globals
 * *************************
 */
SharpIR sensor( SharpIR::GP2Y0A02YK0F, A0 );
WiFiClient wifi;
WebSocketClient client = WebSocketClient(wifi, serverAddress, port);
int status = WL_IDLE_STATUS;

/**
 * *************************
 * Connect to Wifi
 * *************************
 */
void setup() {
  Serial.begin(9600);
  connectToWifi();
}

/**
 * *************************
 * Websocket Client
 * *************************
 */
void loop() {
  // Establish a connection to a web socket

  // Calculate distance for 10 numbers and average them

  // Send calculation to server

  Serial.println("Starting WebSocket client");
  client.begin(path);

  while (client.connected()) {
    short newState = waitForMessage(10);

    switch (newState) {
        case READY:
          send(createHardwareIdRequest());
          continue;
        case CREATED: 
        case UPDATED:
          send(updateDistanceRequest(calculateDistance()));
          continue;
        default:
          // Should have reached the TERMINATE state,
          // client will be disconnected.
          return;
      }
  }

  Serial.println("disconnected");
}

/**
 * *************************
 * Requests
 * *************************
 */
char* createHardwareIdRequest() {
    const int capacity = JSON_OBJECT_SIZE(2);
    StaticJsonDocument<capacity> doc;
    doc["type"] = "create_hardware_id";
    doc["hardware_id"] = HARDWARE_ID;

    static char output[128];

    serializeJson(doc, output);

    return output;
}

char* updateDistanceRequest(int distance) {
    const int capacity = JSON_OBJECT_SIZE(2);
    StaticJsonDocument<capacity> doc;
    doc["type"] = "update_distance";
    doc["distance"] = distance;

    static char output[128];

    serializeJson(doc, output);

    return output;
}

/**
 * *************************
 * State Management
 * *************************
 */
short state(char * json_string) {
  const int capacity = JSON_OBJECT_SIZE(4);
  StaticJsonDocument<capacity> doc;

  DeserializationError err = deserializeJson(doc, json_string);

  if (err) {
    Serial.print(F("deserializeJson() failed with code "));
    Serial.println(err.c_str());

    return TERMINATE;
  }

  char* type = doc["type"];

  if (strcmp(type, "ready") == 0) {
    return READY;
  }

  if (strcmp(type, "created") == 0) {
    return CREATED;
  }
  
  if (strcmp(type, "updated") == 0) {
    return UPDATED;
  }

  // Error -> Terminate
  return TERMINATE;
}

/**
 * *************************
 * Request/Response Handlers
 * *************************
 */
void send(char *request) {
  Serial.println(request);
  
  // send request
  client.beginMessage(TYPE_TEXT);
  client.print(request);
  client.endMessage();
}

short waitForMessage(int seconds) {
  int timer = 0;
  while(timer < seconds) {
    if (client.parseMessage() > 0) {
      String response = client.readString();
      Serial.println("Received a message:");
      Serial.println(response);

      // Convert String to char array
      const unsigned int capacity = response.length() + 1;
      char json_string[capacity];
      response.toCharArray(json_string, capacity);
      
      return state(json_string);
    }

    timer++;

    // Debug checking
    Serial.print("Waiting...");
    Serial.print(timer);
    Serial.println(" second");
    
    delay(1000);
  }

  Serial.println("Timeout. Disconnecting....");
  return TERMINATE;
}


/**
 * *****************************
 * Proximity Sensor Calculations
 * *****************************
 */
int calculateDistance() {
  const int capacity = 10;
  int distances[capacity] = {};

  for(int i = 0; i < capacity; i++) {
     int distance = sensor.getDistance(); // Distance in cm
      distances[i] = distance;

      Serial.print("Distance ");
      Serial.println(distance);
    
      delay(5);
  }

  // Averge distances
  return average(distances, capacity);
}

float average(int *a, int len) {
  int total = 0;
  
  for (int i = 0; i < len; i++) {
    total += a[i];
  }

  return round((double) total / len);
}

/**
 * *************************
 * Wifi Helpers
 * *************************
 */
void connectToWifi() {
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);                   // print the network name (SSID);

    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);
  }

  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}
