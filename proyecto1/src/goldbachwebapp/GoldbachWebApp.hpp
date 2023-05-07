// Copyright 2021 Jose Alberto Solano M. <jose.solanomurillo@ucr.ac.cr>

#ifndef GOLDBACHWEBAPP_H
#define GOLDBACHWEBAPP_H

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include "HttpServer.hpp"
#include "common.hpp"
#include "Consumer.hpp"
#include "Calculator.hpp"
#include "GoldbachCommon.hpp"
#include "Queue.hpp"
#include "Producer.hpp"

/**
* Class inherits from consumer, processes and sends
* the prime factorization answer to the client
*/
class GoldbachWebApp : public Consumer<Socket> ,
public Producer<number_package_t*>{
 protected:
  /**
   * @brief Sets the header and adds format to the HTML response codee
   * @param httpResponse has the body and header to be changed
   * @param title contains the desired tittle for the Html page
   * @return No return
   */
  void setUpHtml(HttpResponse& httpResponse,
     std::string title);

  /**
   * @brief Converts the socket into a httpRequest and htppResponse
   * @param client The socket
   * @return No return
   */
  void settupSocket(Socket& client);

 public:
  /// Constructor
  GoldbachWebApp();
  /// Destructor
  ~GoldbachWebApp();

  /**
   * @brief Consumes the socket of his consuming Queue, creates a
   * httpResponse and a httpResquest correspodingly
   * @param data The socket
   * @return No return
   */
  void consume(Socket& data) override;

  /**
   * @brief Sends the typedata of his producing Queue
   * @param data The number_package_t to send to the disassembler
   * @return No return
   */
  void produce(number_package_t*& data) override;

  /**
   * @brief Run forever the consumption
   * @return Error code
   */
  int run() override;

  /// Analyze the command line arguments
  /// @return true if program can continue execution, false otherwise
  bool analyzeArguments(HttpRequest& httpRequest, HttpResponse& httpResponse);

  /**
   * @brief Sends the homepage as HTTP response
   * @param httpResponse allows to send the Html code
   * @param httpRequest Where the user data is stored
   * @return Return true if it was able to send the message through HttpMessage
   */
  bool serveHomepage(HttpRequest& httpRequest, HttpResponse& httpResponse);

  /**
   * @brief Sends a page for a non found resouce in this server
   * @param httpResponse allows to send the Html code
   * @param httpRequest Where the user data is stored
   * @return Return true if it was able to send the message through HttpMessage
   */
  bool serveNotFound(HttpRequest& httpRequest, HttpResponse& httpResponse);

  /**
   * @brief Calculate the prime factorization of a number asked by the client,
   * and sends the response in HTML format as HTTP response
   * @param httpResponse allows to send the Html code
   * @param httpRequest Where the user data is stored
   * @param numbers is the vector where the solicited numbers for processing
   * are found
   * @return Return true if it was able to send the message through HttpMessage
   */
  bool serveGoldbachSums(HttpRequest& httpRequest
    , HttpResponse& httpResponse, std::vector<int64_t> numbers);

  /**
   * @brief Receives a Queue of integers and returns a Queue of the goldbach sums
   * @details Goes 1 by 1 calculating the goldbach sums for every number in the
   *  input_queue and storing the results in a Queue of Results.
   * @param input_queue A queue of integers to calculate the goldbach sums
   * @return A queue of the results of the goldbach sums
   */
  Queue<goldbach_sums_t*>* CalculateAllGoldbachSums(
      Queue<int64_t>* input_queue);

  /**
   * @brief Gets the results from the structs to a string
   * @param results The struct of results
   * @return Returns the strings with the results
   */
  std::string PrintGoldbachSums(goldbach_sums_t* results);
};

#endif  // GOLDBACHWEBAPP_H
