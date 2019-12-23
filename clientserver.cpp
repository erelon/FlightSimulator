//
// Created by erelon on 21/12/2019.
//

#ifndef UNTITLED3__CLINETSERVER_H_
#define UNTITLED3__CLINETSERVER_H_

#include <sys/socket.h>
#include <iostream>
#include <unistd.h>
#include <mutex>
#include <netinet/in.h>
#include <cstring>
#include <arpa/inet.h>

int serverOpen(int port) {
  std::mutex mute;
  mute.lock();
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
  } else {
    std::cout << "Server is now listening ..." << std::endl;
  }


  // accepting a client
  int client_socket = accept(socketfd, (struct sockaddr *) &address, (socklen_t *) &address);
  mute.unlock();
  if (client_socket == -1) {
    std::cerr << "Error accepting client" << std::endl;
    return -4;
  }
  close(socketfd); //closing the listening socket

  while(true) {
    //reading from client all the time
    char buffer[1024] = {0};
    int valread = read(client_socket, buffer, 1024);

  }
  return 0;
}

void connectCom(int port) {
  //create socket
  int client_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (client_socket == -1) {
    //error
    std::cerr << "Could not create a socket" << std::endl;
    return;
  }

  //We need to create a sockaddr obj to hold address of server
  sockaddr_in address; //in means IP4
  address.sin_family = AF_INET;//IP4
  address.sin_addr.s_addr = inet_addr("127.0.0.1");  //the localhost address
  address.sin_port = htons(port);
  //we need to convert our number (both port & localhost)
  // to a number that the network understands.

  // Requesting a connection with the server on local host with port given
  int is_connect = connect(client_socket, (struct sockaddr *) &address, sizeof(address));
  if (is_connect == -1) {
    std::cerr << "Could not connect to host server" << std::endl;
    return;
  } else {
    std::cout << "Client is now connected to server" << std::endl;
  }

  //if here we made a connection
  char hello[] = "Hi from client";
  int is_sent = send(client_socket, hello, strlen(hello), 0);
  if (is_sent == -1) {
    std::cout << "Error sending message" << std::endl;
  } else {
    std::cout << "Hello message sent to server" << std::endl;
  }

  char buffer[1024] = {0};
  int valread = read(client_socket, buffer, 1024);
  std::cout << buffer << std::endl;

  close(client_socket);
}

#endif //UNTITLED3__CLINETSERVER_H_
