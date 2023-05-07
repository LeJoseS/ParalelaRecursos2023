// Copyright 2021 Fabian Gonzalez Rojas <fabian.gonzalezrojas@ucr.ac.cr>

#ifndef DISASSEMBLER_HPP
#define DISASSEMBLER_HPP

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

class Disassembler : public Consumer<number_package_t*> ,
public Producer<goldbach_package_t*>{
    DISABLE_COPY(Disassembler);

 public:
    // constructor
    Disassembler();

    // destructor
    ~Disassembler();

   /**
   * @brief Consumes the numbers of his consuming Queue, and sends 1 at a time
   * to the calculators with his corresponding position and his data
   * @param data The number_data_package_t to disassemble
   * @return No return
   */
  void consume(number_package_t*& data) override;

  /**
   * @brief Sends the typedata of his producing Queue
   * @param data The goldbach_package_t to send to the calculators
   * @return No return
   */
  void produce(goldbach_package_t*& data) override;

  /**
   * @brief Run forever the consumption
   * @return Error code
   */
  int run() override;
};  // end disassembler

#endif  // DISASSEMBLER_HPP
