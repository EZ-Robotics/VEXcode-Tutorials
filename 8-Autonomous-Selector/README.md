# Screen Autonomous Selector

The goal of this is to have a variable, `AutonNumber`, that changes when you press something on the brain screen.  Then in autonomous, we can read this number and run different autonomous modes depending on what we selected. 


## End Goal
The goal is to have eight rectangles that we can select that changes AutonNumber. 
![](https://cdn.discordapp.com/attachments/620685769945645096/928877840601067540/PXL_20220107_050838878.jpg)

## Drawing the Rectangles
The brain screen is 480 x 240.  If we want 8 rectangles, each rectangle can take up 60 pixels.  In the image above, the rectangles are 50 pixels wide with a little space between each one.  

The `drawRectangle()` command prints a rectangle at x1, y1, x2, y2.  This function would print the first rectangle.  x2 and y2 are relative from x1 y1.

```cpp
void pre_auton() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  Brain.Screen.setPenColor(blue);
  Brain.Screen.drawRectangle(5, 50, 50, 175, blue);
}
```

To print multiple rectangles, we can copy/paste this, but increase `x1` by 60 every time.
```cpp
void pre_auton() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  Brain.Screen.setPenColor(blue);
  Brain.Screen.drawRectangle(5, 50, 50, 175, blue);
  Brain.Screen.drawRectangle(5+60, 50, 50, 175, blue);
  Brain.Screen.drawRectangle(5+120, 50, 50, 175, blue);
  Brain.Screen.drawRectangle(5+180, 50, 50, 175, blue);
  Brain.Screen.drawRectangle(5+240, 50, 50, 175, blue);
  Brain.Screen.drawRectangle(5+300, 50, 50, 175, blue);
  Brain.Screen.drawRectangle(5+360, 50, 50, 175, blue);
  Brain.Screen.drawRectangle(5+420, 50, 50, 175, blue);
}
```

It is bad practice to copy/paste code like this because it is difficult to maintain.  If you want to change the size of the rectangle a little, you need to go through every line of code and modify it, making it extremely easy to make a mistake.

We can use a `for` loop and simplify this code.  I would try to explain for loops, [but this interactive tutorial does a better job then I will](https://www.w3schools.com/cpp/cpp_for_loop.asp). 

In the code above, each time we print a new rectangle, we are adding `60*x` to 5.  So in the for loop, we can add `60*i`.  The first time it runs, `60*0` is `0` so it will just print at `5`.  The second time it runs, `60*1` is `60` so it will print at `65`.  This is the same as the code above. 
```cpp
void pre_auton() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  for (int i = 0; i < 8; i++) {
    Brain.Screen.setPenColor(blue);
    Brain.Screen.drawRectangle(5+(60*i), 50, 50, 175, blue);
  }
}
```

## Finding Selection
After the selection boxes are printed, we will run a while loop to find what pixel was pressed.  

We need to check which box the selection was within.  All of the selections have the same up/down bound, where the selection can be within `50` (y1) and `175` (y2).  We can read selected positions with 


```cpp
int AutonNumber = 1;

void pre_auton() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  for (int i = 0; i < 8; i++) {
    Brain.Screen.setPenColor(blue);
    Brain.Screen.drawRectangle(5+(60*i), 50, 50, 175, blue);
  }

  while (true) {
    // Selected position
    int x = Brain.Screen.xPosition();
    int y = Brain.Screen.yPosition();

    if (50 < y && y < 50+175 && 5 < x && x < 55) {
      // First Box
      AutonNumber = 1;
    }
    else if (50 < y && y < 50+175 && 65 < x && x < 115) {
      // Second Box
      AutonNumber = 2;
    }
    else if (50 < y && y < 50+175 && 125 < x && x < 175) {
      // Third Box
      AutonNumber = 3;
    }
    else if (50 < y && y < 50+175 && 185 < x && x < 235) {
      // Fourth Box
      AutonNumber = 4;
    }
    else if (50 < y && y < 50+175 && 245 < x && x < 295) {
      // Fifth Box
      AutonNumber = 5;
    }
    else if (50 < y && y < 50+175 && 305 < x && x < 355) {
      // Sixth Box
      AutonNumber = 6;
    }
    else if (50 < y && y < 50+175 && 365 < x && x < 415) {
      // Seventh Box
      AutonNumber = 7;
    }
    else if (50 < y && y < 50+175 && 425 < x && x < 475) {
      // Eighth Box
      AutonNumber = 8;
    }

    wait(20, msec);
  }
}
```

But again, we have this copy/pasted nightmare!  We can fix this in the same way we did before, by using a for loop. 

The `y` checks for all the boxes are the same, so we don't need to run the for loop until we know the selection is within that y. 

The `x` is checking `5+(60*i)` for the first one, and `(60*(i+1))-5`.

```cpp
int AutonNumber = 1;

void pre_auton() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  for (int i = 0; i < 8; i++) {
    Brain.Screen.setPenColor(blue);
    Brain.Screen.drawRectangle(5+(60*i), 50, 50, 175, blue);
  }

  while (true) {
    // Selected position
    int x = Brain.Screen.xPosition();
    int y = Brain.Screen.yPosition();

    if (50 < y && y < 50+175) {
      for (int i = 0; i < 8; i++) {
        if (5+(60*i) < x && x < (60*(i+1))-5) {
          AutonNumber = i+1;
        }
      }
    }

    wait(20, msec);
  }
}
```

## Printing AutonNumber
To print text to the screen we first need to define a font using `setFont`.  This only needs to happen once, and should be before the while loop.  

Then to set text, we can use `Brain.Screen.printAt()`.  After writing text we need a delay (every loop needs a delay to avoid wasting the brains resources), and after the delay we can clear the text to allow the text to update.  

```cpp
int AutonNumber = 1;

void pre_auton() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  
  for (int i = 0; i < 8; i++) {
    Brain.Screen.setPenColor(blue);
    Brain.Screen.drawRectangle(5+(60*i), 50, 50, 175, blue);
  }

  Brain.Screen.setFont(monoM); 

  while (true) {
    // Selected position
    int x = Brain.Screen.xPosition();
    int y = Brain.Screen.yPosition();

    if (50 < y && y < 50+175) {
      for (int i = 0; i < 8; i++) {
        if (5+(60*i) < x && x < (60*(i+1))-5) {
          AutonNumber = i+1;
        }
      }
    }

    Brain.Screen.printAt(5, 30, "Current Auto: %d", AutonNumber); // %d is a formatting character that gets replaced with AutonNumber
    wait(20, msec);
    Brain.Screen.clearLine(1);
  }
}
```

## Using in Autonomous

```cpp
void autonomous(void) {
  if (AutonNumber == 1) {
    // Call Auton 
  }
  else if (AutonNumber == 2) {
    // Call Auton 
  }
  else if (AutonNumber == 3) {
    // Call Auton 
  }
  else if (AutonNumber == 4) {
    // Call Auton 
  }
  else if (AutonNumber == 5) {
    // Call Auton 
  }
  else if (AutonNumber == 6) {
    // Call Auton 
  }
  else if (AutonNumber == 7) {
    // Call Auton 
  }
  else if (AutonNumber == 8) {
    // Call Auton 
  }
}
```