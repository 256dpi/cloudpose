# wrapper

This is a basic OpenPose wrapper.

To get it compiled on OSX I needed to `brew install caffe` and run CMake as follows:

```bash
cmake -D BUILD_CAFFE=false -D Caffe_INCLUDE_DIRS=/usr/local/include/caffe -D Caffe_LIBS=/usr/local/lib/libcaffe.dylib ..
```
