/**
 * @file DuplexHandler.h
 * @date 20.06.2020
 * @author Grandeur Technologies
 *
 * Copyright (c) 2019 Grandeur Technologies LLP. All rights reserved.
 * This file is part of the Arduino SDK for Grandeur.
 *
 */

// Including headers
#include "types.h"
#include "macros.h"
#include "EventEmitter/EventEmitter.h"
#include "arduinoWebSockets/WebSocketsClient.h"
#include <list>

#ifndef DUPLEXHANDLER_H_
#define DUPLEXHANDLER_H_

// Class to establish and handle real-time communication channel with Grandeur and send/receive
// messages on this channel.
class DuplexHandler {
  private:
    // We make connection with Grandeur over Websockets.
    // This variable stores information about the websockets connection.
    WebSocketsClient _client;
    // Store query and access token to use while establishing the connection with Grandeur.
    String _query;
    String _token;
    // Stores current status (CONNECTED / DISCONNECTED) of the connection.
    bool _status;
    // Points to the connection handler function to call when connection with Grandeur is successfully
    // estbalished.
    void (*_connectionHandler)(bool);
    // Handles request/response like communication.
    EventEmitter<gId, Callback> _tasks;
    // List of subscribable events.
    const char* _events[1] = {"data"};
    // Handles pub/sub like communication.
    EventEmitter<String, Callback> _subscriptions;
    
    
    void duplexEventHandler(WStype_t eventType, uint8_t* packet, size_t length);
    // Prepares a message.
    Message prepareMessage(const char* task);
    Message prepareMessage(const char* task, const char* payload);
    // Sends a generic duplex message.
    void sendMessage(gId messageId, const char* message);
    // Receives a message from duplex channel.
    void receive(Var header, Var payload);
    // Handles the update packet.
    void publish(const char* event, const char* path, Var data);

    // BUFFER:
    // We use map to implement buffering of messages when duplex channel isn't alive.
    std::map<gId, String> _buffer;
    // Adds a message to the buffer with id.
    void buffer(gId id, String message);
    // Removes a message from the buffer with id.
    void debuffer(gId id);
    // Sends all messages in the buffer to the channel.
    void flushBuffer();

  public:
    // Constructor
    DuplexHandler();
    void init(Config config);
    // Sends a message to duplex channel.
    Message send(const char* task, Callback cb);
    Message send(const char* task, const char* payload, Callback cb);

    // Subscribes to a topic.
    gId subscribe(const char* topic, const char* payload, Callback updateHandler);
    // Unsubscribes from a topic.
    void unsubscribe(gId eventId, const char* payload);


    // Schedules a connection handler function to be called when connection with Grandeur
    // establishes/drops.
    void onConnectionEvent(void connectionCallback(bool));
    // Removes the connection handler function.
    void clearConnectionCallback(void);
    
    // Gets current status (CONNECTED / DISCONNECTED) of the connection.
    bool getStatus(void);
    
    // This runs duplex
    void loop(bool valve);

    #if DEBUG
    // Defines these functions when in debug mode.
    void (*connectionHandler)(bool) = _connectionHandler;
    const char* query = _query.c_str();
    const char* token = _token.c_str();
    #endif /* DEBUG */
};

#endif