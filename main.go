package main

import (
	"fmt"
	"io/ioutil"

	"github.com/256dpi/cloudpose/wrapper"
)

func main() {
	// start openpose
	fmt.Println("starting...")
	wrapper.Start()
	fmt.Println("started!")

	// read file
	data, err := ioutil.ReadFile("photo.jpg")
	if err != nil {
		panic(err)
	}

	// process image
	people := wrapper.Process("", data)
	fmt.Printf("%+v\n", people)

	// stop openpose
	fmt.Println("stopping...")
	wrapper.Stop()
	fmt.Println("stopped!")
}
