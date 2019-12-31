#include "commands.h"
#include "utils.h"
#include <sys/socket.h>
#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <cstring>
#include <arpa/inet.h>
#include <mutex>

#define NUM_OF_ATTR 36

using namespace std;
void loadNames(vector<string> *p_vector);
void preformChange(pair<string, vars *> item, int client_socket);

int serverOpen(int port, unordered_map<string, vars *> *map, int *threadRunMode) {
  //create socket
  int socketfd = socket(AF_INET, SOCK_STREAM, 0);
  if (socketfd == -1) {
    //error
    std::cerr << "Could not create a socket" << std::endl;
    return -1;
  }

  //bind socket to IP address
  // we first need to create the sockaddr obj.
  sockaddr_in address; //in means IP4
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY; //give me any IP allocated for my machine
  address.sin_port = htons(port);
  //we need to convert our number
  // to a number that the network understands.

  //the actual bind command
  if (bind(socketfd, (struct sockaddr *) &address, sizeof(address)) == -1) {
    std::cerr << "Could not bind the socket to an IP" << std::endl;
    return -2;
  }

  //making socket listen to the port
  if (listen(socketfd, 5) == -1) { //can also set to SOMAXCON (max connections)
    std::cerr << "Error during listening command" << std::endl;
    return -3;
  }

  // accepting a client
  int client_socket = accept(socketfd, (struct sockaddr *) &address, (socklen_t *) &address);
  if (client_socket == -1) {
    std::cerr << "Error accepting client" << std::endl;
    return -4;
  }
  close(socketfd); //closing the listening socket

  unordered_map<string, double> allMapData;
  vector<string> names;
  loadNames(&names);
  for (auto iter: names) {
    allMapData[iter.c_str()] = 0;
  }
  while (*threadRunMode != -1) {
    //reading from client all the time
    char buffer[2048] = {0};
    int valread = read(client_socket, buffer, 1024);
    //cout << buffer << endl;
    vector<string> tokens = split(buffer, ',');
    if (tokens.size() != NUM_OF_ATTR)
      continue;
    //saving the data in a map
    auto token = tokens.begin();
    for (auto iter: names) {
      allMapData[iter.c_str()] = atof(token->c_str());
      token++;
    }
    //loading the info to the database
    mutex mute;
    mute.lock();
    for (auto item: *map) {
      (*map)[item.first]->value = allMapData[item.first];
    }
    mute.unlock();
  }
  close(client_socket);
  return 0;
}
void loadNames(vector<string> *p_vector) {
  (*p_vector).push_back("\"/instrumentation/airspeed-indicator/indicated-speed-kt\"");
  (*p_vector).push_back("\"/sim/time/warp\"");
  (*p_vector).push_back("\"/controls/switches/magnetos\"");
  (*p_vector).push_back("\"/instrumentation/heading-indicator/offset-deg\"");
  (*p_vector).push_back("\"/instrumentation/altimeter/indicated-altitude-ft\"");
  (*p_vector).push_back("\"/instrumentation/altimeter/pressure-alt-ft\"");
  (*p_vector).push_back("\"/instrumentation/attitude-indicator/indicated-pitch-deg\"");
  (*p_vector).push_back("\"/instrumentation/attitude-indicator/indicated-roll-deg\"");
  (*p_vector).push_back("\"/instrumentation/attitude-indicator/internal-pitch-deg\"");
  (*p_vector).push_back("\"/instrumentation/attitude-indicator/internal-roll-deg\"");
  (*p_vector).push_back("\"/instrumentation/encoder/indicated-altitude-ft\"");
  (*p_vector).push_back("\"/instrumentation/encoder/pressure-alt-ft\"");
  (*p_vector).push_back("\"/instrumentation/gps/indicated-altitude-ft\"");
  (*p_vector).push_back("\"/instrumentation/gps/indicated-ground-speed-kt\"");
  (*p_vector).push_back("\"/instrumentation/gps/indicated-vertical-speed\"");
  (*p_vector).push_back("\"/instrumentation/heading-indicator/indicated-heading-deg\"");
  (*p_vector).push_back("\"/instrumentation/magnetic-compass/indicated-heading-deg\"");
  (*p_vector).push_back("\"/instrumentation/slip-skid-ball/indicated-slip-skid\"");
  (*p_vector).push_back("\"/instrumentation/turn-indicator/indicated-turn-rate\"");
  (*p_vector).push_back("\"/instrumentation/vertical-speed-indicator/indicated-speed-fpm\"");
  (*p_vector).push_back("\"/controls/flight/aileron\"");
  (*p_vector).push_back("\"/controls/flight/elevator\"");
  (*p_vector).push_back("\"/controls/flight/rudder\"");
  (*p_vector).push_back("\"/controls/flight/flaps\"");
  (*p_vector).push_back("\"/controls/engines/engine/throttle\"");
  (*p_vector).push_back("\"/controls/engines/current-engine/throttle\"");
  (*p_vector).push_back("\"/controls/switches/master-avionics\"");
  (*p_vector).push_back("\"/controls/switches/starter\"");
  (*p_vector).push_back("\"/engines/active-engine/auto-start\"");
  (*p_vector).push_back("\"/controls/flight/speedbrake\"");
  (*p_vector).push_back("\"/sim/model/c172p/brake-parkin\"");
  (*p_vector).push_back("\"/controls/engines/engine/primer\"");
  (*p_vector).push_back("\"/controls/engines/current-engine/mixture\"");
  (*p_vector).push_back("\"/controls/switches/master-bat\"");
  (*p_vector).push_back("\"/controls/switches/master-alt\"");
  (*p_vector).push_back("\"/engines/engine/rpm\"");
}

int connectCom(int port, const unordered_map<string, vars *> *writeMap, int *threadRunMode) {
  //create socket
  int client_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (client_socket == -1) {
    //error
    std::cerr << "Could not create a socket" << std::endl;
    return -1;
  }

  //We need to create a sockaddr obj to hold address of server
  sockaddr_in address; //in means IP4
  address.sin_family = AF_INET;//IP4
  address.sin_addr.s_addr = inet_addr("127.0.0.1");  //the localhost address
  address.sin_port = htons(port);
  //we need to convert our number (both port & localhost)
  // to a number that the network understands.

  // Requesting a connection with the server on local host with port given

  int is_connect = -1;
  while (is_connect == -1) {
    is_connect = connect(client_socket, (struct sockaddr *) &address, sizeof(address));
    sleep(1);
  }

  unordered_map<string, double> changes;

  while (*threadRunMode != -1) {
    if (*threadRunMode == 0) {
      for (auto item: *writeMap) {
        if (changes.find(item.first) == changes.end()) {
          changes[item.first] = item.second->value;
          preformChange(item, client_socket);
        } else if (changes[item.first] != item.second->value)
          preformChange(item, client_socket);
      }
    }
    sleep(1);
  }

  close(client_socket);
  return 0;
}

void preformChange(pair<string, vars *> item, int client_socket) {
  string temp = item.second->sim;
  temp.pop_back();
  temp = temp.substr(1);
  string mess = "set " + temp + " " + to_string(item.second->value) + "\r\n";
  auto is_sent = write(client_socket, mess.c_str(), mess.size());
  if (is_sent == -1) {
    std::cout << "Error sending message" << std::endl;
  }
}
