// Copyright 2021 Fabian Gonzalez Rojas <fabian.gonzalezrojas@ucr.ac.cr>

#include "Disassembler.hpp"
#include <iostream>

Disassembler::Disassembler() {
}

Disassembler::~Disassembler() {
}

/*
typedef struct {
  Queue<int64_t>* numbers;
  socket_data_package_t* socket;
  int64_t processed;
  std::vector<Queue<goldbach_sums_t*>*>* results;
} number_package_t;
*/

void Disassembler::consume(number_package_t*& data) {
  size_t position = 0;
  int64_t number = 0;
  // Puts all numbers of one socket in the numbers_queue
  // 10 20 30 40 50 60
  // position 0, number 10, my_package
  // position 1, number 20, my_package etc
  while (!data->numbers->isEmpty()) {
    number = data->numbers->pop();
    goldbach_package_t* packet = new goldbach_package_t{position, number, data};
    this->produce(packet);
    ++position;
  }
  delete data->numbers;
}

/*
typedef struct {
  size_t position;
  int64_t number;
  number_package_t* my_package;
} goldbach_package_t;
*/

void Disassembler::produce(goldbach_package_t*& data) {
  assert(this->producingQueue);
  this->producingQueue->push(data);
}

int Disassembler::run() {
  this->consumeForever();
  return EXIT_SUCCESS;
}
