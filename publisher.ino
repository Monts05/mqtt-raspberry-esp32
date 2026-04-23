#include <WiFi.h>
#include <PubSubClient.h>

// --- Configuración de Red ---
const char* ssid = "HUAWEI-2.4-5Q6t";
const char* password = "ajNCavHp"; 

// --- Configuración MQTT ---
const char* mqtt_server = "eiot6cm3.local"; 
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastMsg = 0;
int count = 0;

void setup_wifi() {
  delay(10);
  Serial.println("\nConectando a WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi conectado.");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Intentando conexión MQTT a eiot6cm3.local...");
    // ID de cliente único para evitar conflictos
    String clientId = "ESP32_Negrete";
    clientId += String(random(0xffff), HEX);
    
    if (client.connect(clientId.c_str())) {
      Serial.println("¡Conectado!");
    } else {
      Serial.print("Error, rc=");
      Serial.print(client.state());
      Serial.println(" Reintentando en 5 segundos...");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();
  // Publicar cada 5 segundos
  if (now - lastMsg > 5000) {
    lastMsg = now;
    count++;
    
    String payload = "Mensaje #" + String(count) + " desde ESP32 de Negrete";
    Serial.print("Publicando: ");
    Serial.println(payload);
    
    client.publish("escom/iot/equipo/datos", payload.c_str());
  }
}