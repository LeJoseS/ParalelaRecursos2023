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
#include "Sender.hpp"
#include "common.hpp"
#include "GoldbachCommon.hpp"

Sender::Sender() {
}

Sender::~Sender() {
}

/*
typedef struct {
  Queue<int64_t>* numbers;
  socket_data_package_t* socket;
  int64_t processed;
  std::vector<goldbach_sums_t*>* results;
} number_package_t;
*/

void Sender::consume(number_package_t*& data) {
    this->serveGoldbachSums(data->socket->httpRequest,
    data->socket->httpResponse, data->results);
    delete data->socket;
}

int Sender::run() {
  this->consumeForever();
  return EXIT_SUCCESS;
}

bool Sender::serveGoldbachSums(HttpRequest& httpRequest
  , HttpResponse& httpResponse, std::vector<goldbach_sums_t*>* results) {
  (void)httpRequest;
  // Build the body of the response
  std::string title = "Goldbach sums for ";  // + std::to_string(number);
  this->setUpHtml(httpResponse, title);
  for (size_t i = 0; i < results->size(); ++i) {
    httpResponse.body() << this->GoldbachSumstoString(results->at(i));
  }
  delete results;
  httpResponse.body() << "  </ol>\n"
  << "  <hr><p><a href=\"/\">Back</a></p>\n"
  << "</html>\n";
  // Send the response to the client (user agent)
  return httpResponse.send();
}

std::string Sender::GoldbachSumstoString(goldbach_sums_t* results) {
  std::string response = "";
  if ( results->sum_counter == 0 ) {
    response += std::to_string(results->number) + ": NA<br>";
  } else {
    response += std::to_string(results->number) +
     ": " + std::to_string(results->sum_counter) + " sums";
    if (results->strong_queue != NULL || results->weak_queue != NULL) {
      if (results->number%2 == 0) {
        // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        goldbach_strong_node_t* current = NULL;
        if (!results->strong_queue->isEmpty()) {
          response += ": ";
        }
        while (!results->strong_queue->isEmpty()) {
          current = results->strong_queue->pop();
          response += std::to_string(current->first_prime) +
           " + " + std::to_string(current->second_prime);
          delete current;
          if (!results->strong_queue->isEmpty()) {
            response += ", ";
          }
        }
        delete results->strong_queue;
        // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      } else {
        // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        goldbach_weak_node_t* current = NULL;
        if (!results->weak_queue->isEmpty()) {
          response += ": ";
        }
        while (!results->weak_queue->isEmpty()) {
          current = results->weak_queue->pop();
          response +=  std::to_string(current->first_prime) +
           " + " + std::to_string(current->second_prime) +
          " + " + std::to_string(current->third_prime);
          delete current;
          if (!results->weak_queue->isEmpty()) {
            response += ", ";
          }
        }
        delete results->weak_queue;
        // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      }
    }
    response +="<br>";
  }
  delete results;
  return response;
}

void Sender::setUpHtml(HttpResponse& httpResponse,
  std::string title) {
  httpResponse.setHeader("Server", "AttoServer v1.0");
  httpResponse.setHeader("Content-type", "text/html; charset=ascii");

  httpResponse.body() << "<!DOCTYPE html>\n"
    << "<html lang=\"en\">\n"
    << "  <meta charset=\"ascii\"/>\n"
    << "  <title>" << title << "</title>\n"
    << "  <style>body {font-family: monospace} h1 {color: red}</style>\n"
    << "  <h1>" << title << "</h1>\n";
}
