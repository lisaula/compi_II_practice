
namespace utility {

char *int2bin(int a, char *buffer, int buf_size) {
    buffer += (buf_size - 1);

    for (int i = 31; i >= 0; i--) {
        *buffer-- = (a & 1) + '0';

        a >>= 1;
    }

    return buffer;
}

void print_int2bin(int n){
  char buffer[BUF_SIZE];
  buffer[BUF_SIZE - 1] = '\0';
  int2bin(n, buffer, BUF_SIZE - 1);
  int printable = atoi(buffer);
  printf("Binary: %d\n", printable);
}

}
