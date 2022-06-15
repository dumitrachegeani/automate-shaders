
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>
#include <ESP8266WiFi.h>

#define WLAN_SSID       "XXXXXX"             // Your SSID
#define WLAN_PASS       "XXXXX"        // Your password

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME  "adafruitusername"
#define AIO_KEY       "addafruitkey"
/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// or... use WiFiFlientSecure for SSL
//WiFiClientSecure client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/


// Setup a feed called 'onoff' for subscribing to changes.

/****************************** FeedName ************************************/
Adafruit_MQTT_Subscribe Mode = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/blinders");


void MQTT_connect();


void setup() {
  Serial.begin(115200);

  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("... ");
  }
  Serial.println(WiFi.localIP());


  // Setup MQTT subscription for onoff feed.
  mqtt.subscribe(&Mode);

}
String str;
void loop() {

  MQTT_connect();

  Adafruit_MQTT_Subscribe *subscription;

  while ((subscription = mqtt.readSubscription(2))) {
    if (subscription == &Mode) {
      char* cmd = (char *)(Mode.lastread);

      if (strncmp(cmd, "up", 2)  == 0|| strncmp(cmd, "app", 3) == 0){
        Serial.print("lifting");
      }
      else if (strncmp(cmd, "down", 4) == 0) {
        Serial.print("putting down");
      }
      else {
        Serial.print("stop");
      }
    } 
  }

}

void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;

  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
    retries--;
    if (retries == 0) {
      // basically die and wait for WDT to reset me
      //while (1);
      Serial.println("Wait 10 min to reconnect");
      delay(600000);
    }
  }
  Serial.println("MQTT Connected!");
}
