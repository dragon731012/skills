#include "vex.h"

using namespace vex;

competition Competition;
motor bottomleft = motor(PORT10, ratio18_1, true);
motor bottomright = motor(PORT7, ratio18_1, false);
motor topleft = motor(PORT12, ratio18_1, true);
motor topright = motor(PORT11, ratio18_1, false);
motor intake = motor(PORT13, ratio18_1, false);
motor wingright = motor(PORT14, ratio6_1, false);
motor wingleft = motor(PORT15, ratio6_1, true);
motor fly = motor(PORT18, ratio6_1, true);

controller Controller=controller(primary);

motor_group leftmotors=motor_group(topleft,bottomleft);
motor_group rightmotors=motor_group(topright,bottomright);
motor_group wing=motor_group(wingleft,wingright);

bool tank=false;
double leftvalue;
double rightvalue;
bool up=true;
bool wingmoving=false;

void pre_auton(void) {

}

void driveForward(double seconds){
  leftmotors.spin(forward);
  rightmotors.spin(forward);
  wait(seconds, sec);
  rightmotors.stop();
  leftmotors.stop();
}

void driveReverse(double seconds){
  leftmotors.spin(reverse);
  rightmotors.spin(reverse);
  wait(seconds, sec);
  rightmotors.stop();
  leftmotors.stop();
}

void turnLeft(double seconds){
  rightmotors.spin(forward);
  leftmotors.spin(reverse);
  wait(seconds, sec);
  rightmotors.stop();
  leftmotors.stop();
}

void turnRight(double seconds){
  leftmotors.spin(forward);
  rightmotors.spin(reverse);
  wait(seconds, sec);
  leftmotors.stop();
  rightmotors.stop();
}

void turnRight90(){
  leftmotors.spin(forward);
  rightmotors.spin(reverse);
  wait(0.32, sec);
  leftmotors.stop();
  rightmotors.stop();
}

void turnLeft90(){
  leftmotors.spin(reverse);
  rightmotors.spin(forward);
  wait(0.32, sec);
  leftmotors.stop();
  rightmotors.stop();
}

void turnLeft180(){
  leftmotors.spin(reverse);
  rightmotors.spin(forward);
  wait(0.5, sec);
  leftmotors.stop();
  rightmotors.stop();
}

void turnRight180(){
  leftmotors.spin(forward);
  rightmotors.spin(reverse);
  wait(0.5, sec);
  leftmotors.stop();
  rightmotors.stop();
}

void autonomous(void) {
  leftmotors.setVelocity(75, percent);
  rightmotors.setVelocity(75, percent);
  intake.setVelocity(100,percent);
  
  intake.setVelocity(20,percent);
  intake.spin(forward);
  wait(2,sec);
  intake.stop();
  intake.setVelocity(100,percent);
  turnLeft180();
  leftmotors.setVelocity(10,percent);
  rightmotors.setVelocity(10,percent);
  rightmotors.setStopping(coast);
  leftmotors.setStopping(coast);
  driveForward(3);
  leftmotors.setVelocity(75,percent);
  rightmotors.setVelocity(75,percent);
  rightmotors.setStopping(brake);
  leftmotors.setStopping(brake);
  wait(0.5,sec);
  leftmotors.setVelocity(10,percent);
  rightmotors.setVelocity(10,percent);
  rightmotors.setStopping(coast);
  leftmotors.setStopping(coast);
  driveForward(0.3);
  leftmotors.setVelocity(75,percent);
  rightmotors.setVelocity(75,percent);
  rightmotors.setStopping(brake);
  leftmotors.setStopping(brake);
  up=true;
  wingmoving=false;
  fly.setVelocity(100,percent);
  fly.spin(reverse);
  leftmotors.setVelocity(5,percent);
  rightmotors.setVelocity(5,percent);
  rightmotors.spin(forward);
  leftmotors.spin(forward);
}

void switchtype(){
  if (tank){
    tank=false;
    Controller.Screen.print("in use: left joystick drive");
  } else {
    tank=true;
    Controller.Screen.print("in use: tank drive");
  }
}

void usercontrol(void) {
  while (1) {
    if (tank){
      bottomleft.setVelocity(Controller.Axis3.position(), percent);
      topleft.setVelocity(Controller.Axis3.position(), percent);
      topright.setVelocity(Controller.Axis2.position(), percent);
      bottomright.setVelocity(Controller.Axis2.position(), percent);
      bottomleft.spin(forward);
      bottomright.spin(forward);
      topleft.spin(forward);
      topright.spin(forward);
    } else {
      leftvalue=((Controller.Axis3.position()+Controller.Axis4.position())*(Controller.Axis3.position()+Controller.Axis4.position())*(Controller.Axis3.position()+Controller.Axis4.position()))/10000;
      rightvalue=((Controller.Axis3.position()-Controller.Axis4.position())*(Controller.Axis3.position()-Controller.Axis4.position())*(Controller.Axis3.position()-Controller.Axis4.position()))/10000;
      bottomleft.setVelocity(leftvalue, percent);
      topleft.setVelocity(leftvalue, percent);
      topright.setVelocity(rightvalue, percent);
      bottomright.setVelocity(rightvalue, percent);
      bottomleft.spin(forward);
      bottomright.spin(forward);
      topleft.spin(forward);
      topright.spin(forward);
    }
    if (Controller.ButtonR2.pressing()) {
      intake.setVelocity(100.0, percent);
      intake.spin(forward);
    } else if (Controller.ButtonR1.pressing()) {
      intake.setVelocity(100.0, percent);
      intake.spin(reverse);
    } else {
      intake.stop();
    }

    if (Controller.ButtonUp.pressing()) {
      if (!up && !wingmoving){
        wingmoving=true;
        wingleft.setVelocity(70, percent);
        wingright.setVelocity(70, percent);
        wingleft.setStopping(coast);
        wingright.setStopping(coast);
        wingleft.spin(forward);
        wingright.spin(forward);
        wait(0.3,sec);
        wingleft.stop();
        wingright.stop();
        up=true;
        wingmoving=false;
      }
    }

    if (Controller.ButtonDown.pressing()) {
      if (up && !wingmoving){
        wingmoving=true;
        wingleft.setVelocity(70, percent);
        wingright.setVelocity(70, percent);
        wingleft.setStopping(coast);
        wingright.setStopping(coast);
        wingleft.spin(reverse);
        wingright.spin(reverse);
        wait(0.2,sec);
        wingleft.stop();
        wait(0.1,sec);
        wingright.stop();
        up=false;
        wingmoving=false;
      }
    }

    if (Controller.ButtonL2.pressing()) {
      fly.setVelocity(100.0, percent);
      fly.spin(forward);
    } else if (Controller.ButtonL1.pressing()) {
      fly.setVelocity(100.0, percent);
      fly.spin(reverse);
    } else {
      fly.stop();
    }

    Controller.ButtonLeft.pressed(switchtype);
    wait(20, msec); 
  }
}

int main() {
  wing.setStopping(hold);
  wingleft.setStopping(hold);
  wingright.setStopping(hold);
  leftmotors.setStopping(brake);
  rightmotors.setStopping(brake);
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  pre_auton();

  while (true) {
    wait(100, msec);
  }
}
