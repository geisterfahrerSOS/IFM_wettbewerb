import processing.serial.*;

Serial myPort;  
char input [] = new char[100];

String a;
String b;
String c;

void setup() {
  size(400,400);
  printArray(Serial.list());
  myPort = new Serial(this, Serial.list()[5], 9600);
}

void draw() {
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

void filter ()
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