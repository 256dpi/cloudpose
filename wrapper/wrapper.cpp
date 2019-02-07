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

op::Array<float> process(const std::string& filename) {
  // create new input list
  auto inputList = std::make_shared<std::vector<std::shared_ptr<UserDatum>>>();

  // create one space in list
  inputList->emplace_back();

  // get constructed input
  auto& input = inputList->at(0);

  // create new input
  input = std::make_shared<UserDatum>();

  // fill input with image data
  input->cvInputData = cv::imread(filename);

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

  return outputList->at(0)->poseKeypoints;
}

void stop() {
  // stop wrapper
  wrapper->stop();
}
