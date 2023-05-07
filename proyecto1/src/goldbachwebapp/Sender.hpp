// Copyright 2021 Jose Alberto Solano M. <jose.solanomurillo@ucr.ac.cr>

#ifndef SENDER_H
#define SENDER_H

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include "common.hpp"
#include "Consumer.hpp"
#include "GoldbachCommon.hpp"
#include "Queue.hpp"
#include "Producer.hpp"

/**
* Class inherits from consumer, processes and sends
* the prime factorization answer to the client
*/
class Sender : public Consumer<number_package_t*>{
 protected:
  /**
   * @brief Sets the header and adds format to the HTML response codee
   * @param httpResponse has the body and header to be changed
   * @param title contains the desired tittle for the Html page
   * @return No return
   */
  void setUpHtml(HttpResponse& httpResponse,
     std::string title);

 public:
  /// Constructor
  Sender();
  /// Destructor
  ~Sender();

  /**
   * @brief Consumes the typedata of his consuming Queue and prints the results
   * in the web
   * @param data The number_package_t with the results inside to send
   * @return No return
   */
  void consume(number_package_t*& data) override;

  /**
   * @brief Run forever the consumption
   * @return Error code
   */
  int run() override;

  /**
   * @brief Calculate the prime factorization of a number asked by the client,
   * and sends the response in HTML format as HTTP response
   * @param httpResponse allows to send the Html code
   * @param httpRequest Where the user data is stored
   * @param results The results to print in the web
   * @return Return true if it was able to send the message through HttpMessage
   */
  bool serveGoldbachSums(HttpRequest& httpRequest
    , HttpResponse& httpResponse, std::vector<goldbach_sums_t*>* results);

    /**
     * @brief Gets the results from the structs to a string
     * @param results The struct of results
     * @return Returns the strings with the results
     */
    std::string GoldbachSumstoString(goldbach_sums_t* results);
};
#endif  // SENDER_H
