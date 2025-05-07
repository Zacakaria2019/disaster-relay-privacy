## 📘 `README.md`

# Disaster‑Relay Privacy Project

**A simulated MQTT messaging system** with privacy‐enhancing technologies (PETs):  
1. **Baseline** (plaintext)  
2. **AES‑CBC + Base64** encryption  
3. **Pseudonymization + AES‑CBC + Base64**

---

## 🔧 Prerequisites

- Ubuntu 22.04 (or similar Linux)  
- GCC, make, cmake, build-essential  
- Mosquitto broker & clients  
- OpenSSL dev libraries  
- Python 3.10+ with `paho‑mqtt` and `cryptography`

```bash
sudo apt update
sudo apt install git cmake build-essential libssl-dev mosquitto mosquitto-clients libpaho-mqtt-dev python3-pip
pip3 install --user paho-mqtt cryptography
````

---

## 🛠️ Build Instructions

```bash
# AES + Base64 utility
gcc -c aes_utils.c -o aes_utils.o -lssl -lcrypto

# Publisher / Subscriber binaries
gcc mqtt_publisher_aes.c aes_utils.o -o mqtt_pub_aes -lpaho-mqtt3c -lssl -lcrypto
gcc mqtt_subscriber_aes.c aes_utils.o -o mqtt_sub_aes -lpaho-mqtt3c -lssl -lcrypto

# (For PET2) Pseudonymized publisher
gcc mqtt_publisher_pseudo.c aes_utils.o -o mqtt_pub_pseudo -lpaho-mqtt3c -lssl -lcrypto
```

---

## ▶️ Running the System

1. **Start the broker**

   ```bash
   mosquitto -v
   ```

2. **Subscriber** (runs indefinitely)

   ```bash
   ./mqtt_sub_aes
   ```

3. **Publisher** (single‑shot)

   * Baseline (no encryption):

     ```bash
     ./mqtt_pub_plain
     ```
   * Encrypted:

     ```bash
     ./mqtt_pub_aes
     ```
   * Pseudonymized + Encrypted:

     ```bash
     ./mqtt_pub_pseudo
     ```

You should see:

```
Received encrypted message on topic srv/temperature
Decrypted content: [payload]
```

---

## 📊 Evaluation

We measured **average payload size** over 20 messages per mode:

| Mode      | Avg Payload Size (bytes) |
| --------- | -----------------------: |
| Baseline  |                     15.0 |
| Encrypted |                     24.0 |
| Pseudonym |                     44.0 |

(See `evaluate_privacy.py` for details.)

---

## 📂 Repository Structure

```
.
├── bin/
│   ├── publisher/
│   │   ├── Minimal/mqtt_pub_min
│   │   ├── Pet # 1 - Encrypted/mqtt_pub_aes
│   │   └── Pet # 2 - Pseudonym/mqtt_pub_psn
│   └── subscriber/
│       ├── Minimal/mqtt_sub_min
│       ├── Pet # 1 - Encrypted/mqtt_sub_aes
│       └── Pet # 2 - Pseudonym/mqtt_sub_psn
├── eval/
│   ├── evaluate_privacy.py
│   └── requirements.txt
├── lib/
│   └── aes_utils.c
├── src/
│   ├── publisher/
│   │   ├── Minimal/mqtt_publisher_min.c
│   │   ├── Pet # 1 - Encrypted/mqtt_publisher_aes.c
│   │   └── Pet # 2 - Pseudonym/mqtt_publisher_pseudo.c
│   └── subscriber/
│       ├── Minimal/mqtt_subscriber_min.c
│       ├── Pet # 1 - Encrypted/mqtt_subscriber_aes.c
│       └── Pet # 2 - Pseudonym/mqtt_subscriber_pseudo.c
├── mqtt_publisher.c
├── mqtt_subscriber.c
├── Privacy Threat Analysis.md
├── Scenario.md
└── Readme.md
```

---

## 🎥 Presentation Video

\[I will add the link later]

---

## 📄 License

This project is released under the MIT License. See [LICENSE](LICENSE).
