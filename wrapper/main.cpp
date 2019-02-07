#include "wrapper.cpp"

int main(int argc, char *argv[]){
  // set log level
  op::ConfigureLog::setPriorityThreshold(op::Priority::Normal);

  // start
  op::log("starting...", op::Priority::High);
  start();
  op::log("started!", op::Priority::High);

  // process frames
  for (int i=0; i<100; i++) {
    op::log("processing...", op::Priority::High);
    const auto timer = op::getTimerInit();
    process();
    op::log("finished!", op::Priority::High);
    op::printTime(timer, "Total time: ", " seconds.", op::Priority::High);
  }

  // stop
  op::log("stopping...", op::Priority::High);
  stop();
  op::log("stopped!", op::Priority::High);

  return 0;
}
