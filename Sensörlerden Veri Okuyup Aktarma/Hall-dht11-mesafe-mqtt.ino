TaskHandle_t Task1;

#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#define DHT11PIN 32

DHT dht(DHT11PIN, DHT11);

#define Hall_Sensor A0         
#define Hall_Sensor_D 39
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701


const char *ssid = "-"; 
const char *password = "-";  
const char *mqtt_broker = "-";
const char *topic = "-";
const char *mqtt_username = "-";
const char *mqtt_password = "-";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

bool Val=1;   
const uint8_t trigPin = 33;
const uint8_t echoPin = 25;
int duration;
int distanceCm;
float a;
uint8_t b;
float c;
 

void setup() {
Serial.begin(115200); 
wifiBroker();
delay(1000);
dht.begin();

pinMode(trigPin, OUTPUT);
pinMode(echoPin, INPUT); 
pinMode(Hall_Sensor_D,INPUT);

  
  xTaskCreatePinnedToCore(Task1code, "Task1",10000, NULL, 1, &Task1, 0);                       
  delay(500); 

  
  

 client.publish(topic, "start");
}

void Task1code( void * pvParameters ){
  for(;;){
   
   dht11Temp();
   distance(); 
   humidity();
  
   } 
}


void hallEffect(){
  delay(100);
   bool val=digitalRead(Hall_Sensor_D);
  Serial.println(val);
    char valString[8];
   dtostrf(val, 1, 2, valString);
   delay(100);
   if(val!=b){
   client.publish("-/hall",valString);
   b=val;
   }
}
void humidity(){
  float humi = dht.readHumidity();
  Serial.print("Humidity: ");
  Serial.println(humi);
   char humiString[8];
   dtostrf(humi, 1, 2, humiString);
  delay(100);
   if(humi!=c){
  client.publish("-/humi",humiString);  
   c=humi;
     }
}
void dht11Temp(){
 delay(1000);
 float temp = dht.readTemperature();
  Serial.print(F("Temperature: "));
  Serial.print(temp);
  Serial.println(F("ºC "));
   char tempString[8];
   dtostrf(temp, 1, 2, tempString);
  delay(100);
   if(temp!=a&&temp>0){
  client.publish("-/temp",tempString);  
   a=temp;
     }
}
int distance(){
  delay(1000);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distanceCm = duration * SOUND_SPEED/2;
  delay(10);
  char disString[8];
  dtostrf(distanceCm, 1, 2, disString);
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);
  delay(500);
   if(distanceCm!=200){
  client.publish("-/dist",disString);
   
   }
  }

void wifiBroker(){
  
WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.println(F("Connecting to WiFi.."));
 }
 Serial.println(F("Connected to the WiFi network"));
 client.setServer(mqtt_broker, mqtt_port);
 while (!client.connected()) {
     String client_id = "esp32-client-";
     client_id += String(WiFi.macAddress());
     if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
         Serial.println(F("Mqtt broker connected"));
     } else {
         Serial.print(F("failed with state "));
         Serial.print(client.state());
         delay(2000);
     }
 }
}
void loop() {
   hallEffect();
   client.loop();
   delay(50);
}
