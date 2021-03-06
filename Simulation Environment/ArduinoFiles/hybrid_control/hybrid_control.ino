#include <MotorWheel.h>
#include <R2WD.h>
#include <PID_Beta6.h>
#include <PinChangeInt.h>
#include <PinChangeIntConfig.h>
#define _USE_MATH_DEFINES
#include <math.h>


#define PI 3.14159265

enum buffer_state {
  BUFFER_EMPTY,
  STATE_QUERY,
  MANUAL_CONTROL,
  POSE_DATA,
  START_GOAL,
  BUFFER_INVALID
};

const int buffer_size = 100;
const int manual_size = 16;
const int pose_size = 21;
char buffer[buffer_size];
char manual_buffer[manual_size];
char pose_buffer[pose_size];
char junk[buffer_size];

const int maxc = 500;
const int minc = -500;

/* Motor setup */
irqISR(irq1,isr1); // Intterrupt function.on the basis of the pulse, work for wheel1
MotorWheel lwheel(9,8,4,5,&irq1,REDUCTION_RATIO,int(144*PI));
irqISR(irq2,isr2);
MotorWheel rwheel(10,11,6,7,&irq2,REDUCTION_RATIO,int(144*PI));
R2WD drivetrain(&lwheel,&rwheel,WHEELSPAN);

/* Loop variables */
int left = 0;
int right = 0;
boolean ldir = DIR_ADVANCE;
boolean rdir = DIR_BACKOFF;
int x0 = 0; // cm
int y0 = 0; // cm
int x = 0; // cm
int y = 0; // cm
int theta = 0; // degree
int xg = 0; // cm
int yg = 0; // cm
/*===================*/
/* define your own variables here(copy the content of OwnVariables.c here) */


/*===================*/



boolean is_manual_control() {
  // control signal
  // u +010 +005
  if (strncmp(buffer, "manual", 6) != 0) {
    return false;
  }
  int left;
  int right;
  memcpy(manual_buffer, buffer, manual_size);
  sscanf(manual_buffer, "manual %d %d", &left, &right);
  if (left < minc || left > maxc || right < minc || right > maxc) {
    return false;
  }
  return true;
}

boolean is_state_query() {
  return (strncmp(buffer, "state?", 6) == 0);
}

boolean is_pose_data(){
  return (strncmp(buffer, "pose", 4) == 0);
}

boolean is_start_goal(){
  return (strncmp(buffer, "startgoal", 9) == 0);
}

int read_buffer() {
  send_debug("reading buffer");
  Serial.readBytesUntil(':', junk, buffer_size);
  int bytes_read = Serial.readBytesUntil(';', buffer, buffer_size);
  if (bytes_read == 0) {
    // zero bytes were read
    return BUFFER_EMPTY;
  } else if (is_manual_control()) {
    return MANUAL_CONTROL;
  } else if (is_state_query()) {
    return STATE_QUERY;
  } else if (is_pose_data()) {
    return POSE_DATA;
  } else if (is_start_goal()) {
    return START_GOAL;
  } 
  else {
    return BUFFER_INVALID;
  }
}

void send_state(int left, int right) {
  Serial.print(":state ");
  Serial.print(left, DEC);
  Serial.print(" ");
  Serial.print(right, DEC);
  Serial.print(";"); 
}

void send_debug(const char* msg) {
  Serial.print("SYS: ");
  Serial.print(":!");
  Serial.print(msg);
  Serial.print(";");
}

void setup() {
  //TCCR0B=TCCR0B&0xf8|0x01; 
  TCCR1B=TCCR1B&0xf8|0x01;    // Pin9,Pin10 PWM 31250Hz
  //TCCR2B=TCCR2B&0xf8|0x01;  // Pin3,Pin11 PWM 31250Hz
  Serial.begin(9600);
  drivetrain.PIDEnable(0.26,0.02,0,10);
  send_debug("ready");
}

/* the main loop */
void loop() {  
  while (Serial.available()) {
    switch (read_buffer()) {
      case BUFFER_EMPTY:
        break;
      case MANUAL_CONTROL:
        memcpy(manual_buffer, buffer, manual_size);
        sscanf(manual_buffer, "manual %d %d", &left, &right);
        if (left <= 0) {
          ldir = DIR_BACKOFF;
        } else {
          ldir = DIR_ADVANCE;
        }
        if (right <= 0) {
          rdir = DIR_ADVANCE;
        } else {
          rdir = DIR_BACKOFF;
        }
        Serial.print("SYS: ");
        Serial.print(":!set left=");
        Serial.print(left, DEC);
        Serial.print(" right=");
        Serial.print(right, DEC);
        Serial.print(";\n");
        break;
      case POSE_DATA:
        memcpy(pose_buffer, buffer, pose_size);
	    sscanf(pose_buffer, "pose %d %d %d", &x, &y, &theta);
        Serial.print("SYS: ");
        Serial.print(":!pose_x=(cm)");
        Serial.print(x, DEC);
        Serial.print(" pose_y=(cm)");
        Serial.print(y, DEC);
        Serial.print(" pose_theta=(degree)");
        Serial.print(theta, DEC);        
        Serial.print(";\n");
        /*=========================*/    
        /*put your controller here(copy the content of Controller.c here)*/



        /*=========================*/
        if (left <= 0) {
          ldir = DIR_BACKOFF;
        } else {
          ldir = DIR_ADVANCE;
        }
        if (right <= 0) {
          rdir = DIR_ADVANCE;
        } else {
          rdir = DIR_BACKOFF;
        }
        Serial.print("SYS: ");
        Serial.print(":!set left=");
        Serial.print(left, DEC);
        Serial.print(" right=");
        Serial.print(right, DEC);
        Serial.print(";\n");
        break;
      case START_GOAL:
        sscanf(buffer, "startgoal %d %d %d %d", &x0, &y0, &xg, &yg);
        Serial.print("SYS: ");
        Serial.print(":!start_x=(cm)");
        Serial.print(x0, DEC);
        Serial.print(" start_y=(cm)");
        Serial.print(y0, DEC);
        Serial.print(":!goal_x=(cm)");
        Serial.print(xg, DEC);
        Serial.print("goal_y=(cm)");
        Serial.print(yg, DEC);
        Serial.print(";\n");
        /*====================*/
        /* Renew the discrete control state upon receiving new goals(copy the content of RenewControllerState.c here)*/

        /*====================*/
        break;
      case STATE_QUERY:
        send_state(
          drivetrain.wheelLeftGetSpeedMMPS(), 
          drivetrain.wheelRightGetSpeedMMPS());
        // Asking for the state resets it >-(
        drivetrain.wheelLeftSetSpeedMMPS(abs(left), ldir);
        drivetrain.wheelRightSetSpeedMMPS(abs(right), rdir);
        break;
      case BUFFER_INVALID:
        left = 0;
        right = 0;
        send_debug("got an invalid command\n");
        break;
    }
    drivetrain.wheelLeftSetSpeedMMPS(abs(left), ldir);
    drivetrain.wheelRightSetSpeedMMPS(abs(right), rdir);
  }
  drivetrain.PIDRegulate();
}
