#define A1M 5
#define A2M 3
#define B1M 6
#define B2M 4

#define ledL A1
#define ledR A2
#define ledF A3
#define btn 2

#define HA 200
#define HB 200

float spd = 13488.0 ;
float t;
float dis;
int out;
int c  = 0;
int swtch = -1;
int test = -1;
int dll = 600;
int trigL = 7, trigR = A0, trigF = 8;
int echoL = 11, echoR = 10, echoF = 9;
bool chk = true;

int count = 0;
char route[400];
char fin[50];

bool left;
bool right;
bool forward;

void forwardM(){
  analogWrite(A1M,0);
  analogWrite(A2M,HA);
  analogWrite(B1M,HB);
  analogWrite(B2M,0);
}

void leftM(){
  forwardM();
  delay(700);
  analogWrite(A1M,HA);
  analogWrite(A2M,0);
  analogWrite(B1M,HB);
  analogWrite(B2M,0);
  delay(dll);
  stopM();
  delay(1000);
  forwardM();
  delay(1350);
}

void rightM(){
  forwardM();
  delay(500);
  analogWrite(A1M,0);
  analogWrite(A2M,HA);
  analogWrite(B1M,0);
  analogWrite(B2M,HB);
  delay(dll-50);
  stopM();
  delay(1000);
  forwardM();
  delay(700);
}

void stopM(){
  analogWrite(A1M,0);
  analogWrite(A2M,0);
  analogWrite(B1M,0);
  analogWrite(B2M,0);
  delay(500);
}

void backwardM(){
  forwardM();
  delay(300);
  analogWrite(A1M,HA);
  analogWrite(A2M,0);
  analogWrite(B1M,HB);
  analogWrite(B2M,0);
  delay(dll*2);
  stopM();
  delay(1000);
  forwardM();
  delay(700);
}

void rightT() {
  analogWrite(ledL, 0);
  analogWrite(ledR, 255);
  analogWrite(ledF, 0);
  rightM();
}

void leftT() {
  analogWrite(ledL, 255);
  analogWrite(ledR, 0);
  analogWrite(ledF, 0);
  leftM();
}

void forwardT() {
  analogWrite(ledL, 0);
  analogWrite(ledR, 0);
  analogWrite(ledF, 255);
  forwardM();
  delay(400);
}

void backT() {
  analogWrite(ledL, 0);
  analogWrite(ledR, 0);
  analogWrite(ledF, 0);
  backwardM();
}

void stopT() {
  analogWrite(ledL, 255);
  analogWrite(ledR, 255);
  analogWrite(ledF, 255);
  stopM();
}

bool check(int trig, int echo, int low, int high) {
  digitalWrite(trig, LOW);
  delay(3);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  t = pulseIn(echo, HIGH);
  t = t / 1000000.0;
  dis = spd * t;
  dis = dis / 2.0;
  if ( dis >=low && dis <= high) {
    return false;
  }
  else {
    return true;
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(trigL, OUTPUT);
  pinMode(echoL, INPUT);

  pinMode(trigR, OUTPUT);
  pinMode(echoR, INPUT);

  pinMode(trigF, OUTPUT);
  pinMode(echoF, INPUT);

  pinMode(ledL, OUTPUT);
  pinMode(ledR, OUTPUT);
  pinMode(ledF, OUTPUT);

  pinMode(btn, INPUT_PULLUP);
}

void loop() {
  if(chk == true){
    delay(3000);
    chk = false;
  }
   
  left = check(trigL, echoL, 0.2, 6.0);
  forward = check(trigF, echoF, 0.2, 6.0);
  right = check(trigR, echoR, 0.2, 6.0);

  if (left == false && forward == false && right == false) {
    backT();
    if (swtch != 0) {
      route[count] = 'B';
      count += 1;
      swtch = 0;
      Serial.println("BACK");
    }
  }
  else if (left == false && forward == false && right == true) {
    rightT();
    if (swtch != 1) {
      route[count] = 'R';
      swtch = 1;
      count += 1;
      Serial.println("RIGHT");
    }
  }
  else if (left == false && forward == true && right == false) {
    forwardT();
    if (swtch != 2) {
      swtch = 2 ;
      Serial.println("FORWARD");
    }
  }
  else if ( left == false && forward == true && right == true) {
    forwardT();
    if (swtch != 3) {
      route[count] = 'F';
      count += 1;
      swtch = 3;
      Serial.println("FORWARD");
    }
  }
  else if ( left == true && forward == false && right == false) {
    leftT();
    if (swtch != 4) {
      route[count] = 'L';
      count += 1;
      swtch = 4 ;
      Serial.println("LEFT");
    }
  }
  else if ( left == true && forward == false && right == true) {
    leftT();
    if ( swtch != 5) {
      route[count] = 'L';
      count += 1;
      swtch = 5;
      Serial.println("LEFT");
    }
  }
  else if ( left == true && forward == true && right == false) {
    leftT();
    if (swtch != 6) {
      route[count] = 'L';
      count += 1;
      swtch = 6 ;
      Serial.println("LEFT");
    }
  }
  else if ( left == true && forward == true && right == true) {
    stopT();
    if ( swtch != 7 ) {
      swtch = 7;
      Serial.println("STOP");
     
    }
  }
  out = digitalRead(btn);

  if (out == 0) {
    Serial.println(route);
    for (int i = 2; i < count ; i++) {
      if ( route[i - 2] == 'L' && route[i - 1] == 'B' && route[i] == 'L' ) {
        i += 2 ;
        fin[c] = 'F';
        c += 1 ;
      }
      else if( route[i-2]=='F' and route[i-1]=='B' and route[i]=='L')
      {
          fin[c] = 'R';
          i += 2;
          c += 1;
      }
      else if( route[i-2]=='L' and route[i-1]=='B' and route[i]=='R')
      {  
          fin[c] = 'B';
          i += 2;
          c += 1;
      }
      else if( route[i-2]=='L' and route[i-1]=='B' and route[i]=='F')
      {  
          fin[c] = 'R';
          i += 2;
          c += 1;
      }
      else if( route[i-2]=='F' and route[i-1]=='B' and route[i]=='F')
      {
          fin[c] = 'B';
          i += 2;
          c += 1;
      }
      else if( route[i-2]=='R' and route[i-1]=='B' and route[i]=='L')
      {
          fin[c] = 'B';
          i += 2;
          c += 1;
      }
      else{
          fin[c] = route[i-2];
          c += 1;
      }
    }
    for( int i=c-2; i<c ; i++) {
        fin[c] = route[i];
        c += 1;
    }
    Serial.print(fin);
    Serial.println();
    delay(15000);
    swtch = -1 ;
    while(true){
      if (left == false && forward == false && right == false) {
        if (swtch != 0) {
          test += 1;
          swtch = 0;
        }
      }
      else if (left == false && forward == false && right == true) {
        if (swtch != 1) {
          test += 1;
          swtch = 1;
        }
      }
      else if (left == false && forward == true && right == false) {
        if (swtch != 2) {
          test += 1;
          swtch = 2 ;
        }
      }
      else if ( left == false && forward == true && right == true) {
        if (swtch != 3) {
          test += 1;
          swtch = 3;
        }
      }
      else if ( left == true && forward == false && right == false) {
        if (swtch != 4) {
          test += 1;
          swtch = 4 ;
        }
      }
      else if ( left == true && forward == false && right == true) {
        if ( swtch != 5) {
          test += 1;
          swtch = 5;
        }
      }
      else if ( left == true && forward == true && right == false) {
        if (swtch != 6) {
          test += 1;
          swtch = 6 ;
        }
      }
      else if ( left == true && forward == true && right == true) {
        if ( swtch != 7 ) {
          swtch = 7;
          test += 1;  
        }
      }
      if(fin[test] == 'F'){
        forwardT();
      }
      else if(fin[test] == 'L'){
        leftT();
      }
      else if(fin[test] == 'R'){
        rightT();
      }        
      else if(fin[test] == 'B'){
        backT();
      }
      else if(fin[test] == 'S'){
        stopT();
        break ;
      }        
    }

    delay(2000);
    test = 0;
    c = 0;
    count = 0;
    memset(route, 0, sizeof route);
    memset(fin, 0, sizeof fin);
  }
}
