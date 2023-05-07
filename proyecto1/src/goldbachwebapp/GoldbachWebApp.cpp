// Copyright 2021 Jose Alberto Solano M. <jose.solanomurillo@ucr.ac.cr>

#include <bits/stdc++.h>
#include <vector>
#include <string>
#include <cassert>
#include <iostream>
#include <regex>
#include <stdexcept>

#include "NetworkAddress.hpp"
#include "Socket.hpp"
#include "GoldbachWebApp.hpp"
#include "common.hpp"
#include "GoldbachCommon.hpp"

GoldbachWebApp::GoldbachWebApp() {
}

GoldbachWebApp::~GoldbachWebApp() {
}

void GoldbachWebApp::consume(Socket& data) {
  this->settupSocket(data);
}

void GoldbachWebApp::produce(number_package_t*& data) {
  assert(this->producingQueue);
  this->producingQueue->push(data);
}

int GoldbachWebApp::run() {
  this->consumeForever();
  return EXIT_SUCCESS;
}

void GoldbachWebApp::settupSocket(Socket& client) {
  // While the same client asks for HTTP requests in the same connection
  while (true) {
    // Create an object that parses the HTTP request from the socket
    HttpRequest httpRequest(client);

    // If the request is not valid or an error happened
    if (!httpRequest.parse()) {
      // Non-valid requests are normal after a previous valid request. Do not
      // close the connection yet, because the valid request may take time to
      // be processed. Just stop waiting for more requests
      break;
    }

    // A complete HTTP client request was received. Create an object for the
    // server responds to that client's request
    HttpResponse httpResponse(client);

    // Give subclass a chance to respond the HTTP request
    const bool handled = this->analyzeArguments(httpRequest, httpResponse);

    // If subclass did not handle the request or the client used HTTP/1.0
    if (!handled || httpRequest.getHttpVersion() == "HTTP/1.0") {
      // The socket will not be more used, close the connection
      client.close();
    }
  }
}

// void GoldbachWebApp::setQueue(Queue<Socket>* consumingQueue,
// Queue<number_packet_t*>* socket_numbers_queue) {
//   this->consumingQueue = consumingQueue;
//   this->socket_numbers_queue = socket_numbers_queue;
// }   Recordar poner lo de las queues

bool GoldbachWebApp::analyzeArguments(HttpRequest&
  httpRequest, HttpResponse& httpResponse) {
    // If the home page was asked
    if (httpRequest.getMethod() == "GET" && httpRequest.getURI() == "/") {
      return this->serveHomepage(httpRequest, httpResponse);
    }
    std::smatch matches;
    // If a number was asked in the form "/goldbach/1223"
    // or "/goldbach?number=1223, 2423"
    std::regex inQuery(
      "^/goldbach(/|\\?number=|\\?text=)(((-?\\d+)(,|%2C)?)+)$");
    if (std::regex_search(httpRequest.getURI(), matches, inQuery)) {
      assert(matches.length() >=3);
      Queue<int64_t>* goldbach_numbers = new Queue<int64_t>();
      inQuery = ("(?!2C)(-?\\d+)");
      std::smatch num_matches;
      std::string numbers_in_URI = matches[2];
      // extract all numbers un the URI using a regular expression
      while (std::regex_search(numbers_in_URI, num_matches, inQuery)) {
        try {
          int64_t number = std::stoll(num_matches[1]);
          goldbach_numbers->push(number);
        } catch (const std::out_of_range& out_of_range) {
          return this->serveNotFound(httpRequest, httpResponse);
        }
        numbers_in_URI = num_matches.suffix().str();
      }
      // Puts the data into the packet (number_package_t)
      socket_data_package_t * socket =
      new socket_data_package_t{HttpRequest(httpRequest),
      HttpResponse(httpResponse)};
      size_t processed = 0;
      std::vector<goldbach_sums_t*>* results =
      new std::vector<goldbach_sums_t*>(goldbach_numbers->size());
      number_package_t * packet = new number_package_t{goldbach_numbers, socket,
       processed, results};
      this->produce(packet);
      return true;
    }
    // Unrecognized request
    return this->serveNotFound(httpRequest, httpResponse);
}

/*
typedef struct {
  Queue<int64_t>* numbers;
  socket_data_package_t* socket;
  int64_t processed;
  std::vector<Queue<goldbach_sums_t*>*>* results;
} number_package_t;
*/

bool GoldbachWebApp::serveHomepage(HttpRequest& httpRequest
  , HttpResponse& httpResponse) {
    (void)httpRequest;
    // Set HTTP response metadata (headers)
    // Build the body of the response
    std::string title = "Goldbach sums";
    this->setUpHtml(httpResponse, title);
    httpResponse.body()
      << "  <form method=\"get\" action=\"/goldbach\">\n"
      << "    <label for=\"number\">Number</label>\n"
      << "    <input type=\"text\" name=\"number\" required/>\n"
      << "    <button type=\"submit\">Calculate</button>\n"
      << "  </form>\n"
      << "</html>\n";

    // Send the response to the client (user agent)
    return httpResponse.send();
  }

bool GoldbachWebApp::serveNotFound(HttpRequest& httpRequest
  , HttpResponse& httpResponse) {
    (void)httpRequest;

    // Set HTTP response metadata (headers)
    httpResponse.setStatusCode(404);

    // Build the body of the response
    std::string title = "Not found";
    this->setUpHtml(httpResponse, title);
    httpResponse.body()
      << "  <p>The requested resouce was not found in this server.</p>\n"
      << "  <hr><p><a href=\"/\">Homepage</a></p>\n"
      << "</html>\n";

    // Send the response to the client (user agent)
    return httpResponse.send();
  }

void GoldbachWebApp::setUpHtml(HttpResponse& httpResponse, std::string title) {
  httpResponse.setHeader("Server", "AttoServer v1.0");
  httpResponse.setHeader("Content-type", "text/html; charset=ascii");

  httpResponse.body() << "<!DOCTYPE html>\n"
    << "<html lang=\"en\">\n"
    << "  <meta charset=\"ascii\"/>\n"
    << "  <title>" << title << "</title>\n"
    << "  <style>body {font-family: monospace} h1 {color: red}</style>\n"
    << "  <h1>" << title << "</h1>\n";
}
