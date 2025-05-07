// mqtt_publisher.c (with pseudonymization)
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

// Simple pseudonym generator
void generate_pseudonym(char *output, size_t size) {
    snprintf(output, size, "U-%04X", rand() % 0xFFFF);
}

int main() {
    unsigned char key[16] = "0123456789abcdef";
    unsigned char iv[16]  = "abcdef9876543210";

    char real_id[] = "Victim_001";
    char pseudonym[16];
    generate_pseudonym(pseudonym, sizeof(pseudonym));

    char message[128];
    snprintf(message, sizeof(message), "%s: temperature: 85", pseudonym);

    unsigned char ciphertext[128];
    int encrypted_len = encrypt((unsigned char*)message, strlen(message), key, iv, ciphertext);

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
    printf("Published pseudonymized + encrypted message: %s\n", message);
    MQTTClient_waitForCompletion(client, token, TIMEOUT);

    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return 0;
}

