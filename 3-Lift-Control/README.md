# Lift Control

This project uses a button to set a motor to move.  

## set_lift(int input)
Setting a motor using `voltageUnits::mV` wants an input between -12000 and 12000.  I'd like -100 to 100 to be my range, so an input of 100 needs to be multiplied by 120 so the output becomes 12000.  
```cpp
const int SCALE = 120;
void set_lift(int input) {
  lift_motor.spin(fwd, input*SCALE, voltageUnits::mV);
}
```

## User Control
The user control code uses `if` statements to check if the button is being pressed.  

If `Controller1.ButtonL1.pressing()` is `true` (when it's pressed), it will run what's un the curly braces (the {}) and set the motor to full power (100).  

Else If `Controller1.ButtonL2.pressing()` is `true` (when it's pressed), it will run what's un the curly braces (the {}) and set the motor to full power backwards (-100).

When neither buttons are pressed, the motor will be set to 0 and stop moving.  
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

    // If L1 is pressed, move the motor forwards full power
    if (Controller1.ButtonL1.pressing()) {
      set_lift(100);
    }

    // If L2 is pressed, move the motor backwards at full power
    else if (Controller1.ButtonL2.pressing()) {
      set_lift(-100);
    }

    // If no button is pressed, don't move the motor
    else {
      set_lift(0);
    }

    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}
```