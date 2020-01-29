//Test Drive

#include "main.h"
#include "okapi/api.hpp"
using namespace std;
void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}
void initialize() {
pros::lcd::initialize();
pros::lcd::set_text(1, "/kill @e [type=compitetors]");
pros::lcd::register_btn1_cb(on_center_button);
}
int gyro(){
pros::ADIAnalogIn sensor (1);
return sensor.get_value();
while (true){
cout << "distance" << sensor.get_value();
}
}
void disabled() {}
void competition_initialize() {}
pros::Controller master(pros::E_CONTROLLER_MASTER);
pros::Motor left_mtr_frnt(19);
pros::Motor left_mtr_bck(18);
pros::Motor right_mtr_frnt(1);
pros::Motor right_mtr_bck(6);
pros::Motor claw_mtr(8);
pros::Motor lift1(20);
pros::Motor lift2(10);
int distanceCalc(float numb){
return(numb/12.6)*900;}
void straight(float distance, float speed){
int tick_distance = distanceCalc(distance);
while (left_mtr_bck.get_position() < tick_distance && right_mtr_bck.get_position() < tick_distance){
left_mtr_bck.move_velocity(tick_distance);
left_mtr_frnt.move_velocity(tick_distance);
right_mtr_frnt.move_velocity(-tick_distance);
right_mtr_bck.move_velocity(-tick_distance);
pros::delay(10);
}
return;
}
void lift(float hight){
int hight_tick = distanceCalc(hight);
lift1.move_velocity(hight_tick);
lift2.move_velocity(hight_tick);
while(lift1.get_position() >= hight_tick){
pros::delay(10);
}
}
void turn(int degrees, int speed){
left_mtr_frnt.move_velocity(speed);
left_mtr_bck.move_velocity(speed);
right_mtr_frnt.move_velocity(speed);
right_mtr_bck.move_velocity(speed);
while (gyro() >= degrees){
pros::delay(10);
}
left_mtr_frnt.move(0);
left_mtr_bck.move(0);
right_mtr_frnt.move(0);
right_mtr_bck.move(0);
}
void back (float distance, float speed){
int tick_distance = distanceCalc(-distance);
left_mtr_frnt.move_velocity(tick_distance);
left_mtr_bck.move_velocity(tick_distance);
right_mtr_frnt.move_velocity(-tick_distance);
right_mtr_bck.move_velocity(-tick_distance);
pros::delay(10);
}
void claw(){
claw_mtr.move(50);
}
void autonomous(){
straight(54,100);
back(24,100);
while(true)
{
straight(0.1,0.1);
back(0.1,0.1);
pros::delay(10);
}
}
void opcontrol(){
int fwd;
int liftu;
int liftd;
int right;
int speed;
int fwd_fast;
int right_speed;
while (true) {
pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
(pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
(pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);
liftu = master.get_digital(DIGITAL_L1);
liftd = master.get_digital(DIGITAL_L2);
fwd = master.get_analog(ANALOG_RIGHT_X/2);
right = master.get_analog(ANALOG_LEFT_Y/2);
speed = master.get_digital(DIGITAL_A);
fwd_fast = master.get_analog(ANALOG_RIGHT_X);
right_speed = master.get_analog(ANALOG_LEFT_Y);
while  (speed == 1){
	left_mtr_bck = fwd_fast+right_speed;
	left_mtr_frnt = fwd_fast+right_speed;
	right_mtr_frnt = fwd_fast-right_speed;
	right_mtr_bck = fwd_fast-right_speed;
}
if (master.get_digital(DIGITAL_R1) == 1){
claw_mtr.move_velocity(100);
}else{
claw_mtr.move(-10);}if (liftu == 1){
lift2.move_velocity(-30);
lift1.move_velocity(30);}
if (liftd == 1){
lift1.move_velocity(-25);
lift2.move_velocity(25);}
if (liftu == 0 && liftd == 0){
lift1.move_absolute(0,0);
lift2.move_absolute(0,0);}
left_mtr_bck = fwd+right;
left_mtr_frnt = fwd+right;
right_mtr_frnt = fwd-right;
right_mtr_bck = fwd-right;
pros::delay(10);
}
}
