// Copyright 2021 Jeisson Hidalgo-Cespedes. Universidad de Costa Rica. CC BY 4.0

#include <cassert>
#include <iostream>
#include <regex>
#include <stdexcept>
#include <string>

#include "NetworkAddress.hpp"
#include "Socket.hpp"
#include "WebServer.hpp"

#ifndef THREAD_COUNT
#define THREAD_COUNT 4
#endif

const char* const usage =
  "Usage: webserv [port] [max_connections]\n"
  "\n"
  "  port             Network port to listen incoming HTTP requests, default "
    DEFAULT_PORT "\n"
  "  max_connections  Maximum number of allowed client connections\n";

// TODO(you) Make WebServer a singleton class. See the Log class
WebServer::WebServer() {
  socket_queue = new Queue<Socket>();
  socket_numbers_queue = new Queue<number_package_t*>();
  numbers_queue = new Queue<goldbach_package_t*>();
  sums_queue = new Queue<assembler_package_t*>();
  output_queue = new Queue<number_package_t*>();
}

WebServer::~WebServer() {
  delete socket_queue;
  delete socket_numbers_queue;
  delete numbers_queue;
  delete sums_queue;
  delete output_queue;
}

int WebServer::start(int argc, char* argv[]) {
  if (analyzeArguments(argc, argv)) {
    try {
      // Se escucha por peticiones al programa en el puerto de red
      this->listenForConnections(this->port);
      const NetworkAddress& address = this->getNetworkAddress();
      std::cout << "web server listening on " << address.getIP()
      << " port " << address.getPort() << "...\n";
      // Se crean, inician y setean los threads
      this->setThreads();
      this->startThreads();
      this->acceptAllConnections(this->socket_queue);
      this->waitThreads();
    } catch (const std::runtime_error& error) {
      std::cerr << "error: " << error.what() << std::endl;
    }
  } else { std::cerr << "error: invalid arguments" << std::endl;}
  return EXIT_SUCCESS;
}

bool WebServer::analyzeArguments(int argc, char* argv[]) {
  // Traverse all arguments
  for (int index = 1; index < argc; ++index) {
    const std::string argument = argv[index];
    if (argument.find("help") != std::string::npos) {
      std::cout << usage;
      return false;
    }
  }

  if (argc >= 2) {
    this->port = argv[1];
  }

  return true;
}

void WebServer::setThreads() {
  // Iniciando threads de conexion y seteando sus queues
  this->connection_threads.resize(THREAD_COUNT);
  for ( size_t index = 0; index < THREAD_COUNT; ++index ) {
    this->connection_threads[index] = new GoldbachWebApp();
    assert(this->connection_threads[index]);
    this->connection_threads[index]->setConsumingQueue(this->socket_queue);
    this->connection_threads[index]->setProducingQueue(
      this->socket_numbers_queue);
  }
  // Iniciando thread de desensamblador y seteando su queue
  this->disassembler_thread = new Disassembler();
  this->disassembler_thread->setConsumingQueue(this->socket_numbers_queue);
  this->disassembler_thread->setProducingQueue(this->numbers_queue);
  // Iniciando threads de calculadora y seteando sus queues
  this->calcu_threads.resize(THREAD_COUNT);
  for ( size_t index = 0; index < THREAD_COUNT; ++index ) {
    this->calcu_threads[index] = new Calculator();
    assert(this->calcu_threads[index]);
    this->calcu_threads[index]->setConsumingQueue(this->numbers_queue);
    this->calcu_threads[index]->setProducingQueue(this->sums_queue);
  }
  // Iniciando thread de ensamblador y seteando su queue
  this->assembler_thread = new Assembler();
  this->assembler_thread->setConsumingQueue(this->sums_queue);
  this->assembler_thread->setProducingQueue(this->output_queue);
  // Iniciando el sender y seteando su cola
  this->sender_thread = new Sender();
  this->sender_thread->setConsumingQueue(this->output_queue);
}

void WebServer::startThreads() {
  // Start the Server and threads
  for ( size_t index = 0; index < THREAD_COUNT; ++index ) {
    this->connection_threads[index]->startThread();
  }
  this->disassembler_thread->startThread();
  for ( size_t index = 0; index < THREAD_COUNT; ++index ) {
    this->calcu_threads[index]->startThread();
  }
  this->assembler_thread->startThread();
  this->sender_thread->startThread();
}

void WebServer::waitThreads() {
  // Wait for objets to finish the simulation
  for ( size_t index = 0; index < THREAD_COUNT; ++index ) {
    this->connection_threads[index]->waitToFinish();
  }
  this->disassembler_thread->waitToFinish();
  for ( size_t index = 0; index < THREAD_COUNT; ++index ) {
    this->calcu_threads[index]->waitToFinish();
  }
  this->assembler_thread->waitToFinish();
  this->sender_thread->waitToFinish();
}

bool WebServer::handleHttpRequest(HttpRequest& httpRequest,
    HttpResponse& httpResponse) {
  // Print IP and port from client
  const NetworkAddress& address = httpRequest.getNetworkAddress();
  std::cout << "connection established with client " << address.getIP()
    << " port " << address.getPort() << std::endl;

  // Print HTTP request
  std::cout << "  " << httpRequest.getMethod()
    << ' ' << httpRequest.getURI()
    << ' ' << httpRequest.getHttpVersion() << std::endl;

  return true;
  // this->route(httpRequest, httpResponse);
}

/*
bool WebServer::route(HttpRequest& httpRequest, HttpResponse& httpResponse) {
  // socket_data_package_t* data = new socket_data_package_t;
  // data->httpRequest = &httpRequest;
  // data->httpResponse = &httpResponse;
  // if(data == NULL) {
  //   return false;
  // }
  // //return this->goldbach_web_app->analyzeArguments(httpRequest, httpResponse);
  // this->socket_queue->push(data);
  return true;
}
*/
