import processing.serial.*;

Serial myPort;
String myString = null;

void setup()
{
  printArray(Serial.list());
  myPort = new Serial(this, "14201",9600);
}

void draw()
{
  if(myPort.available() > 0)
  {
    myString = myPort.readString();
    println(myString);
  }
}
