//
// Created by erelon on 21/12/2019.
//

#ifndef UNTITLED3__CLINETSERVER_H_
#define UNTITLED3__CLINETSERVER_H_

#include <sys/socket.h>
#include <string>
#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <cstring>
#include <thread>

int serverOpen(int port);
void connectCom(int port);
#endif //UNTITLED3__CLINETSERVER_H_
