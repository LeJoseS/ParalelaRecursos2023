// Copyright 2021 Jeisson Hidalgo-Cespedes. Universidad de Costa Rica. CC BY 4.0

#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <vector>

#include "HttpServer.hpp"
#include "GoldbachWebApp.hpp"
#include "Queue.hpp"
#include "GoldbachCommon.hpp"
#include "Disassembler.hpp"
#include "Assembler.hpp"
#include "Sender.hpp"

#define DEFAULT_PORT "8080"

class WebServer : public HttpServer {
  DISABLE_COPY(WebServer);

 private:
  /// TCP port where this web server will listen for connections
  const char* port = DEFAULT_PORT;
  /// WebServer -> socket_queue -> GoldbachWebApp
  Queue<Socket>* socket_queue;
  /// GoldbachWebApp -> socket_numbers_queue -> Disassembler
  Queue<number_package_t*>* socket_numbers_queue;
  /// Disassembler -> numbers_queue -> Calculator
  Queue<goldbach_package_t*>* numbers_queue;
  /// Calculator -> sums_queue -> Assembler
  Queue<assembler_package_t*>* sums_queue;
  /// Assembler -> output_queue -> Sender -> Salida a pagina web
  Queue<number_package_t*>* output_queue;
  /// threads
  std::vector<GoldbachWebApp*> connection_threads;
  Disassembler* disassembler_thread = nullptr;
  std::vector<Calculator*> calcu_threads;
  Assembler* assembler_thread = nullptr;
  Sender* sender_thread = nullptr;

 public:
  /// Constructor
  WebServer();
  /// Destructor
  ~WebServer();
  /// Start the simulation
  int start(int argc, char* argv[]);

 protected:
  GoldbachWebApp* goldbach_web_app;

  /// Analyze the command line arguments
  /// @return true if program can continue execution, false otherwise
  bool analyzeArguments(int argc, char* argv[]);
  /// Inicializes the thread classes and sets their queues
  void setThreads();
  /// Starts the threads inside of the producer and consumer classes
  void startThreads();
  /// Makes the threads join with the main thread and wait to die
  void waitThreads();
  /// Handle HTTP requests. @see HttpServer::handleHttpRequest()
  bool handleHttpRequest(HttpRequest& httpRequest,
    HttpResponse& httpResponse) override;

 protected:
  /// Route, that provide an answer according to the URI value
  /// For example, home page is handled different than a number
  // bool route(HttpRequest& httpRequest, HttpResponse& httpResponse);

  /// Sends the homepage as HTTP response
  bool serveHomepage(HttpRequest& httpRequest, HttpResponse& httpResponse);
  /// Sends a page for a non found resouce in this server
  bool serveNotFound(HttpRequest& httpRequest, HttpResponse& httpResponse);
  /// Calculate the prime factorization of a number asked by the client, and
  /// sends the response in HTML format as HTTP response
  bool serveGoldbachSums(HttpRequest& httpRequest
    , HttpResponse& httpResponse, int64_t number);
};

#endif  // WEBSERVER_H
