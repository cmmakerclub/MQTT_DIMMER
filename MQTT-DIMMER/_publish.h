MqttConnector::prepare_data_hook_t on_prepare_data = 
[&](JsonObject * root) -> void {
    JsonObject& data = (*root)["d"];

    data["myName"] = DEVICE_NAME;
    data["author"] = AUTHOR;
    data["board"]  = BOARD;
    data["tag"]    = PROJECT;
//    data["value"]  = "YOUR_SENSOR_DATA";
};
