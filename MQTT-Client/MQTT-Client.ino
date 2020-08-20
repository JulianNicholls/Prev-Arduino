#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define LED           (2)

#define MAX_MSG_LEN   (128)

void callback(char *msgTopic, byte *msgPayload, unsigned int msgLength);
void connectMQTT();

const char* ssid = "BTHub6-SZ8H";
const char* password = "RwyNcn7qdmqi";
const char* serverHostName = "HP-LAPTOP";

const char* topic = "/ESP8266/LED";

WiFiClient espClient;
PubSubClient psClient(espClient);

void setup() {
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);

  Serial.begin(115200);

  connectWiFi();

  psClient.setServer(serverHostName, 1883);
  psClient.setCallback(callback);
}

void loop() {
  if (!psClient.connected()) connectMQTT();

  psClient.loop();    // This is what receives the messages

  delay(500);
}

void connectWiFi() {
  delay(10);

  Serial.printf("\nConnecting to %s\n", ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("WiFi connected on IP address ");
  Serial.println(WiFi.localIP());
}

void connectMQTT() {
  String clientID = "ESP8266-" + String(random(0xffff), HEX);
  Serial.printf("MQTT connecting as client %s...\n", clientID.c_str());

  while (!psClient.connected()) {
    if (psClient.connect(clientID.c_str())) {
      Serial.println("MQTT connected");
      psClient.publish(topic, "Hello from ESP8266");

      psClient.subscribe(topic);
    }
    else {
      Serial.printf("MQTT connection failed, state %d, retrying in 2s", psClient.state());
      delay(2000);
    }
  }
}

void callback(char* msgTopic, byte* payload, unsigned int length) {
  static char message[MAX_MSG_LEN + 1];

  if (length > MAX_MSG_LEN) length = MAX_MSG_LEN;

  strncpy(message, (char*) payload, length);
  message[length] = '\0';

  Serial.printf("topic '%s', message received: '%s'\n", msgTopic, message);
  
  if (strcmp(message, "off") == 0)
    setLEDState(false);
  else if (strcmp(message, "on") == 0)
    setLEDState(true);
}

void setLEDState(boolean state) {
  digitalWrite(LED, !state);  // LED logic on a Wemos D1 is inverted, Low = ON
}
