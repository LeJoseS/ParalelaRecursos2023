// Copyright 2021 Fabian Gonzalez Rojas <fabian.gonzalezrojas@ucr.ac.cr>

#ifndef GOLDBACHCOMMON_HPP
#define GOLDBACHCOMMON_HPP

#include <vector>

#include "HttpServer.hpp"
#include "Queue.hpp"

/// struct Node made for the Strong Conjecture Queue
typedef struct {
  int64_t first_prime;
  int64_t second_prime;
} goldbach_strong_node_t;

/// struct Node made for the Weak Conjecture Queue
typedef struct {
  int64_t first_prime;
  int64_t second_prime;
  int64_t third_prime;
} goldbach_weak_node_t;

/// struct for storing the number, the sums and the head of the sum queue
typedef struct {
  int64_t number;
  // if the entry is less than 5, the sum_counter will be 0
  int64_t sum_counter;
  Queue<goldbach_strong_node_t*>* strong_queue;
  Queue<goldbach_weak_node_t*>* weak_queue;
} goldbach_sums_t;

/// A socket struct
typedef struct {
  HttpRequest httpRequest;
  HttpResponse httpResponse;
} socket_data_package_t;

/// Contains the numbers of 1 socket, the socket, the number of processed
/// numbers and vector where to store the results
typedef struct {
  Queue<int64_t>* numbers;
  socket_data_package_t* socket;
  size_t processed;
  std::vector<goldbach_sums_t*>* results;
} number_package_t;

/// Contains a number, his position and a reference to the package where it
/// belongs
typedef struct {
  size_t position;
  int64_t number;
  number_package_t* my_package;
} goldbach_package_t;

/// Contains the sums of a number, his position and a reference to the
/// package where it belongs
typedef struct {
  size_t position;
  goldbach_sums_t* sums;
  number_package_t* my_package;
} assembler_package_t;

#endif  // GOLDBACHCOMMON_HPP
