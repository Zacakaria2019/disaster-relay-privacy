#!/usr/bin/env python3
import time
import threading
from base64 import b64encode
from cryptography.hazmat.primitives import padding
from cryptography.hazmat.primitives.ciphers import (
    Cipher, algorithms, modes
)
from cryptography.hazmat.backends import default_backend
import paho.mqtt.client as mqtt

BROKER = "localhost"
TOPIC  = "srv/temperature"
N      = 20

# AES parameters
KEY = b"0123456789abcdef"
IV  = b"abcdef9876543210"

# storage for payload‐sizes
results = {
    "baseline":    [],
    "encrypted":   [],
    "pseudonym":   []
}

# — Subscriber thread function —
def run_subscriber(label):
    def on_message(client, userdata, msg):
        results[label].append(len(msg.payload))
        if len(results[label]) >= N:
            client.disconnect()

    c = mqtt.Client(client_id=f"sub_{label}")
    c.on_message = on_message
    c.connect(BROKER)
    c.subscribe(TOPIC, qos=1)
    c.loop_forever()

# — Publish N messages using the given payload‐generator —
def publish_series(label, gen_payload):
    c = mqtt.Client(client_id=f"pub_{label}")
    c.connect(BROKER)
    for _ in range(N):
        p = gen_payload()
        c.publish(TOPIC, p, qos=1)
        time.sleep(0.05)
    c.disconnect()

# — Evaluation harness: start subscriber, wait briefly, then publish —
def evaluate(label, gen_payload):
    t = threading.Thread(target=run_subscriber, args=(label,))
    t.start()
    time.sleep(0.1)
    publish_series(label, gen_payload)
    t.join()

# — Payload generators —

def gen_plain():
    return b"temperature: 85"

def gen_encrypted():
    # AES-128-CBC + PKCS7 + Base64
    padder = padding.PKCS7(128).padder()
    data   = padder.update(b"temperature: 85") + padder.finalize()
    cipher = Cipher(algorithms.AES(KEY), modes.CBC(IV), default_backend())
    ct     = cipher.encryptor().update(data) + cipher.encryptor().finalize()
    return b64encode(ct)

import random
def gen_pseudonym():
    uid = f"U-{random.randrange(0xFFFF):04X}".encode()
    msg = uid + b": temperature: 85"
    padder = padding.PKCS7(128).padder()
    data   = padder.update(msg) + padder.finalize()
    cipher = Cipher(algorithms.AES(KEY), modes.CBC(IV), default_backend())
    ct     = cipher.encryptor().update(data) + cipher.encryptor().finalize()
    return b64encode(ct)

# — Main —
def main():
    print(f"Running {N} messages per mode…")
    evaluate("baseline",  gen_plain)
    evaluate("encrypted", gen_encrypted)
    evaluate("pseudonym", gen_pseudonym)

    print("\nMode         Avg Payload Size (bytes)")
    for label in ("baseline","encrypted","pseudonym"):
        avg = sum(results[label]) / len(results[label])
        print(f"{label:12}{avg:10.1f}")

if __name__ == "__main__":
    main()
