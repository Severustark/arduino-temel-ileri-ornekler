void setup() {
  // put your setup code here, to run once:
  pinMode(D6, OUTPUT);
  pinMode(D5,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(D5)==HIGH)
    digitalWrite(D6,HIGH);
  else
    digitalWrite(D6,LOW);
}
