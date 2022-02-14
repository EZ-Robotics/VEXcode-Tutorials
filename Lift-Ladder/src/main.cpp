/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       VEX                                                       */
/*    Created:      Thu Sep 26 2019                                           */
/*    Description:  Competition Template                                      */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// lift_motor           motor         1               
// Controller1          controller                    
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here

// Include the std library for vectors
#include <vector>

// Vector of heights that are encoder positions 
const std::vector<int> lift_positions = {0, 100, 200};

// Set lift using built in PID
void set_lift_position  (int pos, int speed) { 
  lift_motor.startRotateTo(pos, rotationUnits::deg, speed, velocityUnits::pct); 
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
  int current = 1; // Current place in the vector
  bool last_l1 = false; // Last L1
  bool last_l2 = false; // Last L2
  while (1) {
    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo
    // values based on feedback from the joysticks.

    // Increasing through the ladder
    if (Controller1.ButtonL1.pressing() and not last_l1) {
      // If we're at the highest position in the ladder, set to the lowest
      if (current == lift_positions.size()) {
        current = 1;
      }
      // Otherwise, increase the position by 1
      else {
        current = current + 1;
      }
    }
    last_l1 = Controller1.ButtonL1.pressing(); // Keep track of the last press

    // Decreasing through the ladder
    if (Controller1.ButtonL2.pressing() and not last_l2) {
      // If we're at the lowest point in the ladder, set to the highest
      if (current == 1) {
        current = lift_positions.size();
      }
      // Otherwise, decrease the position by 1
      else {
        current = current - 1;
      }
    }
    last_l2 = Controller1.ButtonL2.pressing(); // Keep track of the last press

    // Set the position.  Because vectors start with 0, we need to set it to current - 1
    set_lift_position(lift_positions[current-1], 200);

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
