#include "wrapper.h"

int main(int argc, char *argv[]){
  // set log level
  op::ConfigureLog::setPriorityThreshold(op::Priority::Normal);

  // start
  op::log("starting...", op::Priority::High);
  start();
  op::log("started!", op::Priority::High);

  // process frames
  for (int i=0; i<100; i++) {
    // start timer
    op::log("processing...", op::Priority::High);
    const auto timer = op::getTimerInit();

    // process image
    result_t result = process("./photo.jpg");

    // print result
    for(size_t person = 0; person < result.num; person++) {
      op::log("person " + std::to_string(person) + " (x, y, score):");
      for (size_t point = 0; point < 25; point++) {
        std::string print = "  " + std::to_string(point) + " (";
        print += std::to_string(result.people[person].points[point].x) + ", ";
        print += std::to_string(result.people[person].points[point].y) + ", ";
        print += std::to_string(result.people[person].points[point].score) + ")";
        op::log(print);
      }
    }

    // release result
    release(&result);

    // finish timer
    op::log("finished!", op::Priority::High);
    op::printTime(timer, "total time: ", " seconds.", op::Priority::High);
  }

  // stop
  op::log("stopping...", op::Priority::High);
  stop();
  op::log("stopped!", op::Priority::High);

  return 0;
}
