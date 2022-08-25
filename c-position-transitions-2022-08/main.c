#include <stdio.h>
#include <stdlib.h>

typedef enum Transaction { BUY, SELL } Transaction;

typedef enum PositionType{ NONE, BUY_TO_OPEN, SELL_TO_OPEN } PositionType;

typedef enum AssetType{ STOCK, OPTION } AssetType;

typedef struct Postion {
  char * ticker;
  PositionType type;
  int open_quantity;
} Position;

char * to_str(PositionType type) {
  switch (type) {
    case NONE:
      return "NONE";
    case BUY_TO_OPEN:
      return "BTO";
    case SELL_TO_OPEN:
      return "STO";
  }
}

void print_position(Position * position) {
  printf("Position {\n");
  printf("  ticker = %s\n", position->ticker);
  printf("  open_quantity = %d\n", position->open_quantity);
  printf("  type = %s\n", to_str(position->type));
  printf("}\n");
}



Position * new_position(char * ticker) {
  Position  * position = malloc(sizeof(Position));
  position->ticker = ticker;
  position->type = NONE;
  position->open_quantity = 0;
  return position;
}

void flip(Position * position) {
  switch (position->type) {
    case NONE:
      // NO-OP
      break;
    case BUY_TO_OPEN:
      position->type = SELL_TO_OPEN;
      break;
    case SELL_TO_OPEN:
      position->type = BUY_TO_OPEN;
      break;
  }
}

void reduceAndTransition(Position * position, int quantity) {
  signed int quantity_delta = position->open_quantity - quantity;
  if (quantity_delta == 0) {
    position->type = NONE;
    position->open_quantity = 0;
  } else if (quantity_delta < 0) {
    flip(position);
    position->open_quantity = -quantity_delta;
  } else {
    position->open_quantity -= quantity;
  }
}

void transition(Position * position, Transaction transaction, int quantity) {
  switch (position->type) {
    case NONE:
      if (transaction == BUY){ 
        position->type = BUY_TO_OPEN;
      } else {
        position->type = SELL_TO_OPEN;
      }
      position->open_quantity = quantity;
      break;
    case BUY_TO_OPEN:
      if (transaction == SELL) {
        reduceAndTransition(position, quantity);
      } else {
        position->open_quantity += quantity;
      }
      break;
    case SELL_TO_OPEN:
      if (transaction == BUY) {
        reduceAndTransition(position, quantity);
      } else {
        position->open_quantity += quantity;
      }
      break;
  }
}


void scenario_1(Position * position) {
  puts("Scenario 1: ");
  puts("------------\n");

  transition(position, BUY, 10);
  print_position(position);

  transition(position, BUY, 10);
  print_position(position);

  transition(position, SELL, 10);
  print_position(position);

  transition(position, SELL, 10);
  print_position(position);

  transition(position, SELL, 10);
  print_position(position);
}

void scenario_2(Position * position) {
  puts("Scenario 2: ");
  puts("------------\n");

  transition(position, SELL, 10);
  print_position(position);

  transition(position, SELL, 10);
  print_position(position);

  transition(position, BUY, 10);
  print_position(position);

  transition(position, BUY, 10);
  print_position(position);

  transition(position, BUY, 10);
  print_position(position);
}

int main() {
  Position * position = new_position("test");
  scenario_1(position);
  free(position);

  position = new_position("test");
  scenario_2(position);
  free(position);

  return 0;
}
