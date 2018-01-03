int blue = 0;  // for incoming serial data
int moisture_sensor  = A0;
int moisture;
int limit = 500;
int pump = 3;
int led = 13;
int pump_time = 3000;
void setup()
{
  Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
  pinMode(moisture_sensor,INPUT);
  pinMode(pump,OUTPUT);
  pinMode(led,OUTPUT);
}

void loop() 
{
  moisture = analogRead(moisture_sensor);
  Serial.print("Moisture=");
  Serial.print(moisture);
  Serial.println();
  
  while(Bluetooth()==1)
  {
    Pump(1);
    blue = 0;
    break;
  }
  while(Bluetooth()==0)
  {
    if(moisture<limit)
    {
      Pump(1);
      break;  
    }
    else
    { 
      Pump(0);
      break;
    }
  }
}

int Bluetooth()
{
   // send data only when you receive data:
   if(Serial.available() > 0) 
   {
    // read the incoming byte:
    blue = Serial.read();

    // say what you got:
    Serial.print("Bluetooth");
    Serial.println(blue);
    if(blue == 48)
      return 0;
    if(blue == 49)
      return 1;
  }
  else 
  return 0;
}

void Pump(int stat)
{
  if(stat == 0)
  {
    digitalWrite(pump,stat);
    digitalWrite(led,stat);
    Serial.println("Pump OFF");
  }
  if(stat == 1)
  {
    digitalWrite(pump,stat);
    digitalWrite(led,stat);
    Serial.println("Pump ON");
    delay(pump_time);
  }
}

