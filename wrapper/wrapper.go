package wrapper

/*
#cgo CXXFLAGS: --std=c++11 -DCGO
#cgo darwin pkg-config: opencv4
#cgo linux pkg-config: opencv
#cgo LDFLAGS: -lopenpose

#include <stdlib.h>
#include "wrapper.h"
*/
import "C"
import (
	"sync"
	"unsafe"
)

var mutex sync.Mutex

var started bool

// Person is a single detected person.
type Person struct {
	Points [25]Point `json:"points"`
}

// Point is a single detected point.
type Point struct {
	X float32 `json:"x"`
	Y float32 `json:"y"`
}

// Start will start openpose.
func Start() {
	mutex.Lock()
	defer mutex.Unlock()

	// check if already started
	if started {
		return
	}

	// start wrapper
	C.start()

	// set flag
	started = true
}

// Process will either load the provided file and process it or process the
// specified image from the buffer directly.
func Process(file string, data []byte) []Person {
	mutex.Lock()
	defer mutex.Unlock()

	// process image
	var res _Ctype_result_t
	if file != "" {
		cFile := C.CString(file)
		res = C.process(cFile, nil, 0)
		C.free(unsafe.Pointer(cFile))
	} else {
		res = C.process(nil, unsafe.Pointer(&data[0]), C.size_t(len(data)))
	}

	// prepare people
	people := make([]Person, int(res.num))

	// go through results
	for i := 0; i < len(people); i++ {
		// get c person
		cPersonStart := unsafe.Pointer(res.people)
		cPersonSize := unsafe.Sizeof(_Ctype_person_t{})
		cPerson := (*_Ctype_person_t)(unsafe.Pointer(uintptr(cPersonStart) + cPersonSize*uintptr(i)))

		// iterate through points
		for j := 0; j < 25; j++ {
			// get c point
			cPoint := (*_Ctype_point_t)(unsafe.Pointer(&cPerson.points[j]))

			// set point
			people[i].Points[j] = Point{
				X: float32(cPoint.x),
				Y: float32(cPoint.y),
			}
		}
	}

	// release result
	C.release(&res)

	return people
}

// Stop will stop openpose.
func Stop() {
	mutex.Lock()
	defer mutex.Unlock()

	// check if not already started
	if !started {
		return
	}

	// stop wrapper
	C.stop()

	// set flag
	started = false
}
