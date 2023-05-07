// Copyright 2021 Por determinar
// <fabian.gonzalezrojas@ucr.ac.cr>
// <jose.solanomurillo@ucr.ac.cr>

#ifndef CRYSTALBALL_HPP
#define CRYSTALBALL_HPP

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <bits/stdc++.h>
#include <errno.h>

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <queue>

#include "common.hpp"
#include "Forest.hpp"
#include "Queue.hpp"

/// Visualizer/file admin
class CrystalBall {
 private:
    Queue<Map_t*>* map_queue;

 public:
    DISABLE_COPY(CrystalBall);

    /// Constructor
    explicit CrystalBall(Queue<Map_t*>* map_queue) {
      this->map_queue = map_queue;
    }

    /// Destructor
    ~CrystalBall() {}

   /**
     * @brief Converts Map_t to std::string
     * @param data The map information to read the matrix and create the forest
     * @return Returns a std::string with the data converted from Map_t
     */
    std::string mapToString(Map_t* data);

   /**
     * @brief Converts std::string to Map_t
     * @param data The map information to read the matrix and create the forest
     * @return Returns a Map_t with the data converted from std::string
     */
    Map_t* stringToMap(std::string& data);

     /**
     * @brief Extracts all the maps from the job
     * @param dir The directory to the job
     * @param filename The job file name
     * @return No return
     */
    void divideJob(std::string dir, std::string filename);

    /**
     * @brief Inserts the data of the map into the map_queue
     * @param dir The directory to the map
     * @param map_name The name of the file
     * @param nights The number of nights to process
     * @return No return
     */
    void InsertMap(std::string dir, std::string map_name, const int64_t nights);

    /**
     * @brief Updates the forest for the number of nights, also exports the results
     * @param forest The forest to be proccesed
     * @return No return
     */
    void processForest(Forest* forest);

    /**
     * @brief Creates a Forest with the map_queue() first Map_t info
     * @param map_data The map information to read the matrix and create the forest
     * @return Returns a pointer to the forest created
     */
    Forest* MaterializeForest(Map_t* map_data);

 private:
    /**
     * @brief Exports the forest into a .txt file
     * @param forest The forest to be exported
     * @param current_night The night of the exportation
     * @return No return
     */
    void exportForest(Forest* forest, const int64_t current_night);
};

#endif  // CRYSTALBALL_HPP
