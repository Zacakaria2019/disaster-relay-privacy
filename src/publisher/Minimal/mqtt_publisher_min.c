
// mqtt_publisher.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MQTTClient.h"

#define ADDRESS     "tcp://localhost:1883"
#define CLIENTID    "CClientPublisher"
#define TOPIC       "srv/temperature"
#define PAYLOAD     "temperature: 85"
#define QOS         1
#define TIMEOUT     10000L

int main() {
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;

    MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    MQTTClient_connect(client, &conn_opts);

    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    pubmsg.payload = (void*)PAYLOAD;
    pubmsg.payloadlen = strlen(PAYLOAD);
    pubmsg.qos = QOS;
    pubmsg.retained = 0;

    MQTTClient_deliveryToken token;
    MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token);
    printf("Published: %s\n", PAYLOAD);
    MQTTClient_waitForCompletion(client, token, TIMEOUT);

    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return 0;
}
