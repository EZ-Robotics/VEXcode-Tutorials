# Pneumatics 

## Prerequisites
Setup a piston by using the GUI and selecting `3-wire` -> `Digital Out` then selecting the port and naming it `piston`.  Setup a controller named `Controller1`. 

## Setting a Piston
You set a piston using `piston.set(true);` or `piston.set(false);`.  The code below triggers the piston when L1 or L2 are pressed.  
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

    if (Controller1.ButtonL1.pressing()) {
      piston.set(true);
    }
    else if (Controller1.ButtonL2.pressing()) {
      piston.set(false);
    }

    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}
```