# Clipnum

## clipnum(double input, double max, double min)

`clipnum()` is a function we will write that takes an input and limits it to a maximum and a minimum.  

If we input 1000 but we want a number within 50 and -50, clipnum will limit it to that range.  But if the number is within the range, it will return the input. 

Just like we have functions that are `void`, we can have functions that are `double` or any variable type that equal things.  Sensors are functions that return their values!

The logic for `clipnum()` is just checking if input is greater then max, we want to output max.  If input is less then min, we want to output min.  If it's neither of those, then the input is within the range and we can just output that.

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

To test if this works, we can add some examples in `pre_auton`, and watch what prints in `Terminal`.   

```cpp
void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
  
  printf("\nclipnum(1000, 50, -50) = %f", clipnum(1000, 50, -50));
  printf("\nclipnum(-1000, 50, -50) = %f", clipnum(-1000, 50, -50));
  printf("\nclipnum(20, 50, -50) = %f", clipnum(20, 50, -50));
}
```