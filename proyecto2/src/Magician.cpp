// Copyright 2021 Por determinar
// <fabian.gonzalezrojas@ucr.ac.cr>
// <jose.solanomurillo@ucr.ac.cr>

#include "Magician.hpp"

int Magician::start(int argc, char* argv[]) {
  if (MPI_Init(&argc, &argv) == MPI_SUCCESS) {
    int my_rank = -1;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);  //!< Devuelve id. de un proceso.

    int process_count = -1;
    MPI_Comm_size(MPI_COMM_WORLD, &process_count);  //!< cantidad de procesos

    char hostname[MPI_MAX_PROCESSOR_NAME];  //!< agarra nombre del host
    int hostname_len = -1;
    MPI_Get_processor_name(hostname, &hostname_len);
    /// mensaje enviado por trabajadores para indicar que pueden consumir
    int32_t can_work = 1;
    if (my_rank == 0) {
      std::ofstream archivo;
      archivo.open("./results/time.txt");
      // std::chrono::milliseconds mil(1000);
      auto start = std::chrono::system_clock::now();
      //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      if (this->analyzeArguments(argc, argv)) {
        return 1;
      }
      if (this->Simulate(can_work, process_count)) {
        return 2;
      }
      //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      auto end = std::chrono::system_clock::now();
      double processing_time = std::chrono::duration_cast
      <std::chrono::milliseconds>(end-start).count();
      std::cout << "Time taken: " << processing_time/1000 << '\n';
      archivo << "Time taken: " << processing_time/1000 << '\n';
      archivo.close();
    } else {
      mpiConsumingProcess(can_work, argc, argv);
    }
    MPI_Finalize();
  } else {
    fprintf(stderr, "Error: could not init MPI environment\n");
    return 3;
  }

  return 0;
}

int Magician::analyzeArguments(int argc, char* argv[]) {
  if (argc >= 3) {
    if (argv[2][0] == 'm') {
      crystal_ball->InsertMap(argv[1], argv[2], atoi(argv[3]));
    } else {
      if (argv[2][0] == 'j') {
        std::cout << "divided job pog" << '\n';
        crystal_ball->divideJob(argv[1], argv[2]);
      }
    }
  } else {
    fprintf(stderr, "Invalid arguments: Needs at least 3\n");
    return 1;
  }
  return 0;
}

/*
  Forest* forest = crystal_ball->MaterializeForest(map_queue->pop());
  crystal_ball->processForest(forest);
  delete forest;
*/

int Magician::Simulate(int32_t& can_work, int& process_count) {
    while (!map_queue->isEmpty()) {
        /// Adapted from
        /// https://stackoverflow.com/questions
        /// /4348900/mpi-recv-from-an-unknown-source
        /// Answer by KeithB
        MPI_Status status;
        /// receive message from any source
        MPI_Recv(&can_work, 1, MPI_INT32_T, MPI_ANY_SOURCE,
        MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        /// send reply back to sender of the message received above
        std::string info = crystal_ball->mapToString(map_queue->pop());
        MPI_Send(info.c_str(), info.size(), MPI_CHAR,
        status.MPI_SOURCE, 0, MPI_COMM_WORLD);
    }
    /// Send stop conditions to other process
    std::string stop = "-1";
    for (int dest = 1; dest < process_count; ++dest) {
      MPI_Status status;
      /// receive message from any source
      MPI_Recv(&can_work, 1, MPI_INT32_T, MPI_ANY_SOURCE,
      MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      MPI_Send(stop.c_str(), stop.size() , MPI_CHAR,
      status.MPI_SOURCE, 0, MPI_COMM_WORLD);
    }
    return 0;
}

void Magician::mpiConsumingProcess(int32_t& can_work, int argc, char* argv[]) {
  // if (argv[2][0] == 'm') {
  //   if (argc == 5) {
  //     omp_set_num_threads(std::atoi(argv[4]));
  //   }
  // } else {
  //   if (argv[2][0] == 'j') {
  //     if (argc == 4) {
  //       omp_set_num_threads(std::atoi(argv[3]));
  //     }
  //   }
  // }
  while (true) {
    // Adapted from
    // https://mpitutorial.com/tutorials/dynamic-receiving-with-mpi-probe-and-mpi-status/
    // by Wes Kendall
    // and
    // http://www.mathcs.emory.edu/~cheung/Courses/355/Syllabus/92-MPI/send+recv-adv.html
    // by Dr. Shun Yan Cheung
    MPI_Status status;
    int  mapSize;
    MPI_Send(&can_work , 1 , MPI_INT32_T, 0, 0 , MPI_COMM_WORLD);
    /// Espera a que le envian un mensaje y revisa el tamaño para
    /// poder hacer el MPI_Recv que necesita el tamaño
    MPI_Probe(0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    /// Obtiene el tamaño del mensaje en mapSize
    MPI_Get_count(&status, MPI_CHAR, &mapSize);
    /// Asigna memoria para guardar mapa como char*
    char* map = new char[mapSize];
    /// Recibe el mapa con el map size
    MPI_Recv(map, mapSize, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
    std::string mapString(map);
    /// Condicion de parada para salirse si ya todos terminaron
    std::string first_two = mapString.substr(0, 2);
    if (first_two == "-1") {
      std::cout << "Im die, thank you forever" <<'\n';
      break;
    }
    delete[] map;
    /// llama a boosque y comienza la simulación
    Forest* forest = crystal_ball->
    MaterializeForest(crystal_ball->stringToMap(mapString));
    crystal_ball->processForest(forest);
  }
}
