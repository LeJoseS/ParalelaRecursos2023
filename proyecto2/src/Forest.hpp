// Copyright 2021 Por determinar
// <fabian.gonzalezrojas@ucr.ac.cr>
// <jose.solanomurillo@ucr.ac.cr>

#ifndef FOREST_HPP
#define FOREST_HPP

#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <omp.h>

#include <iostream>
#include <string>

#include "common.hpp"

/// Simulates the Forest described by the Mage
class Forest {
 private:
    int64_t row_l;
    int64_t col_l;
    char** old_forest;
    char** updated_forest;
    bool export_all;
    int64_t nights;
    std::string dir;
    std::string filename;

    DISABLE_COPY(Forest);

 public:
    /// Receives a matrix of chars that has been already created and readed
    Forest(char** forest, int64_t row_l, int64_t col_l,
    int64_t nights, std::string dir, std::string input_filename) {
        this->row_l = row_l;
        this->col_l = col_l;
        this->export_all = true;
        this->updated_forest = forest;
        this->nights = nights;
        this->dir = dir;
        this->filename = input_filename;
        this->filename.resize(6);
        if (this->nights < 0) {
          this->nights *= -1;
          this->export_all = false;
        }
        this->old_forest = new char*[this->row_l];
        for (int i = 0; i < this->row_l; ++i) {
            this->old_forest[i] = new char[this->col_l];
            for (int col = 0; col < this->col_l; ++col) {
              this->old_forest[i][col] = 'e';
            }
        }
    }

    /// Destructor
    ~Forest() {
        for (int i = 0; i < this->row_l; ++i) {
            delete[] this->old_forest[i];
            delete[] this->updated_forest[i];
        }
        delete[] this->old_forest;
        delete[] this->updated_forest;
        this->row_l = 0;
        this->col_l = 0;
    }

    /**
     * @brief Gets the Forest Matrix
     * @return The pointer to the Forest Matrix
     */
    char** getMatrix() {
      return this->updated_forest;
    }

    /**
     * @brief Get the Nights
     * @return The total nights to update
     */
    int64_t getNights() {
      return this->nights;
    }

    /**
     * @brief Returns the value that stores if export
     * @return Returns the export_all bool value
     */
    bool getExport() {
      return this->export_all;
    }

    /**
     * @brief Gets the dir
     * @return Returns the dir asociated with the forest
     */
    std::string getDir() {
       return this->dir;
    }

    /**
     * @brief Gets the filename
     * @return Returns the filename asociated with the forest
     */
    std::string getFilename() {
       return this->filename;
    }

    /**
     * @brief Gets the row_l
     * @return Returns the total rows of the forest
     */
    int64_t getRow_l() {
      return this->row_l;
    }


    /**
    * @brief Gets the col_l
     * @return Returns the total columns of the forest
     */
    int64_t getCol_l() {
      return this->col_l;
    }

    /**
     * @brief Updates the forest 1 day applying all the rules
     * @return No return
     */
    void UpdateForest();

 private:
    /**
     * @brief Updates an entire row of the forest
     * @param row The row to be updated
     * @return No return
     */
    void UpdateRow(const int64_t row);

    /**
     * @brief Updates the content of 1 cell, checking the 8 spaces around
     * @param row The cell row to be updated
     * @param col The cell col to be updated
     * @return No return
     */
    void UpdateCell(const int64_t row, const int64_t col);

    /**
     * @brief Checks the rules of a tree and selects a return char
     * @param lake_counter The amount of lakes around the char
     * @param tree_counter The amount of trees around the char
     * @return Returns the updated char based on the rules of Tree
     */
    char Tree(const int64_t lake_counter, const int64_t tree_counter);

    /**
     * @brief Checks the rules of a lake and selects a return char
     * @param lake_counter The amount of lakes around the char
     * @return Returns the updated char based on the rules of lake
     */
    char Lake(const int64_t lake_counter);

    /**
     * @brief Checks the rules of a lake and selects a return char
     * @param tree_counter The amount of trees around the char
     * @return Returns the updated char based on the rules of lake
     */
    char Prairie(const int64_t tree_counter);
};

#endif  // FOREST_HPP
