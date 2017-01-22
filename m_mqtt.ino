// m_mqtt


void EspPrepareMqtt()
{
    // Set-up callbacks for events and initialize with es-link.
    mqtt.connectedCb.attach(mqttConnected);
    mqtt.disconnectedCb.attach(mqttDisconnected);
    mqtt.publishedCb.attach(mqttPublished);
    mqtt.dataCb.attach(mqttData);
    mqtt.setup();

    //Serial3.println("ARDUINO: setup mqtt lwt");
    //mqtt.lwt("/lwt", "off line", 0, 0); //or mqtt.lwt("/lwt", "off line");

    SerialESP.println("EL-MQTT ready");


    uint32_t timeEsp = cmd.GetTime();
    SerialESP.print("ESP Time: ");
    SerialESP.println(timeEsp);
    SerialESP.print("Woody-Time: ");
    SerialESP.println(RTC_GetUnixTime());

    if (timeEsp >1485007200)
        RTC_SetByUnixTime(timeEsp,0);

//https://www.confirado.de/tools/timestamp-umrechner.html

}




bool connected;

// Callback when MQTT is connected
void mqttConnected(void* response)
{
    SerialESP.println("MQTT connected!");
    mqtt.subscribe("/esp-link/1");
    mqtt.subscribe("/hello/world/#");
    //mqtt.subscribe("/esp-link/2", 1);
    //mqtt.publish("/esp-link/0", "test1");
    connected = true;
}

// Callback when MQTT is disconnected
void mqttDisconnected(void* response)
{
    SerialESP.println("MQTT disconnected");
    connected = false;
}

// Callback when an MQTT message arrives for one of our subscriptions
void mqttData(void* response)
{
    ELClientResponse *res = (ELClientResponse *)response;

    SerialESP.print("Received: topic=");
    String topic = res->popString();
    SerialESP.println(topic);

    SerialESP.print("data=");
    String data = res->popString();
    SerialESP.println(data);
}

void mqttPublished(void* response)
{
    SerialESP.println("MQTT published");
}





