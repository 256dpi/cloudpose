package wrapper

/*
#cgo CXXFLAGS: --std=c++11 -DCGO
#cgo pkg-config: opencv4
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

type Person struct {
	Points [25]Point
}

type Point struct {
	X, Y float32
}

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

func Process(file string) []Person {
	// convert file
	cFile := C.CString(file)

	// process image
	res := C.process(cFile)

	// prepare slice
	var people []Person

	// go through results
	for i := 0; i < int(res.num); i++ {
		// prepare person
		var person Person

		// get c person
		cPersonStart := unsafe.Pointer(res.people)
		cPersonSize := unsafe.Sizeof(_Ctype_person_t{})
		cPerson := (*_Ctype_person_t)(unsafe.Pointer(uintptr(cPersonStart) + cPersonSize*uintptr(i)))

		for j := 0; j < 25; j++ {
			// get c point
			cPoint := (*_Ctype_point_t)(unsafe.Pointer(&cPerson.points[j]))

			person.Points[j] = Point{
				X:     float32(cPoint.x),
				Y:     float32(cPoint.y),
			}
		}

		// add person
		people = append(people, person)
	}

	// release result
	C.release(&res)

	// release file
	C.free(unsafe.Pointer(cFile))

	return people
}

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
