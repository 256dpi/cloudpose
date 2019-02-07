package main

import (
	"bufio"
	"encoding/binary"
	"encoding/json"
	"io"
	"log"
	"net"
	"os"
	"os/signal"
	"syscall"

	"github.com/256dpi/cloudpose/wrapper"
)

func main() {
	// start openpose
	log.Println("starting...")
	wrapper.Start()
	log.Println("started!")

	// start listener
	l, err := net.Listen("tcp", "0.0.0.0:1337")
	if err != nil {
		log.Panic(err)
	}

	// run acceptor
	go accept(l)

	// wait for signal
	sig := make(chan os.Signal, 1)
	signal.Notify(sig, syscall.SIGTERM, syscall.SIGINT)
	<-sig

	// stop openpose
	log.Println("stopping...")
	wrapper.Stop()
	log.Println("stopped!")
}

func accept(l net.Listener) {
	// accept connection
	for {
		// wait for next connection
		conn, err := l.Accept()
		if err != nil {
			log.Panic(err)
		}

		// run handler
		go handle(conn)
	}
}

func handle(conn net.Conn) {
	log.Printf("new connection from %s", conn.RemoteAddr().String())

	// create buffers
	reader := bufio.NewReader(conn)
	writer := bufio.NewWriter(conn)

	for {
		// read frame length
		frameLengthBytes := make([]byte, 8)
		_, err := io.ReadFull(reader, frameLengthBytes)
		if err != nil {
			log.Println(err.Error())
			return
		}

		// parse frame length
		frameLength := binary.BigEndian.Uint64(frameLengthBytes)

		// read frame
		frameBytes := make([]byte, frameLength)
		_, err = io.ReadFull(reader, frameBytes)
		if err != nil {
			log.Println(err.Error())
			return
		}

		// log info
		log.Printf("received frame from %s", conn.RemoteAddr().String())

		// process image
		people := wrapper.Process("", frameBytes)
		// TODO: Error handling.

		// encode result
		data, err := json.Marshal(people)
		if err != nil {
			log.Println(err.Error())
			return
		}

		// write frame length
		binary.BigEndian.PutUint64(frameLengthBytes, uint64(len(data)))
		_, err = writer.Write(frameLengthBytes)
		if err != nil {
			log.Println(err.Error())
			return
		}

		// write frame
		_, err = writer.Write(data)
		if err != nil {
			log.Println(err.Error())
			return
		}

		// flush writer
		err = writer.Flush()
		if err != nil {
			log.Println(err.Error())
			return
		}

		log.Printf("sent frame to %s", conn.RemoteAddr().String())
	}
}
