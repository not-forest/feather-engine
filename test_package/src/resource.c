#include "feather.h"
#include "res.h"
#include <stdint.h>

/* Here we define a new data type, that we wish to use. */
typedef struct {
    uint8_t hp;
    uint8_t speed;
} tCharacter;

/* Here we define a constructor that will be used during initialization phase. */
tCharacter constructor(void) {
    return (tCharacter){ .hp = 0, .speed = 0 };
}

/* If resource does some dynamic allocations, additional destructor can be implemented. (optional) */ 
//void destructor(tCharacter *data) {};

// Marking the structure as a resource. Here the destructor is not required, since no allocations are made.
FeatherResource(tCharacter, constructor, NULL);
