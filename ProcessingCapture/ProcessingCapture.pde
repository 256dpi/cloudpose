import processing.net.*;
import processing.video.*;

Capture video;

Client client; 

Person[] people;

boolean sent = false;
long time;
long delay;

void setup() {
  size(640, 480);
  
  video = new Capture(this, width, height, 15);
  video.start();
  
  client = new Client(this, "cloudpose.joelgaehwiler.com", 1337); 
}

void draw() {
  // read video frame if available
  if (video.available()) {
    video.read();
  }
  
  // read client data if available
  if (client.available() > 0) {
    // read frame length
    int length = decodeInt(client.readBytes(8));
    
    // read frame
    people = decodeJSON(decodeString(client.readBytes(length)));
    
    // log
    println("received " + (8 + length));
    
    // set flag
    sent = false;
    delay = millis() - time;
  }
  
  // draw video image
  image(video, 0, 0);
  
  // draw people
  if (people != null) {
    for (Person person : people) {
      person.draw();
    }
  }
  
  // draw delay
  text(delay, 10, 20);
  
  if (!sent) {
    // encode image
    byte[] encoded = encodeJPG(video);
    
    // encode length
    byte[] length = encodeInt(encoded.length);
    
    // send data
    client.write(length);
    client.write(encoded);
    
    // log
    println("sent " + (8 + encoded.length));
    
    // set flag
    sent = true;
    time = millis();
  }
}
