/**
 * @file FullExample.ino
 * @date 24.03.2020
 * @author Grandeur Technologies
 *
 * Copyright (c) 2019 Grandeur Technologies LLP. All rights reserved.
 * This file is part of the Hardware SDK for Grandeur Cloud.
 *
 * Apollo.h is used for device's communication to Grandeur Cloud.
 * Arduino_JSON.h is for parsing and serializing JSON packets.
 */

#include <Apollo.h>

String deviceID = "YOUR-DEVICE-ID";
String apiKey = "YOUR-PROJECT-APIKEY";
String token = "YOUR-ACCESS-TOKEN";
String ssid = "YOUR-WIFI-SSID";
String passphrase = "YOUR-WIFI-PASSWORD";

unsigned long current;
short numberOfTimes = 2;
ApolloDevice device;

void setup() {
    Serial.begin(9600);
    // Initialize the global object "apollo" with your configurations.
    device = apollo.init(deviceID, apiKey, token, ssid, passphrase);

    // This sets a callback function to be called when the device makes a successful
    // connection with the Cloud.
    device.onApolloConnected([](JSONObject message) {
        // Initializing the millis counter for the five
        // seconds timer.
        current = millis();

        device.onSummaryUpdated([](JSONObject updatedSummary) {
            Serial.printf("Updated Voltage is: %d\n", (int) updatedSummary["voltage"]);
            Serial.printf("Updated Current is: %d\n", (int) updatedSummary["current"]);
            /* Here you can set some pins or trigger events that depend on
            ** device's summary update.
            */
        });

        device.onParmsUpdated([](JSONObject updatedParms) {
            Serial.printf("Updated State is: %d\n", (int) updatedParms["state"]);
            /* Here you can set some pins or trigger events that depend on
            ** device's parms update.
            */
        }); 
    });
}

void loop() {
    // Keep updating the TCP buffer
    device.update();
    if(device.getState() == APOLLO_CONNECTED) {
        if(millis() - current >= 5000) {
            // This if-condition makes sure that the code inside this block runs only after
            // every five seconds.
            
            // Requests the cloud for device's summary.
            device.getSummary([](JSONObject payload) {
                if(payload["code"] == "DEVICE-SUMMARY-FETCHED") {
                    // If there were no problems in fetching the device's summary.
                    Serial.printf("voltage: %d, current: %d\n", (int) payload["deviceSummary"]["voltage"], (int) payload["deviceSummary"]["current"]);
                    
                    /* You can set some pins or trigger events that depend on
                    ** device's summary here.
                    */

                    return;
                }
                // If for some reason, summary could not be fetched.
                Serial.println("Failed to Fetch Summary");
                return;
            });
            
            // Gets the device's parms from the Cloud
            device.getParms([](JSONObject payload) {
                if(payload["code"] == "DEVICE-PARMS-FETCHED") {
                    Serial.printf("state: %d\n", (int) payload["deviceParms"]["state"]);

                    /* You can set some pins or trigger events that depend on
                    ** device's parms here.
                    */
                    return;
                }
                Serial.println("Failed to Fetch Parms");
                return;
            });

            // This updates the device's summary on the Cloud
            JSONObject summary;
            summary["voltage"] = millis();
            summary["current"] = millis();
            device.setSummary(summary, [](JSONObject payload) {
                if(payload["code"] == "DEVICE-SUMMARY-UPDATED") {
                    Serial.printf("Voltage is updated to: %d\n", (int) payload["update"]["voltage"]);
                    Serial.printf("Current is updated to: %d\n", (int) payload["update"]["current"]);

                    /* You can set some pins or trigger events that depend on successful
                    ** device summary update here.
                    */
                    return;
                }
                Serial.println("Failed to Update Summary");
            });

            // This updates the device's parms on the Cloud
            JSONObject parms;
            parms["state"] = millis();
            device.setParms(parms, [](JSONObject payload) {
                if(payload["code"] == "DEVICE-PARMS-UPDATED") {
                    Serial.printf("State is updated to: %d\n", (int) payload["update"]["state"]);

                    /* You can set some pins or trigger events that depend on successful
                    ** device parms update here.
                    */
                    return;
                }
                Serial.println("Failed to Update Parms");
                return;
            });

            // This updates the millis counter for
            // the five seconds scheduler.
            current = millis();
        }
    }
}