import processing.core.*; 
import processing.data.*; 
import processing.event.*; 
import processing.opengl.*; 

import processing.serial.*; 

import java.util.HashMap; 
import java.util.ArrayList; 
import java.io.File; 
import java.io.BufferedReader; 
import java.io.PrintWriter; 
import java.io.InputStream; 
import java.io.OutputStream; 
import java.io.IOException; 

public class convertToXML extends PApplet {



Serial myPort;  
char input [] = new char[100];

String a;
String b;
String c;

public void setup() {
  
  printArray(Serial.list());
  myPort = new Serial(this, Serial.list()[5], 9600);
}

public void draw() {
  int i = 0;
  while (myPort.available() > 0) {
    char inByte = myPort.readChar();
    input[i]=inByte;
    println(inByte);
    i++;
  }

  filter();
  printArray(input);
  println("a");
  println(a);
  println("b");
  println(b);
  println("c");
  println(c);
  stop();
}

public void filter ()
{
  for(int i = 0; i<100; i++)
  {
    switch(input[i])
    {
      case 'a':
      {
        int d = i+1;
        while(input[d]!='.')
        {
          a += str(input[d]);
          
        }
      }
      case 'b':
      {
        int d = i+1;
        while(input[d]!='.')
        {
          b += str(input[d]);
          
        }
      }
      case 'c':
      {
        int d = i+1;
        while(input[d]!='.')
        {
          c += str(input[d]);
          
        }
      }
    }
  }
}
  public void settings() {  size(400,400); }
  static public void main(String[] passedArgs) {
    String[] appletArgs = new String[] { "convertToXML" };
    if (passedArgs != null) {
      PApplet.main(concat(appletArgs, passedArgs));
    } else {
      PApplet.main(appletArgs);
    }
  }
}
