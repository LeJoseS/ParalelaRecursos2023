// Copyright 2021 Fabian Gonzalez Rojas <fabian.gonzalezrojas@ucr.ac.cr>

#include "Assembler.hpp"
#include <iostream>

Assembler::Assembler() {
}

Assembler::~Assembler() {
}

/*
typedef struct {
  size_t position;
  int64_t number;
  number_package_t* my_package;
} goldbach_package_t;
*/

void Assembler::consume(assembler_package_t*& data) {
  // Puts all the processed sums of one number in their
  // respective socket's vector
  data->my_package->results->at(data->position) = data->sums;
  ++data->my_package->processed;
  if (data->my_package->processed == data->my_package->results->size()) {
    this->produce(data->my_package);
  }
}

void Assembler::produce(number_package_t*& data) {
  assert(this->producingQueue);
  this->producingQueue->push(data);
}

int Assembler::run() {
  this->consumeForever();
  return EXIT_SUCCESS;
}
