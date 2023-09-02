#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// WiFi
const char *ssid = "Ramos"; // Enter your WiFi name
const char *password = "2020133038";  // Enter WiFi password

// MQTT Broker
const char *mqtt_broker = "iee6c76f.ala.cn-hangzhou.emqxsl.cn"; // broker address
const char *topic = "test"; // define topic 
const char *mqtt_username = "ESP12FTest01"; // username for authentication
const char *mqtt_password = "public"; // password for authentication
const int mqtt_port = 8883; // port of MQTT over TLS/SSL

// init wifi client
WiFiClientSecure espClient;
PubSubClient client(espClient);

/*
  The common fingerprints of EMQX broker, for reference only.
  If you are not using EMQX Cloud Serverless or public EMQX broker,
  you need to calculate the sha1 fingerprint of your server certificate
  and update the 'fingerprint' variable below.
*/
// 1. fingerprint of public emqx broker. Host: broker.emqx.io
//const char* fingerprint = "B6 C6 FF 82 C6 59 09 BB D6 39 80 7F E7 BC 10 C9 19 C8 21 8E";
// 2. fingerprint of EMQX Cloud Serverless. Host: *.emqxsl.com
// const char* fingerprint = "42:AE:D8:A3:42:F1:C4:1F:CD:64:9C:D7:4B:A1:EE:5B:5E:D7:E2:B5";
// 3. fingerprint of EMQX Cloud Serverless. Host: *.emqxsl.cn
 const char* fingerprint = "7E:52:D3:84:48:3C:5A:9F:A4:39:9A:8B:27:01:B1:F8:C6:AD:D4:47";


void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to the WiFi network");

  // set fingerprint
  espClient.setFingerprint(fingerprint);
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);
  while (!client.connected()) {
    String client_id = "esp8266-client-";
    client_id += String(WiFi.macAddress());
    Serial.printf("The client %s connects to the mqtt broker\n", client_id.c_str());
    if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
        Serial.println("Connected to MQTT broker.");
    } else {
        Serial.print("Failed to connect to MQTT broker, rc=");
        Serial.print(client.state());
        Serial.println(" Retrying in 5 seconds.");
        delay(5000);
    }
  }

    // publish and subscribe
    client.publish(topic, "hello emqx"); // publish message to the topic
    client.subscribe(topic); // subscribe message from the topic
}

void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    Serial.print("Message:");
    for (int i = 0; i < length; i++) {
        Serial.print((char) payload[i]);
    }
    Serial.println();
    Serial.println("-----------------------");
}

void loop() {
  if (!client.connected()) {
//    reconnect();    //报错了
  }
  client.loop();
}
