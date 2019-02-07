#include "wrapper.h"

#ifndef CGO

int main(int argc, char *argv[]){
  // start
  start();

  // process image
  result_t result = process("photo.jpg", nullptr, 0);
  printf("detected people: %ld\n", result.num);

  // release result
  release(&result);

  // stop
  stop();

  return 0;
}

#endif /* CGO */
