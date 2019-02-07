#include <gflags/gflags.h>
#include <openpose/headers.hpp>

struct UserDatum : public op::Datum{
  bool someBool;
  UserDatum(const bool _someBool = false) : someBool{_someBool} {}
};

// declare wrapper
op::WrapperT<UserDatum> * wrapper;

void start() {
  op::log("starting...", op::Priority::High);

  // alloc wrapper
  wrapper = new op::WrapperT<UserDatum>{op::ThreadManagerMode::Asynchronous};

  // use default pose configuration
  op::WrapperStructPose pose{};
  pose.renderMode = op::RenderMode::None;
  pose.netInputSize = op::Point<int>{320, 176};
  wrapper->configure(pose);

  // disable face
  op::WrapperStructFace face{};
  face.renderMode = op::RenderMode::None;
  wrapper->configure(face);

  // disable hand
  op::WrapperStructHand hand{};
  hand.renderMode = op::RenderMode::None;
  wrapper->configure(hand);

  // disable extra
  op::WrapperStructExtra extra{};
  wrapper->configure(extra);

  // disable input
  op::WrapperStructInput input{};
  wrapper->configure(input);

  // disable output
  op::WrapperStructOutput output{};
  wrapper->configure(output);

  // disable gui
  op::WrapperStructGui gui{};
  wrapper->configure(gui);

  // start openpose
  wrapper->start();

  op::log("started!", op::Priority::High);
}

void process() {
  op::log("processing...", op::Priority::High);
  const auto timer = op::getTimerInit();

  // create new input list
  auto inputList = std::make_shared<std::vector<std::shared_ptr<UserDatum>>>();

  // create one space in list
  inputList->emplace_back();

  // get constructed input
  auto& input = inputList->at(0);

  // create new input
  input = std::make_shared<UserDatum>();

  // fill input with image data
  input->cvInputData = cv::imread("./photo.jpg");

  op::log("emplacing...", op::Priority::High);

  // process list
  auto ok = wrapper->waitAndEmplace(inputList);
  if (!ok) {
    throw std::domain_error("failed wait and emplace");
  }

  // prepare output list
  std::shared_ptr<std::vector<std::shared_ptr<UserDatum>>> outputList;

  op::log("popping...", op::Priority::High);

  // get next output
  ok = wrapper->waitAndPop(outputList);
  if (!ok) {
    throw std::domain_error("failed wait and pop");
  }

  op::log("parsing...", op::Priority::High);

  // get keypoints
  const auto &poseKeypoints = outputList->at(0)->poseKeypoints;

  // iterate through all key points
  for (auto person = 0; person < poseKeypoints.getSize(0); person++) {
    op::log("Person " + std::to_string(person) + " (x, y, score):");

    // iterate through body parts
    for (auto bodyPart = 0; bodyPart < poseKeypoints.getSize(1); bodyPart++) {
      std::string print;
      for (auto score = 0; score < poseKeypoints.getSize(2); score++) {
        print += std::to_string(poseKeypoints[{person, bodyPart, score}]) + " ";
      }
      op::log(print);
    }
  }

  op::log("finished!", op::Priority::High);
  op::printTime(timer, "Total time: ", " seconds.", op::Priority::High);
}

void stop() {
  op::log("stopping...", op::Priority::High);

  // stop wrapper
  wrapper->stop();

  op::log("stopped!", op::Priority::High);
}

int main(int argc, char *argv[]){
  // set log level
  op::ConfigureLog::setPriorityThreshold(op::Priority::Normal);

  // start
  start();

  // run
  for (int i=0; i<100; i++) {
    process();
  }

  // stop
  stop();

  return 0;
}
