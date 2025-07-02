#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <ArduinoOTA.h>
#include <ps5Controller.h>
#include "_private.h"


int r = 255;
int g = 0;
int b = 0;
bool controllerstarted = false;

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
  while (WiFi.waitForConnectResult() != WL_CONNECTED)
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
}

void loop()
{
  ArduinoOTA.handle();

  // if (ps5.isConnected()) // && !controllerstarted)
  // {
  //   controllerstarted = true;

  //   // Sets the color of the controller's front light
  //   // Params: Red, Green,and Blue
  //   // See here for details: https://www.w3schools.com/colors/colors_rgb.asp
  //   ps5.setLed(r, g, b);
  //   nextRainbowColor();

  //   // Sets how fast the controller's front light flashes
  //   // Params: How long the light is on in ms, how long the light is off in ms
  //   // Range: 0->2550 ms, Set to 0, 0 for the light to remain on
  //   ps5.setFlashRate(ps5.LStickY() * 10, ps5.RStickY() * 10);

  //   // Sets the rumble of the controllers
  //   // Params: Weak rumble intensity, Strong rumble intensity
  //   // Range: 0->255
  //   ps5.setRumble(ps5.LStickY(), ps5.R2Value());

  //   // Sends data set in the above three instructions to the controller
  //   ps5.sendToController();

  //   // Don't send data to the controller immediately, will cause buffer overflow
  //   delay(10);
  // }

  while (ps5.isConnected() == true)
  {

    // only control the hoverboard if the controller is connected
    Serial.printf("Controller is connected\n");

    // if (ps5.Right()) Serial.println("Right Button");
    // if (ps5.Down()) Serial.println("Down Button");
    // if (ps5.Up()) Serial.println("Up Button");
    // if (ps5.Left()) Serial.println("Left Button");

    // if (ps5.Square()) Serial.println("Square Button");
    // if (ps5.Cross()) Serial.println("Cross Button");
    // if (ps5.Circle()) Serial.println("Circle Button");
    // if (ps5.Triangle()) Serial.println("Triangle Button");

    // if (ps5.UpRight()) Serial.println("Up Right");
    // if (ps5.DownRight()) Serial.println("Down Right");
    // if (ps5.UpLeft()) Serial.println("Up Left");
    // if (ps5.DownLeft()) Serial.println("Down Left");

    // if (ps5.L1()) Serial.println("L1 Button");
    // if (ps5.R1()) Serial.println("R1 Button");

    // if (ps5.Share()) Serial.println("Share Button");
    // if (ps5.Options()) Serial.println("Options Button");
    // if (ps5.L3()) Serial.println("L3 Button");
    // if (ps5.R3()) Serial.println("R3 Button");

    // if (ps5.PSButton()) Serial.println("PS Button");
    // if (ps5.Touchpad()) Serial.println("Touch Pad Button");

    // if (ps5.L2()) {
    //   Serial.printf("L2 button at %d\n", ps5.L2Value());
    // }
    // if (ps5.R2()) {
    //   Serial.printf("R2 button at %d\n", ps5.R2Value());
    // }

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

    // if (ps5.Charging()) Serial.println("The controller is charging"); //doesn't work
    // if (ps5.Audio()) Serial.println("The controller has headphones attached"); //doesn't work
    // if (ps5.Mic()) Serial.println("The controller has a mic attached"); //doesn't work

    // Serial.printf("Controller Battery Level : %d\n", ps5.Battery()); //doesn't work

    Serial.println();
    // This delay is to make the output more human readable
    // Remove it when you're not trying to see the output
    delay(300);
  }

  controllerstarted = false;

  // If the controller is disconnected, disable the hoverboard
  if (!ps5.isConnected())
  {
    //Serial.println("Controller disconnected, disabling hoverboard");
    // Add code here to disable the hoverboard
    // For example, you might want to stop sending commands to the hoverboard
  }
}