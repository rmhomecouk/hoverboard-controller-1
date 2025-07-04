#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <ArduinoOTA.h>
#include <ps5Controller.h>
#include "private.h"

int r = 255;
int g = 0;
int b = 0;
bool controllerConnected = false;

void task_btController(void *pvParameters);
void task_MotorController(void *pvParameters);

void nextRainbowColor()
{
  if (r > 0 && b == 0)
  {
    r--;
    g++;
  }
  if (g > 0 && r == 0)
  {
    g--;
    b++;
  }
  if (b > 0 && g == 0)
  {
    r++;
    b--;
  }
}

void setup()
{
  Serial.begin(115200);
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult(20000) != WL_CONNECTED)
  {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  // Port defaults to 3232
  // ArduinoOTA.setPort(3232);

  // Hostname defaults to esp3232-[MAC]
  // ArduinoOTA.setHostname("myesp32");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA
      .onStart([]()
               {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH) {
        type = "sketch";
      } else {  // U_SPIFFS
        type = "filesystem";
      }

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type); })
      .onEnd([]()
             { Serial.println("\nEnd"); })
      .onProgress([](unsigned int progress, unsigned int total)
                  { Serial.printf("Progress: %u%%\r", (progress / (total / 100))); })
      .onError([](ota_error_t error)
               {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) {
        Serial.println("Auth Failed");
      } else if (error == OTA_BEGIN_ERROR) {
        Serial.println("Begin Failed");
      } else if (error == OTA_CONNECT_ERROR) {
        Serial.println("Connect Failed");
      } else if (error == OTA_RECEIVE_ERROR) {
        Serial.println("Receive Failed");
      } else if (error == OTA_END_ERROR) {
        Serial.println("End Failed");
      } });

  ArduinoOTA.begin();

  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  ps5.begin("24:a6:fa:b6:e6:11");

  // setup our tasks
  xTaskCreatePinnedToCore(
      task_btController,
      "task_btcontroller", // Task name
      8192,
      NULL, // Task parameters
      1,
      NULL, // Task handle
      1     // Core ID (1 for core 1, 0 for core 0)
  );

  xTaskCreatePinnedToCore(
      task_MotorController,
      "task_bmotorontroller", // Task name
      8192,
      NULL, // Task parameters
      1,
      NULL, // Task handle
      1     // Core ID (1 for core 1, 0 for core 0)
  );
}

void task_btController(void *pvParameters)
{
  // This task will handle the Bluetooth controller
  while (true)
  {
    if (ps5.isConnected())
    {
      controllerConnected = true;
      if (ps5.LStickX())
      {
        Serial.printf("Left Stick x at %d\n", ps5.LStickX());
      }
      if (ps5.LStickY())
      {
        Serial.printf("Left Stick y at %d\n", ps5.LStickY());
      }
      if (ps5.RStickX())
      {
        Serial.printf("Right Stick x at %d\n", ps5.RStickX());
      }
      if (ps5.RStickY())
      {
        Serial.printf("Right Stick y at %d\n", ps5.RStickY());
      }
    }
    else
    {
      controllerConnected = false; // Disable motors on loss of controller connection
    }
    vTaskDelay(10 / portTICK_PERIOD_MS); // Delay to prevent task starvation
  }
}

void task_MotorController(void *pvParameters)
{
  // This task will handle the motor controller
  while (true)
  {
    if(!ps5.isConnected())
    {
      Serial.println("Controller not connected, stopping motors.");
    }
    else
    {
      Serial.println("Controller connected, controlling motors.");
    }
    // Add code to control the hoverboard motors based on the PS5 controller input
    // For example, you can read the joystick values and send commands to the motors

    vTaskDelay(10 / portTICK_PERIOD_MS); // Delay to prevent task starvation
  }
}

void loop()
{
  ArduinoOTA.handle();
}

