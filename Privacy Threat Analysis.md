# Privacy Threat Analysis (LINDDUN)

We analyze our MQTT system (victim→broker→C2) against each LINDDUN category:

## 1. Linkability  
**Threat:** An adversary observing message patterns (timestamps, topics, pseudonym tokens) can link multiple messages as coming from the same victim.  
**Example:** Even with pseudonyms, “U-49F6” always publishes on `/srv/temperature`, enabling linkage.

## 2. Identifiability  
**Threat:** If adversary obtains mapping between pseudonyms and real IDs, victims become re-identifiable.  
**Example:** If the mapping file on subscriber is leaked, pseudonyms “U-XXXX” reveal “Victim_001”.

## 3. Non-repudiation  
**Threat:** Strong encryption without forward secrecy means victims can’t plausibly deny having sent a message if keys leak later.  
**Example:** AES-CBC logs + broker retention → proof of message origin.

## 4. Detectability  
**Threat:** Even if payloads are encrypted, an adversary can detect that a message was sent (topic usage, packet sizes).  
**Mitigation:** Use uniform-sized padding or dummy traffic.

## 5. Disclosure  
**Threat:** Unencrypted metadata (topic name, QoS, retained flag) discloses existence and timing of communications.  
**Mitigation:** Move sensitive data into encrypted payload; consider dynamic topic names.

## 6. Unawareness  
**Threat:** Users (victims) may not realize their location or ID is exposed in plaintext or via metadata.  
**Mitigation:** Clearly document the privacy configuration and user consent.

## 7. Non-compliance  
**Threat:** If data isn’t fully minimized or pseudonym mappings aren’t purged, system may violate GDPR/other privacy laws.  
**Mitigation:** Implement data-retention policies and secure key management.

---

