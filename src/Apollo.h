/**
 * @file Apollo.h
 * @date 20.03.2020
 * @author Grandeur Technologies
 *
 * Copyright (c) 2019 Grandeur Technologies LLP. All rights reserved.
 * This file is part of the Hardware SDK for ESP8266 module.
 *
 */

// Including headers
#include <ESP8266WiFi.h>
#include "DuplexClient.h"
#include "Duplex.h"
#include "EventTable.h"
#include "apollotypes.h"
#include "apollomacros.h"

#ifndef APOLLO_H_
#define APOLLO_H_

class ApolloDevice {
    // Class for handling device related functions
    private:
        // Device IP Address
        static char _deviceIP[IP_SIZE];
        
        // Handlers array for callback functions
        static Callback _handlers[4];
        // Events Table
        static EventTable _eventsTable;

        // Subscription Array for update handler functions
        static Callback _subscriptions[8];
        
        // Apollo state variable
        static short _state;
        
        static void ping();
        
        void _send(const char* task, const char* payload, Callback callback);
        void _subscribe(short event, const char* payload, Callback updateHandler);
        
    public:
        // Device constructor
        ApolloDevice();
        // Getter for Apollo state
        short getState(void);
        char* getStringifiedState(void);
        
        static void apolloEventHandler(WStype_t eventType, uint8_t* packet, size_t length);

        // Getter methods for Apollo configurations
        char* getSSID(void);
        char* getPassphrase(void);
        char* getDeviceIP(void);
        char* getApiKey(void);
        char* getToken(void);

        // Async methods
        void getSummary(Callback callback);
        void getParms(Callback callback);
        void setSummary(JSONObject summary, Callback callback);
        void setParms(JSONObject parms, Callback callback);

        // Method to update the device's TCP buffer
        void update(void);

        // Event handlers
        static void onWiFiConnected(const WiFiEventStationModeConnected& event);
        static void onWiFiDisconnected(const WiFiEventStationModeDisconnected& event);
        // When device makes/breaks connection with the Cloud
        void onApolloConnected(Callback receiver);
        void onApolloDisconnected(Callback receiver);

        // Listeners for events from the Cloud
        void onSummaryUpdated(Callback callback);
        void onParmsUpdated(Callback callback);
};

class Apollo {
    private:
    public:
        ApolloDevice init(const char* deviceID, const char* apiKey, const char* token, const char* ssid, const char* passphrase);
        ApolloDevice init(String deviceID, String apiKey, String token, String ssid, String passphrase);
};

extern Apollo apollo;

#endif