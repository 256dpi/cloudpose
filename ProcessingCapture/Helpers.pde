import java.nio.*;
import java.nio.charset.*;
import java.io.*;
import java.awt.*;
import java.awt.image.*;
import javax.imageio.*;

byte[] encodeInt(int i) {
    final ByteBuffer bb = ByteBuffer.allocate(8);
    bb.order(ByteOrder.BIG_ENDIAN);
    bb.putLong(i);
    return bb.array();
}

int decodeInt(byte[] data) {
    final ByteBuffer bb = ByteBuffer.wrap(data);
    bb.order(ByteOrder.BIG_ENDIAN);
    return int(bb.getLong());
}

byte[] encodeJPG(PImage img) {
  try {
    BufferedImage image = new BufferedImage(img.width, img.height, BufferedImage.TYPE_INT_RGB);
    image.setRGB(0, 0, img.pixelWidth, img.pixelHeight, img.pixels, 0, img.pixelWidth);

    ByteArrayOutputStream baos = new ByteArrayOutputStream();
    ImageIO.write(image, "jpg", baos);
    baos.flush();
    byte[] bytes = baos.toByteArray();
    baos.close();

    return bytes;
  }
  catch(IOException e) {
    System.out.println(e.getMessage());
    return null;
  }
}

String decodeString(byte[] data) {
  return new String(data, StandardCharsets.UTF_8);
}
