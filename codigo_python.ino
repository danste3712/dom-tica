/*
 Basic ESP8266 MQTT example

 This sketch demonstrates the capabilities of the pubsub library in combination
 with the ESP8266 board/library.

 It connects to an MQTT server then:
  - publishes "hello world" to the topic "outTopic" every two seconds
  - subscribes to the topic "inTopic", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary
  - If the first character of the topic "inTopic" is an 1, switch ON the ESP Led,
    else switch it off

 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.

 To install the ESP8266 board, (using Arduino 1.6.4+):
  - Add the following 3rd party board manager under "File -> Preferences -> Additional Boards Manager URLs":
       http://arduino.esp8266.com/stable/package_esp8266com_index.json
  - Open the "Tools -> Board -> Board Manager" and click install for the ESP8266"
  - Select your ESP8266 in "Tools -> Board"

*/
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.

const char* ssid = "LAB_ELECTRONICA1";
const char* password = "ELECTRONICA1";
const char* mqtt_server = "10.1.128.166";
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
char msg2[50];
int value = 0;
const int sensor = D0;
int estaon = LOW;
float caliente = 0;
const int encender = D1;
const int encender2 = D2;
int aux2 = 0;
int valorsensor = 0;
int aux= 0;
int tempC; // Variable para almacenar el valor obtenido del sensor (0 a 1023)
int pinLM35 = A0; // Variable del pin de entrada del sensor (A0)

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

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) 
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

 //  Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }


/*

  if ((char)payload[0] == '1') {    //TEMPERATURA
    digitalWrite(D3, HIGH); 
  }
 if ((char)payload[0] == '2') {
    digitalWrite(D3, LOW); 
  }
  if ((char)payload[0] == '3') {    //MOVIMIENTO
    digitalWrite(D4, HIGH); 
  }
  if ((char)payload[0] == '4') {
    digitalWrite(D4, LOW); 
  }
*/  
 }



void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("laboratorios/lab1/sensor2", "enviando el primer mensaje");
      // ... and resubscribe
      client.subscribe("laboratorios/lab1/receptor1");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  pinMode(sensor,INPUT);
  pinMode(pinLM35,INPUT);
  pinMode(encender, OUTPUT);
  pinMode(encender2, OUTPUT);
  pinMode(D3,OUTPUT);
  pinMode(D4,OUTPUT);
  
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  
  estaon = digitalRead(sensor);
   if (estaon == HIGH) {
    // ENTONCES ENCENDEMOS EL LED
    digitalWrite(encender, HIGH);
    delay(1000);
    digitalWrite(encender, LOW);
  } 
  aux2 = analogRead(pinLM35); 
   
  
    aux2 = (5.0 * aux2 * 100.0)/1024.0;
    //aux2 = aux2*0.322265625;
  if (aux2 >= 60 || aux2 <= 200) {
    // ENTONCES ENCENDEMOS EL LED
    digitalWrite(encender2, LOW);
     delay(10000);
    digitalWrite(encender2, HIGH);
  }
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 2000) {
    valorsensor = digitalRead(sensor);
    tempC = analogRead(pinLM35); 
   
  
    tempC = (5.0 * tempC * 100.0)/1024.0;
    //tempC = aux2*0.322265625;
    Serial.println(tempC);
    if (valorsensor == 1){
      aux++;
    }
    
    Serial.println(aux);
    lastMsg = now;
    ++value;
    snprintf (msg, 50, "movimientos: %3d", aux);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("laboratorios/lab1/sensor1", msg);
    snprintf (msg2 , 50, "Temperatura: %3d", tempC);
    Serial.print("Publish message: ");
    Serial.println(msg2);
    client.publish("laboratorios/lab1/sensor3", msg2);
  }
  
}
