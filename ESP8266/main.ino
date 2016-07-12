#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "";
const char* password = "";
const char* mqtt_server = "";
int mqtt_server_port = ; //1883;

WiFiClient espClient;
PubSubClient client(espClient);
int loopCount = 0;
const int delayTime = 3;
const int pinHold = 12;
const int pinClock = 13;
const int exposureTime = 4;
const int wifi_led = 5;
const int cloud_led = 4;
int tick = 0;
char msg[100];
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  pinMode(pinHold, OUTPUT);
  pinMode(pinClock, OUTPUT);
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  pinMode(wifi_led , OUTPUT);
  pinMode(cloud_led , OUTPUT);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_server_port);
  client.setCallback(callback);
}

int analogReadMQTT(int port) {
  int value = analogRead(A0);    // read the input pin
  sprintf(msg, "%d", value);
  delay(1000);
  client.publish("/test", msg);
  return value;
}

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  digitalWrite(wifi_led, HIGH);
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is acive low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("muscle", "wxjejhlq", "IeKtcyta0kBV")) {
      digitalWrite(cloud_led, HIGH);
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("/outTopic", "hello world from esp8266");
      // ... and resubscribe
      client.subscribe("/inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

    for (int i=0; i<20; i++) {
        int val = analogReadMQTT(A0);
    }
    int noise = random(1024);
    client.publish("/test", "512");
    delay(exposureTime);
    delay(100);
}

