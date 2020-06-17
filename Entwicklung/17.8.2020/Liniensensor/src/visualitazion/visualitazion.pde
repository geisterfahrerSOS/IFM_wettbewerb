import processing.serial.*;
Serial myPort;
String portStream;
boolean paused = false;


ArrayList<Integer> data = new ArrayList<Integer>(); 


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
    int max = 0;
    for(int i = 0; i < data.size(); i++)
    {
      if(max < data.get(i))
      {
        max = data.get(i);
      }
    }
    for(int i = 0; i < data.size(); i++)
    {
      println(data.get(i));
      fill(0);
      rect(150 + i * 50 - 25, 100, 50, map(data.get(i), 0, max, 0, 500));
      textSize(20);
      fill(255);
      text(data.get(i), 150 + i * 50-20,  int(map(data.get(i), 0, max, 0, 500) + 90));
    }
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
    portStream = tmp;
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
