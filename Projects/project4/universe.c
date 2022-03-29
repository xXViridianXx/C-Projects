#include "universe.h"

#include <inttypes.h>
#include <stdlib.h>

struct Universe { //created struct to store user variables
    uint32_t rows;
    uint32_t cols;
    bool **grid;
    bool toroidal;
};

Universe *uv_create(uint32_t rows, uint32_t cols, bool toroidal) {
    Universe *u = malloc(sizeof(Universe)); //allocates struct first

    u->grid = (bool **) calloc(rows, sizeof(bool *)); //allocates rows and cols
    for (uint32_t r = 0; r < rows; r++) {
        u->grid[r] = (bool *) calloc(
            cols, sizeof(bool)); // idea taken from assignment doc and Eugene's section
    }

    u->rows = rows;
    u->cols = cols; //setting user inputted vars to struct vars
    u->toroidal = toroidal;
    return u;
}

void uv_delete(Universe *u) {
    for (uint32_t i = 0; i < uv_rows(u); i++) { //reversing allocation process
        free(u->grid[i]);
        u->grid[i] = NULL; //to stop error pf accessing free'd element
    }
    free(u->grid);
    u->grid = NULL;
    free(u);
    u = NULL;
}

uint32_t uv_rows(Universe *u) {
    return u->rows;
}

uint32_t uv_cols(Universe *u) {
    return u->cols;
}

void uv_live_cell(Universe *u, uint32_t r, uint32_t c) {
    u->grid[r][c] = true;
}
void uv_dead_cell(Universe *u, uint32_t r, uint32_t c) {
    u->grid[r][c] = false;
}

bool uv_get_cell(Universe *u, uint32_t r, uint32_t c) {
    return u->grid[r][c];
}

bool uv_populate(Universe *u, FILE *infile) {
    uint32_t row;
    uint32_t col;

    while (fscanf(infile, "%" SCNu32 " %" SCNu32 "\n", &row, &col)
           != EOF) { //reads numbers after rows and cols in a loop
        if ((row < uv_rows(u)) && (col < uv_cols(u))) {
            uv_live_cell(u, row, col); // sets each coord to live cell
        } else {
            return false; //return false if coord out of range of grid
        }
    }

    return true;
}

uint32_t checker(Universe *u, uint32_t row_step, uint32_t col_step, uint32_t live) {
    if ((row_step < uv_rows(u))
        && (col_step < uv_cols(u))) { //if within bounds and live, increase live counter by 1
        if (uv_get_cell(u, row_step, col_step) == true) {
            live += 1;
        }
    }
    return live;
}

uint32_t nontoroidal(Universe *u, uint32_t r, uint32_t c, uint32_t alive) {
    uint32_t row_step = r - 1; //setting to left diag of given cell
    uint32_t col_step = c - 1;
    for (uint32_t i = 0; i < 3; i++) {
        alive = checker(u, row_step, col_step, alive); //iterating over top
        col_step += 1;
    }
    row_step = r;
    col_step = c - 1;
    for (uint32_t i = 0; i < 2; i++) {
        alive = checker(u, row_step, col_step, alive); //iterating middle
        col_step += 2;
    }
    row_step = r + 1;
    col_step = c - 1;
    for (uint32_t i = 0; i < 3; i++) {
        alive = checker(u, row_step, col_step, alive); //iterating bottom
        col_step += 1;
    }

    return alive;
}

uint32_t rowtori_next(Universe *u, uint32_t row, int step) { //mod function for toroidal
    uint32_t new_step;
    if (row < uv_rows(u) && (row >= 0)) {
        if (step == -1) {
            new_step = (row + uv_rows(u) - 1) % uv_rows(u); //mod function given by Eugene
            return new_step;
        }
        if (step == 1) {
            return new_step = (row + 1) % uv_rows(u); //mod func given by Eugene
        }
    }
    return -1;
}

uint32_t coltori_next(Universe *u, uint32_t col, int step) { //similar to prev func
    uint32_t new_step;
    if (col < uv_cols(u) && (col >= 0)) {
        if (step == -1) {
            new_step = (col + uv_cols(u) - 1) % uv_cols(u);
            return new_step;
        }
        if (step == 1) {
            return new_step = (col + 1) % uv_cols(u);
        }
        if (step == 2) {
            return new_step = (col + 2) % uv_cols(u);
        }
    }
    return -1;
}

uint32_t toroidal(Universe *u, uint32_t r, uint32_t c, uint32_t alive) {
    uint32_t row_step = rowtori_next(u, r, -1); //apply mod functions
    uint32_t col_step = coltori_next(u, c, -1);
    for (uint32_t i = 0; i < 3; i++) {
        alive = checker(u, row_step, col_step, alive); //use checker to see if neighbors alive
        col_step = coltori_next(u, col_step, 1);
    }
    row_step = r;
    col_step = c;
    col_step = coltori_next(u, c, -1);
    for (uint32_t i = 0; i < 2; i++) {
        alive = checker(u, row_step, col_step, alive);
        col_step = coltori_next(u, col_step, 2);
    }
    row_step = r;
    col_step = c;
    row_step = rowtori_next(u, r, 1);
    col_step = coltori_next(u, c, -1);
    for (uint32_t i = 0; i < 3; i++) {
        alive = checker(u, row_step, col_step, alive);
        col_step = coltori_next(u, col_step, 1);
    }

    return alive;
}

uint32_t uv_census(Universe *u, uint32_t r, uint32_t c) {
    uint32_t alive = 0;
    if (u->toroidal == false) {
        alive = nontoroidal(u, r, c, alive); //calls nontoroidal
    }
    if (u->toroidal == true) {
        alive = toroidal(u, r, c, alive); //calls toroidal
    }
    return alive;
}

void uv_print(Universe *u, FILE *outfile) {
    for (uint32_t r = 0; r < uv_rows(u); r++) {
        for (uint32_t c = 0; c < uv_cols(u); c++) {
            if (uv_get_cell(u, r, c) == true) { //prints out final generation; nested for loop
                fprintf(outfile, "o");
            } else {
                fprintf(outfile, ".");
            }
        }
        fprintf(outfile, "\n"); //when row done, print next row on new line
    }
}
