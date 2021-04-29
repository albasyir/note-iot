#include <Arduino.h>

/**
 * Depedencies List
 */
#include <ESP8266HTTPClient.h> // HTTP Client
#include <ESP8266WiFi.h>       // Getting WiFi Adapeter
#include <WiFiClient.h>        // WiFi Accessor

/**
 * Macro Definition
 */
#define SERIAL_DEBUG Serial

enum LED
{
  ON,
  OFF
};

/**
 * Global Variable
 */
String host = "http://dweet.io/dweet/for/mdp-";
String npm = "1822250092";

const char *wifi_name = "ssid_name";
const char *wifi_pass = "ssid_password";

void led_toggle(int situation = !digitalRead(LED_BUILTIN))
{
  digitalWrite(LED_BUILTIN, situation);
}

void serial_setup()
{
  Serial.begin(115200);        // IDK what is that
  Serial.setDebugOutput(true); // this option when u want debuging
}

void led_setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
}

/**
 * Wifi Setup!
 * 
 */
void wifi_connection_setup()
{
  Serial.println("Connecting to WiFi...");

  WiFi.mode(WIFI_STA);
  WiFi.begin(wifi_name, wifi_pass);

  while (!WiFi.isConnected())
  {
    led_toggle();
    delay(250);
  }

  Serial.print("Connected with id = ");
  Serial.println(WiFi.localIP().toString());

  led_toggle(LED::OFF);
}

/**
 * Example Requets HTTP!
 * 
 */
void dweetRequestExample()
{
  // we dont carryout it if wifi doesn't connect!
  if (WiFi.status() != WL_CONNECTED)
  {
    return;
  }

  HTTPClient request;

  String ChipIdHex = String(ESP.getChipId(), HEX);
  String FreeRam = String(ESP.getFreeHeap(), DEC);

  // pripare your endpoint
  String endpoint = (host + npm + "?npm=" + npm + "&hex=" + ChipIdHex + "&mem=" + FreeRam);

  Serial.print("Getting from : ");
  Serial.println(endpoint);

  // pripare your HTTP Client
  request.begin(endpoint.c_str());

  // let's GET it!
  int result = request.GET();

  // if not OK!
  if (result != 200)
  {
    Serial.print("Error Code : ");
    Serial.println(result);
    request.end();
    return;
  }

  // if 200 OK! then print all response
  Serial.println(request.getString());
  request.end();
}

void setup()
{
  delay(1000);
  serial_setup();
  led_setup();
  wifi_connection_setup();
  led_toggle(LED::ON);
}

void loop()
{
  dweetRequestExample();
  delay(30000);
}
