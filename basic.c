#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum {
  HEAD, TAIL, CONDUCTOR
} cell_type_t;

struct cell {
  cell_type_t cell_type;
  int x;
  int y;
  cell_type_t new_cell_type;
};

struct world {
  int size;
  struct cell * cell_array;
  int num_heads;
};

void destroy_world(struct world * world) {
  free(world->cell_array);
  free(world);
}

struct world * read(char * filename) {
  FILE *fp = fopen(filename, "r");
  int size;
  int i, num_heads;

  fscanf(fp, "%d", &size);

  struct cell * cell_array = malloc(size * sizeof(struct cell));
  struct cell newcell;
  for (i = 0; i < size; i++) {
    int x, y, cell_type;
    fscanf(fp, "%d %d %d", &x, &y, &cell_type);
    newcell.x = x;
    newcell.y = y;
    newcell.cell_type = (cell_type_t) cell_type;
    if (cell_type == HEAD)
      num_heads++;
    cell_array[i] = newcell;
  }

  struct world * result = malloc(sizeof(struct world));
  result->size = size;
  result->cell_array = cell_array;
  result->num_heads = num_heads;
  return result;
}

void print_world(struct world * world) {
  int i;
  printf("x\ty\ttype\n");
  for (i = 0; i < world->size; i++) {
    struct cell cell = world->cell_array[i];
    printf("%d\t%d\t%d\n", cell.x, cell.y, (int) cell.cell_type);
  }
  printf("\n");
}

bool is_neighbour(struct cell cell, int x, int y) {
  return (abs(cell.x - x) <= 1) && (abs(cell.y - y) <= 1);
}

void evolve_world(struct world * world) {
  int i, num_heads;
  for (i = 0; i < world->size; i++) {
    struct cell cell = world->cell_array[i];
    int j, count;

    switch (cell.cell_type) {
      case HEAD:
        cell.new_cell_type = TAIL;
        break;
      case TAIL:
        cell.new_cell_type = CONDUCTOR;
        break;
      case CONDUCTOR:
        count = 0;
        for (j = 0; j < world->size; j++) {
          if (i!=j && is_neighbour(world->cell_array[j], cell.x, cell.y)) {
            if (world->cell_array[j].cell_type == HEAD) {
              count ++;
            }
          }
        }
        if (count == 1 || count == 2) {
          cell.new_cell_type = HEAD;
          num_heads++;
        } else {
          cell.new_cell_type = CONDUCTOR;
        }
    }
    world->cell_array[i] = cell;
  }

  for (i = 0; i < world->size; i++) {
    world->cell_array[i].cell_type = world->cell_array[i].new_cell_type;
  }
  world->num_heads = num_heads;
}

int main (int argc, char * argv[]) {
  int i, cycles = 10;
  printf("reading %s\n", argv[1]);
  struct world * world = read(argv[1]);

  print_world(world);
  for (i = 0; i < cycles; i++) {
    evolve_world(world);
    print_world(world);
  }
  destroy_world(world);
}