// Copyright 2021 Por determinar
// <fabian.gonzalezrojas@ucr.ac.cr>
// <jose.solanomurillo@ucr.ac.cr>

#include "CrystalBall.hpp"

void CrystalBall::divideJob(std::string dir, std::string job_filename) {
  std::ifstream job;
  job.open(dir + job_filename);
  std::string linea;
  std::string map_name;
  int64_t nights;
  while (std::getline(job, linea)) {
      std::istringstream args(linea);
      args >> map_name;
      args >> nights;
      InsertMap(dir, map_name, nights);
    }
  job.close();
}

void CrystalBall::InsertMap(std::string dir, std::string map_name,
const int64_t nights) {
    Map_t* map = new Map_t {dir, map_name, nights};
    map_queue->push(map);
}


Forest* CrystalBall::MaterializeForest(Map_t* map_data) {
  std::ifstream file;
  file.open(map_data->dir + map_data->filename);
  int64_t input_rows = 0;
  int64_t input_cols = 0;
  file >> input_rows;
  file >> input_cols;
  char** new_forest = new char*[input_rows];
  for (int64_t i = 0; i < input_rows; ++i) {
    new_forest[i] = new char[input_cols];
  }
  for (int64_t row = 0; row < input_rows; ++row) {
    for (int64_t col = 0; col < input_cols; ++col) {
        file >> new_forest[row][col];
    }
  }
  file.close();
  Forest* forest = new Forest(new_forest, input_rows, input_cols,
  map_data->nights, map_data->dir, map_data->filename);
  delete map_data;
  return forest;
}

void CrystalBall::processForest(Forest* forest) {
    int64_t total_updates = forest->getNights();
    for (int64_t i = 1; i <= total_updates; ++i) {
      forest->UpdateForest();
      if (forest->getExport() || i == forest->getNights()) {
        this->exportForest(forest, i);
      }
    }
}

void CrystalBall::exportForest(Forest* forest, const int64_t current_night) {
  char** matrix = forest->getMatrix();
  std::string output = "./results/" + forest->getFilename() + "-" +
  std::to_string(current_night) + ".txt";
  std::ofstream out(output);
  out << current_night << ":\n";
  int64_t row_l = forest->getRow_l();
  int64_t col_l = forest->getCol_l();
  for (int row = 0; row < row_l; row++) {
    for (int col = 0; col < col_l; col++) {
      out << matrix[row][col];
    }
    out << "\n";
  }
  out.close();
}

std::string CrystalBall::mapToString(Map_t* data) {
  std::string map = data->dir + " " + data->filename + " " +
  std::to_string(data->nights);
  delete data;
  return map;
}

Map_t* CrystalBall::stringToMap(std::string& data) {
  Map_t* map = new Map_t;
  std::stringstream line(data);
  line >> map->dir;
  line >> map->filename;
  line >> map->nights;
  return map;
}
