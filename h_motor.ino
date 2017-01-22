// g_motor.h
//
// Motor movement
//==================================================================================
/*Highly modified functions inspired from http://playground.arduino.cc/Main/AdafruitMotorShield */
// motor
//
// Select the motor (1-2), the command,
// and the speed (0-255).
// The commands are: FORWARD, BACKWARD, BRAKE, RELEASE.
//
// The motorTimer is the function for smooth motor start and stop function with a given acceleration
// will be called every x seconds automatically because this was set in setup
void FeetSpeedTimer() {
  if (moveflag == true) {
    if (speedNow < speedTarget) {
      speedNow += speedAcceleration / 3;
    } else {
      speedNow = speedTarget;                  // a little stupid...
    }
    if (speedNow > 255) {
      speedNow = 255;
    }
    analogWrite(MoRiPwm, speedNow);
    analogWrite(MoLePwm, speedNow);
  }
}
//==================================================================================
void feetMovement(int direction_code) {

  analogWrite(MoRiPwm, 0);
  analogWrite(MoLePwm, 0);

  switch (direction_code) {
    case feetGoForward:
      eye.println("foreward");
      digitalWrite(MoRi1, 1);
      digitalWrite(MoRi2, 0);
      digitalWrite(MoLe1, 1);
      digitalWrite(MoLe2, 0);
      break;
    case feetGoBackward:
      eye.println("backward");
      digitalWrite(MoRi1, 0);
      digitalWrite(MoRi2, 1);
      digitalWrite(MoLe1, 0);
      digitalWrite(MoLe2, 1);
      break;
    case feetGoRight:
      eye.println("right");
      digitalWrite(MoRi1, 0);
      digitalWrite(MoRi2, 1);
      digitalWrite(MoLe1, 1);
      digitalWrite(MoLe2, 0);
      break;
    case feetGoLeft:
      eye.println("left");
      digitalWrite(MoRi1, 1);
      digitalWrite(MoRi2, 0);
      digitalWrite(MoLe1, 0);
      digitalWrite(MoLe2, 1);
      break;
    case feetStop:            //cut off the power but let the motors run down
      eye.println("stop");
      digitalWrite(MoRi1, 0);
      digitalWrite(MoRi2, 0);
      digitalWrite(MoLe1, 0);
      digitalWrite(MoLe2, 0);
      break;
    case feetBreak:
      eye.println("break");   //shorting the motors for an immedialtely stop
      digitalWrite(MoRi1, 1);
      digitalWrite(MoRi2, 1);
      digitalWrite(MoLe1, 1);
      digitalWrite(MoLe2, 1);
  }
  analogWrite(MoRiPwm, speedTarget / 2);
  analogWrite(MoLePwm, speedTarget / 2);

  moveflag = true;
}
//==================================================================================
void bumpnRunPrepare() {
  speedTarget = 200;
  speedNow = 100;
  moveflag = true;
  speedAcceleration = 50;

  eye.clearBuffer();	   // clear menu side eye
  eye.select(eyeLeft);       // switch to oppisite eye
  eye.display();
  eye.select(eyeRight);       // switch to oppisite eye
  eye.display();

  eye.setTextSize(2);
  eye.setTextColor(WHITE);
  eye.setCursor(0, 0);
  eye.println("Bump'n run");
  eye.display();

  delay(2000);
  initializeEyes();
  drawEye();
  feetMovement(feetGoForward); // go forward
}
//==================================================================================
void bumpLoop() {
  if (digitalRead(MFRight) == 0) {
    speedTarget = 200;
    speedNow = 100;
    feetMovement(feetGoBackward);
    scrollRight();
    delay(2000);
    scrollStop();
    delay(500);
    speedNow = 100;

    feetMovement(feetGoRight);
    scrollLeft();
    delay(2000);
    scrollStop();
    feetMovement(feetStop);
 
    int randNumber = random(5);
    delay(randNumber * 1000);
    
    speedNow = 100;
    speedTarget = 250;
    feetMovement(feetGoForward);
    drawEye();
  }
  if (digitalRead(MFLeft) == 0) {
    speedTarget = 200;
    speedNow = 100;
    feetMovement(feetGoBackward);
    scrollLeft();
    delay(2000);
    scrollStop();
    feetMovement(feetStop);
 
    int randNumber = random(5);
    delay(randNumber * 1000);

    speedNow = 100;
    feetMovement(feetGoLeft);
    scrollRight();
    delay(2000);
    scrollStop();
    delay(1000);
    speedNow = 100;
    speedTarget = 200;
    feetMovement(feetGoForward);
    drawEye();
  }
}
//==================================================================================
void motorTest() {
  speedTarget = 200;
  speedNow = 150;
  moveflag = true;
  speedAcceleration = 100;

  eye.setTextSize(2);
  eye.setTextColor(WHITE);

  eye.clearBuffer();
  eye.setCursor(10, 10);
  eye.println("test run");
  eye.select(eyeLeft);
  eye.display();

  eye.select(eyeRight);

  eye.clearBuffer();
  eye.setCursor(0, 0);
  eye.println("foreward");
  eye.display();
  feetMovement(feetGoForward);
  delay(4000);

  eye.clearBuffer();
  eye.setCursor(0, 0);
  eye.println("backward");
  eye.display();

  feetMovement(feetGoBackward);
  delay(4000);
  //
  eye.clearBuffer();
  eye.setCursor(0, 0);
  eye.println("right");
  eye.display();
  feetMovement(feetGoRight);
  delay(4000);
  //
  eye.clearBuffer();
  eye.setCursor(0, 0);
  eye.println("left");
  eye.display();
  feetMovement(feetGoLeft);
  delay(4000);
  //
  eye.clearBuffer();
  eye.setCursor(0, 0);
  eye.println("stop");
  eye.display();
  feetMovement(feetStop);

  delay(2000);
  eye.clearBuffer();
  eye.display();
  eye.select(eyeLeft);
  eye.display();
}
//==================================================================================
























