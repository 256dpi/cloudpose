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
    // start timer
    op::log("processing...", op::Priority::High);
    const auto timer = op::getTimerInit();

    // process image
    auto keypoints = process("./photo.jpg");

    // print all keypoints
    for (auto person = 0; person < keypoints.getSize(0); person++) {
      op::log("person " + std::to_string(person) + " (x, y, score):");

      // iterate through body parts
      for (auto bodyPart = 0; bodyPart < keypoints.getSize(1); bodyPart++) {
        std::string print;
        for (auto score = 0; score < keypoints.getSize(2); score++) {
          print += std::to_string(keypoints[{person, bodyPart, score}]) + " ";
        }
        op::log(print);
      }
    }

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
