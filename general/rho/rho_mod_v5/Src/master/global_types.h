#ifndef global_types_h
#define global_types_h

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

typedef struct
{
  uint8_t Address;
  uint8_t Value;
} register_t;

typedef struct
{
  uint32_t * port;
  uint8_t pin;
} gpio_t;

#endif /* global_types_h */