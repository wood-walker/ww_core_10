// ****************************************************************************
//      - - - /   a_variables.ino
//    -( W-W )        www.wood-walker.org - Rainer Radow
//      - - - \          Jan 22, 2017, version 1.v0
// ****************************************************************************
// definition and setting of all global variables of the project
//==================================================================================
#define feetStop       0      // ==== motor move status ====
#define feetBreak      1
#define feetGoForward  2
#define feetGoBackward 3
#define feetGoRight    4
#define feetGoLeft     5
// ------------------------------------ speed and move control ---------------------------- ????
boolean moveflag = false;  // should we drive?????
int speedNow = 0;          // the actual speed will be influenced FeedSpeedTimer()
int speedTarget = 0;       // the target speed that should be reached after the acceleration time
int speedAcceleration = 0; // acceleration = how fast should we reach the speedTarget
//==================================================================================
#define m_action  1           // ==== menu action status ====
#define m_return  2
#define m_deeper  3
// ------------------------------------------------------------------------------------
#define mFeelerListen    100  // ==== menu item status ====

#define mWalker         1000
#define mMotorTest      1011
#define mBump           1012
#define mBumpLoop       1013   // 4

#define mRTC            2100
#define mRTCSetTime     2111
#define mRTCEspSync     2112
#define mOldClock       2120
#define mOldClockLoop   2121
#define mOldClockLow     2130
#define mOldClockLowLoop 2131
#define mRTCDisplayTime 2140
#define mSleepMode      2200
#define mUSBOff         2300
#define mUSBOn          2350  //10

#define mEye            3000
#define mEyeCircle      3010
#define mEyeIcon        3013
#define mEyeRScroll     3014
#define mEyeLScroll     3015
#define mEyeNoScroll    3016
#define mEyeBright      3017
#define mEyeDim         3018 //7

#define mMon            4000
#define mMonEnergy      4010
#define mMonLight       4020
#define mMonIR          4030
#define mMonKnock       4040
#define mMonKnockLoop   4045
#define mMonTemp        4050
#define mMonTempLoop    4055
#define mMonEncoder     4060
#define mMonEncLoop     4065
#define mMonGiro        4100
#define mMonGiroLoop    4105  //12

#define mSound          6000
#define mPlay1          6001  //2

#define mMicAClap       6010
#define mMicAClapLoop   6011
#define mMicDClap       6012
#define mMicDClapLoop   6013  //4

#define mNeo            7000
#define mNeoRainbow     7001
#define mNeoRandom      7002
#define mNeoNight       7003
#define mNeoNightLoop   7004
#define mNeoOff         7005  // 6

#define mBT             7500
#define mBTPio10        7510
#define mBTPio11        7520
#define mBTInfo         7530  // 4

#define mESP            7800
#define mESPOn          7801
#define mESPProg        7802
#define mESPOff         7803
#define mESPInfo        7804
#define mEspMqtt        7805
#define mEspMqttLoop    7806  // 7

#define mOff            9998
#define mOffYes         9999  //2

// ---------------------------------------------   list of the active menue  ---------------------------------------
struct menuVariables {
    byte startact;        // active item in each level of the actual menu chain
    byte listcount;       // # of items in each level of the actual menu chain
    byte list[25];        // list of the numbers of all menu entries in the actual display

    byte levelact;        // active level
    byte listact;         // selected item# in the actual shown menue. Item 1 = 0, Item 2 = 1 and so on
    byte itemactlevel[4]; // storage for active members of the levels above.
};
menuVariables menu;
// ---------------------------------------------  indicates which action to perform in the loop
int activeAction = 0;
int m_actionIndicator = 4;
byte buttonPressed = 0;            // idicates if a button was pressed

#define Serial_Baud 115200

//====================================================== BT ============================
#define serialBT      Serial2
#define serialBTBaud  115200  // Default baud rate is 115200, N, 8, 1
// ===================================================== SD-CARD =================
byte SdOK = LOW;       // tells all following actions if an SD-card is present and readable.
//====================================================== other ============================
volatile boolean knockKnock = 0; // will be used from Knock Knock Interrupt routine
boolean toggle;        // can be used as toggle state from every function - be aware what others do with it
//====================================================== Giro ============================
MPU6050 mpu(0x68);     // MPU6050 specific I2C addresses
bool blinkState = false;

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high

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
//==================================================================================

String chipUID;                   // will be read out of the SAM Chip

//====================================================== MQTT ============================
// Initialize a connection to esp-link using the normal hardware Serial3 port both for
// SLIP and for debug messages.
ELClient esp(&SerialESP, &SerialESP);

// Initialize CMD client (for GetTime)
ELClientCmd cmd(&esp);

// Initialize the MQTT client
ELClientMqtt mqtt(&esp);

//====================================================== WIFI ============================
uint8_t wifiStatusFlag = STATION_IDLE;
uint8_t espStatusFlag = 0;
// --------------------------------------------- 
String wifiStatusTxt[6] {
    "IDLE",        /**< Idle status */
    "CONNECTING",      /**< Trying to connect */
    "WRONG_PW",  /**< Connection error, wrong password */
    "NO_AP_FOUND",     /**< Connection error, AP not found */
    "CONNECT_FAIL",    /**< Connection error, connection failed */
    "GOT_IP"           /**< Connected, received IP */
}; /**< Enumeration of possible WiFi status */
//==================================================================================
//==================================================================================


