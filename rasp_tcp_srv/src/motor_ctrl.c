#include <wiringPi.h> /* include wiringPi library */
#include <stdio.h>    
#include <unistd.h>
#include <softPwm.h>  /* include header file for software PWM */

#define SPEED_STEP      10
#define MOTOR_DIRECTION_FORWARD         (1)
#define MOTOR_DIRECTION_BACKWARD        (2)
	// static int motor1_pwm_pin = 1;            			
	// static int motor1_ina_pin = 4;
	// static int motor1_inb_pin = 5;

	// static int motor2_pwm_pin = 23;
	// static int motor2_ina_pin = 21;
	// static int motor2_inb_pin = 22;


	static int motor1_pwm_pin = 23;//1;            			
	static int motor1_ina_pin = 21;//4;
	static int motor1_inb_pin = 22;//5;

	static int motor2_pwm_pin = 1;//23;
	static int motor2_ina_pin = 4;//21;
	static int motor2_inb_pin = 5;//22;

    static int current_right_speed = 0;
    static int current_left_speed = 0;


	static int current_right_direction = MOTOR_DIRECTION_FORWARD;
	static int current_left_direction = MOTOR_DIRECTION_FORWARD;
int set_motor1_forward_direction()
{
	if (current_right_direction != MOTOR_DIRECTION_FORWARD)
	{
		digitalWrite(motor1_ina_pin, LOW);
		digitalWrite(motor1_inb_pin, HIGH);
		current_right_direction = MOTOR_DIRECTION_FORWARD;
	}
	return 0;
}
int set_motor1_backward_direction()
{
	if (current_right_direction != MOTOR_DIRECTION_BACKWARD)
	{
		digitalWrite(motor1_ina_pin, HIGH);
		digitalWrite(motor1_inb_pin, LOW);
		current_right_direction = MOTOR_DIRECTION_BACKWARD;
	}
			return 0;
}
int set_motor2_forward_direction()
{
	if (current_left_direction != MOTOR_DIRECTION_FORWARD)
	{
		digitalWrite(motor2_ina_pin, LOW);
		digitalWrite(motor2_inb_pin, HIGH);
		current_left_direction = MOTOR_DIRECTION_FORWARD;
	}	
	return 0;
}
int set_motor2_backward_direction()
{
	if (current_left_direction != MOTOR_DIRECTION_BACKWARD)
	{
		digitalWrite(motor2_ina_pin, HIGH);
		digitalWrite(motor2_inb_pin, LOW);
		current_left_direction = MOTOR_DIRECTION_BACKWARD;
	}
	return 0;
}

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
	
	
	set_motor1_forward_direction();
	set_motor2_forward_direction();
	
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

int motor_intensity_update(int motor1_intensity, int motor2_intensity)
{
	current_right_speed = motor1_intensity;
	current_left_speed = motor2_intensity;
	update_speed();
}
