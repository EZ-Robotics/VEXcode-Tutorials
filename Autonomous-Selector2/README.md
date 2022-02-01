# Autonomous Selector 2

This project is a cleaned version of the previous example using better practices. 

## Prerequisites
The `pre_auton` function from the previous example. 
```cpp
int AutonNumber = 1; // Current Autonomous

void pre_auton() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  // . . .
  // Put your pre auton code here!
  // . . .

  // Print 8 rectangles 
  for (int i = 0; i < 8; i++) {
    Brain.Screen.setPenColor(blue);
    Brain.Screen.drawRectangle(5+(60*i), 50, 50, 175, blue);
  }

  // Set font to monoM
  Brain.Screen.setFont(monoM); 

  while (true) {

    // Selected position
    int x = Brain.Screen.xPosition(); // X position of finger
    int y = Brain.Screen.yPosition(); // Y position of finger

    // Check if finger is within vertical selection of the boxes
    if (50 < y && y < 50+175) {
      for (int i = 0; i < 8; i++) {
        // Check which x value the finger is within
        if (5+(60*i) < x && x < (60*(i+1))-5) {
          AutonNumber = i+1;
        }
      }
    }

    // Print AutonNumber to brain
    Brain.Screen.printAt(5, 30, "Current Auto: %d", AutonNumber); // %d is a formatting character that gets replaced with AutonNumber
    wait(20, msec);
    Brain.Screen.clearLine(1);
  }
}
```

## Removing Hard Coded Numbers
Currently, the amount of boxes and size of the boxes are hardcoded.  If we wanted to change them, we'd have to find every instance of the number and modify it.  It would be difficult to do this without making mistakes.  

We can create variables that we use throughout the code.  For the rectangle we have `(x1, y1)` and `(x2, y2)` that can be defined.  Each rectangle is spaced 60 pixels apart, and we have 8 rectangles.  Now these variables can be replaced throughout the code. 

Even though this code is longer, it is easier to maintain.  

```cpp
int AutonNumber = 1; // Current Autonomous

void pre_auton() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  // . . .
  // Put your pre auton code here!
  // . . .

  // Variables 
  int x1 = 5;
  int y1 = 50;
  int x2 = 50;
  int y2 = 175;
  int spacing = 60;
  int rect_amount = 8;

  // Print 8 rectangles 
  for (int i = 0; i < rect_amount; i++) {
    Brain.Screen.setPenColor(blue);
    Brain.Screen.drawRectangle(x1+(spacing*i), y1, x2, y2, blue);
  }

  // Set font to monoM
  Brain.Screen.setFont(monoM); 

  while (true) {

    // Selected position
    int x = Brain.Screen.xPosition(); // X position of finger
    int y = Brain.Screen.yPosition(); // Y position of finger

    // Check if finger is within vertical selection of the boxes
    if (y1 < y && y < y1+y2) {
      for (int i = 0; i < rect_amount; i++) {
        // Check which x value the finger is within
        if (x1+(spacing*i) < x && x < (spacing*(i+1))-x1) {
          AutonNumber = i+1;
        }
      }
    }

    // Print AutonNumber to brain
    Brain.Screen.printAt(5, 30, "Current Auto: %d", AutonNumber); // %d is a formatting character that gets replaced with AutonNumber
    wait(20, msec);
    Brain.Screen.clearLine(1);
  }
}
```

## Changing Color on Selected Rectangle
This is a small detail but makes the selector a little clearer.  We will change the color of the selected autonomous.
![](https://cdn.discordapp.com/attachments/620685769945645096/928863436740370502/PXL_20220107_040952089.jpg)


### Initial Print
When we first draw the boxes, `AutonNumber` is set to a number.  We should initially draw that box a different color.  

In the same way we created variables for the positions earlier, we can create variables for colors.  By checking if `i+1` is equal to `AutonNumber`, we can change the color of `current_color` to be `selected` or `unselected`. 
```cpp
  color selected = red;
  color unselected = blue;

  // Print 8 rectangles 
  for (int i = 0; i < rect_amount; i++) {
    color current_color;
    if (i+1 == AutonNumber) {
      current_color = selected;
    } else {
      current_color = unselected;
    }
    Brain.Screen.setPenColor(current_color);
    Brain.Screen.drawRectangle(x1+(spacing*i), y1, x2, y2, current_color);
  }
  ```

  ### Update on Selection
  We need to update the colors when we press the screen. 

  First we need to change the color of the previously selected box to unselected.  Then we can update `AutonNumber`, and change the color of the new box.  
```cpp
      // Check if finger is within vertical selection of the boxes
    if (y1 < y && y < y1+y2) {
      for (int i = 0; i < rect_amount; i++) {
        // Check which x value the finger is within
        if (x1+(spacing*i) < x && x < (spacing*(i+1))-x1) {
          // Change color of previously selected box to unselected
          Brain.Screen.setPenColor(unselected);
          Brain.Screen.drawRectangle(x1+(spacing*(AutonNumber-1)), y1, x2, y2, unselected);
          AutonNumber = i+1; // Update AutonNumber
          // Change color of new box to selected
          Brain.Screen.setPenColor(selected);
          Brain.Screen.drawRectangle(x1+(spacing*(AutonNumber-1)), y1, x2, y2, selected);
        }
      }
    }
```

### Bug Fixing
The color of the text starts as `unselected`, but changes to `selected` when you select a box.  This can be fixed by forcing the pne color to be `selected` before we print the text.  
```cpp
    // Print AutonNumber to brain
    Brain.Screen.setPenColor(selected);
    Brain.Screen.printAt(5, 30, "Current Auto: %d", AutonNumber); // %d is a formatting character that gets replaced with AutonNumber
    wait(20, msec);
    Brain.Screen.clearLine(1);
```

## Final Code
```cpp
int AutonNumber = 1; // Current Autonomous

void pre_auton() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  // . . .
  // Put your pre auton code here!
  // . . .

  // Variables 
  int x1 = 5;
  int y1 = 50;
  int x2 = 50;
  int y2 = 175;
  int spacing = 60;
  int rect_amount = 8;
  color selected = red;
  color unselected = blue;

  // Print 8 rectangles 
  for (int i = 0; i < rect_amount; i++) {
    color current_color;
    // Check if i is AutonNumber to change current_color
    if (i+1 == AutonNumber) {
      current_color = selected;
    } else {
      current_color = unselected;
    }
    Brain.Screen.setPenColor(current_color);
    Brain.Screen.drawRectangle(x1+(spacing*i), y1, x2, y2, current_color);
  }

  // Set font to monoM
  Brain.Screen.setFont(monoM); 

  while (true) {

    // Selected position
    int x = Brain.Screen.xPosition(); // X position of finger
    int y = Brain.Screen.yPosition(); // Y position of finger

    // Check if finger is within vertical selection of the boxes
    if (y1 < y && y < y1+y2) {
      for (int i = 0; i < rect_amount; i++) {
        // Check which x value the finger is within
        if (x1+(spacing*i) < x && x < (spacing*(i+1))-x1) {
          // Change color of previously selected box to unselected
          Brain.Screen.setPenColor(unselected);
          Brain.Screen.drawRectangle(x1+(spacing*(AutonNumber-1)), y1, x2, y2, unselected);
          AutonNumber = i+1; // Update AutonNumber
          // Change color of new box to selected
          Brain.Screen.setPenColor(selected);
          Brain.Screen.drawRectangle(x1+(spacing*(AutonNumber-1)), y1, x2, y2, selected);
        }
      }
    }

    // Print AutonNumber to brain
    Brain.Screen.setPenColor(selected);
    Brain.Screen.printAt(5, 30, "Current Auto: %d", AutonNumber); // %d is a formatting character that gets replaced with AutonNumber
    wait(20, msec);
    Brain.Screen.clearLine(1);
  }
}
```