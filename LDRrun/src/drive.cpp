#include "main.h"
#include "drive.h"

int clawTargetR;
int clawTargetL;
ADIAnalogIn outer_limitL ('A');
ADIAnalogIn outer_limitR ('B');
//____________________________________________________________________________//
/////////////////////////////EXPO DRIVE FUNC////////////////////////////////////
//____________________________________________________________________________//
Controller master (CONTROLLER_MASTER); //
//expo drive
int exponentialD(int joyVal, float driveExpon, int joystkDead, int motorMin){
  int joystkSign;
  int joyMax = 127 - joystkDead;
  int joyLive = abs(joyVal) - joystkDead;
  if (joyVal > 0) joystkSign = 1;
  else if (joyVal < 0) joystkSign = -1;
  else joystkSign = 0;
  int power = joystkSign * (motorMin + (127 - motorMin) * pow(joyLive, driveExpon) / pow(joyMax, driveExpon));
  return power;}
//testing upload to github
void opClass::opControl() {
    int Y = exponentialD(master.get_analog(ANALOG_LEFT_Y), 1.7, 8, 15);
 		//int X = exponentialD(master.get_analog(ANALOG_LEFT_X), 1.7, 8, 15);
 		int Z = exponentialD(master.get_analog(ANALOG_RIGHT_X), 1.7, 8, 15);
      driveLB.move(-Y /*- X*/ - Z); 
 	    driveRF.move(0.8*(-Y /*- X*/ + Z)); //lmao lets see how long until william notices
 	    driveLF.move(-Y /*+ X*/ - Z); 
 	    driveRB.move(-Y /*+ X*/ + Z); 
    };
//____________________________________________________________________________//
/////////////////////////////GET VELOCITY FUNC//////////////////////////////////
//____________________________________________________________________________//
    double getVelocity(Motor motor) {
      return motor.get_actual_velocity(); //return velocity of motor
    };

////////////////////////////////////////////////////////////////////////////////
/////////////////////////////ROLLER INTAKE FUNC/////////////////////////////////
//____________________________________________________________________________//
Controller partner (CONTROLLER_PARTNER);
void opClass::Rollers() { //using line sensors as limit switches
    int holdboiL = lClaw.get_position();
    int holdboiR = rClaw.get_position();
    if (partner.get_digital(DIGITAL_R1) && outer_limitR.get_value() > 2500){ //if the intakes want to move out and the line sensor is NOT getting values
      rClaw.move_velocity(-100); //move out at 50% speed
    }
    else if (partner.get_digital(DIGITAL_R2)){ //if intakes want to move in, let them move at 100% speed
      rClaw.move_velocity(200); //move in at 100% speed
    }
    else{rClaw.move_absolute(holdboiR, 0);} //otherwise, do not move intakes

    if (partner.get_digital(DIGITAL_L1) && outer_limitL.get_value() > 2500){ //if left intake wants to move out and line sensor is NOT getting values
      lClaw.move_velocity(-100); //move out at 50% speed
    }
    else if (partner.get_digital(DIGITAL_L2)){ //if it wants to move in...
      lClaw.move_velocity(200); //move in at 100% speed
    }
    else{lClaw.move_absolute(holdboiL, 0);} //otherwise do not move
////////////////////////////////////////////////////////////////////////////////
/////////////////////////////ROLLER ZUKKER FUNC/////////////////////////////////
//____________________________________________________________________________//
  if(master.get_digital(DIGITAL_R1)){ //if button R1 on the master controller is pressed...
    roller.move(127); //move ball up
  }
  else if(master.get_digital(DIGITAL_R2)){ //if button R2 on master controller is pressed...
    roller.move(-127); //move ball down
  }
  else{roller.move(0);} //otherwise don't move
}
  //____________________________________________________________________________//
  /////////////////////////////////TASK FUNCTION//////////////////////////////////
  //____________________________________________________________________________//
void AccTask_fn(void*par) {
  while (true) {
  } //not using task right now, but have used in past and will use in future
};
//____________________________________________________________________________//
////////////////////////////Don't crash into others/////////////////////////////
//____________________________________________________________________________//
bool goingToCrash;
void dont() { //meme function
  driveLB.move_absolute(driveLB.get_position(),0);
  driveRF.move_absolute(driveRF.get_position(),0);
  driveLF.move_absolute(driveLF.get_position(),0);
  driveRB.move_absolute(driveRB.get_position(),0);
}
//____________________________________________________________________________//
/////////////////////////////////STOP FUNC//////////////////////////////////////
//____________________________________________________________________________//
void dpidClass::stop (void) { //PID stop fucntion
      driveLB.move(0); //do not move motors
      driveRB.move(0);
      driveLF.move(0);
      driveRF.move(0);
  	};
//____________________________________________________________________________//
/////////////////////////////////MOVE PID///////////////////////////////////////
//____________________________________________________________________________//
void dpidClass::movePID(int direction, int target, int timeout, int cap) {
  //variables
    float kP = 0.7; //THESE ARE PLACEHOLDERS FOR UNTESTED VALUES
    float kI = 0; //THESE ARE PLACEHOLDERS FOR UNTESTED VALUES
    float kD = 1.1; //THESE ARE PLACEHOLDERS FOR UNTESTED VALUES
    int error = 0;
    int diffError = 0;
    int errorLast = 0;
    int sumError = 0;
    int power;
    int cp = 0;
    float p;
    float d;
    float i = 0; //integral
  	int startTime = millis();

    driveLB.tare_position(); //reset motor value

  	while((millis() - startTime) < timeout){ //while the timeout has not expired
  			error = target - driveLB.get_position(); //find error by subracting target from current pos
  			sumError += error;
  			diffError = error - errorLast;
        errorLast = error; //store last error value
        p = kP * error; //proportional
        d = kD * diffError; //derivative

        power = p+i+d;

        driveLB.move(direction * power); //direction as 1 will make it move forwards, -1 backwards
        driveLF.move(direction * power);
        driveRB.move(direction * power);
        driveRF.move(direction * power);

        if(power > cap){power = cap;} //cap motor speed
        if(power < -cap){power = -cap;}

        delay(10);
  	}
};
//____________________________________________________________________________//
/////////////////////////////////TURN PID///////////////////////////////////////
//____________________________________________________________________________//
void dpidClass::turnPID(int direction, int target, int timeout) {
  //variables
    float kP = 0.7; //THESE ARE PLACEHOLDERS FOR UNTESTED VALUES
    float kI = 0; //THESE ARE PLACEHOLDERS FOR UNTESTED VALUES
    float kD = 1.1; //THESE ARE PLACEHOLDERS FOR UNTESTED VALUES
    int error = 0;
    int diffError = 0;
    int errorLast = 0;
    int sumError = 0;
    int power;
    int cp = 0;
    float p;
    float d;
    float i = 0; //integral
  	int startTime = millis();

    driveRB.tare_position();
    driveLB.tare_position();

    while((millis() - startTime) < timeout){ //while the timeout has not expired
        cp = abs(direction == 1 ? driveLB.get_position() : driveRB.get_position()); //if abs() is not there, we can't find proper target in the next line
        error = target - cp; //find error by subracting target from current pos
  			sumError += error;
  			diffError = error - errorLast;
        errorLast = error; //store last error value
        p = kP * error; //proportional
        d = kD * diffError; //derivative

        power = p+i+d;

        driveLB.move(-direction * power); //direction as 1 will make it turn left, -1 right
        driveLF.move(-direction * power);
        driveRB.move(direction * power);
        driveRF.move(direction * power);

        /*if(power > cap){power = cap;} //cap motor speed
        if(power < -cap){power = -cap;}*/

        delay(10);
  	}
};
//____________________________________________________________________________//
/////////////////////////////////STRAFE PID/////////////////////////////////////
//____________________________________________________________________________//
void dpidClass::strafePID(int direction, int target, int timeout, int cap) {
  //variables
    float kP = 0.7; //THESE ARE PLACEHOLDERS FOR UNTESTED VALUES
    float kI = 0; //THESE ARE PLACEHOLDERS FOR UNTESTED VALUES
    float kD = 1.1; //THESE ARE PLACEHOLDERS FOR UNTESTED VALUES
    int error = 0;
    int diffError = 0;
    int errorLast = 0;
    int sumError = 0;
    int power;
    int cp = 0;
    float p;
    float d;
    float i = 0; //integral
  	int startTime = millis();

    driveLB.tare_position(); //reset motor value

  	while((millis() - startTime) < timeout){ //while the timeout has not expired
  			error = target - driveLB.get_position(); //find error by subracting target from current pos
  			sumError += error;
  			diffError = error - errorLast;
        errorLast = error; //store last error value
        p = kP * error; //proportional
        d = kD * diffError; //derivative

        power = p+i+d;

        driveLB.move(-direction * power); //direction as 1 will make it move forwards, -1 backwards
        driveLF.move(direction * power);
        driveRB.move(direction * power);
        driveRF.move(-direction * power);

        if(power > cap){power = cap;} //cap motor speed
        if(power < -cap){power = -cap;}

        delay(10);
  	}
}
    //examples on main.cpp
