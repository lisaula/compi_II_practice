#define BUF_SIZE 33
#define BIN 0
#define HEX 1
#define DEC 2

#define EQUAL 3
#define NOT_EQUAL 4
#define GREATER_THAN 5
#define LESS_THAN 6
#define GREATER_OR_EQUAL 7
#define LESS_OR_EQUAL 8

#include <stdlib.h>
namespace utility {
  char *int2bin(int a, char *buffer, int buf_size);
  void print_int2bin(int n);
}
