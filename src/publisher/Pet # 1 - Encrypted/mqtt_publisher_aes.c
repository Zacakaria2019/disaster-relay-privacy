// mqtt_publisher.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MQTTClient.h"
#include "../../lib/aes_utils.c"

#define ADDRESS     "tcp://localhost:1883"
#define CLIENTID    "EncryptedPublisher"
#define TOPIC       "srv/temperature"
#define QOS         1
#define TIMEOUT     10000L

int main() {
    unsigned char key[16] = "0123456789abcdef";   // 16 bytes key (128 bits)
    unsigned char iv[16]  = "abcdef9876543210";   // 16 bytes IV

    unsigned char plaintext[] = "temperature: 85";
    unsigned char ciphertext[128];
    int encrypted_len = encrypt(plaintext, strlen((char*)plaintext), key, iv, ciphertext);

    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    MQTTClient_connect(client, &conn_opts);

    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    pubmsg.payload = ciphertext;
    pubmsg.payloadlen = encrypted_len;
    pubmsg.qos = QOS;
    pubmsg.retained = 0;

    MQTTClient_deliveryToken token;
    MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token);
    printf("Published encrypted message.\n");
    MQTTClient_waitForCompletion(client, token, TIMEOUT);

    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return 0;
}

