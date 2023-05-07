// Copyright 2021 Fabian Gonzalez Rojas <fabian.gonzalezrojas@ucr.ac.cr>

#ifndef ASSEMBLER_HPP
#define ASSEMBLER_HPP

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

class Assembler : public Consumer<assembler_package_t*>,
public Producer<number_package_t*>{
    DISABLE_COPY(Assembler);

 public:
    // constructor
    Assembler();

    // destructor
    ~Assembler();

  /**
   * @brief Consumes the assembler_package_t of his consuming Queue, and puts
   * together one number at a time in his corresponding response vector. If
   * the vector is already full, sends the data to the sender
   * @param data The assembler_package_t to assemble
   * @return No return
   */
  void consume(assembler_package_t*& data) override;

  /**
   * @brief Sends the typedata of his producing Queue
   * @param data The number_package_t to send to the sender
   * @return No return
   */
  void produce(number_package_t*& data) override;

  /**
   * @brief Run forever the consumption
   * @return Error code
   */
  int run() override;
};  // end Assembler

#endif  // ASSEMBLER_HPP
