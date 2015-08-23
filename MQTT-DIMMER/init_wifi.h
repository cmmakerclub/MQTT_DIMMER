
void init_wifi()
{
  // use flash memory ssid & smartconfig
  // wifi = new WiFiConnector();
  wifi = new WiFiConnector("NAT.11", "guestnetwork");


  wifi->on_connecting([&](const void* message)
  {
    Serial.print("connecting -> ");
    Serial.println(wifi->counter);
    Serial.println(wifi->get("ssid") + ", " + wifi->get("password"));
    // Serial.println ((char*)message);
    delay(500);
  });

  wifi->on_connected([&](const void* message)
  {
    Serial.println("WIFI CONECTED: ");
    // Print the IP address
    Serial.println(WiFi.localIP());
    // Serial.println ((char*)message);
      delay(200);
     //attachInterrupt(switchPin, switchDetect, CHANGE);
     attachInterrupt(zcPin, zcDetect, RISING);
  });

  wifi->on_disconnected([&](const void* message)
  {
    detachInterrupt(zcPin);
    Serial.println("WIFI DISCONECTED.");
    // detachInterrupt()
    // Serial.println ((char*)message);
  });

  wifi->on_smartconfig_enter([&](const void* message)
  {
    Serial.println("ENTER SMARTCONFIG.");
  });

  wifi->on_smartconfig_done([&](const void* message)
  {
    Serial.println("SMARTCONFIG DONE.");
  });

  wifi->on_smartconfig_doing([&](const void* message)
  {
    // Serial.println("CONFIGURING WIFI..");
    // delay(500);
  });

  wifi->connect();

}
