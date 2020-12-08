#include <dummy.h>

#include <Wire.h> //I2C통신을 위한 객체02

#include <WiFi.h>
#include <PubSubClient.h> //mqtt를 위한 객체
#include <sps30.h>

#include <stdio.h> 
#define _CRT_SECURE_NO_WARNINGS



//#define SP30_COMMS SERIALPORT2
//#define DEBUG 0
//#define TX_PIN 16
//#define RX_PIN 17
const char* ssid = "ecover-2.4";
const char* password = "easy@1234";
const char* mqtt_server = "192.168.1.85";
WiFiClient espclient;
int pinA = 15;
int pinB = 14;
int blue = 32;
int airQuality;  // Air quality level 0-3
//SPS30 sps30;
float PM25_val;


PubSubClient client(mqtt_server, 1883, espclient);

void setup() {

  Serial.begin(115200);
  pinMode(pinA, INPUT);
  pinMode(pinB, INPUT);
  pinMode(blue, OUTPUT);

  
  Serial.print("connecting");
  WiFi.begin(ssid, password);        //SSID,PASSWORD
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  sensirion_i2c_init();
  
  while (sps30_probe() != 0) {
    Serial.print("SPS sensor probing failed\n");
    delay(500);
  }
  int16_t ret;
  ret = sps30_start_measurement();
  if (ret < 0) {
    Serial.print("error starting measurement\n");
  }

}//end setup()





int now = 0;
int lastMeasure = 0;


void loop() {
  if (!client.connected()) {
    reconnect();
   //Serial.println("client disconnected");
  }
  client.loop();



  // Timer variable with current time
  now = millis();
  if (now - lastMeasure > 3000) {
    airQuality = ZP01_MP503(pinA, pinB);
    lastMeasure = now;
    
    String data1 = String(airQuality);
    Serial.println(airQuality);
    client.publish("/esp1/tvoc", data1.c_str());
    client.publish("/esp2/tvoc", data1.c_str());
    
    Get_sps30();
    Serial.println(PM25_val);
    String data2 = String(PM25_val); 
    client.publish("/esp1/pm25", data2.c_str());
    client.publish("/esp2/pm25", data2.c_str());
    
  }
  
}

int ZP01_MP503(int pinA, int pinB) {
  //int pinA=15, int pinB=14
  /* Initial */
  int valueA = digitalRead(pinA);
  int valueB = digitalRead(pinB);
  int result;

  if (valueA == 0) {
    if (valueB == 0) result = 0;
    if (valueB == 1) result = 1;
  }

  if (valueA == 1) {
    if (valueB == 0) result = 2;
    if (valueB == 1) result = 3;
  }
  return result;
  // return air quality level result
}



/**
 *  @brief : display error message
 *  @param mess : message to display
 *  @param r : error code
 *
 */


/**
 * serialTrigger prints repeated message, then waits for enter
 * to come in from the serial port.
 */


void reconnect(){
  while(WiFi.status()!=WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  while(!client.connected()){
    if(client.connect("ESP32Client 123456789")){
      Serial.println("connected");
      digitalWrite(blue,HIGH);
      
      client.subscribe("/esp1/tvoc");
      client.subscribe("/esp1/pm25");
      client.subscribe("/esp2/tvoc");
      client.subscribe("/esp2/pm25");
    }
    else{
      Serial.print("failed,rc=");
      Serial.println(client.state());
      delay(500);
    }
  } 
}


void Get_sps30() {
  struct sps30_measurement m;
  char serial[SPS30_MAX_SERIAL_LEN];
  uint16_t data_ready;
  int16_t ret;

  do {
    ret = sps30_read_data_ready(&data_ready);
    if (ret < 0) {
      Serial.print("error reading data-ready flag: ");
      Serial.println(ret);
    } else if (!data_ready)
      Serial.print("data not ready, no new measurement available\n");
    else
      break;
    delay(100); /* retry in 100ms */
  } while (1);

  ret = sps30_read_measurement(&m);
  PM25_val = m.mc_2p5;
}
