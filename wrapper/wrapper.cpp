#include "wrapper.h"

struct UserDatum : public op::Datum{
  bool someBool;
  UserDatum(const bool _someBool = false) : someBool{_someBool} {}
};

// declare wrapper
op::WrapperT<UserDatum> * wrapper;

void start() {
  // set log level
  op::ConfigureLog::setPriorityThreshold(op::Priority::NoOutput);

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

result_t process(const char * file, void * buf, size_t len) {
  // create new input list
  auto inputList = std::make_shared<std::vector<std::shared_ptr<UserDatum>>>();

  // create one space in list
  inputList->emplace_back();

  // get constructed input
  auto& input = inputList->at(0);

  // create new input
  input = std::make_shared<UserDatum>();

  // fill input with image data
  if (file != nullptr && strlen(file) > 0) {
    input->cvInputData = cv::imread(std::string(file));
    if (input->cvInputData.data == nullptr) {
      throw std::domain_error("failed to read image");
    }
  } else {
    std::vector<char> vec((char*)buf, (char*)buf + len);
    input->cvInputData = cv::imdecode(vec, cv::IMREAD_COLOR);
    if (input->cvInputData.data == nullptr) {
      throw std::domain_error("failed to decode image");
    }
  }

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

  // get keypoints
  auto &keypoints = outputList->at(0)->poseKeypoints;

  // prepare result
  result_t result = {.num = (size_t)keypoints.getSize(0)};

  // allocate persons and points
  result.people = (person_t *)calloc(result.num, sizeof(person_t));

  // go through all persons
  for (auto person = 0; person < keypoints.getSize(0); person++) {
    // go through all body parts
    for (auto bodyPart = 0; bodyPart < keypoints.getSize(1); bodyPart++) {
      result.people[person].points[bodyPart].x = keypoints[{person, bodyPart, 0}];
      result.people[person].points[bodyPart].y = keypoints[{person, bodyPart, 1}];
    }
  }

  return result;
}

void release(result_t * result) {
  free(result->people);
}

void stop() {
  // stop wrapper
  wrapper->stop();
}
