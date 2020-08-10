#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>

const char* ssid = <your wifi ssid>;
const char* password = <your wifi password>;
#define MQTT_SERVER <mqtt broker host>
#define MQTT_PORT <mqtt broker port>
#define MIN 1
#define MAX 20

WiFiClient espClient;
PubSubClient client(espClient);
int status = WL_IDLE_STATUS;

void setup() {
  Serial.begin(9600);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  /* Connect to wifi */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  /* Bind PubSub info */
  client.setServer(MQTT_SERVER, MQTT_PORT);
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("abcd")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

// the loop function runs over and over again forever
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  char buffer[256];
  /* Build json payload */
  StaticJsonDocument<200> doc;
  doc["x"] = random(MIN, MAX);
  doc["y"] = random(MIN, MAX);
  doc["z"] = random(MIN, MAX);
  serializeJson(doc, buffer);
  if(client.publish("data/test", buffer)) {
    Serial.println(buffer);
    Serial.println("sended");
  } else {
    Serial.println("send failed");
  }
  delay(10000);
}
