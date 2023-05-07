// Copyright 2021 Por determinar
// <fabian.gonzalezrojas@ucr.ac.cr>
// <jose.solanomurillo@ucr.ac.cr>

#ifndef MAGICIAN_HPP
#define MAGICIAN_HPP

#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <mpi.h>

#include <chrono>
#include <queue>
#include <string>
#include <fstream>

#include "common.hpp"
#include "Forest.hpp"
#include "Queue.hpp"
#include "CrystalBall.hpp"

/// Controller class
class Magician {
 private:
    Queue<Map_t*>* map_queue;
    CrystalBall* crystal_ball;

 public:
    DISABLE_COPY(Magician);

    /// Constructor
    Magician() {
      map_queue = new Queue<Map_t*>();
      crystal_ball = new CrystalBall(map_queue);
    }

    /// Destructor
    ~Magician() {
      delete crystal_ball;
      delete map_queue;
    }

    /**
     * @brief Starts the program
     * @param argc Number of args
     * @param argv Arguments
     * @return Returns the error code
     */
    int start(int argc, char* argv[]);

    /**
    * @brief Receives the arguments entered in the terminal
     * @param argc Number of args
     * @param argv Arguments
     * @return Returns the error code
     */
    int analyzeArguments(int argc, char* argv[]);

    /**
     * @brief Simulates the advance of time in the Forests
     * @return Returns the error code
     */
    int Simulate(int32_t& can_work, int& process_count);

    /**
    *  @brief The secondary process function, it receives a map indefinitly
    * inside a loop, until it's told to stop by the main process.
    * The maps are read, process and written inside a file here
     * @param can_work The needed value to notify the main process that
     * @param argc Number of arguments
     * @param argv Arguments
     * this process can work
     * @return No return
     */
    void mpiConsumingProcess(int32_t& can_work, int argc, char* argv[]);
};

#endif  // MAGICIAN_HPP
