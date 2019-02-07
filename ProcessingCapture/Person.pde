class Person {
  static final int Nose =  0;
  static final int Neck =  1;
  static final int RShoulder =  2;
  static final int RElbow =  3;
  static final int RWrist =  4;
  static final int LShoulder =  5;
  static final int LElbow =  6;
  static final int LWrist =  7;
  static final int MidHip =  8;
  static final int RHip =  9;
  static final int RKnee = 10;
  static final int RAnkle = 11;
  static final int LHip = 12;
  static final int LKnee = 13;
  static final int LAnkle = 14;
  static final int REye = 15;
  static final int LEye = 16;
  static final int REar = 17;
  static final int LEar = 18;
  static final int LBigToe = 19;
  static final int LSmallToe = 20;
  static final int LHeel = 21;
  static final int RBigToe = 22;
  static final int RSmallToe = 23;
  static final int RHeel = 24;
  static final int Background = 25;

  PVector[] points;
}

void drawLine(PVector a, PVector b) {
  line(a.x, a.y, b.x, b.y);
}

void drawPerson(Person person) {
  // set style
  stroke(255, 0, 0);
  strokeWeight(2);

  drawLine(person.points[Person.Nose], person.points[Person.Neck]);
}

Person[] decodeJSON(String data) {  
  // parse json
  JSONArray array = parseJSONArray(data);
  if (array == null) {
    return null;
  }

  // prepare people
  Person[] people = new Person[array.size()];

  // go through array
  for (int i=0; i<array.size(); i++) {
    // get object
    JSONObject obj = array.getJSONObject(i);
    if (obj == null) {
      return null;
    }

    // get points
    JSONArray array2 = obj.getJSONArray("points");
    if (array2 == null) {
      return null;
    }

    // create person
    people[i] = new Person();

    // allocate vectors
    people[i].points = new PVector[25];

    // set points
    for (int j=0; j<array2.size(); j++) {
      // get object
      JSONObject obj2 = array2.getJSONObject(j);
      if (obj2 == null) {
        return null;
      }

      // set points
      people[i].points[j] = new PVector(obj2.getFloat("x"), obj2.getFloat("y"));
    }
  }

  return people;
}
