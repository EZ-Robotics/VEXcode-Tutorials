/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Jess Zarchi                                               */
/*    Created:      Fri Jan 7  2022                                           */
/*    Description:  IMU                                                       */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// left_front           motor         1               
// left_back            motor         2               
// right_front          motor         3               
// right_back           motor         4               
// Controller1          controller                    
// imu                  inertial      5               
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here
// Set voltage
const int SCALE = 120;
void set_tank(int left, int right) {
  left_front.spin(fwd, left*SCALE, voltageUnits::mV);
  left_back.spin(fwd, left*SCALE, voltageUnits::mV);
  right_front.spin(fwd, right*SCALE, voltageUnits::mV);
  right_back.spin(fwd, right*SCALE, voltageUnits::mV);
}

// Turn P loop
void turn(double target) {
  double kP = 0.5; // kP (scaling number)
  int x = 0; // Timer for exit condition
  while (true) {
    double error = target - imu.rotation(deg); // error = (target - current)
    set_tank(error * kP, -error * kP); // Set motors to (error * kP)

    // If the velocity of the left and right motors are 0...
    if (left_back.velocity(pct) == 0 && right_back.velocity(pct) == 0) {
      x+=10; // Increase x by 10
      if (x >= 50) { // If x is 50 (meaning the motors were at 0 velocity for 50ms)...
        break; // Break the while loop
      }
    } 
    // If the velocity of the left and right motors are not 0...
    else {
      x = 0; // Set the timer to 0
    }

    wait(10, msec);
  }
  set_tank(0, 0); // Make sure motors are off before leaving this function
}

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  wait(2, sec); // Give the IMU time to calibrate
  Controller1.rumble(".");

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void autonomous(void) {
  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................
  turn(180);
  turn(90);
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void usercontrol(void) {
  // User control code here, inside the loop
  while (1) {
    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo
    // values based on feedback from the joysticks.

    // ........................................................................
    // Insert user code here. This is where you use the joystick values to
    // update your motors, etc.
    // ........................................................................

    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
