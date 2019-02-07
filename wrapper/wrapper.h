#ifndef _WRAPPER_H_
#define _WRAPPER_H_

#ifdef __cplusplus
#include <openpose/headers.hpp>

extern "C" {
#endif

typedef struct {
  float x, y;
  float score;
} point_t;

typedef struct {
  point_t points[25];
} person_t;

typedef struct {
  size_t num;
  person_t * people;
} result_t;

void start();

result_t process(const char * filename);

void release(result_t * result);

void stop();

#ifdef __cplusplus
}
#endif

#endif //_WRAPPER_H_
