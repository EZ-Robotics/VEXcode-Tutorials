# IMU P Controller

More information on PID:  
[George Gillard's PID Guide](http://georgegillard.com/documents/2-introduction-to-pid-controllers)  
[BLRS Wiki](https://wiki.purduesigbots.com/software/control-algorithms/pid-controller)

We will change the feedback controller to work for turning using the IMU. 

## Prerequisites
Create 4 motors (`left_back`, `left_front`, `right_back`, `right_front`), an inertial named `imu`, and the controller named `Controller1`.  You'll have to reverse one of the sides of the chassis!  


You'll need the same `set_tank` function used in Joystick Control, and we'll be modifying the Feedback Controller. 
```cpp
// Set voltage
const int SCALE = 120;
void set_tank(int left, int right) {
  left_front.spin(fwd, left*SCALE, voltageUnits::mV);
  left_back.spin(fwd, left*SCALE, voltageUnits::mV);
  right_front.spin(fwd, right*SCALE, voltageUnits::mV);
  right_back.spin(fwd, right*SCALE, voltageUnits::mV);
}
```

```cpp
double clipnum(double input, double max, double min) {
  if (input > max) {
    return max;
  }
  else if (input < min) {
    return min;
  }
  return input;
}

// Feedback controller
void feedback(double target, int speed) {
  double kP = 0.5; // kP (scaling number)
  int x = 0; // Timer for exit condition
  while (true) {
    double error = target - lift_motor.position(deg); // error = (target - current)
    set_lift(clipnum(error * kP, speed, -speed)); // Set motors to (error * kP) and limit the speed 

    // If the velocity of the motor is 0...
    if (lift_motor.velocity(pct) == 0) {
      x+=10; // Increase x by 10
      if (x >= 50) { // If x is 50 (meaning the motors were at 0 for 50ms)...
        break; // Break the while loop
      }
    } 
    // If the velocity of the motor is not 0...
    else {
      x = 0; // Reset the timer
    }

    wait(10, msec);
  }
  set_lift(0); // Set the motors to 0 before exiting this function
}
```

## IMU Setup
The inertial sensor needs 2 seconds to calibrate at the start of the program.  During this 2 seconds, the robot CANNOT move, or the IMU will not read correctly.  To make it easier for us to know when the 2 seconds is over, we can have the controller vibrate after the 2 seconds by adding this code to `pre_auton`.
```cpp
void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  wait(2, sec); // Give the IMU time to calibrate
  Controller1.rumble(".");

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}
```

## Modifying feedback()
To make the `feedback` function work for turning, we need to replace `current` (in this case, `lift_motor.position(deg)`) to read the current imu position.  We can do this with `imu.rotation(deg);`.  

The function should be renamed to `turn`.  

Instead of `set_tank(error * kP, -error * kP);`, we want to point turn the robot by moving one side forward and one side backwards.  We will create an integer called `output` and set that equal to `clipnum(error * kP, speed, -speed);`.  Then we can set `output` and `-output`to the left and right side. 

The exit condition should be replaced to `left_back`, and an added `&&` for checking `right_back` too. 
```cpp
void turn(double target, int speed) {
  double kP = 0.5; // kP (scaling number)
  int x = 0; // Timer for exit condition
  while (true) {
    double error = target - imu.rotation(deg); // error = (target - current)
    int output = clipnum(error * kP, speed, -speed);
    set_tank(output, -output); // Set motors to (error * kP) and clip it to speed

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

void autonomous(void) {
  turn(180, 50); // Turn 180 degrees at half poewr
  turn(90, 100); // Turn 90 degrees at full power
}
```

## Tuning a P Controller
The goal of a feedback controller is to slow down as you get to the target to not overshoot, and you accomplish this by modifying your constants (kP, kI and kD).  

If your kP is too high, your robot will oscillate around the target.  If your kP is too low, you'll undershoot and not make your target.  You need to trial and error your kP until you get to a value that gets you to your position consistently. 