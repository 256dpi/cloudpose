#include <openpose/headers.hpp>

struct UserDatum : public op::Datum{
  bool someBool;
  UserDatum(const bool _someBool = false) : someBool{_someBool} {}
};

// declare wrapper
op::WrapperT<UserDatum> * wrapper;

void start() {
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
}

void process() {
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

  // process list
  auto ok = wrapper->waitAndEmplace(inputList);
  if (!ok) {
    throw std::domain_error("failed wait and emplace");
  }

  // prepare output list
  std::shared_ptr<std::vector<std::shared_ptr<UserDatum>>> outputList;

  // get next output
  ok = wrapper->waitAndPop(outputList);
  if (!ok) {
    throw std::domain_error("failed wait and pop");
  }

//  // get keypoints
//  const auto &poseKeypoints = outputList->at(0)->poseKeypoints;
//
//  // iterate through all key points
//  for (auto person = 0; person < poseKeypoints.getSize(0); person++) {
//    op::log("Person " + std::to_string(person) + " (x, y, score):");
//
//    // iterate through body parts
//    for (auto bodyPart = 0; bodyPart < poseKeypoints.getSize(1); bodyPart++) {
//      std::string print;
//      for (auto score = 0; score < poseKeypoints.getSize(2); score++) {
//        print += std::to_string(poseKeypoints[{person, bodyPart, score}]) + " ";
//      }
//      op::log(print);
//    }
//  }
}

void stop() {
  // stop wrapper
  wrapper->stop();
}
