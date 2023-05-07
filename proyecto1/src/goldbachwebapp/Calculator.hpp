// Copyright 2021 Fabian Gonzalez Rojas <fabian.gonzalezrojas@ucr.ac.cr>

#ifndef CALCULATOR_HPP
#define CALCULATOR_HPP

#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <cassert>

#include "common.hpp"
#include "GoldbachCommon.hpp"
#include "Queue.hpp"
#include "Consumer.hpp"
#include "Producer.hpp"

class Calculator : public Consumer<goldbach_package_t*> ,
public Producer<assembler_package_t*>{
    DISABLE_COPY(Calculator);

 public:
    // constructor
    Calculator();

    // destructor
    ~Calculator();

   /**
   * @brief Consumes a number and calculates the goldbach sums for it, then
   * sends its sums with the corresponding position and his data
   * @param data The number to process and his data
   * @return No return
   */
  void consume(goldbach_package_t*& data) override;

  /**
   * @brief Sends the typedata of his producing Queue
   * @param data The assembler_package_t to send to the assembler
   * @return No return
   */
  void produce(assembler_package_t*& data) override;

  /**
   * @brief Run forever the consumption
   * @return Error code
   */
  int run() override;

  /**
   * @brief Receives the number and calculates the goldbach conjeture for it
   * @details Converts the negatives into positives and selects the strong or weak conjeture
   *  for each number, if the number is negative, it stores all the sums
   * Invalid numbers as 5 will be stored with a total of 0 sums
   * @param number The pair or odd number to calcule the strong or weak Goldbach conjecture
   * @return Returns the total sums and the sums themselfs
   */
  goldbach_sums_t* CalculateGoldbach(const int64_t number);

 private:
    /**
     * @brief Calculates the strong goldbach conjecture and prints the amount of combinations
     * @details Goes from 3 to half of the number testing for every prime number
     * for the first number of the sum and then test the second number with the formula:
     * (number - the first prime). If both numbers are prime, the number of sums is increase
     * by 1 and stored them in dynamic memory based on the arguments
     * @param number The pair number to calcule the strong conjecture
     * @param print_sums If true, prints all the sums and if false, doesn't print
     * @return No return
     */
    goldbach_sums_t* CalculateGoldbachStrong(
        const int64_t number, const bool print_sums);

    /**
     * @brief Calculates the weak goldbach conjecture and prints the amount of combinations
     * @details  Goes from 2 to the number testing for every prime number that finds,
     * for the first number of the sum and then test the second number with the formula:
     * (number - the first prime). Then test the third number with the formula:
     * number - (first_prime + second_prime).
     * If the 3 numbers are prime, the number of sums is increase
     * by 1 and stored them in dynamic memory based on the arguments
     * @param number The odd number to calcule the weak conjecture
     * @param print_sums If true, prints all the sums and if false, doesn't print
     * @return No return
     */
    goldbach_sums_t* CalculateGoldbachWeak(
        const int64_t number, const bool print_sums);

    /**
     * @brief Calculates if the number is prime
     * @details Goes from 2 to the sqrt of the number dividing by
     * every prime number to check if the number is indeed a prime.
     * @param number The number that is going to test if it's prime
     * @return Returns a bool value based on the "prime-ness" of the number
     */
    bool IsThisNumberPrime(int64_t number);

    /**
     * @brief Puts the data collected in a result struct and returns
     * @param number The number calculated
     * @param sum_counter The total of goldbach sums
     * @param strong_queue Pointer to the sums of the strong conjecutre, NUll for no sums
     * @param weak_queue Pointer to the sums of the strong conjecutre, NUll for no sums
     * @return Returns a bool value based on the "prime-ness" of the number
     */
    goldbach_sums_t* CreateResults(int64_t number, int64_t sum_counter,
    Queue<goldbach_strong_node_t*>* strong_queue,
    Queue<goldbach_weak_node_t*>* weak_queue);
};  // end Calculator

#endif  // CALCULATOR_HPP
