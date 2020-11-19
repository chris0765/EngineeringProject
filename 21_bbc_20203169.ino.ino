#include <Servo.h>

// Arduino pin assignment
#define PIN_SERVO 10
#define PIN_IR A0

// configurable parameters
#define _DUTY_MIN 553 // servo full clockwise position (0 degree)
#define _DUTY_NEU 1476 // servo neutral position (90 degree)
#define _DUTY_MAX 2399 // servo full counterclockwise position (180 degree)

#define _DUTY_D 1440
#define _DUTY_E 1850
#define _DUTY_F 1000

#define _SERVO_SPEED 3 // servo speed limit (unit: degree/second)
#define INTERVAL 20  // servo update interval

// global variables
unsigned long last_sampling_time; // unit: ms
double duty_chg_per_interval; // maximum duty difference per interval
int toggle_interval, toggle_interval_cnt;
float pause_time; // unit: sec
unsigned long mov_time;
Servo myservo;
double duty_target, duty_curr;
int a, b; // unit: mm

void setup() {
// initialize GPIO pins
  myservo.attach(PIN_SERVO); 
  duty_target = duty_curr = _DUTY_D;
  myservo.writeMicroseconds(_DUTY_NEU);
  
// initialize serial port
  Serial.begin(57600);

// convert angle speed into duty change per interval.
  duty_chg_per_interval = (_DUTY_MAX - _DUTY_MIN) * (_SERVO_SPEED / 180.0) * (INTERVAL / 1000.0);

// initialize variables for servo update.
  pause_time = 1;
  toggle_interval = (180.0 / _SERVO_SPEED + pause_time) * 1000 / INTERVAL;
  toggle_interval_cnt = toggle_interval;
  
// initialize last sampling time
  last_sampling_time = 0;

  mov_time=0;
  a = 98;
  b = 960;
}

float ir_distance(void){ // return value unit: mm
  float val;
  float volt = float(analogRead(PIN_IR));
  val = ((6762.0/(volt-9.0))-4.0) * 10.0;
  return val;
}

void loop() {
  // wait until next sampling time. 
  // millis() returns the number of milliseconds since the program started. Will overflow after 50 days.
  if(millis() < last_sampling_time + INTERVAL) return;
  
  float raw_dist = ir_distance();
  float dist_cali = 100 + 300.0 / (b - a) * (raw_dist - a);
  if(dist_cali<255){
    duty_target = _DUTY_E;
  }
  else if(dist_cali>255){
    duty_target = _DUTY_F;
  }
  if(duty_target > duty_curr) {
    duty_curr += duty_chg_per_interval;
    if(duty_curr > duty_target) duty_curr = duty_target;
  }
  else {
    duty_curr -= duty_chg_per_interval;
    if(duty_curr < duty_target) duty_curr = duty_target;
  }
  
  myservo.writeMicroseconds(duty_curr);
}
