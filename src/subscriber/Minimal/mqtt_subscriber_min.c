// mqtt_subscriber.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MQTTClient.h"

#define ADDRESS     "tcp://localhost:1883"
#define CLIENTID    "CClientSubscriber"
#define TOPIC       "srv/temperature"
#define QOS         1

int messageArrived(void *context, char *topicName, int topicLen, MQTTClient_message *message){
    printf("Received on topic %s: %.*s\n", topicName, message->payloadlen, (char*)message->payload);
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}

int main() {
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;

    MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    MQTTClient_setCallbacks(client, NULL, NULL, messageArrived, NULL);
    MQTTClient_connect(client, &conn_opts);
    MQTTClient_subscribe(client, TOPIC, QOS);

    printf("Subscribed to %s\n", TOPIC);
    while (1);  // Infinite loop to keep the subscriber alive

    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return 0;
}

