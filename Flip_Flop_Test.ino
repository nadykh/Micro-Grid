double I=0;
double Iref = 1;
double b = 0.5;
double i;
bool S,R;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  S = I<=Iref-b;
  R = I>=Iref+b;
  if(S) i=0.1;
  if(R) i=-0.1;
  I+=i;
  Serial.println(I);
  delay(200);

}
