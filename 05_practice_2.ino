int count = 0;

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(115200);
  pinMode(7,OUTPUT);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }
}

void loop() {
  digitalWrite(7,0);
  delay(1000);
  while(count++ < 5)
  {
    digitalWrite(7,1);
    delay(200);
    digitalWrite(7,0);
    delay(200);
  }
  digitalWrite(7,1);
  while(1){}
}
