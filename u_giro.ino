// *****************************************************************************
//      - - - /    giro_simple.ino
//    -( W-W )        CC Attribution 4.0 International License
//      - - - \          www.wood-walker.org - Rainer Radow
//                          2015 OCtober 14 - version 1
// *****************************************************************************
// This sketch reads data from the giro (MPU6050)
// This code is based of the code fromm 6/21/2012 by Jeff Rowberg <jeff@rowberg.net>
// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project

void giroStuff() {

bool blinkState = false;

// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU1
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector
float euler[3];         // [psi, theta, phi]    Euler angle container
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

#define SerialDebug SerialUSB  // 'Native USB Port' on Wood-Walker PCB = 'SerialUSB'
}


// ================================================================
// ===               INTERRUPT DETECTION ROUTINE                ===
// ================================================================
void dmpDataReady() {
  mpuInterrupt = true;
}

// =============================================================================
void monGiroPrepare() {
  // join I2C bus (I2Cdev library doesn't do this automatically)
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
  Wire.begin();
  //TWBR = 24; // 400kHz I2C clock (200kHz if CPU is 8MHz)
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
  Fastwire::setup(400, true);
#endif

  // initialize serial communication
  SerialDebug.begin(115200); //You have to read fast to avoid a Fifo overflow!
  while (!SerialDebug); // wait for established communication,
  //otherwhise the program would continue immediately

  // initialize device
  SerialDebug.println(F("Initializing I2C devices..."));
  mpu.initialize();

  // verify connection
  SerialDebug.println(F("Testing device connections..."));
  SerialDebug.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));

  // load and configure the DMP
  SerialDebug.println(F("Initializing DMP..."));
  devStatus = mpu.dmpInitialize();

  // supply your own gyro offsets here, scaled for min sensitivity
  mpu.setXGyroOffset(220);
  mpu.setYGyroOffset(76);
  mpu.setZGyroOffset(-85);
  mpu.setZAccelOffset(1788); // 1688 factory default for my test chip

  // make sure it worked (returns 0 if so)
  if (devStatus == 0) {
    // turn on the DMP, now that it's ready
    SerialDebug.println(F("Enabling DMP..."));
    mpu.setDMPEnabled(true);

    // enable Arduino interrupt detection
    SerialDebug.println(F("Enabling interrupt detection (Arduino external interrupt GiInter)..."));
    attachInterrupt(GiInter, dmpDataReady, RISING);
    mpuIntStatus = mpu.getIntStatus();

    // set our DMP Ready flag so the main loop() function knows it's okay to use it
    SerialDebug.println(F("DMP ready! Waiting for first interrupt..."));
    dmpReady = true;

    // get expected DMP packet size for later comparison
    packetSize = mpu.dmpGetFIFOPacketSize();
  } else {
    // ERROR!
    // 1 = initial memory load failed
    // 2 = DMP configuration updates failed
    // (if it's going to break, usually the code will be 1)
    SerialDebug.print(F("DMP Initialization failed (code "));
    SerialDebug.print(devStatus);
    SerialDebug.println(F(")"));
  }

  // configure LED for output
  pinMode(Led13, OUTPUT);
  //pmc_mck_set_prescaler(PMC_PCK_PRES_CLK_64);
}
// =============================================================================
void monGiroLoop() {
  // if programming failed, don't try to do anything
  if (!dmpReady) {
    return;
  }
  // wait for MPU interrupt or extra packet(s) available
  while (!mpuInterrupt && fifoCount < packetSize) {
    // other program behavior stuff here
  }
  // reset interrupt flag and get INT_STATUS byte
  mpuInterrupt = false;
  mpuIntStatus = mpu.getIntStatus();

  // get current FIFO count
  fifoCount = mpu.getFIFOCount();

  // check for overflow (this should never happen unless our code is too inefficient)
  if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
    // reset so we can continue cleanly
    mpu.resetFIFO();
    SerialDebug.println(F("FIFO overflow!"));
    // otherwise, check for DMP data ready interrupt (this should happen frequently)
  }
  else if (mpuIntStatus & 0x02) {
    // wait for correct available data length, should be a VERY short wait
    while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

    // read a packet from FIFO
    mpu.getFIFOBytes(fifoBuffer, packetSize);

    // track FIFO count here in case there is > 1 packet available
    // (this lets us immediately read more without waiting for an interrupt)
    fifoCount -= packetSize;

    // display quaternion values in easy matrix form: w x y z
    mpu.dmpGetQuaternion(&q, fifoBuffer);

    // calculate gravity vector
    float gravityQ[3];
    gravityQ[0] = 2 * (q.x * q.z - q.w * q.y);
    gravityQ[1] = 2 * (q.w * q.x + q.y * q.z);
    gravityQ[2] = q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z;

    // calculate yaw/pitch/roll angles
    ypr[0] = atan2(2 * q.x * q.y - 2 * q.w * q.z, 2 * q.w * q.w + 2 * q.x * q.x - 1);
    ypr[1] = atan(gravityQ[0] / sqrt(gravityQ[1] * gravityQ[1] + gravityQ[2] * gravityQ[2]));
    ypr[2] = atan(gravityQ[1] / sqrt(gravityQ[0] * gravityQ[0] + gravityQ[2] * gravityQ[2]));

    SerialDebug.print(" YPR: ");
    if (ypr[0] >= 0) SerialDebug.print(" ");
    SerialDebug.print(ypr[0]);
    SerialDebug.print(" ");
    if (ypr[1] >= 0) SerialDebug.print(" ");
    SerialDebug.print(ypr[1]);

    if (ypr[1] >   0.2)                  SerialDebug.print(" ^, ");
    if (ypr[1] >= -0.2 && ypr[1] <= 0.2) SerialDebug.print(" -, ");
    if (ypr[1] <  -0.2)                  SerialDebug.print(" v, ");
    if (ypr[2] >= 0) SerialDebug.print(" ");
    SerialDebug.print(ypr[2]);
    if (ypr[2] >   0.2)                  SerialDebug.print(" >, ");
    if (ypr[2] >= -0.2 && ypr[2] <= 0.2) SerialDebug.print(" -, ");
    if (ypr[2] <  -0.2)                  SerialDebug.print(" <, ");
    SerialDebug.print(" ");

    // display initial world-frame acceleration, adjusted to remove gravity
    // and rotated based on known orientation from quaternion
    // acceleration
    // components with gravity removed and adjusted for the world frame of
    // reference (yaw is relative to initial orientation, since no magnetometer
    // is present in this case). Could be quite handy in some cases.

    //you have to read all this values in this sequence - don't know why...
    mpu.dmpGetAccel(&aa, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);
    mpu.dmpGetLinearAccelInWorld(&aaWorld, &aaReal, &q);
    SerialDebug.print(" aworld");
    SerialDebug.print(" x=");
    fAusgabe(aaWorld.x);
    SerialDebug.print(" y=");
    fAusgabe(aaWorld.y);
    SerialDebug.print(" z=:");
    fAusgabe(aaWorld.z);

    SerialDebug.println("");

    // blink LED to indicate activity
    blinkState = !blinkState;
    digitalWrite(Led13, blinkState);
  }
}

void monGiroOff(){
  
}

// =============================================================================
void fAusgabe(int wert) {  // this just to bring the  output
  if (wert >= 0) SerialDebug.print(" "); //space for the missing minus
  if (abs(wert) < 10) SerialDebug.print(" ");
  if (abs(wert) < 100) SerialDebug.print(" ");
  if (abs(wert) < 1000) SerialDebug.print(" ");

  SerialDebug.print(wert);
}
// *****************************************************************************


