#include "Navio/MPU9250.h"
#include "Navio/LSM9DS1.h"
#include "Navio/Util.h"
#include <unistd.h>

#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sstream>


int main(int argc, char **argv)
{
 	/***********************/
	/* Initialize The Node */
	/***********************/
	ros::init(argc, argv, "imu_readings");
	ros::NodeHandle n;
	ros::Publisher chatter_pub = n.advertise<std_msgs::String>("Acceleration", 1000);

	//running rate = 2 Hz
	ros::Rate loop_rate(2);

	/*************************/
	/* Initialize the Sensor */
	/*************************/
	InertialSensor *imu;

	printf("Selected: MPU9250\n");
	imu = new MPU9250();

	/***************/
	/* Test Sensor */
	/***************/
	if (!sensor->probe()) 
	{
		printf("Sensor not enabled\n");
		return EXIT_FAILURE;
	}

	sensor->initialize();

	float ax, ay, az;

	while (ros::ok())
	{
		sensor->update();
		sensor->read_accelerometer(&ax, &ay, &az);
		ROS_INFO("Accelerometer : X = %+7.3f, Y = %+7.3f, Z = %+7.3f", ax, ay, az);


		chatter_pub.publish(msg);

		ros::spinOnce();

		loop_rate.sleep();

	}


  	return 0;
}

