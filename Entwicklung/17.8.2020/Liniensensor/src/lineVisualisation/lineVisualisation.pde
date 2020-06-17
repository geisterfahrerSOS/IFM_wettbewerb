import processing.serial.*;
Serial myPort;
String portStream;
boolean paused = false;


/*

  Schwelle finden
  in 1 und 0 umwandeln
  Calibrierung on the fly
  dementscprechend bewegen
  Calibrierung ohne etwas

*/

int lColor;
float blackWhiteBorder = 35;

ArrayList<Integer> data = new ArrayList<Integer>(); 
ArrayList<Boolean> line = new ArrayList<Boolean>();


void setup()
{
  println(Serial.list());
  size(440, 662);
}

void draw()
{
  clear();
  background(255);
  stroke(0);
  fill(0);
  line(100, 100, 440, 100);
  line(100, 100, 100, 662);
  textSize(20);
  text("Line",  10, 120);
  text("Pos:",  10, 612);
  text("zwischen",  10, 632);
  text("0 und 5", 10, 652);
  text("1", 150, 70);
  text("2", 200, 70);  
  text("3", 250, 70);
  text("4", 300, 70);
  text("5", 350, 70);
  text("6", 400, 70);  
  
  if(data.size() != 0)
  {
    //////////////////////////////////////////
    for(int i = 0; i < data.size(); i++)
    {
      if(lColor == 1)
        line.add(data.get(i) >= blackWhiteBorder); 
      else
      {
        line.add((1023 - data.get(i)) >= blackWhiteBorder); 
      }
    }
    for(int i = 0; i < data.size(); i++)
    {
      if(line.get(i))
      {
        rect(150 + i * 50 - 25, 100, 50, 480);
      }
    }
    
    
    line.clear();
    
    /*
    int top = 0, bottom = 0;
    float sensor;
    for(int i = 0; i < data.size(); i++)
    {
      top += i * 1000 * data.get(i);
      bottom += data.get(i);
    }
    sensor = float(top) / float(bottom) / 1000.0;
    textSize(20);
    fill(150);
    float x = 150 + sensor * 50;
    rect(x - 25, 100, 50, 480);
    fill(0);
    text(sensor, x - 35, 612);
    */
    
    
    //////////////////////////////////////////
  }
  else if(!paused)
  //portStream == null
  {
    textSize(40);
    text("No Data", 200, 200);
  }
  
  if(!findPort())
  {
    textSize(40);
    if(paused)
      text("PAUSED", 200, 200);
    else
      text("PORT BUSY", 200, 250);      
  }
    
  if(portStream != null)
  {
    try
    {
      data.clear();
      String[] dataString = split(portStream, ";");            
      for(int i = 0; i < dataString.length - 1; i++)
      {
        data.add(int(dataString[i]));
      }
    }
    catch(Exception e)
    {
      println(e);
    }
  }
}

boolean findPort()
{
  if(paused)
  {
    if(myPort != null)
      myPort.stop();
    myPort = null;
    return false;
  }
  if(myPort != null)
  {
    return true;
  }
   for(int i = 0; i < Serial.list().length; i++)
   {  
     try
     {
        myPort = new Serial(this, Serial.list()[i], 9600);
        myPort.bufferUntil('\n'); 
     }
     catch(Exception e)
     {
     }
   }
   return myPort != null;
}

void serialEvent(Serial myPort)
{
  String tmp = myPort.readString();
  if(tmp.substring(0, 4).equals("SLS;") && tmp.substring(tmp.length() - 6, tmp.length() - 2).equals("ELS;"))
  {
    tmp = tmp.substring(4, tmp.length() - 6);
    if(!tmp.equals("BLACK;") && !tmp.equals("WHITE;") && !tmp.equals(""))
    {
      portStream = tmp;
    }
    else if(tmp.equals("BLACK;"))
    {
      lColor = 1;
    }
    else if(tmp.equals("WHITE;"))
    {
      lColor = 0;
    }
  }
}

void keyReleased()
{
   if(key =='P' || key =='p')
      if(paused)
        paused = false;
      else
        paused = true;
}
