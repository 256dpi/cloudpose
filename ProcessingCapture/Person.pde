class Person {
  static final int Nose = 0;
  static final int Neck = 1;
  static final int RShoulder = 2;
  static final int RElbow = 3;
  static final int RWrist = 4;
  static final int LShoulder = 5;
  static final int LElbow = 6;
  static final int LWrist = 7;
  static final int MidHip = 8;
  static final int RHip = 9;
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

  PVector[] points;

  boolean available(int part) {
    return part >= 0 && part < 25 && points[part].x > 0 && points[part].y > 0;
  }

  void drawLine(int from, int to) {
    if (available(from) && available(to)) {
      PVector a = points[from];
      PVector b = points[to];
      line(a.x, a.y, b.x, b.y);
    }
  }

  void draw() {
    // set style
    stroke(255, 0, 0);
    strokeWeight(2);

    // draw lines
    drawLine(Nose, Neck);
    drawLine(Neck, RShoulder);
    drawLine(RShoulder, RElbow);
    drawLine(RElbow, RWrist);
    drawLine(Neck, LShoulder);
    drawLine(LShoulder, LElbow);
    drawLine(LElbow, LWrist);
    drawLine(Neck, MidHip);
    drawLine(MidHip, RHip);
    drawLine(RHip, RKnee);
    drawLine(RKnee, RAnkle);
    drawLine(MidHip, LHip);
    drawLine(LHip, LKnee);
    drawLine(LKnee, LAnkle);
    drawLine(Nose, REye);
    drawLine(REye, REar);
    drawLine(Nose, LEye);
    drawLine(LEye, LEar);
    drawLine(LAnkle, LBigToe);
    drawLine(LAnkle, LBigToe);
    drawLine(LAnkle, LHeel);
    drawLine(LBigToe, LSmallToe);
    drawLine(RAnkle, RBigToe);
    drawLine(RAnkle, RBigToe);
    drawLine(RAnkle, RHeel);
    drawLine(RBigToe, RSmallToe);
  }
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
