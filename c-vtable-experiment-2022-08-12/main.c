#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Placeholders
int no_op_int(void * reciever) { return 0; }
bool no_op_bool(void * reciever) { return false; }

typedef struct ThingA {
  // Different order of properties from ThingB
  char propery_1;
  int propery_a;
  char propery_2;
  int propery_b;
  char * other_do;
} ThingA;

typedef struct ThingB {
  // Different order of properties from Do
  char propery_a;
  int propery_1;
  char propery_b;
  int propery_2;
  long long other_dont;
} ThingB;

typedef struct Interface {
  int (* aggregate_property_1)(void *);
  int (* aggregate_property_2)(void *);
  bool (* matched_property_a)(void *);
  bool (* matches_property_b)(void *);
} Interface; 

typedef struct Box {
  struct Interface ops;
  void * self;
} Box;

int ops_aggregate_property_1(struct Box w) {
  return w.ops.aggregate_property_1(w.self);
}

int thingA_property_1(void * receiver) {
  ThingA * self = receiver;
  return 'a' - self->propery_1;
}

int thingB_property_1(void * receiver) {
  ThingB * self = receiver;
  return self->propery_1;
}

int main(void) {
  ThingA thingA = {'a', 1, 'b', 2, NULL};
  Interface thingADelegate = { thingA_property_1, no_op_int, no_op_bool, no_op_bool };
  Box boxThingA = { thingADelegate, &thingA};

  ThingB thingB = {1, 'a', 2, 'b', 0};
  Interface thingBDelegate = { thingB_property_1, no_op_int, no_op_bool, no_op_bool };
  Box boxThingB = { thingBDelegate, &thingB};

  printf("ThingA: 1: %d \n", ops_aggregate_property_1(boxThingA));
  printf("ThingB: 1: %d \n", ops_aggregate_property_1(boxThingB));
  return 0;
}
