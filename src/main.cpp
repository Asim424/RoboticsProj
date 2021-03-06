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
	pros::lcd::set_text(1, "Hello PROS User!");

	pros::lcd::register_btn1_cb(on_center_button);

	pros::Motor lft_mtr_frnt(1,pros::E_MOTOR_GEARSET_18,false,pros::E_MOTOR_ENCODER_COUNTS);
}
int gyro(){
	pros::ADIAnalogIn sensor (1);
	return sensor.get_value();
	while (true){
		cout << "distance" << sensor.get_value();
	}
}
//gyro works


void disabled() {}

void competition_initialize() {

}


pros::Controller master(pros::E_CONTROLLER_MASTER);
pros::Motor left_mtr_frnt(1);
pros::Motor left_mtr_bck(5);
pros::Motor right_mtr_frnt(3);
pros::Motor right_mtr_bck(6);
pros::Motor intake1(8);
pros::Motor intake2(9);
pros::Motor aa(10);
int distanceCalc(float numb){
	return (numb/12.6)*900;
}

//distance inch
//speed velocity
void straight(float distance, int speed){
	int tick_distance = distanceCalc(distance);
	left_mtr_frnt.move_absolute(tick_distance, speed);
	left_mtr_bck.move_absolute(tick_distance, speed);
	right_mtr_frnt.move_absolute(tick_distance, speed);
	right_mtr_bck.move_absolute(tick_distance, speed);
	while (left_mtr_bck.get_position() < tick_distance && right_mtr_bck.get_position() < tick_distance){
		pros::delay(10);
	}
}
//positive degrees = right
//speed = velocity
void turn(int degrees, int speed){
	left_mtr_bck.move_velocity(speed);
	right_mtr_frnt.move_velocity(speed);
	right_mtr_bck.move_velocity(speed);
	while (gyro() >= degrees){
		pros::delay(10);
	}
	left_mtr_frnt.move_velocity(speed);
	left_mtr_frnt.move_velocity(0);
	left_mtr_bck.move_velocity(0);
	right_mtr_frnt.move_velocity(0);
	right_mtr_bck.move_velocity(0);
}



void autonomous() {
// no claw
	/*
	straight(72, 100);
	pros::delay(100);
	straight(-72, 100);
	*/
}

void opcontrol() {
	int fwd;
	int right;
	int intake;

	while (true) {
		pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		                 (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		                 (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);
		fwd = master.get_analog(ANALOG_LEFT_Y);
		right = master.get_analog(ANALOG_RIGHT_X);
		intake = master.get_digital(DIGITAL_L1);
		left_mtr_bck = fwd+right;
		left_mtr_frnt = fwd+right;
		right_mtr_frnt = -fwd+right;
		right_mtr_bck = -fwd+right;
		intake1 = intake;
		intake2 = intake;
		if (master.get_digital(DIGITAL_L1) == 1 ){
			intake1.move_velocity(200);
			intake2.move_velocity(-200);
		}
		if (master.get_digital(DIGITAL_R1) == 1){
			aa.move(20);
		} else if(master.get_digital(DIGITAL_R2) == 1){
			aa.move(-20);
		} else{
			aa.set_brake_mode(MOTOR_BRAKE_HOLD);
		}
		/*if (std::abs(right) >= 30){
			left_mtr_bck = right;
			left_mtr_frnt = right;
			right_mtr_frnt = right2;
			right_mtr_bck = right2;
		}*/

		pros::delay(10);
 		}
}
