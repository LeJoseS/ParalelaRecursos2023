// Copyright 2021 Fabian Gonzalez Rojas <fabian.gonzalezrojas@ucr.ac.cr>

#include "Calculator.hpp"
#include <iostream>

Calculator::Calculator() {
}

Calculator::~Calculator() {
}

/*
typedef struct {
  size_t position;
  int64_t number;
  number_package_t* my_package;
} goldbach_package_t;
*/

void Calculator::consume(goldbach_package_t*& data) {
  int64_t goldbach_number = data->number;
  goldbach_sums_t* sums = this->CalculateGoldbach(goldbach_number);
  assembler_package_t* packet = new assembler_package_t{
    data->position, sums, data->my_package};
  this->produce(packet);
}

/*
typedef struct {
  size_t position;
  goldbach_sums_t* sums;
  number_package_t* my_package;
} assembler_package_t;
*/

void Calculator::produce(assembler_package_t*& data) {
  assert(this->producingQueue);
  this->producingQueue->push(data);
}

int Calculator::run() {
  this->consumeForever();
  return EXIT_SUCCESS;
}

goldbach_sums_t* Calculator::CalculateGoldbach(int64_t number) {
  // default is to not print, if the number is negative, prints the sums
  std::cout << "My current number is: [" << number << "]\n";
  bool print_sums = false;
  goldbach_sums_t* results = NULL;
  if (number < 0) {
    number *= -1;
    print_sums = true;
  }
  if (number > 5) {
    if ((number%2) == 0) {
      results = this->CalculateGoldbachStrong(number, print_sums);
    } else {
      results = this->CalculateGoldbachWeak(number, print_sums);
    }
    if (print_sums) {
      results->number *= -1;
    }
  } else {
    results = this->CreateResults(number, 0, NULL, NULL);
  }
  return results;
}

goldbach_sums_t* Calculator::CalculateGoldbachStrong(
  const int64_t number, const bool print_sums) {
  Queue<goldbach_strong_node_t*>* strong_queue;
  if(print_sums){
    strong_queue = new Queue<goldbach_strong_node_t*>();
  }
  goldbach_strong_node_t* new_node = NULL;
  int64_t sum_counter = 0;
  const int64_t limit = number/2;
  int64_t index_sum_counter = -1;
  for (int64_t first_prime = 3; first_prime <= limit; first_prime += 2) {
    if (index_sum_counter != 2) {
      if (this->IsThisNumberPrime(first_prime)) {
        if (this->IsThisNumberPrime(number - first_prime)) {
          if (print_sums) {

            new_node = new goldbach_strong_node_t;
            if ( !new_node ) {
              fprintf(stderr,
              "Could not create new Node from dynamic memory\n");
            }
            new_node->first_prime = first_prime;
            new_node->second_prime = number - first_prime;
            strong_queue->push(new_node);
          }
          ++sum_counter;
        }
      }
    } else {
      index_sum_counter = 0;
    }
  }
  return CreateResults(number, sum_counter, strong_queue, NULL);
}

goldbach_sums_t* Calculator::CalculateGoldbachWeak(
  const int64_t number, const bool print_sums) {
  Queue<goldbach_weak_node_t*>* weak_queue = new Queue<goldbach_weak_node_t*>();
  goldbach_weak_node_t* new_node = NULL;
  int64_t sum_counter = 0;
  const int64_t limit = number/3;
  for (int64_t first_prime = 2; first_prime <= limit; ++first_prime) {
    if ( this->IsThisNumberPrime(first_prime) ) {
      const int64_t second_limit = (number - first_prime)/2;
      for (int64_t second_prime=2; second_prime <=
      second_limit; ++second_prime) {
        if (this->IsThisNumberPrime(second_prime)) {
          int64_t third_prime = number - (first_prime + second_prime);
          if ( this->IsThisNumberPrime(third_prime) ) {
            if ((first_prime <= second_prime) &&
            (second_prime <= third_prime)) {
              if (print_sums) {
                new_node = new goldbach_weak_node_t;
                if ( !new_node ) {
                  fprintf(stderr,
                  "Could not create new Node from dynamic memory\n");
                }
                new_node->first_prime = first_prime;
                new_node->second_prime = second_prime;
                new_node->third_prime = third_prime;
                weak_queue->push(new_node);
              }
              ++sum_counter;
            }
          }
        }
      }
    }
  }
  return CreateResults(number, sum_counter, NULL, weak_queue);
}

bool Calculator::IsThisNumberPrime(int64_t number) {
  bool is_prime = true;
  if (number == 1) {
    return false;
  }
  if ( is_prime && number > 2 ) {
    if (number%2 == 0) {
      return false;
    }
    if (number > 3) {
      if (number%3 == 0) {
        return false;
      }
      if (number > 5) {
        const int64_t limit = ((int64_t)sqrt(number)) + 1;
        int64_t index_sum_counter = 0;
        for (int64_t i=5; i < limit; i += 2) {
          if ( index_sum_counter != 2 ) {
            if ( number%i == 0 ) {
              return false;
            }
            ++index_sum_counter;
          } else {
            index_sum_counter = 0;
          }
        }
      }
    }
  }
  return is_prime;
}

goldbach_sums_t* Calculator::CreateResults(int64_t number, int64_t sum_counter,
Queue<goldbach_strong_node_t*>* strong_queue,
Queue<goldbach_weak_node_t*>* weak_queue) {
  goldbach_sums_t* results = new goldbach_sums_t;
  results->number = number;
  results->sum_counter = sum_counter;
  results->strong_queue = strong_queue;
  results->weak_queue = weak_queue;
  return results;
}

// Queue<goldbach_sums_t*>* GoldbachWebApp::CalculateAllGoldbachSums(
//   Queue<int64_t>* input_queue) {
//     Calculator* calcu = new Calculator();
//     goldbach_sums_t* new_node = NULL;
//     Queue<goldbach_sums_t*>* results_queue = new Queue<goldbach_sums_t*>();
//     while ( !input_queue->isEmpty() ) {
//         new_node = calcu->CalculateGoldbach(input_queue->pop());
//         results_queue->push(new_node);
//     }
//     delete input_queue;
//     delete calcu;
//     return results_queue;
// }
