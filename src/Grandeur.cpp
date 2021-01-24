/**
 * @file Grandeur.cpp
 * @date 24.03.2020
 * @author Grandeur Technologies
 *
 * Copyright (c) 2019 Grandeur Technologies LLP. All rights reserved.
 * This file is part of the Arduino SDK for Grandeur.
 *
 */

#include "Grandeur.h"
#include "DuplexHandler.h"

Grandeur grandeur;

Grandeur::Grandeur() {}

Project Grandeur::init(String apiKey, String token) {
  // Setting config
  _config = {apiKey, token};

  // Creating a new project reference.
  Project project;

  // Duplex handles the realtime connection with the project.
  project._duplexHandler = DuplexHandler(_config);

  // Starting Duplex.
  project._duplexHandler.init();

  // Return project object
  return project;
}

Project::Project() {}

void Project::onConnection(void connectionCallback(bool)) {
  // Connection handler for duplex
  _duplexHandler.onConnectionEvent(connectionCallback);
}

bool Project::isConnected(void) {
  // Return status of duplex
  return _duplexHandler.getStatus() == CONNECTED;
}

Device Project::device(String deviceID) {
  // Return new device class object
  return Device(deviceID, _duplexHandler);
}

Datastore Project::datastore(void) {
  // Return new datastore class object 
  return Datastore(_duplexHandler);
}

void Project::loop(bool valve) {
  // Running duplex loop
  _duplexHandler.loop(valve);
}