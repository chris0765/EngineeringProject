void setup() {
  Serial.begin(115200);
  pinMode(7,OUTPUT);
  while (!Serial) {
    ;
  }
}

void loop() {
  int now_period=set_period(100);
  for(double i=0; i<=100; i+=(double(now_period)/5000))
  {
    digitalWrite(7,0);
    delayMicroseconds(int(now_period*set_duty(i)));
    digitalWrite(7,1);
    delayMicroseconds(int(now_period*set_duty(100-i)));
  }
  for(double i=100; i>=0; i-=(double(now_period)/5000))
  {
    digitalWrite(7,0);
    delayMicroseconds(int(now_period*set_duty(i)));
    digitalWrite(7,1);
    delayMicroseconds(int(now_period*set_duty(100-i)));
    //Serial.println(i);
  }
}

int set_period(int period){
  return period;
}

double set_duty(double duty) {
  return duty/100.0;
}
