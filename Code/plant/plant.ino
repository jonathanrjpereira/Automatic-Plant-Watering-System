// *********** Library Include Files ***********
#include <SoftwareSerial.h>


// *********** Pin settings ***********
#define MOISTURE_SENSOR_PIN A0
#define TX_PIN               3
#define RX_PIN               2
#define PUMP_CONTROLLER_PIN 13
#define LED_PIN             11

// *********** CONTROL CONSTANTS ***********
#define PUMP_ON 1
#define PUMP_OFF 2
#define PUMP_PULSE 3
#define PUMP_RESET_TIMEOUT 7
#define CHECK_MOISTURE 4
#define BUMP_LIMIT_UP 5
#define BUMP_LIMIT_DOWN 6

// ********** CUSTOMIZATION VALUES *********
#define INITIAL_MOISTURE_TRIGGER 2000   // Set initial trigger v. high
#define LOWER_MOISTURE_LIMIT  100       
#define UPPER_MOISTURE_LIMIT 1000 

   

// *********** Class Headers ***********
class MoistureSensor {
  private:
  int moist_sensor_pin ;
  int moist_level ;
  int moist_limit ;
  const int moist_limit_upper_cap = UPPER_MOISTURE_LIMIT ;
  const int moist_limit_lower_cap = LOWER_MOISTURE_LIMIT ;

  public:
  MoistureSensor(int pin, int limit = INITIAL_MOISTURE_TRIGGER) ;
  bool tooDry() ;
  bool tooMoist() ;
  int setLimit(int l) ;
  int getLimit() ;
  int getMoistLevel() ;
  int increaseLimit(int i) ;
  int decreaseLimit(int d) ;

} ;

class Pump {
  private:
  int pump_pin;
  int led_pin ;
  unsigned long  burnout_time_ms ;
  unsigned long  pulse_time_ms ;
  unsigned long pump_start_ms ;
  unsigned long pump_stop_ms ;
  bool pump_on ;
  bool timeout_triggered ; 

  unsigned long elapsed_start_time_ms ;
  unsigned long run_time_ms ;
  unsigned int run_count ;

  public:
  Pump(int p, int l, unsigned long p_ms = 1000, unsigned long b_ms = 10000) ;

  unsigned long getElapsedTimeHr() ;
  unsigned long getRunTimeSec() ;
  unsigned int getRunCount() ;
  
  void PumpOn() ;
  void PumpOff() ;
  void PumpReset() ;
  void PumpPulse(unsigned long override_pulse_ms = 0) ;  // Pulse pump on/off
  bool PumpCheck() ;                            // Turn off before burnout
} ;


// *********** Globals ***********
MoistureSensor moisture(MOISTURE_SENSOR_PIN) ;
Pump pump(PUMP_CONTROLLER_PIN, LED_PIN) ;
SoftwareSerial btSerial(RX_PIN, TX_PIN) ;
byte pinState = 0 ;
int buffer ;                    // buffer for serial port



// *********** Setup ***********
void setup()
{
  // set up pins
  pinMode(RX_PIN, INPUT) ;
  pinMode(TX_PIN, OUTPUT) ;
  pinMode(LED_PIN, OUTPUT) ;

  // Set data rate for the SoftwareSerial port
  btSerial.begin(9600);                 // open serial port, set dr
  btSerial.println("Setup complete.");
}


// ***********  Loop ***********
// In each loop do *one* of he following:
//    1. If the pump has been running too long, go into timeout mode.
//    2. If there is a pending action, take that action.
//    3. If the soil is too dry, add water.
//
void loop()  {

  // 1. Check that pump has not been running itself too long
  if (pump.PumpCheck())    {             // Burnout protection
    btSerial.println("PumpCheck Timeout");
    return ;
  }


  // 2. Take action based on serial command
  int new_limit ;
  int command ;
  int ml ;

  command = readBluetooth() ;
  if(command > 0) {

    switch (command) {
      case PUMP_ON :
        btSerial.println("Pump ON") ;
        pump.PumpOn() ;
        break ;

      case PUMP_OFF :
        btSerial.println("Pump OFF") ;
        pump.PumpOff() ;
        break ;

      case PUMP_PULSE :
        btSerial.println("Pump PULSE") ;
        pump.PumpPulse() ;
        break ;

      case PUMP_RESET_TIMEOUT :
        btSerial.println("Pump RESET") ;
        pump.PumpReset() ;
        break ;
        
      case CHECK_MOISTURE :
        btSerial.println("----------------------") ;
        ml = moisture.getMoistLevel() ;
        btSerial.print("Moisture Level: ") ;
        btSerial.println(ml);
        btSerial.print("Moisture Limit: ");
        btSerial.println(moisture.getLimit());
        btSerial.println(" ") ;
        btSerial.print("Time since start/reset: ") ;
        btSerial.print(pump.getElapsedTimeHr()) ;
        btSerial.println(" hr") ;
        btSerial.print("# times pump has run: ") ;
        btSerial.println(pump.getRunCount()) ;
        btSerial.print("Pump running time: ") ;
        btSerial.print(pump.getRunTimeSec()) ;
        btSerial.println(" sec") ;
        btSerial.println("----------------------") ;
       break ;

      case BUMP_LIMIT_UP :
        new_limit = moisture.increaseLimit(10) ;
        btSerial.println("Increasing Moisture Limit");
        btSerial.print("New Moisture Limit: ");
        btSerial.println(new_limit);
        break ;

      case BUMP_LIMIT_DOWN :
        new_limit = moisture.decreaseLimit(10) ;
        btSerial.println("Decreasing Moisture Limit");
        btSerial.print("New Moisture Limit: ");
        btSerial.println(new_limit);
        break ;

      default :
        btSerial.print("Unknown command: ") ;
        btSerial.println(command) ;
    }
    return ;
  }

  // 3. Trigger pump if moisture level is low
  if (moisture.tooDry()) {
    int ml = moisture.getMoistLevel() ;
    btSerial.println("Too Dry Triggered... ");
    btSerial.print("Moisture Level: ");
    btSerial.println(ml);
    btSerial.print("Moisture Limit: ");
    btSerial.println(moisture.getLimit());
    pump.PumpPulse() ;

    return ;
  }


  return ;
}

// *********** Function Definitions **************
int readBluetooth()  {
    // send data only when you receive data:
    int cmd = 0 ;                   // Default if no data
    if(btSerial.available() > 0)     {
      int buf ;

      buf = btSerial.read();
//    btSerial.print("Bluetooth: ");
//    btSerial.println(buf);

      switch (buf) {
        case 49 :             // Ascii 1
          cmd = PUMP_ON ;
          break ;
        case 50 :             // Ascii 2
          cmd = PUMP_OFF ;
          break ;
        case 51 :             // Ascii 3
          cmd = PUMP_PULSE ;
          break ;
        case 55 :             // Ascii 7
          cmd = PUMP_RESET_TIMEOUT ;
          break ;
        case 52 :             // Ascii 4
          cmd = CHECK_MOISTURE ;
          break ;
        case 53 :             // Ascii 5
          return BUMP_LIMIT_UP ;
          break ;
        case 54 :             // Ascii 6
          return  BUMP_LIMIT_DOWN;
          break ;
        default :
          cmd = -1 ;
          break ;
        }
  }
  return cmd ;

}


// *********** Class Definitions **************

// MoistureSensor Class
MoistureSensor::MoistureSensor(int pin, int limit ) {
  moist_limit = limit ;
  moist_sensor_pin = pin ;
  pinMode(moist_sensor_pin, INPUT) ;
}
bool MoistureSensor::tooDry() {
  moist_level = analogRead(moist_sensor_pin) ;
  return (moist_level > moist_limit) ;
}
bool MoistureSensor::tooMoist() {
  moist_level = analogRead(moist_sensor_pin) ;
  return (moist_level <= moist_limit) ;
}
int MoistureSensor::getMoistLevel() {
  moist_level = analogRead(moist_sensor_pin) ;
  return moist_level ;
}
int MoistureSensor::setLimit(int new_limit) {

  int n = new_limit ;
  if (n > moist_limit_upper_cap)
    n = moist_limit_upper_cap ;
  else if (n < moist_limit_lower_cap)
    n = moist_limit_lower_cap ;

  moist_limit = n ;
  return moist_limit ;
}
int MoistureSensor::getLimit() {
  return moist_limit ;
}
int MoistureSensor::increaseLimit(int i) {
  return setLimit(moist_limit + i) ;
}
int MoistureSensor::decreaseLimit(int d) {
  return setLimit(moist_limit - d) ;
}

// Pump Class
Pump::Pump(int p_pin, int l_pin, unsigned long p_time_ms, unsigned long b_min ) {
  pump_pin = p_pin ;
  led_pin = l_pin ;
  pulse_time_ms = p_time_ms ;
  burnout_time_ms = b_min ;

  pinMode(pump_pin, OUTPUT) ;
  pinMode(led_pin, OUTPUT) ;
  digitalWrite(pump_pin,0) ;
  digitalWrite(led_pin,0) ;

  elapsed_start_time_ms = millis() ;
  run_time_ms = 0 ;
  run_count = 0 ;

  pump_start_ms = pump_stop_ms = 0 ;
  pump_on = false ;
  timeout_triggered = false ;
}

unsigned long Pump::getElapsedTimeHr() {
  return (millis() - elapsed_start_time_ms)/(1000*60*60) ;
}

unsigned long Pump::getRunTimeSec() {
  return (run_time_ms / 1000) ;
}

unsigned int Pump::getRunCount() {
   return run_count ;
}

void Pump::PumpOff() {
  digitalWrite(pump_pin,0) ;
  digitalWrite(led_pin,0) ;
  timeout_triggered = false ;   // reset timeout flag
  pump_stop_ms = millis() ;     // log when pump was stopped
  pump_on = false ;
  run_time_ms += (pump_stop_ms - pump_start_ms) ;
}
void Pump::PumpOn() {
  if (! pump_on && ! timeout_triggered) {    // No pump, if it's already on or in timeout
    digitalWrite(pump_pin, 1) ;
    digitalWrite(led_pin,1) ;
    pump_on = true ;
    run_count++ ;

    unsigned long time_since_last_stop = millis() - pump_stop_ms ;
    if (time_since_last_stop > 100)          // If sufficient pause since last cycle
      pump_start_ms = millis() ;             // reset the start counter
    else {                                   // OW, don't reset and treat as one long run
      run_time_ms -= (millis() - pump_start_ms);  // But adjust runtime clock.
      run_count-= 1 ;                        // And counter ;
    }
  }
}
void Pump::PumpPulse(unsigned long override_pulse_ms)  {
  unsigned long p ;
  if (override_pulse_ms == 0) p = pulse_time_ms ;
  else p = override_pulse_ms ;

  if (! timeout_triggered) {
    PumpOn() ;
    delay(p) ;
    if (! PumpCheck() )
      PumpOff() ;
  }
}
// if pump is on and has been running more than "burnout" minutes
// turn it off
bool Pump::PumpCheck() {
  if (! timeout_triggered && pump_on && ((millis() - pump_start_ms) >= burnout_time_ms)) {
    PumpOff() ;
    timeout_triggered = true ;
    return (true) ;
  }
  else
    return false ;
}
void Pump::PumpReset() {        // Pump Off does all the functions needed.
  // reset statistics and turn off pump
  // (Turning off pump will reset the timeout flag.
  PumpOff() ;
  elapsed_start_time_ms = millis() ;
  run_time_ms = 0 ;
  run_count = 0 ;
}
