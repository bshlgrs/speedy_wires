// this file expects stuff to be input in column-major form
// that is, [(0,0),(0,1),(1,0),(1,1)]

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
  char neighboring_heads;
};

struct world {
  int size;
  struct cell * cell_array;
};

// struct head_list {
//   int x;
//   int y;
//   struct head_list * tail;
// };


bool cell_greater_than(struct cell cell1, int x, int y) {
  return (cell1.y > y || (cell1.y ==  y && cell1.x > x));
}

int binary_search(int x, int y, struct world * world) {
  int start = 0;
  int end = world->size;
  while (abs(start - end) > 1) {

    int middle = (end + start) / 2;
    struct cell middle_cell = world->cell_array[middle];

    if (middle_cell.x == x && middle_cell.y == y) {
      return middle;
    }

    if (cell_greater_than(middle_cell, x, y)) {
      end = middle;
    } else {
      start = middle;
    }
  }
  return -1;
}

void destroy_world(struct world * world) {
  free(world->cell_array);
  free(world);
}

struct world * read(char * filename) {
  FILE *fp = fopen(filename, "r");
  int size;
  int i;

  fscanf(fp, "%d", &size);

  struct cell * cell_array = malloc(size * sizeof(struct cell));
  struct cell newcell;
  int prev_x = -1;
  int prev_y = -1;

  for (i = 0; i < size; i++) {
    int x, y, cell_type;

    fscanf(fp, "%d %d %d", &x, &y, &cell_type);
    newcell.x = x;
    newcell.y = y;
    newcell.cell_type = (cell_type_t) cell_type;
    newcell.neighboring_heads = 0;

    if (y < prev_y || (y == prev_y && x < prev_x)) {
      printf("the input was in the wrong order!\n");
      exit(1);
    }

    prev_x = x;
    prev_y = y;

    cell_array[i] = newcell;
  }

  struct world * result = malloc(sizeof(struct world));
  result->size = size;
  result->cell_array = cell_array;
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

    if (cell.cell_type == HEAD) {
      int x, y;
      for (x = -1; x <= 1; x++) {
        for (y = -1; y <= 1; y++) {
          if (x != 0 || y != 0) {
            int pos = binary_search(cell.x + x, cell.y + y, world);
            if (pos != -1) {
              world->cell_array[pos].neighboring_heads++;
            }
          }
        }
      }
    }
  }

  for (i = 0; i < world->size; i++) {
    switch (world->cell_array[i].cell_type) {
      int n;
      case HEAD:
        world->cell_array[i].cell_type = TAIL;
        break;
      case TAIL:
        world->cell_array[i].cell_type = CONDUCTOR;
        break;
      case CONDUCTOR:
        n = world->cell_array[i].neighboring_heads;
        if (n == 1 || n == 2) {
          world->cell_array[i].cell_type = HEAD;
        } else {
          world->cell_array[i].cell_type = CONDUCTOR;
        }
        break;
    }
    world->cell_array[i].neighboring_heads = 0;
  }
}

int main (int argc, char * argv[]) {
  int i, cycles = 10000;
  printf("reading %s\n", argv[1]);
  struct world * world = read(argv[1]);

  // print_world(world);
  for (i = 0; i < cycles; i++) {
    evolve_world(world);
    // print_world(world);
  }
  destroy_world(world);
  printf("done\n");
}