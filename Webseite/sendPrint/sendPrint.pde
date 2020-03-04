import processing.serial.*;

Serial myPort; 

void setup() {
    size(400,400);
    myPort = new Serial(this, Serial.list()[4], 9600);
}

void draw() {
    myPort.write("a728364.b287635.c917387");
}
