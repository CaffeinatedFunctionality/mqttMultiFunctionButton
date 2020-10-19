#include <PubSubClient.h>
#include <ESP8266WiFi.h>

#define WIFI_AP "XXXXXXXXX"
#define WIFI_PASSWORD "XXXXXXXXXXX"

#define MQTT_SERVER "XXX.XXX.XXX.XXX"
#define userName "XXXXXX"
#define passWord "XXXXXXXX"
#define BUTTON_PIN 2



WiFiClient wifiClient;

PubSubClient mqttClient(wifiClient);

int status = WL_IDLE_STATUS;
unsigned long lastRead;
bool buttonPressed = false;

void setup()
{
  Serial.begin(115200);
  delay(10);
  InitWiFi();
  mqttClient.setServer( MQTT_SERVER, 1883 );
  Serial.println("Setting up");
  // Set button input pin
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  digitalWrite(BUTTON_PIN, HIGH);
}

void loop()
{
  if ( !mqttClient.connected() ) {
    reconnect();
  }

  int b = checkButton();
  if (b == 1) triggerEvent(0);
  if (b == 2) triggerEvent(1);
}

//=================================================
// Event to trigger

void triggerEvent(int type) {
  String eventName = "";
  if(type == 0){ eventName = "single"; }
  else if(type == 1){ eventName = "double"; }
  mqttClient.publish("footSwitch/event", eventName.c_str());
}

//=================================================
//  MULTI-CLICK:  One Button, Multiple Events

// Button timing variables
int debounce = 20;          // ms debounce period to prevent flickering when pressing or releasing the button
int DCgap = 1000;           // max ms between clicks for a double click event

// Button variables
boolean buttonVal = HIGH;   // value read from button
boolean buttonLast = HIGH;  // buffered value of the button's previous state
long downTime = -1;         // time the button was pressed down
long upTime = -1;           // time the button was released
int clicks = 0;             // checks for click amounts before the double click gap

int checkButton() {    
   int event = 0;
   buttonVal = digitalRead(BUTTON_PIN);
   // Button pressed down
   if (buttonVal == LOW && buttonLast == HIGH && (millis() - upTime) > debounce)
   {
       downTime = millis();
   }
   // Button released
   else if (buttonVal == HIGH && buttonLast == LOW && (millis() - downTime) > debounce)
   {    
       upTime = millis();
       clicks++;
   }
   // Test for normal click event: DCgap expired
   if ( buttonVal == HIGH && (millis()-upTime) >= DCgap)
   {
       char cstr[16];
       if(clicks == 1){
        event = 1;
       }
       if(clicks >= 2){
        event = 2;
       }
       clicks = 0;
   }
   
   buttonLast = buttonVal;
   return event;
}


void InitWiFi()
{
  Serial.println("Connecting to AP ...");
  // attempt to connect to WiFi network

  WiFi.begin(WIFI_AP, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to AP");
}


void reconnect() {
  // Loop until we're reconnected
  while (!mqttClient.connected()) {
    status = WiFi.status();
    if ( status != WL_CONNECTED) {
      WiFi.begin(WIFI_AP, WIFI_PASSWORD);
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
      }
      Serial.println("Connected to AP");
    }
    Serial.print("Connecting to mqtt broker ...");
    if ( mqttClient.connect("footSwitch", userName, passWord) ) {
      Serial.println( "[DONE]" );
    } else {
      Serial.print( "[FAILED] [ rc = " );
      Serial.print( mqttClient.state() );
      Serial.println( " : retrying in 5 seconds]" );
      // Wait 5 seconds before retrying
      delay( 5000 );
    }
  }
}
