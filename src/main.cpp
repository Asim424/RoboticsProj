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
pros::Motor left_mtr_bck(2);
pros::Motor right_mtr_frnt(3);
pros::Motor right_mtr_bck(4);
pros::Motor claw_mtr(6);
pros::Motor lift1(7);
pros::Motor lift2(8);
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

void clawopen()
{
	claw_mtr.move(50);
}
void clawclose()
{
	claw_mtr.move(-50);
}

void autonomous() {
	//
	//
	//
	//
	//
	straight(12, 100);
	clawopen();
	clawclose();
	turn(180,100);
	straight(12, 100);
	turn(90,100);
	straight(4,100);
	clawopen();
	straight(-24,100);
}

void opcontrol() {
	int fwd;
	int liftu;
	int liftd;
	int right;
	int x;

	while (true) {
		pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		                 (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		                 (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);
		liftu = master.get_digital(DIGITAL_L1);
		liftd = master.get_digital(DIGITAL_L2);
		fwd = master.get_analog(ANALOG_LEFT_Y);
		right = master.get_analog(ANALOG_RIGHT_X);
		x = lift1.get_voltage();
		if (x == lift1.get_voltage()){
			lift2 = lift1.get_voltage()*-1;
		}

		if (master.get_digital(DIGITAL_R1) == 1)
		{
			claw_mtr.move(50);
		}

		else
		{
			claw_mtr.move(0);
		}


		if (liftu == 1)
		{

			lift2.move_velocity(100);
			lift1.move_velocity(100);

		}

		if (liftd == 1)
		{
			lift1.move_velocity(100);;
			lift2.move_velocity(100);
		}

		if (liftu == 0 && liftu == 0)
		{
			lift1.move_velocity(0);
			lift2.move_velocity(0);
		}

		left_mtr_bck = fwd+right;
		left_mtr_frnt = fwd+right;
		right_mtr_frnt = fwd-right;
		right_mtr_bck = fwd-right;
		/*if (std::abs(right) >= 30){
			left_mtr_bck = right;
			left_mtr_frnt = right;
			right_mtr_frnt = right2;
			right_mtr_bck = right2;
		}*/

		pros::delay(10);
 		}
}
