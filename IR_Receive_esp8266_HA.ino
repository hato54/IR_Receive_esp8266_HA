/*******************************************************
*   IR_Receive ESP 82666
*   Decodes a IR-remote controler and send the signals
*   to Home Assistant using MQTT
*   Using decoder crankyoldgit/IRremoteESP8266 (github)
*   2024-12-11 HATO
********************************************************/

//#define DEBUG   // This will print status messages


#include <ESP8266WiFi.h>
#include <IRremoteESP8266.h>
#include <PubSubClient.h>
#include <IRrecv.h>
#include <IRutils.h>

#define ZERO  0xff6897
#define ONE   0xff30cf
#define TWO   0xff18e7
#define TREE  0xff7a85
#define FOUR  0xff10ef
#define FIVE  0xff38c7
#define SIX   0xff5aa5
#define SEVEN 0xff42bd
#define EIGHT 0xff4ab5
#define NINE  0xff52ad
#define POWER 0xffa25d
#define MODE  0xff629d
#define SPEEK 0xffe21d
#define PLAY  0xff22dd
#define REV   0xff02fd
#define FWD   0xffc23d
#define EQ    0xffe01f
#define VOLPLUS  0xffa857
#define VOLMINUS 0xff906f
#define RPT   0xff9867
#define U_SD  0xffb04f
#define REPEAT 0xffffff

#define wifi_ssid "xxxxxx"
#define wifi_password "xxxxxx"
#define mqtt_server "192.168.1.36"
#define mqtt_user "xxxxxx"
#define mqtt_password "xxxxxx"

#define state_zero    "remote/ir/input/zero"
#define state_one     "remote/ir/input/one"
#define state_two     "remote/ir/input/two"
#define state_tree    "remote/ir/input/tree"
#define state_four    "remote/ir/input/four"
#define state_five    "remote/ir/input/five"
#define state_six     "remote/ir/input/six"

#define HOSTNAME HA_IRremote   // WiFi name

// ESP2866 IR input pin
const uint16_t kRecvPin = 14;

uint32_t IR_code;

// IR decoding
IRrecv irrecv(kRecvPin);
decode_results results;

WiFiClient espClient;
PubSubClient client(espClient);




void setup_wifi(){
  delay(10);
  Serial.print("\nConnecting to ");
  Serial.println(wifi_ssid);
  WiFi.hostname("HOSTNAME");
  WiFi.begin(wifi_ssid, wifi_password);

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.print("\nWiFi connected \n");
  Serial.print("IP adress: ");
  Serial.println(WiFi.localIP());
}


void reconnect(){
  // Loop until we are connected
  while(!client.connected()){
    Serial.print("Atempting MQTT connection ...");
    // Create random client ID
    String clientId = "MQTT_Relay-";
    clientId += String(random(0xffff),HEX);

    // Attempt to connect
    if(client.connect(clientId.c_str(), mqtt_user, mqtt_password)){
      Serial.println("connected");
      // Once connected, publish an announcement..
       //client.publish(state_topic1, "OFF");  
      
      // ... and resubscribe
     // client.subscribe(inTopic1);
    }else{
      Serial.print("failed, rc = ");
      Serial.print(client.state());
      Serial.println("try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
    
  }
} 

void callback(char* topic1,byte* payload, unsigned int length){
#ifdef  DEBUG
  Serial.print("Message arrived on topic: ");
  Serial.print(topic1);
  Serial.print(". Message: ");
#endif
}

void setup(){
  
  irrecv.enableIRIn();  // Start the receiver
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  Serial.begin(115200);
  while (!Serial)  // Wait for the serial connection to be establised.
    delay(50);
}


void loop(){

  static bool ZeroActive, OneActive, TwoActive, TreeActive, FourActive, FiveActive, SixActive = false;

  if(!client.connected()){
    reconnect();
  }
  client.loop();

  if (irrecv.decode(&results)) {
    IR_code = (uint32_t)results.value;
#ifdef DEBUG 
    Serial.println(IR_code, HEX);
#endif
    irrecv.resume();  // Receive the next value
    delay(100);
  }



  switch(IR_code){
    case ZERO:
      if(!ZeroActive){
        ZeroActive = true;
#ifdef DEBUG        
        Serial.println("ZeroActive = TRUE");
#endif        
        client.publish(state_zero, "ON");
      }else{
        ZeroActive = false;
#ifdef DEBUG 
        Serial.println("ZeroActive = FALSE");
#endif                 
        client.publish(state_zero, "OFF");
      }
      break;
    case ONE:
      if(!OneActive){
        OneActive = true;
#ifdef DEBUG          
        Serial.println("OnActive = TRUE");
#endif         
        client.publish(state_one, "ON");
      }else{
        OneActive = false;
#ifdef DEBUG          
        Serial.println("OnActive = FALSE");
#endif        
        client.publish(state_one, "OFF");
      }
      break;
    case TWO:
      if(!TwoActive){
        TwoActive = true;
#ifdef DEBUG
        Serial.println("TwoActive = TRUE");
#endif                  
        client.publish(state_two, "ON");
      }else{
        TwoActive = false;
#ifdef DEBUG 
        Serial.println("TwoActive = FALSE");
#endif                 
        client.publish(state_two, "OFF");
      }
      break;
    case TREE:
      if(!TreeActive){
        TreeActive = true;
#ifdef DEBUG
        Serial.println("TreeActive = TRUE");
#endif                  
        client.publish(state_tree, "ON");
      }else{
        TreeActive = false;
#ifdef DEBUG
        Serial.println("TreeActive = FALSE");
#endif                  
        client.publish(state_tree, "OFF");
      }
      break;
    case FOUR:
      if(!FourActive){
        FourActive = true;
#ifdef DEBUG
        Serial.println("FourActive = TRUE");
#endif                  
        client.publish(state_four, "ON");
      }else{
        FourActive = false;
#ifdef DEBUG 
        Serial.println("FourActive = FALSE");
#endif                 
        client.publish(state_four, "OFF");
      }
      break;
    case FIVE:
      if(!FiveActive){
        FiveActive = true;
#ifdef DEBUG
        Serial.println("FiveActive = TRUE");
#endif                  
        client.publish(state_five, "ON");
      }else{
        FiveActive = false;
#ifdef DEBUG
        Serial.println("FiveActive = FALSE");
#endif                  
        client.publish(state_five, "OFF");
      }
      break; 
    case SIX:
      if(!SixActive){
        SixActive = true;
#ifdef DEBUG
        Serial.println("SixActive = TRUE");
#endif                  
        client.publish(state_six, "ON");
      }else{
        SixActive = false;
#ifdef DEBUG
        Serial.println("SixActive = FALSE");
#endif                  
        client.publish(state_six, "OFF");
      }
      break;               
    default:
      //
      break;
  }

}