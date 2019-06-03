#include <wiringPi.h> /* include wiringPi library */
#include <stdio.h>    
#include <unistd.h>
#include <softPwm.h>  /* include header file for software PWM */

	int motor1_pwm_pin = 1;            			
	int motor1_ina_pin = 4;
	int motor1_inb_pin = 5;

	int motor2_pwm_pin = 23;
	int motor2_ina_pin = 21;
	int motor2_inb_pin = 22;

    static int current_right_speed = 0;
    static int current_left_speed = 0;
#define SPEED_STEP      10


int init_motor()
{
	wiringPiSetup();		/* initialize wiringPi setup */
	
	/* right motor control */
	pinMode(motor1_pwm_pin,OUTPUT);
	pinMode(motor1_ina_pin,OUTPUT);
	pinMode(motor1_inb_pin,OUTPUT);
	
	/* left motor control */
	pinMode(motor2_pwm_pin,OUTPUT);
	pinMode(motor2_ina_pin,OUTPUT);
	pinMode(motor2_inb_pin,OUTPUT);
	
	
	digitalWrite(motor1_ina_pin, LOW);
	digitalWrite(motor1_inb_pin, HIGH);
	digitalWrite(motor2_ina_pin, LOW);
	digitalWrite(motor2_inb_pin, HIGH);
	
	softPwmCreate(motor1_pwm_pin,0,100);
	
	softPwmCreate(motor2_pwm_pin,0,100);
//	softPwmWrite (motor1_pwm_pin, 65);
//	softPwmWrite (motor2_pwm_pin, 65);
	
//	delay (5);

//	softPwmWrite (motor1_pwm_pin, 0);
//	softPwmWrite (motor2_pwm_pin, 0);

//	softPwmStop (motor1_pwm_pin);
//	softPwmStop (motor2_pwm_pin);
	
	return 0;
}

void update_speed()
{
	softPwmWrite (motor1_pwm_pin, current_right_speed);
	softPwmWrite (motor2_pwm_pin, current_left_speed);
}

int forward()
{
    current_right_speed += SPEED_STEP;
    current_left_speed += SPEED_STEP;
    update_speed();
    return 0;
}

int stop()
{
    current_right_speed = 0;
    current_left_speed = 0;
    update_speed();
    return 0;
}

int left()
{
    current_right_speed += SPEED_STEP;
    current_left_speed -= SPEED_STEP;
    update_speed();
    return 0;
}

int right()
{
    current_right_speed -= SPEED_STEP;
    current_left_speed += SPEED_STEP;
    update_speed();
    return 0;
}

