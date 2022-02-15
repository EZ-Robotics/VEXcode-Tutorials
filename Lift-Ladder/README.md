# Lift Ladder Controls 

Ladder controls are when you have multiple preset positions that you increase / decrease through.  

If we are current at the third position and increase the ladder, we want to go to the second position.  Increasing the ladder will always go up.  Going down 

Increasing the ladder while in the third position will bring us to the fourth.  Decreasing would bring us to second.  The code will wrap around when you're at the end of the limits.  So when you're at position 0 and decrease, you'll go to the maximum position and vice versa. 
```
0 - 0     
1 - 100   
2 - 200  <--
3 - 300
4 - 400
5 - 500
```

## Prerequisites
Understanding button toggles are necessary.  A motor named `lift_motor` and controller named `Controller`. 

## Finding Encoder Values 
Go to the `Devices` menu on your brain and select the motor your lift motor.  Click through the `18:1` section until it's the cartridge you're using (green, red, or blue).  Bring your lift all the way down, and press the `Degrees` box to zero your sensor.  Now raise your lift to the desired height(s) and keep note of all of them.  

For this example we'll be using these positions

```
0, 100, 200, 300
```

## Using Built In PID
The build in PID can be called using `startRotateTo`.  I like to make a wrapper function that has `pos` (the encoder input) and `speed` (number from 0 to your cartridge rpm).
```cpp
// Set lift using built in PID
void set_lift_position  (int pos, int speed) { 
  lift_motor.startRotateTo(pos, rotationUnits::deg, speed, velocityUnits::pct); 
}
```

This function can be used in opcontrol like this
```CPP
void opcontrol() {
  while (true) {
    // Go to the highest macro position if L1 is pressed
    if (Controller1.ButtonL1.pressing()) {
      set_lift_position(500, 200);
    }

    // Bring the lift back down if L2 is pressed
    else if (Controller1.ButtonL2.pressing()) {
      set_lift_position(0, 200);
    }

    wait (20, msec);
  }
}
```

## Arrays

Arrays are a series of elements and can be created like this.  They give us indexes for elements that we can set and call.
```cpp
const int NO_LIFT_POSITIONS = 4;
const int lift_positions[NO_LIFT_POSITIONS] = {0, 100, 200, 300};

void pre_auton() {
  vexcodeInit();

  printf("\n Lift Position 0: %i", lift_positions[0]); // this outputs 0
  printf("\n Lift Position 1: %i", lift_positions[1]); // this outputs 100
  printf("\n Lift Position 2: %i", lift_positions[2]); // this outputs 200
  printf("\n Lift Position 3: %i", lift_positions[3]); // this outputs 300
}
```

For 

## Increasing in the Ladder
When `L1` is pressed we want the ladder to increase until it's at the max height, then we want it to go to the lowest position.  Let's start with the former. 

We will use an integer `current` to be our current index.  Using the `last_` logic from the button tutorial, we will increase `current` by `1` every time this is ran.  

As seen above, the index starts from 0 and goes to 3.  So when the value is **greater than or equal NO_LIFT_POSITIONS**, we want `current` to be the first index, 0.  We can confirm this works by running this code and checking terminal. 

```cpp
void usercontrol(void) {
  // User control code here, inside the loop
  int current = 0; // Current place in the array
  bool last_l1 = false; // Last L1
  while (1) {
    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo
    // values based on feedback from the joysticks.

    // ........................................................................
    // Insert user code here. This is where you use the joystick values to
    // update your motors, etc.
    // ........................................................................

    // Increasing through the ladder
    if (Controller1.ButtonL1.pressing() and not last_l1) {
      // Increase current by 1
      current = current + 1;
      // If we're at the highest position in the ladder, set to the lowest
      if (current >= NO_LIFT_POSITIONS) {
        current = 0;
      }
      printf("\nCurrent %i", current);
    }
    last_l1 = Controller1.ButtonL1.pressing(); // Keep track of the last press

    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}
```

## Decreasing the ladder 

When `L2` is pressed we want the ladder to decrease until it's at the minimum index, then we want it to go to the max index. 

Every new press we will make `current` 1 smaller.  When it's **less than or equal to -1**, we will set it to the maximum index, which is `NO_LIFT_POSITIONS-1`.  We need to subtract one because `lift_positions` holds 4 values, but the max index is `3` because it starts at `0`.  

We can confirm this works by running this code and checking terminal. 

```cpp
void usercontrol(void) {
  // User control code here, inside the loop
  int current = 0; // Current place in the array
  bool last_l2 = false; // Last L2
  while (1) {
    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo
    // values based on feedback from the joysticks.

    // ........................................................................
    // Insert user code here. This is where you use the joystick values to
    // update your motors, etc.
    // ........................................................................

    // Decreasing through the ladder
    if (Controller1.ButtonL2.pressing() and not last_l2) {
      // Decrease current by 1
      current = current - 1;
      // If we're at the lowest point in the ladder, set to the highest
      if (current <= -1) {
        current = NO_LIFT_POSITIONS - 1; // Because arrays index at 0, our max ladder position is NO_LIFT_POSITIONS - 1
      }
      printf("\nCurrent %i", current);
    }
    last_l2 = Controller1.ButtonL2.pressing(); // Keep track of the last 

    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}
```

## Final Code

Now all we have to do is set the motor!  We can do this with `set_lift_position(lift_positions[current], 200);`.  

Our final code looks like this
```cpp
void usercontrol(void) {
  // User control code here, inside the loop
  int current = 0; // Current place in the array
  bool last_l1 = false; // Last L1
  bool last_l2 = false; // Last L2
  while (1) {
    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo
    // values based on feedback from the joysticks.

    // ........................................................................
    // Insert user code here. This is where you use the joystick values to
    // update your motors, etc.
    // ........................................................................

    // Increasing through the ladder
    if (Controller1.ButtonL1.pressing() and not last_l1) {
      // Increase current by 1
      current = current + 1;
      // If we're at the highest position in the ladder, set to the lowest
      if (current >= NO_LIFT_POSITIONS) {
        current = 0;
      }
    }
    last_l1 = Controller1.ButtonL1.pressing(); // Keep track of the last press

    // Decreasing through the ladder
    if (Controller1.ButtonL2.pressing() and not last_l2) {
      // Decrease current by 1
      current = current - 1;
      // If we're at the lowest point in the ladder, set to the highest
      if (current <= -1) {
        current = NO_LIFT_POSITIONS - 1; // Because arrays index at 0, our max ladder position is NO_LIFT_POSITIONS - 1
      }
    }
    last_l2 = Controller1.ButtonL2.pressing(); // Keep track of the last 
    
    set_lift_position(lift_positions[current], 200);

    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}
```