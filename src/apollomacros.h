/**
 * @file apollomacros.h
 * @date 24.03.2020
 * @author Grandeur Technologies
 *
 * Copyright (c) 2019 Grandeur Technologies LLP. All rights reserved.
 * This file is part of the Arduino SDK for Grandeur Cloud.
 *
 */

// Connection macros
#define APOLLO_URL "api.grandeur.tech"
#define APOLLO_PORT 443
#define APOLLO_FINGERPRINT ""

// Debugging macros
#define DEBUG_PORT Serial

#ifdef DEBUG_PORT
#define DEBUG_APOLLO(...) DEBUG_PORT.printf(__VA_ARGS__)
#else
//#define DEBUG_DUPLEX(...) os_printf( __VA_ARGS__ )
#endif

#ifndef DEBUG_APOLLO
#define DEBUG_APOLLO(...)
#endif

// Strings sizes
#define SSID_SIZE 32
#define PASSPHRASE_SIZE 32
#define DEVICEID_SIZE 32
#define APIKEY_SIZE 35
#define TOKEN_SIZE 512
#define IP_SIZE 16
#define FINGERPRINT_SIZE 256
#define PACKET_SIZE 512
#define PING_PACKET_SIZE 64
#define TASK_SIZE 32

// Send Queue Size
#define SENDQUEUE_SIZE 16

// Ping interval in milliseconds
#define PING_INTERVAL 25000

// Macros for device states
#define WIFI_DISCONNECTED 0
#define WIFI_CONNECTED 1
#define APOLLO_CONNECTED 2

// Reusable macros for connection events (Cloud and WiFi)
#define DISCONNECTED 0
#define CONNECTED 1

// Indexes for update handler callbacks in subscriptions array
#define NUMBER_OF_TOPICS 2
#define SUMMARYUPDATE 0
#define PARMSUPDATE 1
