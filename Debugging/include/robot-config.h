using namespace vex;

extern brain Brain;

// VEXcode devices
extern controller Controller1;
extern motor left_front;
extern motor left_back;
extern motor right_front;
extern motor right_back;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );