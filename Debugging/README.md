# Debugging

## Prerequisites
This code is based off of the joystick control code.

## What is debugging?
Your code will do what you tell it to do, literally.  

At a competition, when you tell a team to "get on the field", we know that we want the teams robot on the field.  Code would take the instruction "get on the field" and step over the field wall and step on the field.  

Sometimes when we're coding, our robot isn't doing things we want it to be doing and we need to figure out where the issues are happening in our code.  One way to do that is by using `printf` statements to show us what variables are doing.  

## How do we see the printf?
At the bottom of your VEXcode, there's a tab called `Terminal`.  When we run our code, stay plugged into the robot / controller and watch the `Terminal`. 

## How do I use printf?
[This guide has all of the parameters you can use for printf](https://www.cplusplus.com/reference/cstdio/printf/)

Within the quotation marks is the string that will be printed.  There's some formatting characters that we can use too.  Below, `\n` and `%i` are used. 

`\n` creates a new line to print on.  

`%i` is an integer, and they get replaced with the value of the variables after the tsring.  The first `%i` gets replaced with `left_joy`, and the second `%i` gets replaced with `right_joy`. 

```cpp
printf("\nLeft: %i  Right: %i", left_joy, right_joy);
```

Combined with the rest of our code, we see this.  And if we run this code in terminal, we will see what the motors are being set to. 

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

    // Print the left and right joystick values to terminal
    printf("\nLeft: %i  Right: %i", left_joy, right_joy);

    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}
```