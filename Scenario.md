## 📑 `SCENARIO.md`

# Scenario & Experiment Guide

Follow these steps to reproduce the experiments.

---

## 1. Launch the MQTT Broker

```bash
mosquitto -v
````

---

## 2. Run Baseline (Plaintext)

1. **Open Terminal A**

   ```bash
   ./mqtt_sub_plain
   ```
2. **Open Terminal B**

   ```bash
   ./mqtt_pub_plain
   ```
3. **Verify** that subscriber prints:

   ```
   Received on topic srv/temperature: temperature: 85
   ```

---

## 3. Run Encrypted (AES‑CBC + Base64)

1. **Terminal A** (restart subscriber if needed)

   ```bash
   ./mqtt_sub_aes
   ```
2. **Terminal B**

   ```bash
   ./mqtt_pub_aes
   ```
3. **Verify** that subscriber prints the decrypted payload.

---

## 4. Run Pseudonymized + Encrypted

1. **Terminal A**

   ```bash
   ./mqtt_sub_aes
   ```
2. **Terminal B**

   ```bash
   ./mqtt_pub_pseudo
   ```
3. **Verify** that subscriber prints:

   ```
   Decrypted content: U-XXXX: temperature: 85
   ```

---

## 5. Evaluation

1. **Ensure** broker is running.
2. **Run** the evaluation script:

   ```bash
   chmod +x evaluate_privacy.py
   ./evaluate_privacy.py
   ```
3. **Review** the printed average payload sizes for each mode.

---

## 6. Cleanup

To clear any retained messages before re-testing:

```bash
mosquitto_pub -h localhost -t "srv/temperature" -r -n
```

---