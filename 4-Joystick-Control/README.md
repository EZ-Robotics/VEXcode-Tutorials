# Joystick Control

## Prerequisites
Create 4 motors (`left_back`, `left_front`, `right_back`, `right_front`) and the controller named `Controller1`. 

Note: you'll have to reverse one of the sides of the chassis!

## set_tank()
We want independent control over the left and right side of the drive.  We can create a `set_tank` function that has two input parameters, one for the left side and one for the right.  
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

## User Control
We could just use `set_tank(Controller1.Axis3.value(), Controller2.Axis2.value());` and call it a day, but the joysticks don't always come back perfectly to 0.  When this happens, the motors move very slowly forward when you want them to stop.  We can fix this with a programmed dead zone. 

Using if statements, we can check when the absolute value of the joystick is less then 5.  When it is, we set `let_joy` or `right_joy` to `0` so the motors wouldn't move.  

```cpp
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

    // Variables for the left and right joystick
    int left_joy = Controller1.Axis3.value();
    int right_joy = Controller1.Axis2.value();

    //
    // Joysticks don't always come back to 0 perfectly, so a programmed threshold makes driving easier.
    //

    // If the absolute value of left joy is less then 5...
    if (abs(left_joy) < 5) {
      left_joy = 0; // Set left_joy to 0
    }

    // If the absolute value of right joy is less then 5...
    if (abs(right_joy) < 5) {
      right_joy = 0; // Set right_joy to 0
    }
    
    // Set the drive to left joy and right joy
    set_tank(left_joy, right_joy);

    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}
```
