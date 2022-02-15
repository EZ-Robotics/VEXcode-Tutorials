# Feedback Controller 

More information on PID:  
[George Gillard's PID Guide](http://georgegillard.com/documents/2-introduction-to-pid-controllers)  
[BLRS Wiki](https://wiki.purduesigbots.com/software/control-algorithms/pid-controller)

## Prerequisites 
One motor setup called `lift_motor`, and the `set_lift()` function below. 
```cpp
const int SCALE = 120;
void set_lift(int input) {
  lift_motor.spin(fwd, input*SCALE, voltageUnits::mV);
}
```

Clipnum function 
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
```

## How Feedback Controllers Work
Feedback controllers use sensors to react and adjust.  We want to tell our robot to move some distance, and have it move that distance.  When the robot detects it's not where we want it to be, we want it to react by moving to get there.  

For this example we will be using a P loop, the first part of PID.  

P loops work by running `target - current`.  The chart below shows how error becomes smaller as the robot gets closer to the target. 

| target  | current | error |
| :---:   |  :----: | :---: |
| 100     | 0       | 100   |
| 100     | 25      | 75    |
| 100     | 50      | 50    |
| 100     | 75      | 25    |
| 100     | 100     | 0     |

Sometimes this won't be aggressive enough, or it will be too aggressive.  We can tune it by adding a scalar value called `kP`.  The motor would be set to `error * kP`.  

## Implementation
This code would move the motor one full rotation.  
```cpp
void feedback(double target) {
  double kP = 0.5;
  while (true) {
    double error = target - lift_motor.position(deg);
    set_lift(error * kP);

    wait(10, msec);
  }
}

void autonomous(void) {
  feedback(360); // Spin the motor 1 full rotation
}
```

## Speed Parameter
Using the `clipnum` function, we can add a `speed` parameter and make that the max/min speed out of 100.  
```cpp
void feedback(double target, int speed) {
  double kP = 0.5;
  while (true) {
    double error = target - lift_motor.position(deg);
    set_lift(clipnum(error * kP, speed, -speed));

    wait(10, msec);
  }
}

void autonomous(void) {
  feedback(360, 100); // Spin the motor 1 full rotation at full power
}
```

## Exit Conditions
But the code above is in a while loop, so this would never exit and you wouldn't be able to do anything else in your autonomous!  We want this loop to run until the motor is there.  So we need some code to detect what "there" means.  We call this exit conditions. 

There's many ways to do this, one of the simpler and more consistent ways is to check if the velocity of the motor is 0 for some amount of time.  

When the velocity of the motor is 0, `x` increases by 10 (this is so x is in ms).  When `x` is greater than or equal to 50, this means the velocity has been 0 for 50ms, and the code `breaks` the while loop.  

```cpp
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

void autonomous(void) {
  feedback(360, 100); // Spin the motor 1 full rotation at full power
  feedback(0, 50);   // Spin the motor 1 full rotation backwards at half power
}
```

## Tuning a P Controller
The goal of a feedback controller is to slow down as you get to the target to not overshoot, and you accomplish this by modifying your constants (kP, kI and kD).  

If your kP is too high, your robot will oscillate around the target.  If your kP is too low, you'll undershoot and not make your target.  You need to trial and error your kP until you get to a value that gets you to your position consistently. 