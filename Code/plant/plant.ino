// *********** Library Include Files ***********
#include <SoftwareSerial.h>


// *********** Pin settings ***********
#define MOISTURE_SENSOR_PIN A6
#define TX_PIN               6        // Connect to BT Rx 5
#define RX_PIN               4        // Connect to BT Tx 3
#define PUMP_CONTROLLER_PIN 12
#define LED_PIN              8

// *********** CONTROL CONSTANTS ***********
#define PUMP_ON 1
#define PUMP_OFF 2
#define PUMP_PULSE 3
#define PUMP_RESET_TIMEOUT 7
#define CHECK_MOISTURE 4
#define BUMP_LIMIT_UP 5
#define BUMP_LIMIT_DOWN 6

// ********** CUSTOMIZATION VALUES *********
// Moisture control values
#define INITIAL_MOISTURE_TRIGGER 500    // Set initial trigger v. high
#define LOWER_MOISTURE_LIMIT  100       
#define UPPER_MOISTURE_LIMIT 1050       // Max dry is usually 1024 

// Pump values
#define PUMP_PULSE_TIME_MS 1500       // Pulse for 1.5 sec
#define PUMP_BURNOUT_TIME_MS 10000    // Don't let pump run longer than 10 sec

// Control loop values
#define POST_TRIGGER_PAUSE_SEC 300    // Pause 5 min after last pump stop 
                                      // before auto trigger (let moisture settle)


// *********** Class Headers ***********
class MoistureSensor {
  private:
  int moist_sensor_pin ;
  int moist_level ;
  int moist_limit ;
  const int moist_limit_upper_cap = UPPER_MOISTURE_LIMIT ;
  const int moist_limit_lower_cap = LOWER_MOISTURE_LIMIT ;

  public:
  MoistureSensor(int pin, int limit) ;
  void setup() ;
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
  unsigned long last_millis_ms ;    // used in case of millis rollover
  bool pump_on ;
  bool timeout_triggered ; 

  unsigned long elapsed_start_time_ms ;
  unsigned long run_time_ms ;
  unsigned int run_count ;
  unsigned int rollover_count ;

  unsigned long safeMillis() ;

  public:
  Pump(int p, int l, unsigned long, unsigned long b_ms) ;

  unsigned long getTimeSinceLastStopSec() ;
  unsigned long getElapsedTimeSec() ;
  unsigned long getRunTimeSec() ;
  unsigned int getRunCount() ;
  unsigned int getRolloverCount() ;
  bool isPumpRunning() ;
  bool isTimeoutFlagSet() ;
 
  void setup() ;
  void reset() ;
  void pumpOn() ;
  void pumpOff() ;
  void pumpPulse(unsigned long override_pulse_ms = 0) ;  // Pulse pump on/off
  bool pumpCheck() ;                            // Turn off before burnout
} ;


// *********** Globals ***********
MoistureSensor moisture(MOISTURE_SENSOR_PIN, INITIAL_MOISTURE_TRIGGER) ;
Pump pump(PUMP_CONTROLLER_PIN, LED_PIN, PUMP_PULSE_TIME_MS, PUMP_BURNOUT_TIME_MS) ;
SoftwareSerial btSerial(RX_PIN, TX_PIN) ;
byte pinState = 0 ;
int buffer ;                    // buffer for serial port



// *********** Setup ***********
void setup()
{
  // Setup pump object
  pump.setup() ;
 
  // Moisture sensor object
  moisture.setup() ;

  // Serial port object
  pinMode(RX_PIN, INPUT) ;
  pinMode(TX_PIN, OUTPUT) ;
  btSerial.begin(9600);                 // open serial port, set dr

  // All done
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
  if (pump.pumpCheck())    {             // Burnout protection
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
        if (pump.isTimeoutFlagSet())
          btSerial.println("*** Pump is in timeout mode ***") ;  
        else { 
          btSerial.println("Pump ON") ;
          pump.pumpOn() ;
        }
        break ;

      case PUMP_OFF :
        btSerial.println("Pump OFF") ;
        pump.pumpOff() ;
        break ;

      case PUMP_PULSE :
        if (pump.isTimeoutFlagSet())
          btSerial.println("*** Pump is in timeout mode ***") ;  
        else { 
          btSerial.println("Pump PULSE") ;
          pump.pumpPulse() ;
        }
        break ;

      case PUMP_RESET_TIMEOUT :
        btSerial.println("Pump RESET") ;
        pump.reset() ;
        break ;
        
      case CHECK_MOISTURE :
        unsigned long lastStopMS ;
        btSerial.println("----------------------") ;
        ml = moisture.getMoistLevel() ;
        btSerial.print("Moisture Level: ") ;
        btSerial.println(ml);
        btSerial.print("Moisture Limit: ");
        btSerial.println(moisture.getLimit());
        btSerial.println(" ") ;
        btSerial.print("Time since start/reset: ") ;
        btSerial.print(pump.getElapsedTimeSec()/60) ;
        btSerial.print(" min and ") ;
        btSerial.print(pump.getRolloverCount()) ;
        btSerial.println(" clock rollovers") ;
        btSerial.print("# times pump has run: ") ;
        btSerial.println(pump.getRunCount()) ;
        btSerial.print("Pump running time: ") ;
        btSerial.print(pump.getRunTimeSec()) ;
        btSerial.println(" sec") ;
        btSerial.print("Pump time since last stop: ") ;
        lastStopMS = pump.getTimeSinceLastStopSec() ;
        btSerial.print(lastStopMS) ;
        btSerial.println(" sec") ;
        if (pump.isPumpRunning())
          btSerial.println("*** Pump is running ***") ;  
        if (pump.isTimeoutFlagSet())
          btSerial.println("*** Pump is in timeout mode ***") ;  
        if (lastStopMS < POST_TRIGGER_PAUSE_SEC ) {
          btSerial.print("*** Moisture sensor offline for ") ;      
          btSerial.print (POST_TRIGGER_PAUSE_SEC - lastStopMS) ;
          btSerial.println (" more sec ***") ;
        }
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
  //    - AND the pump is not currently running
  //    - AND has not run in the last 15 sec (so water has time to distribute)
  if (  moisture.tooDry()    ) {
  
    if ( pump.getTimeSinceLastStopSec() > POST_TRIGGER_PAUSE_SEC ) {
      int ml = moisture.getMoistLevel() ;
      btSerial.println("----------------------") ;
      btSerial.println("Too Dry Triggered... ");
      btSerial.print("Moisture Level: ");
      btSerial.println(ml);
      btSerial.print("Moisture Limit: ");
      btSerial.println(moisture.getLimit());
      btSerial.print("Pump time since last stop: ") ;
      btSerial.print(pump.getTimeSinceLastStopSec()) ;
      btSerial.println(" sec") ;
      btSerial.println("----------------------") ;
      pump.pumpPulse() ;
    }
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
}
void MoistureSensor::setup() {
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
}

// Gracefully handle millis() rollover
unsigned long Pump::safeMillis() {
   unsigned long current_ms = millis() ;
   if (current_ms < last_millis_ms) {
      pump_start_ms = pump_stop_ms = elapsed_start_time_ms = last_millis_ms = current_ms ;
      rollover_count ++ ;
   }
   else  {
     last_millis_ms = current_ms ;
     return last_millis_ms ;
   }
}

bool Pump::isTimeoutFlagSet() {
   return timeout_triggered ;
}

unsigned long Pump::getElapsedTimeSec() {
  return ((safeMillis() - elapsed_start_time_ms)/1000) ;
}

unsigned long Pump::getRunTimeSec() {
  return (run_time_ms / 1000) ;
}

unsigned int Pump::getRunCount() {
   return run_count ;
}
unsigned int Pump::getRolloverCount() {
   return rollover_count ;
}
bool Pump::isPumpRunning() {  // Returns true if pump is currently running
    return pump_on ;
}
unsigned long Pump::getTimeSinceLastStopSec() {
  return ((safeMillis() - pump_stop_ms)/1000) ;
}
void Pump::pumpOff() {
  // Whether pump running flag is on or not, force pump stop
  // and reset trigger flag
  digitalWrite(pump_pin,0) ;
  digitalWrite(led_pin,0) ;
  timeout_triggered = false ;   // reset timeout flag
  if (pump_on) {
    pump_stop_ms = safeMillis() ;     // log when pump was stopped
    run_time_ms += (pump_stop_ms - pump_start_ms) ;
    pump_on = false ;
  }
}

void Pump::setup() {

  pinMode(pump_pin, OUTPUT) ;
  pinMode(led_pin, OUTPUT) ;
  digitalWrite(pump_pin,0) ;
  digitalWrite(led_pin,0) ;

  pump_start_ms = pump_stop_ms = elapsed_start_time_ms = last_millis_ms = millis() ;
  run_time_ms = 0 ;
  run_count = 0 ;
  rollover_count = 0 ;

  pump_start_ms = pump_stop_ms = 0 ;
  pump_on = false ;
  timeout_triggered = false ;
  
}

void Pump::pumpOn() {
  if (! pump_on && ! timeout_triggered) {    // No pump, if it's already on or in timeout
    digitalWrite(pump_pin, 1) ;
    digitalWrite(led_pin,1) ;
    pump_on = true ;
    run_count++ ;

    unsigned long time_since_last_stop = safeMillis() - pump_stop_ms ;
    if (time_since_last_stop > 100)          // If sufficient pause since last cycle
      pump_start_ms = safeMillis() ;             // reset the start counter
    else                                     // OW, don't reset and treat as one long run
      run_time_ms -= (safeMillis() - pump_start_ms);  // But adjust runtime clock.
  }
}
void Pump::pumpPulse(unsigned long override_pulse_ms)  {
  unsigned long p ;
  if (override_pulse_ms == 0) p = pulse_time_ms ;
  else p = override_pulse_ms ;

  if (! timeout_triggered) {
    pumpOn() ;
    delay(p) ;
    if (! pumpCheck() )
      pumpOff() ;
  }
}
// if pump is on and has been running more than "burnout" minutes
// turn it off
bool Pump::pumpCheck() {
  if (! timeout_triggered && pump_on && ((safeMillis() - pump_start_ms) >= burnout_time_ms)) {
    pumpOff() ;
    timeout_triggered = true ;
    return (true) ;
  }
  else
    return false ;
}
void Pump::reset() {        // Pump Off does all the functions needed.
  // reset statistics and turn off pump
  // (Turning off pump will reset the timeout flag.
  pumpOff() ;
  elapsed_start_time_ms = safeMillis() ;
  run_time_ms = 0 ;
  run_count = 0 ;
}
