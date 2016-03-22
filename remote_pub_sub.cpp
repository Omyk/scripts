#include "RCInput.h"
#include "PWM.h"
#include "Util.h"
#include <unistd.h>

#include "ros/ros.h"
#include "std_msgs/Float64MultiArray.h"
#include "std_msgs/MultiArrayLayout.h"
#include "std_msgs/MultiArrayDimension.h"
#include "std_msgs/Float64.h"
#include <sstream>

#define MOTOR_PWM_OUT 9
#define SERVO_PWM_OUT 1

int main(int argc, char **argv)
{
 	/***********************/
	/* Initialize The Node */
	/***********************/
	ros::init(argc, argv, "remote_reading_handler");
	ros::NodeHandle n;
	ros::Publisher remote_pub = n.advertise<std_msgs::Float64MultiArray>("remote_readings", 1000);
	
	//running rate = 30 Hz
	ros::Rate loop_rate(30);

	/*******************************************/
	/* Initialize the RC input, and PWM output */
	/*******************************************/

	RCInput rcin;
	rcin.init();
	PWM servo;
	PWM motor;

	if (!motor.init(MOTOR_PWM_OUT)) {
		fprintf(stderr, "Motor Output Enable not set. Are you root?\n");
		return 0;
    	}

	if (!servo.init(SERVO_PWM_OUT)) {
		fprintf(stderr, "Servo Output Enable not set. Are you root?\n");
		return 0;
    	}

	motor.enable(MOTOR_PWM_OUT);
	servo.enable(SERVO_PWM_OUT);

	motor.set_period(MOTOR_PWM_OUT, 50); //frequency 50Hz
	servo.set_period(SERVO_PWM_OUT, 50); 

	int motor_input = 0;
	int servo_input = 0;

	//msg stuff
	std_msgs::Float64 a[] = {0.0, 0.0};
	std_msgs::MultiArrayDimension dim;
	dim.size = sizeof(a);
	dim.label = "REMOTEmsg";
	dim.stride = sizeof(a);

	std_msgs::Float64MultiArray apub;
	apub.data = a;
	apub.layout.dim.append(dim);
	apub.layout.data_offset = 0;

	while (ros::ok())
	{

		//read input from rc
		servo_input = rcin.read(2); // Roll steering
		motor_input = rcin.read(3); // Throttle

		//write readings on pwm output
		motor.set_duty_cycle(MOTOR_PWM_OUT, ((float)motor_input)/1000.0f); 
		servo.set_duty_cycle(SERVO_PWM_OUT, ((float)servo_input)/1000.0f);
		
		//save values into msg container a
		a[0] = motor_input;
		a[1] = servo_input;

		//debug info
		ROS_INFO("Thrust usec = %d    ---   Steering usec = %d", a[0], a[1]);
	
		apub.data = a;

		remote_pub.publish(apub);

		/*sensor_msgs::Imu imu_msg;
		sensor_msgs::MagneticField mf_msg;

		init_imu_msg(&imu_msg);
		init_mf_msg(&mf_msg);
		
		update_imu_msg(&imu_msg, sensor);
		update_mf_msg(&mf_msg, sensor);

		imu_pub.publish(imu_msg);
		mf_pub.publish(mf_msg);
		*/
		ros::spinOnce();

		loop_rate.sleep();

	}


  	return 0;
}

