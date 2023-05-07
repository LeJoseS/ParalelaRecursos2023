// Copyright 2021 Por determinar
// <fabian.gonzalezrojas@ucr.ac.cr>
// <jose.solanomurillo@ucr.ac.cr>

#include "Forest.hpp"

void Forest::UpdateForest() {
    char** tmp = old_forest;
    old_forest = updated_forest;
    updated_forest = tmp;
    // #pragma omp parallel for num_threads(omp_get_thread_num()) \
    // default(none) schedule(dynamic)
        for (int64_t i = 0; i < row_l; ++i) {
            this->UpdateRow(i);
        }
}

void Forest::UpdateRow(const int64_t row) {
    for (int64_t i = 0; i < this->col_l; ++i) {
        this->UpdateCell(row, i);
    }
}

void Forest::UpdateCell(const int64_t cell_row, const int64_t cell_col) {
    int64_t tree_counter = 0;
    int64_t lake_counter = 0;
    for (int64_t row = cell_row-1; row < cell_row+2; ++row) {
        for (int64_t col = cell_col-1; col < cell_col+2; ++col) {
            if (row != cell_row || col != cell_col) {
                if (((row >= 0) && (col >= 0) &&
                (row < this->row_l) && (col < this->col_l))) {
                    if (this->old_forest[row][col] == 'l') {
                        ++lake_counter;
                    } else if (this->old_forest[row][col] == 'a') {
                        ++tree_counter;
                    }
                }
            }
        }
    }
    switch (this->old_forest[cell_row][cell_col]) {
        case 'a':
            this->updated_forest[cell_row][cell_col] = Tree(lake_counter,
            tree_counter);
        break;
        case 'l':
            this->updated_forest[cell_row][cell_col] = Lake(lake_counter);
        break;
        case '-':
            this->updated_forest[cell_row][cell_col] = Prairie(tree_counter);
        break;
        // error
        default:
            this->updated_forest[cell_row][cell_col] = 'e';
        break;
    }
}

char Forest::Tree(const int64_t lake_counter, const int64_t tree_counter) {
    if (lake_counter >= 4) {
        // Inundación: Si la celda tiene un árbol y al menos 4 vecinos
        // que son lago encantado, entonces el lago ahoga el árbol,
        // y pasa a ser lago encantado.
        return 'l';
    } else if (tree_counter > 4) {
        // Hacinamiento: Si la celda es un árbol y tiene más
        // de 4 vecinos árbol, el exceso de sombra
        // evita que crezca y entonces pasa a ser pradera.
        return '-';
    }
    // Estabilidad: Cualquier otra situación, la celda permanece como está.
    return 'a';
}

char Forest::Lake(const int64_t lake_counter) {
    if (lake_counter < 3) {
        // Sequía: Si la celda es lago encantado y tiene
        // menos de 3 vecinos que sean lago encantado,
        // entonces el lago se seca y pasa a ser pradera.
        return '-';
    }
    // Estabilidad: Cualquier otra situación, la celda permanece como está.
    return 'l';
}

char Forest::Prairie(const int64_t tree_counter) {
    if (tree_counter >= 3) {
        // Reforestación: Si la celda es pradera y tiene al
        // menos 3 vecinos árboles, las semillas tendrán
        // espacio para crecer y la celda se convierte en árbol.
        return 'a';
    }
    // Estabilidad: Cualquier otra situación, la celda permanece como está.
    return '-';
}
