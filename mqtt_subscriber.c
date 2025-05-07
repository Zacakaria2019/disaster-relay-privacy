#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MQTTClient.h"
#include "../../lib/aes_utils.c"

#define ADDRESS     "tcp://localhost:1883"
#define CLIENTID    "EncryptedSubscriber"
#define TOPIC       "srv/temperature"
#define QOS         1

int messageArrived(void *context, char *topicName, int topicLen, MQTTClient_message *message) {
    unsigned char key[16] = "0123456789abcdef";   // Same key
    unsigned char iv[16]  = "abcdef9876543210";   // Same IV

    unsigned char decrypted[128];
    int decrypted_len = decrypt(message->payload, message->payloadlen, key, iv, decrypted);
    decrypted[decrypted_len] = '\0'; // Null-terminate

    printf("Received encrypted message on topic %s\nDecrypted content: %s\n", topicName, decrypted);

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

    printf("Waiting for encrypted messages on topic: %s\n", TOPIC);
    while (1);  // Keep the subscriber running
    return 0;
}

