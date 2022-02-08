# One Button Toggle

## Prerequisites 
This code will use the `set_intake` function that was made in the intake control section, and a controller named `Controller`.

```cpp
const int SCALE = 120;
void set_intake(int input) {
  intake_motor.spin(fwd, input*SCALE, voltageUnits::mV);
}
```

## Button Toggle
When we press a button, we want to flip a `boolean` (a variable that can be true or false) to the opposite of itself.  If it's currently true, we want it to be false and vice versa.  

An issue we will run into is when the button is pressed, we don't want the `boolean` to flip every iteration the code runs.  We can demonstrate this happening with the code below.

Opening `terminal` while running this code and holding the button, we will see every time it prints the value changes from `1` (true) to `0` (false) rapidly.  This is a bug, and we only want the boolean to toggle every new press.

```cpp
void usercontrol(void) {
  bool button_toggle = false;
  // User control code here, inside the loop
  while (1) {
    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo
    // values based on feedback from the joysticks.

    // ........................................................................
    // Insert user code here. This is where you use the joystick values to
    // update your motors, etc.
    // ........................................................................

    // If L1 is pressed, set button_toggle to the opposite of itself
    if (Controller1.ButtonL1.pressing()) {
      button_toggle = !button_toggle;
    }

    printf("\nBoolean: %i", button_toggle);

    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}
```

To make the boolean toggle every new press, we can keep track of the previous state of the button and only switch the value of `button_toggle` when the last press is released. 

Running the code below and checking `Terminal`, we will see that every new press the boolean changes from `1` (true) to `0` (false).  

```cpp
void usercontrol(void) {
  bool button_toggle = false;
  int last_press = 0;
  // User control code here, inside the loop
  while (1) {
    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo
    // values based on feedback from the joysticks.

    // ........................................................................
    // Insert user code here. This is where you use the joystick values to
    // update your motors, etc.
    // ........................................................................

    // If L1 is pressed, set button_toggle to the opposite of itself
    if (Controller1.ButtonL1.pressing() && last_press == 0) {
      button_toggle = !button_toggle;
    }
    last_press = Controller1.ButtonL1.pressing();

    printf("\nBoolean: %i", button_toggle);

    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}
```

Now that our boolean only changes on new presses, we can do stuff with it!  In this example, we will set the intake to spin at full power.

This is just a simple if/else, and you can put anything you want here.  Setting pistons, intakes, lifts to PID heights, even RGB lights. 

```cpp
void usercontrol(void) {
  bool button_toggle = false;
  int last_press = 0;
  // User control code here, inside the loop
  while (1) {
    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo
    // values based on feedback from the joysticks.

    // ........................................................................
    // Insert user code here. This is where you use the joystick values to
    // update your motors, etc.
    // ........................................................................

    // If L1 is pressed, set button_toggle to the opposite of itself
    if (Controller1.ButtonL1.pressing() && last_press == 0) {
      button_toggle = !button_toggle;
    }
    last_press = Controller1.ButtonL1.pressing();

    // If button toggle is true, run motor forwards
    if (button_toggle) {
      set_intake(100);
    }
    // If false, don't run motor
    else {
      set_intake(0);
    }

    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}
```